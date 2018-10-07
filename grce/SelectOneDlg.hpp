/*
  The SelectOneDlg class (header file).

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

#ifndef grce_SelectOneDlg_hpp
#define grce_SelectOneDlg_hpp

#include "grce/Wizard.hpp"
#include "FL/Fl_Window.H"
#include "FL/Fl_Hold_Browser.H"
#include "FL/Fl_Return_Button.H"
#include <vector>
#include <string>

namespace grce
{

class SelectDlg : public Fl_Window
{
public:
  enum ModalResult {MR_CANCEL = 0, MR_OK = 1};
private:
  Fl_Return_Button *btn_ok;
  Fl_Button *btn_cancel;

  Fl_Hold_Browser *browser;

  static void btn_ok_cb(Fl_Widget *, void *);
  static void btn_cancel_cb(Fl_Widget *, void *);

  ModalResult modalresult;
public:
  ModalResult modalResult() const;
  int handle(int);

  int value();

  SelectDlg(std::vector<std::string> &);
  ~SelectDlg();
};

}

#endif
