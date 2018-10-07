/*
  The DocWindow class (header file).

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

#ifndef grce_DocWindow_hpp
#define grce_DocWindow_hpp

#include "grce/DocToolbox.hpp"
#include "grce/GraphEdit.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/filename.H>
#include <FL/Fl_Output.H>

namespace grce
{

class MainWindow;
class DocControlItem;
class DocControl;

class DocWindow : public Fl_Double_Window
{
  Fl_Menu_Bar* menubar;
  GraphEdit* edit_box;
  Fl_Box* statusbar;

  DocToolbox* doc_toolbox;

  DocControlItem* control_item;
  DocControl* docControl;
  void close();

  std::string idstr;
  std::string desc;
  void updateLabel();
public:
  const std::string idString() const { return idstr; }
  void idString(const std::string& idStr);
  const std::string description() const { return desc; }
  void description(const std::string& descr);

  GraphEdit* EditBox() const {return edit_box;};

  void on_edit_mode_set();
  void edit_mode_set(GRCE_EDIT_MODE);
  static Fl_Menu_Item menutable[];

  static void mnu_visualize_cb(Fl_Widget *, void *);
  static void mnu_duplicate_cb(Fl_Widget *, void *);
  static void mnu_save_cb(Fl_Widget *, void *);
  static void mnu_rename_cb(Fl_Widget *, void *);
  static void close_cb(Fl_Widget *, void *);
  static void test_cb(Fl_Widget *, void *);

  static void mnu_edit_mode_cb(Fl_Widget *, void *);

  static void mnu_select_all_cb(Fl_Widget *, void *);
  static void mnu_deselect_all_cb(Fl_Widget *, void *);
  static void mnu_select_all_vertexes_cb(Fl_Widget *, void *);
  static void mnu_label_vertexes_cb(Fl_Widget *, void *);
  static void mnu_deselect_all_vertexes_cb(Fl_Widget *, void *);
  static void mnu_select_all_edges_cb(Fl_Widget *, void *);
  static void mnu_deselect_all_edges_cb(Fl_Widget *, void *);

  static void mnu_reset_all_vertex_weights_cb(Fl_Widget *, void *);
  static void mnu_reset_all_edge_weights_cb(Fl_Widget *, void *);
  static void mnu_reset_all_weights_cb(Fl_Widget *, void *);

  static void mnu_open_selection_cb(Fl_Widget *, void *);
  static void mnu_duplicate_selection_cb(Fl_Widget *, void *);
  static void mnu_edit_selection_prop_cb(Fl_Widget *, void *);

  static void mnu_toolbox_cb(Fl_Widget *, void *);
  static void mnu_mainwindow_cb(Fl_Widget *, void *);

  static void mnu_refresh_cb(Fl_Widget *, void *);
  static void mnu_scale_to_window_cb(Fl_Widget *, void *);

  static void mnu_basic_properties_cb(Fl_Widget *, void *);
  static void mnu_concomp_cb(Fl_Widget *, void *);
  template <typename FindOrbitsAlg, typename OrbitsAttribute>
  void findOrbits();
  template <typename FindOrbitsAlg, typename OrbitsAttribute>
  static void mnu_find_orbits_cb(Fl_Widget *w, void *);
  static void mnu_cmp_find_orbits_cb(Fl_Widget *, void *);
  static void mnu_optimize_expsimp_cb(Fl_Widget *, void *);
  static void mnu_opti_intersect_cb(Fl_Widget *, void *);
  template <typename ProductAlg>
  static void mnu_product_cb(Fl_Widget *, void *);
  static void mnu_iso_CMR_cb(Fl_Widget *, void *);

  // static void help_cb(Fl_Widget *, void *);
  // static void manual_cb(Fl_Widget *, void *);

  static DocWindow* activeDocWindow();

  int  handle(int);
  DocWindow(int,int,int,int, const std::string idStr, DocControl*);
  void LoadFromGraph(const grctk::AdjMatrix&);
  ~DocWindow();

  friend class DocControlItem;
};

}

#endif
