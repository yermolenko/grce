/*
  The PropEdit class (header file).

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

#ifndef grce_PropEdit_hpp
#define grce_PropEdit_hpp

#include "VPropEdit.hpp"
#include "EPropEdit.hpp"
#include "grce/Wizard.hpp"
#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Window.H>
#include <FL/fl_ask.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Check_Button.H>

namespace grce
{

class PropEdit : public Fl_Window
{
  const std::set<grctk::Object> vSet;
  const std::set<grctk::Object> eSet;

  Fl_Return_Button *btn_ok;
  Fl_Button *btn_cancel;

  Fl_Check_Button *checkbtn_altered_only;

  VPropEdit *vpropedit;
  EPropEdit *epropedit;
  Fl_Tabs *tabs;

  static void btn_ok_cb(Fl_Widget *, void *);
  static void btn_cancel_cb(Fl_Widget *, void *);

  enum ModalResult {MR_NO = -1,MR_CANCEL = 0,MR_OK = 1};
  ModalResult modalresult;
public:
  ModalResult modalResult() const;
  int handle(int);
  PropEdit(std::set<grctk::Object> vertexSet,
           std::set<grctk::Object> edgeSet);
  ~PropEdit();
};

}

#endif
