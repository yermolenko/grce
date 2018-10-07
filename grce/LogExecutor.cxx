/*
  The LogExecutor class.

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

#include "LogExecutor.hpp"
#include <yaatk/yaatk.hpp>
#include <fstream>
#include <iostream>

namespace grce
{

LogExecutor::LogExecutor(const std::string& title)
  :Fl_Window(Fl::w()/2-300,Fl::h()/2-200,600,400,""),
   StringLog(),
   log_buffer(),err_buffer(),
   // logging(true),
   canceled(false),
   modalresult(MR_CANCEL),
   runnable(NULL)
{
  // loggable->setLog(this);

  copy_label(title.c_str());
  set_modal();

  btn_cancel = new Fl_Button(5,355,110,40,"Cancel");
  btn_save_log = new Fl_Button(w()-115,btn_cancel->y(),110,40,"Save Logs...");
  btn_ok = new Fl_Return_Button(
    btn_cancel->x()+btn_cancel->w()+10,btn_cancel->y(),
    btn_save_log->x()-btn_cancel->x()-btn_cancel->w()-20,40,
    "OK");
  output_log = new Fl_Text_Display(5,20,w()-10,165,"Events Log");
  output_log->textcolor(FL_BLUE);
  output_log->align(FL_ALIGN_TOP);
  output_log->buffer(log_buffer);
  output_err = new Fl_Text_Display(5,205,w()-10,145,"Errors Log");
  output_err->textcolor(FL_RED);
  output_err->align(FL_ALIGN_TOP);
  output_err->buffer(err_buffer);
  end();
  btn_ok->callback((Fl_Callback*)btn_ok_cb);
  btn_cancel->callback((Fl_Callback*)btn_cancel_cb);
  btn_save_log->callback((Fl_Callback*)btn_save_log_cb);

  callback(window_cb);
}

LogExecutor::~LogExecutor()
{
  output_log->buffer(NULL);
  output_err->buffer(NULL);
}

LogExecutor::ModalResult
LogExecutor::modalResult()const
{
  return modalresult;
}

int
LogExecutor::handle(int e)
{
  switch(e)
  {
  default:
    return Fl_Window::handle(e);
  }
}

void
LogExecutor::btn_ok_cb(Fl_Widget *w, void *)
{
  LogExecutor*  LogExecutor_Ptr;
  LogExecutor_Ptr = (LogExecutor*)(w->parent());

  LogExecutor_Ptr->Finish();
}

void
LogExecutor::btn_cancel_cb(Fl_Widget *w, void *)
{
  LogExecutor*  LogExecutor_Ptr;
  LogExecutor_Ptr = (LogExecutor*)(w->parent());

  LogExecutor_Ptr->Cancel();
}

void
LogExecutor::btn_save_log_cb(Fl_Widget *w, void *)
{
  LogExecutor* log_wnd = (LogExecutor*)(w->parent());

  {
    char *filename = fl_file_chooser(
      "Choose a file to save log messages to ...",
      "Text files (*.txt)",
      (yaatk::getDateTime_YYYYMMDD_HHMMSS() + "-messages.txt").c_str(),
      0);
    if (filename && (!fl_filename_isdir(filename)))
      if (log_wnd->log_buffer.savefile(filename))
        fl_alert("An error ocurred when saving to %s",filename);
  }

  {
    char *filename = fl_file_chooser(
      "Choose a file to save errors log to ...",
      "Text files (*.txt)",
      (yaatk::getDateTime_YYYYMMDD_HHMMSS() + "-errors.txt").c_str(),
      0);
    if (filename && (!fl_filename_isdir(filename)))
      if (log_wnd->err_buffer.savefile(filename))
        fl_alert("An error ocurred when saving to %s",filename);
  }
}

void
LogExecutor::ErrOutL(const char *ErrMsg)
{
  int dlen = strlen(ErrMsg);
  if (dlen > 0)
    errors = true;

  output_err->insert_position(err_buffer.length());
  output_err->insert(ErrMsg);
  output_err->show_insert_position();
}

void
LogExecutor::LogOutL(const char *LogMsg)
{
  output_log->insert_position(log_buffer.length());
  output_log->insert(LogMsg);
  output_log->show_insert_position();
}

void
LogExecutor::startLogging(ZThread::Runnable* loggable)
{
  StringLog::startLogging();
  // loggable->setLog(this);

  Fl::lock();

  log_buffer.remove(0, log_buffer.length());
  err_buffer.remove(0, err_buffer.length());

  this->set_modal();
  this->show();
  this->redraw();
  canceled = false;
  btn_cancel->activate();
  btn_cancel->label("Cancel");
  btn_ok->deactivate();
  btn_save_log->deactivate();
  modalresult = MR_CANCEL;
//  Fl::wait();
  // logging = true;

  thread = new ZThread::Thread(loggable);

  Fl::unlock();
  Fl::awake((void*)0);
}

void
LogExecutor::stopLogging()
{
  StringLog::stopLogging();

  Fl::lock();

  // btn_cancel->deactivate();
  btn_cancel->label("Discard");
  btn_ok->activate();
  btn_save_log->activate();
//  Fl::wait();
  // logging = false;

  Fl::unlock();
  Fl::awake((void*)0);
}

void
LogExecutor::Cancel()
{
  if (!canceled)
  {
    if (btn_ok->active())
      errout("Discarding results....\n");
    else
    {
      errout("Trying to interrupt....\n");
      thread->interrupt();
    }
    btn_cancel->deactivate();
    canceled = true;
  }
}

void
LogExecutor::Finish()
{
  if (!canceled && !errors)
    modalresult = MR_OK;
  else
    modalresult = MR_CANCEL;
  hide();
}

void
LogExecutor::flushStreams()
{
  Fl::lock();

  LogOutL(osLog.str().c_str());
  ErrOutL(osErr.str().c_str());
  osErr.str("");osLog.str("");
//  Fl::check();

  Fl::unlock();
  Fl::awake((void*)0);
}

void
LogExecutor::window_cb(Fl_Widget*, void*)
{
}

}
