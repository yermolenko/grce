/*
  The AlgBase and the AbortAlgException classes.

  Copyright (C) 2001, 2002, 2003, 2004, 2015, 2017, 2018 Alexander
  Yermolenko <yaa.mbox@gmail.com>

  This file is part of GRCE, the Graph Research and Computing Environment.

  GRCE is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  GRCE is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with GRCE.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef grctk_AlgBase_hpp
#define grctk_AlgBase_hpp

#include "Loggable.hpp"
#include <yaatk/yaatk.hpp>
#include "zthread/Thread.h"
#include <string>
#include <iostream>

namespace grctk
{

class AbortAlgException
{
  std::string _msg;
public:
  AbortAlgException() : _msg("Abort Algorithm exception") { }
  AbortAlgException(const char* msg) : _msg(msg) { }
  const char* what() const { return _msg.c_str(); }
};

inline
void checkAborted()
{
  if (ZThread::Thread::interrupted())
    throw grctk::AbortAlgException("Exiting via the flag...");
}

class AlgBase : public Loggable
{
public:
  AlgBase(Log& setlog = nullLog):Loggable(setlog) {}
  virtual ~AlgBase() {}
  std::ostream& errStream() { return log.errStream(); }
  std::ostream& logStream() { return log.logStream(); }
  void flushLogStreams() { log.flushStreams(); }
};

} //namespace grctk

#endif
