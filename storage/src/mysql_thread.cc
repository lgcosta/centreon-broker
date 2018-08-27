/*
** Copyright 2018 Centreon
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
** For more information : contact@centreon.com
*/
#include <iostream>
#include "com/centreon/broker/exceptions/msg.hh"
#include "com/centreon/broker/logging/logging.hh"
#include "com/centreon/broker/storage/mysql_thread.hh"

using namespace com::centreon::broker;
using namespace com::centreon::broker::storage;

void mysql_thread::_run(mysql_task_run* task) {
  if (mysql_query(_conn, task->query.c_str())) {
    std::cout << "run query failed: " << mysql_error(_conn) << std::endl;
    logging::error(logging::medium)
      << "storage: Error while sending query '" << task->query << "'";
  }
  else if (task->fn) {
    // FIXME DBR: we need a way to send an error to the main thread
    int ret(task->fn(_conn));
    logging::info(logging::medium)
      << "storage: callback returned " << ret;
  }
}

void mysql_thread::_run_sync(mysql_task_run_sync* task) {
  QMutexLocker locker(&_result_mutex);
  if (mysql_query(_conn, task->query.c_str())) {
    std::cout << "run sync query failed: " << mysql_error(_conn) << std::endl;
    logging::error(logging::medium)
      << "storage: Error while sending query '" << task->query << "'";
  }
  _result = new mysql_result(mysql_store_result(_conn));
  _result_condition.wakeAll();
}

misc::shared_ptr<mysql_result> mysql_thread::get_result() {
  return _result;
}

void mysql_thread::_prepare(mysql_task_prepare* task) {
  misc::shared_ptr<MYSQL_STMT> stmt(mysql_stmt_init(_conn));
  if (!stmt.data()) {
    logging::error(logging::medium)
      << "storage: Could not initialize statement";
  }
  if (mysql_stmt_prepare(stmt.data(), task->query.c_str(), task->query.size())) {
    logging::error(logging::medium)
      << "storage: Could not prepare statement";
  }
  _stmt.push_back(stmt);
}

void mysql_thread::_statement(mysql_task_statement* task) {
  if (mysql_stmt_bind_param(_stmt[task->statement_id].data(), const_cast<MYSQL_BIND*>(task->bind.get_bind()))) {
    logging::error(logging::medium)
      << "storage: Error while binding values in statement: "
      << mysql_stmt_error(_stmt[task->statement_id].data());
  }
  if (mysql_stmt_execute(_stmt[task->statement_id].data())) {
    logging::error(logging::medium)
      << "storage: Error while sending prepared query: "
      << mysql_stmt_error(_stmt[task->statement_id].data());
  }
}

void mysql_thread::run() {
  while (!_finished) {
    std::cout << "run mutex lock" << std::endl;
    QMutexLocker locker(&_list_mutex);
    std::cout << "run mutex locked" << std::endl;
    if (!_queries_list.empty()) {
      std::cout << "new task" << std::endl;
      misc::shared_ptr<mysql_task> task(_queries_list.front());
      _queries_list.pop_front();
      std::cout << "unlock mutex" << std::endl;
      locker.unlock();
      std::cout << "mutex unlocked" << std::endl;
      switch (task->type) {
       case mysql_task::RUN:
        std::cout << "run RUN" << std::endl;
        _run(static_cast<mysql_task_run*>(task.data()));
        break;
       case mysql_task::RUN_SYNC:
        std::cout << "run RUN SYNC" << std::endl;
        _run_sync(static_cast<mysql_task_run_sync*>(task.data()));
        break;
       case mysql_task::PREPARE:
        std::cout << "run PREPARE" << std::endl;
        _prepare(static_cast<mysql_task_prepare*>(task.data()));
        break;
       case mysql_task::STATEMENT:
        std::cout << "run STATEMENT" << std::endl;
        _statement(static_cast<mysql_task_statement*>(task.data()));
        break;
       case mysql_task::FINISH:
        std::cout << "run FINISH" << std::endl;
        _finished = true;
        break;
       default:
        std::cout << "run DEFAULT SITUATION with type = " << task->type << std::endl;
        logging::error(logging::medium)
          << "storage: Error type not managed...";
        break;
      }
    }
    else {
      std::cout << "run wait for condition on queries or finish" << std::endl;
      _tasks_condition.wait(locker.mutex());
      std::cout << "run condition realized" << std::endl;
    }
  }
  std::cout << "run return" << std::endl;
}

mysql_thread::mysql_thread(
                std::string const& address,
                std::string const& user,
                std::string const& password,
                std::string const& database,
                int port)
  : _conn(mysql_init(NULL)),
    _finished(false) {
  std::cout << "mysql_thread constructor" << std::endl;
  if (!_conn) {
    std::cout << "mysql_thread throw exception" << std::endl;
    throw exceptions::msg()
      << "storage: Unable to initialize the MySQL client connector: "
      << mysql_error(_conn);
  }

  std::cout << "mysql_thread real connect..." << std::endl;
  if (!mysql_real_connect(
         _conn,
         address.c_str(),
         user.c_str(),
         password.c_str(),
         database.c_str(),
         port,
         NULL,
         0)) {
    std::cout << "mysql_thread constructor real connect failed" << std::endl;
    std::cout << "mysql_thread throw exception" << std::endl;
    throw exceptions::msg()
      << "storage: The connection to '"
      << database << ":" << port << "' MySQL database failed: "
      << mysql_error(_conn);
  }
  std::cout << "mysql_thread start thread..." << std::endl;
  start();
  std::cout << "mysql_thread return" << std::endl;
}

mysql_thread::~mysql_thread() {
  std::cout << "destructor" << std::endl;
  mysql_close(_conn);
  mysql_thread_end();
  std::cout << "destructor return" << std::endl;
}

void mysql_thread::_push(misc::shared_ptr<mysql_task> const& q) {
  QMutexLocker locker(&_list_mutex);
  _queries_list.push_back(q);
  _tasks_condition.wakeAll();
}

void mysql_thread::run_query(std::string const& query) {
  _push(misc::shared_ptr<mysql_task>(new mysql_task_run(query)));
}

void mysql_thread::run_query_sync(std::string const& query) {
  QMutexLocker locker(&_result_mutex);
  _push(misc::shared_ptr<mysql_task>(new mysql_task_run_sync(query)));
  _result_condition.wait(locker.mutex());
}

void mysql_thread::run_statement(int statement_id, mysql_bind const& bind) {
  _push(misc::shared_ptr<mysql_task>(new mysql_task_statement(statement_id, bind)));
}

void mysql_thread::prepare_query(std::string const& query) {
  _push(misc::shared_ptr<mysql_task>(new mysql_task_prepare(query)));
}

void mysql_thread::run_query_with_callback(
                     std::string const& query,
                     mysql_callback fn) {
  _push(misc::shared_ptr<mysql_task>(new mysql_task_run(query, fn)));
}

void mysql_thread::finish() {
  std::cout << "mysql_thread finish" << std::endl;
  _push(misc::shared_ptr<mysql_task>(new mysql_task_finish()));
}
