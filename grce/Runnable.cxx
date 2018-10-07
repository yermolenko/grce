/*
  The Runnable class.

  Copyright (C) 2001, 2002, 2003, 2004, 2011, 2015, 2017, 2018
  Alexander Yermolenko <yaa.mbox@gmail.com>

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

#include "Runnable.hpp"
#include "zthread/Thread.h"
#include <grctk/algo/AlgBase.hpp>

namespace grce
{

Runnable::Runnable(LogExecutor& setLog):
  ZThread::Runnable(),
  exitedCorrectly(false),
  paused(false),
  canceled(false),
  logger(setLog)
{
}

Runnable::~Runnable()
{
}

void
Runnable::run()
{
  try
  {
    execute();
    exitedCorrectly = true;
  }
  catch (ZThread::Interrupted_Exception&)
  {
    exitedCorrectly = false;
    logger.errStream() << "Exiting via exception from a blocking operation\n";
    logger.flushStreams();
  }
  catch (grctk::AbortAlgException& aae)
  {
    exitedCorrectly = false;
    logger.errStream() << aae.what() << "\n";
    logger.flushStreams();
  }
  catch (std::exception& e)
  {
    exitedCorrectly = false;
    logger.errStream() << e.what() << "\n";
    logger.flushStreams();
  }
  catch (...)
  {
    exitedCorrectly = false;
    logger.errStream() << "Unknown exception. Save your work and restart the application\n";
    logger.flushStreams();
  }

  logger.stopLogging();

  // logger.logStream() << "Ready to return the result." << "\n";
  // logger.flushStreams();

  while(isPaused())
    ZThread::Thread::sleep(10);

  // logger.logStream() << "Waiting to be destroyed" << "\n";
  // logger.flushStreams();

  while(!isCanceled())
    ZThread::Thread::sleep(10);

  // logger.logStream() << "Destroying myself" << "\n";
  // logger.flushStreams();
}

}
