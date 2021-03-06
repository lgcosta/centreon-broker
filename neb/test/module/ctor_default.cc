/*
** Copyright 2012-2013 Centreon
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

#include <cstdlib>
#include "com/centreon/broker/io/events.hh"
#include "com/centreon/broker/neb/internal.hh"
#include "com/centreon/broker/neb/module.hh"

using namespace com::centreon::broker;

/**
 *  Check module's default constructor.
 *
 *  @return EXIT_SUCCESS on success.
 */
int main() {
  // Object.
  neb::module mod;

  // Check.
  return (((mod.source_id != 0)
           || (mod.destination_id != 0)
           || (mod.args != "")
           || (mod.enabled != true)
           || (mod.filename != "")
           || (mod.loaded != false)
           || (mod.should_be_loaded != false)
           || (mod.type()
               != io::events::data_type<
                                io::events::neb,
                                neb::de_module>::value))
          ? EXIT_FAILURE
          : EXIT_SUCCESS);
}
