/*
**  Copyright 2009 MERETHIS
**  This file is part of CentreonBroker.
**
**  CentreonBroker is free software: you can redistribute it and/or modify it
**  under the terms of the GNU General Public License as published by the Free
**  Software Foundation, either version 2 of the License, or (at your option)
**  any later version.
**
**  CentreonBroker is distributed in the hope that it will be useful, but
**  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
**  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
**  for more details.
**
**  You should have received a copy of the GNU General Public License along
**  with CentreonBroker.  If not, see <http://www.gnu.org/licenses/>.
**
**  For more information : contact@centreon.com
*/

#include <cstring>
#include "events/downtime.h"

using namespace CentreonBroker::Events;

/**************************************
*                                     *
*           Private Methods           *
*                                     *
**************************************/

/**
 *  This internal method is used to copy data defined inside the Downtime class
 *  from an object to the current instance. This means that no superclass data
 *  are copied. This method is used in Downtime copy constructor and in the =
 *  operator overload.
 *
 *  \see Downtime(const Downtime&)
 *  \see operator=
 *
 *  \param[in] downtime Object to copy from.
 */
void Downtime::InternalCopy(const Downtime& downtime)
{
  memcpy(this->bools_, downtime.bools_, sizeof(this->bools_));
  memcpy(this->ints_, downtime.ints_, sizeof(this->ints_));
  memcpy(this->shorts_, downtime.shorts_, sizeof(this->shorts_));
  for (unsigned int i = 0; i < STRING_NB; i++)
    this->strings_[i] = downtime.strings_[i];
  memcpy(this->timets_, downtime.timets_, sizeof(this->timets_));
  return ;
}

/**************************************
*                                     *
*           Public Methods            *
*                                     *
**************************************/

/**
 *  Downtime default constructor. Set all members to their default value (0,
 *  NULL or equivalent).
 */
Downtime::Downtime()
{
  memset(this->bools_, 0, sizeof(this->bools_));
  memset(this->ints_, 0, sizeof(this->ints_));
  memset(this->shorts_, 0, sizeof(this->shorts_));
  memset(this->timets_, 0, sizeof(this->timets_));
}

/**
 *  Downtime copy constructor.
 *
 *  \param[in] downtime Object to copy from.
 */
Downtime::Downtime(const Downtime& downtime) : Event(downtime)
{
  this->InternalCopy(downtime);
}

/**
 *  Downtime destructor, release all acquired ressources.
 */
Downtime::~Downtime()
{
}

/**
 *  Overload of the = operator.
 *
 *  \param[in] downtime Object to copy from.
 *
 *  \return *this
 */
Downtime& Downtime::operator=(const Downtime& downtime)
{
  this->Event::operator=(downtime);
  this->InternalCopy(downtime);
  return (*this);
}

/**
 *  Get the name of the user who defined the downtime.
 *
 *  \return The name of the user who defined the downtime.
 */
const std::string& Downtime::GetAuthorName() const throw ()
{
  return (this->strings_[AUTHOR_NAME]);
}

/**
 *  Get the comment associated with the downtime.
 *
 *  \return The comment associated with the downtime.
 */
const std::string& Downtime::GetCommentData() const throw ()
{
  return (this->strings_[COMMENT_DATA]);
}

/**
 *  XXX : need fix
 */
int Downtime::GetDowntimeId() const throw ()
{
  return (this->ints_[DOWNTIME_ID]);
}

/**
 *  Get the type of the downtime (XXX : what are the available types ?).
 *
 *  \return The type of the downtime.
 */
short Downtime::GetDowntimeType() const throw ()
{
  return (this->shorts_[DOWNTIME_TYPE]);
}

/**
 *  Get the duration of the downtime.
 *
 *  \return The duration of the downtime.
 */
int Downtime::GetDuration() const throw ()
{
  return (this->ints_[DURATION]);
}

/**
 *  XXX : need fix
 */
time_t Downtime::GetEndTime() const throw ()
{
  return (this->timets_[END_TIME]);
}

/**
 *  XXX : need fix
 */
time_t Downtime::GetEntryTime() const throw ()
{
  return (this->timets_[ENTRY_TIME]);
}

/**
 *  Determines whether or not the downtime is fixed.
 *
 *  \return true if the downtime is fixed, false otherwise.
 */
bool Downtime::GetFixed() const throw ()
{
  return (this->bools_[FIXED]);
}

/**
 *  Get the name of the host associated with the downtime.
 *
 *  \return The name of the host associated with the downtime.
 */
const std::string& Downtime::GetHost() const throw ()
{
  return (this->strings_[HOST]);
}

/**
 *  XXX : need fix
 */
time_t Downtime::GetStartTime() const throw ()
{
  return (this->timets_[START_TIME]);
}

