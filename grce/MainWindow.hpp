/*
  The MainWindow class (header file).

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

#ifndef grce_MainWindow_hpp
#define grce_MainWindow_hpp

#include "grce/DocControl.hpp"
#include "grce/DocWindow.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>

namespace grce
{

class MainWindow : public Fl_Window
{
  static Fl_Menu_Item menuitems[];
  Fl_Menu_Bar menubar;
  static void new_graph_cb(Fl_Widget *w, void *);
  static void load_env_cb(Fl_Widget *, void *);
  static void append_env_cb(Fl_Widget *, void *);
  static void save_env_cb(Fl_Widget *, void *);
  static void save_env_as_cb(Fl_Widget *, void *);
  static void import_cb(Fl_Widget *, void *);
  static void import_bic_cb(Fl_Widget *, void *);
  static void quit_cb(Fl_Widget*, void*);

  template <typename ProductAlg>
  static void product_cb(Fl_Widget *, void *);
  static void iso_CMR_cb(Fl_Widget *, void *);
  static void iso_opti_cb(Fl_Widget *, void *);
  static void gen_complete_cb(Fl_Widget *, void *);
  static void gen_random_cb(Fl_Widget *, void *);

  static void about_cb(Fl_Widget *, void *);
  // static void help_cb(Fl_Widget *, void *);
  // static void manual_cb(Fl_Widget *, void *);

  static void menu_placeholder_test_cb(Fl_Widget*, void*);
public:
  DocControl* docControl;
  int handle(int);
  void quit();
  MainWindow(const char *title = 0);
  ~MainWindow();
};

}

#endif
