/*
  The DocToolbox class (header file).

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

#ifndef grce_DocToolbox_hpp
#define grce_DocToolbox_hpp

#include <FL/Fl.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

namespace grce
{

class DocWindow;

enum GRCE_EDIT_MODE
{
  GRCE_EDIT_MOVE_VERTEX = 0,
  GRCE_EDIT_CREATE_VERTEX,
  GRCE_EDIT_REMOVE_VERTEX,
  GRCE_EDIT_CREATE_EDGE,
  GRCE_EDIT_REMOVE_EDGE,
  GRCE_EDIT_VERTEX_PROPERTIES,
  GRCE_EDIT_EDGE_PROPERTIES,
  GRCE_EDIT_ADD_VERTEX_TO_SELECTION,
  GRCE_EDIT_ADD_EDGE_TO_SELECTION
};

class SButton : public Fl_Button
{
  const char *hint;
  Fl_Widget *statusbar;
public:
  SButton(int x, int y, int w, int h, const char *l=0);
  void set_hint(Fl_Widget *sb, const char *h);
  int handle(int event);
};

class DocToolbox : public Fl_Window
{
  GRCE_EDIT_MODE grce_edit_mode_;

  DocWindow* doc_parent;
  static int w0,h0;
  static const char* default_sb_hint;
  Fl_Box        *statusbar;

  Fl_Group* btn_group;

  SButton
  *btn_move_vertex,
    *btn_create_vertex,
    *btn_remove_vertex,
    *btn_create_edge,
    *btn_remove_edge,
    *btn_vertex_properties,
    *btn_edge_properties,
    *btn_add_vertex_to_selection,
    *btn_add_edge_to_selection;
  static void btn_move_vertex_cb(Fl_Widget *, void *);
  static void btn_create_vertex_cb(Fl_Widget *, void *);
  static void btn_remove_vertex_cb(Fl_Widget *, void *);
  static void btn_create_edge_cb(Fl_Widget *, void *);
  static void btn_remove_edge_cb(Fl_Widget *, void *);
  static void btn_vertex_properties_cb(Fl_Widget *, void *);
  static void btn_edge_properties_cb(Fl_Widget *, void *);
  static void btn_add_vertex_to_selection_cb(Fl_Widget *, void *);
  static void btn_add_edge_to_selection_cb(Fl_Widget *, void *);
  void on_grce_edit_mode(GRCE_EDIT_MODE);
public:
  void onset(GRCE_EDIT_MODE);
  GRCE_EDIT_MODE grce_edit_mode();
  int  handle(int);
  DocToolbox(DocWindow*);
  ~DocToolbox();
  void update_label();
  void setDocWindow(DocWindow*);
};

}

#endif
