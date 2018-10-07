/*
  Main source file of GRCE.

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

#include <grce/GRCE.hpp>
#include <grce/Saver.hpp>
#include <yaatk/yaatk.hpp>
#include <FL/Fl.H>
#include <zthread/Thread.h>
#include <exception>

void emergency_save_and_quit()
{
  fl_alert("GRCE has crashed. You will be presented with the emergency backup dialog");

  grce::MainWindow* MainWindow_Ptr;
  MainWindow_Ptr = grce::grce->mainWindow;

  grce::Saver saver(grctk::Universe::singleton(),
                    MainWindow_Ptr->docControl->listItems(),
                    MainWindow_Ptr->docControl->universeName()+"-emergency-backup-"
                    +yaatk::getDateTime_YYYYMMDD_HHMMSS(),
                    MainWindow_Ptr->docControl->universeDirectory());
  saver.show();
  while (saver.shown())
    Fl::wait();
  // if (!saver())
  //   return;

  MainWindow_Ptr->quit();
}

int main(int argc, char **argv)
{
  grce::grce = new grce::GRCE;
  Fl::lock();
  try
  {
    return Fl::run();
  }
  catch(ZThread::Synchronization_Exception& e)
  {
    std::cerr << "Synchronization_Exception in the main thread: "
              << e.what() << std::endl;
    emergency_save_and_quit();
  }
  catch(std::exception& e)
  {
    std::cerr << "Exception in the main thread: "
              << e.what() << std::endl;
    emergency_save_and_quit();
  }
  catch(...)
  {
    std::cerr << "Unknown exception in the main thread.";
    emergency_save_and_quit();
  }

}
