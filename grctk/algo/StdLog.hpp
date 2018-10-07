/*
  The StdLog class (header file).

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

#ifndef grctk_StdLog_hpp
#define grctk_StdLog_hpp

#include "Log.hpp"

namespace grctk
{

class StdLog : public Log
{
protected:
  virtual void errout(const char *ErrMsg);
  virtual void logout(const char *logMsg);
public:
  virtual void startLogging();
  virtual void stopLogging();
  virtual std::ostream& errStream() {errors = true; return std::cerr;};
  virtual std::ostream& logStream() {return std::cout;};
  virtual void flushStreams() {std::cerr.flush();std::cout.flush();};
  StdLog():Log(){;};
  virtual ~StdLog(){;};
};

extern StdLog stdLog;

}

#endif
