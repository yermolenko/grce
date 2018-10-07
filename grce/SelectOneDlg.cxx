/*
  The SelectOneDlg class.

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

#include "SelectOneDlg.hpp"
#include <string>
#include <FL/fl_ask.H>

namespace grce
{

int
SelectDlg::value()
{
  return browser->value();
}

int
SelectDlg::handle(int e)
{
  switch(e)
  {
  default:
    return Fl_Window::handle(e);
  }
}

SelectDlg::SelectDlg(std::vector<std::string> &v):
  Fl_Window(400,370,"Select one graph"),
  modalresult(MR_OK)
{
  set_modal();

  btn_ok = new Fl_Return_Button(50,320,100,30,"Ok");
  btn_ok->callback((Fl_Callback*)btn_ok_cb);

  btn_cancel = new Fl_Button(250,320,100,30,"Cancel");
  btn_cancel->callback((Fl_Callback*)btn_cancel_cb);

  browser = new Fl_Hold_Browser(50,20,350,200);
  for(int i = 0; i < v.size(); i++)
  {
    const char *s = v[i].c_str();
    browser->add(s);
  }

  end();
  show();
}

SelectDlg::~SelectDlg()
{
}

void
SelectDlg::btn_ok_cb(Fl_Widget *w, void *)
{
  SelectDlg* SelectDlg_Ptr;
  SelectDlg_Ptr = (SelectDlg*)(w->parent());

  if (SelectDlg_Ptr->value() == 0)
  {
    fl_alert("You should select ONE graph!");
  }
  else
  {
    SelectDlg_Ptr->modalresult = MR_OK;
    SelectDlg_Ptr->hide();
  }
}

void
SelectDlg::btn_cancel_cb(Fl_Widget *w, void *)
{
  SelectDlg* SelectDlg_Ptr;
  SelectDlg_Ptr = (SelectDlg*)(w->parent());

  SelectDlg_Ptr->modalresult = MR_CANCEL;
  SelectDlg_Ptr->hide();
}

SelectDlg::ModalResult
SelectDlg::modalResult()const
{
  return modalresult;
}

}
