/*
** Copyright 2011-2014 Centreon
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

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include "com/centreon/broker/config/applier/init.hh"
#include "com/centreon/broker/file/stream.hh"
#include "com/centreon/broker/io/raw.hh"

using namespace com::centreon::broker;

#define TEMP_FILE_NAME "broker_file_stream_write"

/**
 *  Check that file stream can be properly written to.
 *
 *  @param[in] argc Argument count.
 *  @param[in] argv Argument values.
 *
 *  @return 0 on success.
 */
int main(int argc, char* argv[]) {
  // Qt core object.
  QCoreApplication app(argc, argv);

  // Initialization.
  config::applier::init();

  // Generate file name.
  QString filename(QDir::tempPath());
  filename.append("/" TEMP_FILE_NAME);

  // Remove old file.
  QFile::remove(filename);

  // Generate data packet.
  std::shared_ptr<io::raw> data(new io::raw);
  data->append("0123456789abcdefghijklmnopqrstuvwxyz");

  {
    // Open file stream for writing.
    file::stream fs(filename.toStdString());

    // Write data in file.
    for (unsigned int i = 0; i < 10000; ++i)
      fs.write(data);
  }

  // Return value.
  int retval(0);

  // Open file to check.
  QFile f(filename);
  if (!f.open(QIODevice::ReadOnly))
    retval |= 1;

  // Skip header.
  {
    char header[8];
    unsigned int current(0);
    while (current != sizeof(header)) {
      f.waitForReadyRead(-1);
      current += f.read(header + current, sizeof(header - current));
    }
  }

  // Read and compare data.
  char buffer[36];
  unsigned int count(0);
  unsigned int current(0);
  while (!retval && (count < 10000)) {
    f.waitForReadyRead(-1);
    qint64 rb(f.read(buffer + current, sizeof(buffer) - current));
    if (rb <= 0)
      retval |= 1;
    else {
      current += rb;
      if (current == sizeof(buffer)) {
        retval |= memcmp(buffer, "0123456789abcdefghijklmnopqrstuvwxyz", sizeof(buffer));
        ++count;
        current = 0;
      }
    }
  }
  // EOF must be reached.
  f.waitForReadyRead(-1);
  retval |= (f.read(buffer, 1) > 0);

  // Remove temporary file.
  QFile::remove(filename);

  // Cleanup.
  config::applier::deinit();

  return (retval);
}
