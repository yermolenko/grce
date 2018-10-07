/*
  The Saver class (header file).

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

#ifndef grce_Saver_hpp
#define grce_Saver_hpp

#include "grce/DocControl.hpp"
#include "grce/ParamsDialog.hpp"
#include <FL/Fl_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_File_Input.H>
#include <grctk/AdjMatrix.hpp>
#include <grctk/Universe.hpp>
#include <yaatk/yaatk.hpp>

namespace grce
{

class Saver : public Fl_Window
{
  Fl_Return_Button* btn_ok;
  Fl_Button* btn_defaults;
  Fl_Button* btn_cancel;

  std::string defaultPrefix;
  std::string defaultDir;

  std::vector<CheckboxParam*> graphs;
  std::vector<DocWindow*> graphItems;
  std::vector<CheckboxParam*> attrs;
  std::vector<grctk::GenericAttribute*> attrItems;

  Fl_Input* inp_universe_id;
  Fl_File_Input* inp_dir;
  Fl_Button* btn_dir_browse;
public:
  enum ModalResult {MR_CANCEL = 0, MR_OK = 1};
private:
  ModalResult modalresult;
protected:
  Fl_Tabs* tabs;
public:
  ModalResult modalResult() const { return modalresult; }
  bool operator()() { return modalResult() == MR_OK /*&& valuesAreCorrect()*/; }
  const std::string universeName() const { return inp_universe_id->value(); }
  const std::string universeDirectory() const { return inp_dir->value(); }
  Saver(const grctk::Universe&,
        const std::vector<DocWindow*>&,
        const std::string& prefix = "univ00",
        const std::string& dir = yaatk::getcwd());
  virtual ~Saver();
  static void btn_ok_cb(Fl_Widget *, void *);
  static void btn_cancel_cb(Fl_Widget *, void *);
  static void btn_defaults_cb(Fl_Widget *, void *);

  static void btn_dir_browse_cb(Fl_Widget *, void *);

  virtual void loadDefaults();
};

}

#endif
