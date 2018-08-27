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
#include "com/centreon/broker/storage/mysql.hh"

using namespace com::centreon::broker;
using namespace com::centreon::broker::storage;

/**
 *  Constructor.
 *
 *  @param[in] db_cfg  Database configuration.
 */
mysql::mysql(database_config const& db_cfg)
  : _db_cfg(db_cfg),
    _version(database::v3),
    _current_thread(0),
    _prepare_count(0) {
  std::cout << "mysql constructor" << std::endl;
  if (mysql_library_init(0, NULL, NULL))
    throw exceptions::msg()
      << "storage: unable to initialize the MySQL connector";

  for (int i(0); i < db_cfg.get_connections_count(); ++i) {
    std::cout << "mysql constructor thread " << i << " construction" << std::endl;
    _thread.push_back(new mysql_thread(
                            db_cfg.get_host(),
                            db_cfg.get_user(),
                            db_cfg.get_password(),
                            db_cfg.get_name(),
                            db_cfg.get_port()));
  }
  std::cout << "mysql constructor return" << std::endl;
}

mysql::~mysql() {
  std::cout << "mysql destructor" << std::endl;
  bool retval(finish());
  if (!retval)
    logging::error(logging::medium)
      << "storage: A thread was forced to stop after a timeout of 20s";
  std::cout << "mysql destructor return" << std::endl;
}

void mysql::run_query(std::string const& query, int thread) {
  if (thread < 0) {
    // Here, we use _current_thread
    thread = _current_thread++;
    if (_current_thread >= _thread.size())
      _current_thread = 0;
  }
  _thread[thread]->run_query(query);
}

int mysql::run_query_sync(std::string const& query, int thread) {
  if (thread < 0) {
    // Here, we use _current_thread
    thread = _current_thread++;
    if (_current_thread >= _thread.size())
      _current_thread = 0;
  }
  _thread[thread]->run_query_sync(query);
  return thread;
}

misc::shared_ptr<mysql_result> mysql::get_result(int thread_id) {
  return _thread[thread_id]->get_result();
}

void mysql::run_statement(int statement_id, mysql_bind const& bind, int thread) {
  if (thread < 0) {
    // Here, we use _current_thread
    thread = _current_thread++;
    if (_current_thread >= _thread.size())
      _current_thread = 0;
  }
  _thread[thread]->run_statement(statement_id, bind);
}

void mysql::run_query_with_callback(std::string const& query,
              mysql_callback fn, int thread) {
  if (thread < 0) {
    // Here, we use _current_thread
    thread = _current_thread++;
    if (_current_thread >= _thread.size())
      _current_thread = 0;
  }
  _thread[thread]->run_query_with_callback(query, fn);
}

int mysql::prepare_query(std::string const& query) {
  for (std::vector<misc::shared_ptr<mysql_thread> >::const_iterator
         it(_thread.begin()),
         end(_thread.end());
       it != end;
       ++it) {
    (*it)->prepare_query(query);
  }
  return _prepare_count++;
}

bool mysql::finish() {
  bool retval(true);
  for (std::vector<misc::shared_ptr<mysql_thread> >::const_iterator
         it(_thread.begin()),
         end(_thread.end());
       it != end;
       ++it) {
    std::cout << "mysql destructor send finish to thread" << std::endl;
    (*it)->finish();
    std::cout << "mysql destructor wait for thread to finish" << std::endl;
    retval &= (*it)->wait(20000);
  }
  return retval;
}
