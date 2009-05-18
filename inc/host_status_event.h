/*
** host_status_event.h for CentreonBroker in ./inc
** 
** Made by Matthieu Kermagoret <mkermagoret@merethis.com>
** 
** Copyright Merethis
** See LICENSE file for details.
** 
** Started on  05/07/09 Matthieu Kermagoret
** Last update 05/18/09 Matthieu Kermagoret
*/

#ifndef HOST_STATUS_EVENT_H_
# define HOST_STATUS_EVENT_H_

# include <string>
# include "host_service_status_event.h"

namespace              CentreonBroker
{
  class                EventSubscriber;

  /**
   *  The HostStatusEvent represents the corresponding event generated by
   *  Nagios. It has all fields specified in the NDO database schema.
   */
  class                HostStatusEvent : public HostServiceStatusEvent
  {
   private:
    enum               String
    {
      HOST = 0,
      STRING_NB
    };
    enum               TimeT
    {
      LAST_TIME_DOWN = 0,
      LAST_TIME_UNREACHABLE,
      LAST_TIME_UP,
      TIMET_NB
    };
    std::string        strings_[STRING_NB];
    time_t             timets_[TIMET_NB];
    void               InternalCopy(const HostStatusEvent& hse);

   public:
                       HostStatusEvent();
                       HostStatusEvent(const HostStatusEvent& hse);
                       ~HostStatusEvent();
    HostStatusEvent&   operator=(const HostStatusEvent& hse);
    // Getters
    int                GetType() const throw ();
    const std::string& GetHost() const throw ();
    time_t             GetLastTimeUp() const throw ();
    time_t             GetLastTimeDown() const throw ();
    time_t             GetLastTimeUnreachable() const throw ();
    // Setters
    void               SetHost(const std::string& host);
    void               SetLastTimeUp(time_t ltu) throw ();
    void               SetLastTimeDown(time_t ltd) throw ();
    void               SetLastTimeUnreachable(time_t ltu) throw ();
  };
}

#endif /* !HOST_STATUS_EVENT_H_ */
