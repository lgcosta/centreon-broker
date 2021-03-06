/*
** Copyright 2014 Centreon
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

#ifndef CCB_BAM_TIMEPERIOD_MAP_HH
#  define CCB_BAM_TIMEPERIOD_MAP_HH

#  include <vector>
#  include <map>
#  include <memory>
#  include "com/centreon/broker/namespace.hh"
#  include "com/centreon/broker/time/timeperiod.hh"

CCB_BEGIN()

namespace          bam {
  /**
   *  @class timeperiod_map timeperiod_map.hh "com/centreon/broker/bam/timeperiod_map.hh"
   *  @brief Timeperiod map made to be shared between threads.
   */
  class            timeperiod_map {
  public:
                   timeperiod_map();
                   ~timeperiod_map();
                   timeperiod_map(timeperiod_map const&);
    timeperiod_map&
                   operator=(timeperiod_map const&);
    bool           operator==(timeperiod_map const& other) const;

    time::timeperiod::ptr
                   get_timeperiod(unsigned int id) const;
    void           add_timeperiod(
                     unsigned int id,
                     time::timeperiod::ptr ptr);
    void           clear();
    void           add_relation(
                     unsigned int ba_id,
                     unsigned int timeperiod_id,
                     bool is_default);
    std::vector<std::pair<time::timeperiod::ptr, bool> >
                    get_timeperiods_by_ba_id(
                      unsigned int ba_id) const;

  private:
    std::map<unsigned int, time::timeperiod::ptr>
                   _map;
    typedef std::multimap<unsigned int,
                          std::pair<unsigned int, bool> >
                  timeperiod_relation_map;
    timeperiod_relation_map
                  _timeperiod_relations;
  };
}

CCB_END()

#endif // !CCB_BAM_TIMEPERIOD_MAP_HH