/**
 *  Get the name of the service associated with the downtime.
 *
 *  \return The name of the service associated with the downtime.
 */
const std::string& Downtime::GetService() const throw ()
{
  return (this->strings_[SERVICE]);
}

/**
 *  XXX : need fix
 */
int Downtime::GetTriggeredBy() const throw ()
{
  return (this->ints_[TRIGGERED_BY]);
}

/**
 *  Returns the type of this event (Event::DOWNTIME).
 *
 *  \return Event::DOWNTIME
 */
int Downtime::GetType() const throw ()
{
  return (Event::DOWNTIME);
}

/**
 *  Determines whether or not the downtime was cancelled.
 *
 *  \return true if the downtime was cancelled, false otherwise.
 */
bool Downtime::GetWasCancelled() const throw ()
{
  return (this->bools_[WAS_CANCELLED]);
}

/**
 *  Determines whether or not the downtime was started.
 *
 *  \return true if the downtime was started, false otherwise.
 */
bool Downtime::GetWasStarted() const throw ()
{
  return (this->bools_[WAS_STARTED]);
}

/**
 *  Set the name of the user who defined the downtime.
 *
 *  \see GetAuthorName
 *
 *  \param[in] an The name of the user who defined the downtime.
 */
void Downtime::SetAuthorName(const std::string& an)
{
  this->strings_[AUTHOR_NAME] = an;
  return ;
}

/**
 *  Set the comment associated with the downtime.
 *
 *  \see GetCommentData
 *
 *  \param[in] cd The comment associated with the downtime.
 */
void Downtime::SetCommentData(const std::string& cd)
{
  this->strings_[COMMENT_DATA] = cd;
  return ;
}

/**
 *  XXX : need fix
 *
 *  \see GetDowntimeId
 */
void Downtime::SetDowntimeId(int di) throw ()
{
  this->ints_[DOWNTIME_ID] = di;
  return ;
}

/**
 *  Set the type of the downtime (XXX : what are the available types ?).
 *
 *  \see GetDowntimeType
 *
 *  \param[in] dt The type of the downtime.
 */
void Downtime::SetDowntimeType(short dt) throw ()
{
  this->shorts_[DOWNTIME_TYPE] = dt;
  return ;
}

/**
 *  Set the duration of the downtime.
 *
 *  \see GetDuration
 *
 *  \param[in] d The duration of the downtime.
 */
void Downtime::SetDuration(int d) throw ()
{
  this->ints_[DURATION] = d;
  return ;
}

/**
 *  XXX : need fix
 *
 *  \see GetEndTime
 */
void Downtime::SetEndTime(time_t et) throw ()
{
  this->timets_[END_TIME] = et;
  return ;
}

/**
 *  XXX : need fix
 *
 *  \see GetEntryTime
 */
void Downtime::SetEntryTime(time_t et) throw ()
{
  this->timets_[ENTRY_TIME] = et;
  return ;
}

/**
 *  Set whether or not the downtime is fixed.
 *
 *  \see GetFixed
 *
 *  \param[in] f true if the downtime is fixed, false otherwise.
 */
void Downtime::SetFixed(bool f) throw ()
{
  this->bools_[FIXED] = f;
  return ;
}

/**
 *  Set the name of the host associated with the downtime.
 *
 *  \see GetHost
 *
 *  \param[in] h The name of the host associated with the downtime.
 */
void Downtime::SetHost(const std::string& h)
{
  this->strings_[HOST] = h;
  return ;
}

/**
 *  XXX : need fix
 *
 *  \see GetStartTime
 */
void Downtime::SetStartTime(time_t st) throw ()
{
  this->timets_[START_TIME] = st;
  return ;
}

/**
 *  Set the name of the service associated with the downtime.
 *
 *  \see GetService
 *
 *  \param[in] service The name of the service associated with the downtime.
 */
void Downtime::SetService(const std::string& service)
{
  this->strings_[SERVICE] = service;
  return ;
}

/**
 *  XXX : need fix
 *
 *  \see GetTriggeredBy
 */
void Downtime::SetTriggeredBy(int tb) throw ()
{
  this->ints_[TRIGGERED_BY] = tb;
  return ;
}

/**
 *  Set whether or not the downtime was cancelled.
 *
 *  \see GetWasCancelled
 *
 *  \param[in] wc true if the downtime was cancelled, false otherwise.
 */
void Downtime::SetWasCancelled(bool wc) throw ()
{
  this->bools_[WAS_CANCELLED] = wc;
  return ;
}

/**
 *  Set whether or not the downtime was started.
 *
 *  \see GetWasStarted
 *
 *  \param[in] ws true if the downtime was started, false otherwise.
 */
void Downtime::SetWasStarted(bool ws) throw ()
{
  this->bools_[WAS_STARTED] = ws;
  return ;
}
