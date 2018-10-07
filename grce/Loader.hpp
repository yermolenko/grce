/*
  The Loader class (header file).

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

#ifndef grce_Loader_hpp
#define grce_Loader_hpp

#include "grce/DocControl.hpp"
#include "grce/ParamsDialog.hpp"
#include <grctk/AdjMatrix.hpp>
#include <grctk/Universe.hpp>
#include <grctk/algo/formats/Environment.hpp>

namespace grce
{

class Loader : public Fl_Window
{
  Fl_Return_Button* btn_ok;
  Fl_Button* btn_reset_choices;
  Fl_Button* btn_cancel;

  grctk::Universe& universe;
  DocControl* docControl;

  std::vector<std::string> universeIds;

  std::vector<CheckboxParam*> graphs;
  std::vector<grctk::GraphInfo> graphItems;
  Fl_Scroll* graphs_scroll_group;

  std::vector<CheckboxParam*> attrs;
  std::vector<grctk::GenericAttribute*> attrItems;
  Fl_Scroll* attrs_scroll_group;

  Fl_File_Input* inp_dir;
  Fl_Button* btn_dir_browse;
  Fl_Choice* inp_universe_id;
public:
  enum ModalResult {MR_CANCEL = 0, MR_OK = 1};
private:
  ModalResult modalresult;
protected:
  Fl_Tabs* tabs;
public:
  ModalResult modalResult() const { return modalresult; }
  bool operator()() { return modalResult() == MR_OK /*&& valuesAreCorrect()*/; }
  Loader(grctk::Universe&, DocControl*);
  virtual ~Loader();
  static void btn_ok_cb(Fl_Widget *, void *);
  static void btn_cancel_cb(Fl_Widget *, void *);

  static void btn_reset_choices_cb(Fl_Widget *, void *);
  virtual void resetChoices();

  static void inp_dir_cb(Fl_Widget *, void *);
  static void btn_dir_browse_cb(Fl_Widget *, void *);
  void updateSavedUniversesList();

  static void inp_universe_cb(Fl_Widget *, void *);
  void updateGraphAndAttrLists();
};

}

#endif
