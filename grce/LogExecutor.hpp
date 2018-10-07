/*
  The LogExecutor class (header file).

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

#ifndef grce_LogExecutor_hpp
#define grce_LogExecutor_hpp

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_File_Chooser.H>
#include "zthread/Thread.h"
#include "zthread/Runnable.h"
#include <grctk/algo/StringLog.hpp>

namespace grce
{

class LogExecutor : public Fl_Window, public grctk::StringLog
{
public:
  enum ModalResult {MR_CANCEL = 0, MR_OK = 1};
protected:
  Fl_Return_Button* btn_ok;
  Fl_Button* btn_cancel;
  Fl_Button* btn_save_log;

  Fl_Text_Display* output_log;
  Fl_Text_Display* output_err;
  Fl_Text_Buffer log_buffer;
  Fl_Text_Buffer err_buffer;

  // bool logging;
  bool canceled;

  ModalResult modalresult;

  ZThread::Runnable *runnable;
  ZThread::Thread* thread;

  void Cancel();
  void Finish();

  int handle(int);
  static void btn_ok_cb(Fl_Widget *, void *);
  static void btn_cancel_cb(Fl_Widget *, void *);
  static void btn_save_log_cb(Fl_Widget *, void *);
  static void window_cb(Fl_Widget *, void *);
  void ErrOutL(const char *ErrMsg);
  void LogOutL(const char *logMsg);
public:
  LogExecutor(const std::string& title = "Progress");
  ~LogExecutor();
  ModalResult modalResult()const;
  bool operator()() { return modalResult() == MR_OK; }

  virtual void startLogging(ZThread::Runnable*);
private:
  virtual void startLogging() {};
public:
  virtual void stopLogging();

  virtual void flushStreams();
};

}

#endif

