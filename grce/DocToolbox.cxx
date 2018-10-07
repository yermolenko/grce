/*
  The DocToolbox class.

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

#include "DocToolbox.hpp"
#include "grce/DocWindow.hpp"
#include "toolbox/toolbox_artwork.h"

namespace grce
{

SButton::SButton(int x, int y, int w, int h, const char *l)
  : Fl_Button(x, y, w, h, l)
{
  statusbar = 0; hint = 0;
}

void SButton::set_hint(Fl_Widget *sb, const char *h)
{
  statusbar = sb; hint = h;
}

int SButton::handle(int event)
{
  switch (event)
  {
  case FL_ENTER:
    if (hint && statusbar) statusbar->label(hint);
    break;
  case FL_LEAVE:
    if (hint && statusbar) statusbar->label("");
    break;
  }
  return Fl_Button::handle(event);
}

DocToolbox::DocToolbox(DocWindow* pw)
  :Fl_Window(w0,h0),doc_parent(pw)
{
  update_label();

  statusbar = new Fl_Box(0,h0-25,w0,25);
  statusbar->box(FL_EMBOSSED_BOX);
  statusbar->labelcolor(FL_BLUE);
  statusbar->labelfont(FL_TIMES_BOLD);
  statusbar->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
  statusbar->label(default_sb_hint);

  btn_group = new Fl_Group(0, 0, w0, h0-25);
  btn_move_vertex = new SButton(2+50*0,2,50,h0-30);
  btn_move_vertex->image(
    new Fl_Bitmap(btn_move_vertex_bits,btn_move_vertex_width,
                  btn_move_vertex_height)
    );
  btn_move_vertex->type(102);
  btn_move_vertex->selection_color(FL_YELLOW);
  btn_move_vertex->tooltip("Move vertex");
  btn_move_vertex->set_hint(statusbar,btn_move_vertex->tooltip());
  btn_move_vertex->callback((Fl_Callback*)btn_move_vertex_cb);
  btn_create_vertex = new SButton(2+50*1,2,50,h0-30);
  btn_create_vertex->image(
    new Fl_Bitmap(btn_create_vertex_bits,btn_create_vertex_width,
                  btn_create_vertex_height)
    );
  btn_create_vertex->type(102);
  btn_create_vertex->selection_color(FL_YELLOW);
  btn_create_vertex->tooltip("Create vertex");
  btn_create_vertex->set_hint(statusbar,btn_create_vertex->tooltip());
  btn_create_vertex->callback((Fl_Callback*)btn_create_vertex_cb);
  btn_remove_vertex = new SButton(2+50*2,2,50,h0-30);
  btn_remove_vertex->image(
    new Fl_Bitmap(btn_remove_vertex_bits,btn_remove_vertex_width,
                  btn_remove_vertex_height)
    );
  btn_remove_vertex->type(102);
  btn_remove_vertex->selection_color(FL_YELLOW);
  btn_remove_vertex->tooltip("Remove vertex");
  btn_remove_vertex->set_hint(statusbar,btn_remove_vertex->tooltip());
  btn_remove_vertex->callback((Fl_Callback*)btn_remove_vertex_cb);
  btn_create_edge = new SButton(2+50*3,2,50,h0-30);
  btn_create_edge->image(
    new Fl_Bitmap(btn_create_edge_bits,btn_create_edge_width,
                  btn_create_edge_height)
    );
  btn_create_edge->type(102);
  btn_create_edge->selection_color(FL_YELLOW);
  btn_create_edge->tooltip("Create edge");
  btn_create_edge->set_hint(statusbar,btn_create_edge->tooltip());
  btn_create_edge->callback((Fl_Callback*)btn_create_edge_cb);
  btn_remove_edge = new SButton(2+50*4,2,50,h0-30);
  btn_remove_edge->image(
    new Fl_Bitmap(btn_remove_edge_bits,btn_remove_edge_width,
                  btn_remove_edge_height)
    );
  btn_remove_edge->type(102);
  btn_remove_edge->selection_color(FL_YELLOW);
  btn_remove_edge->tooltip("Remove edge");
  btn_remove_edge->set_hint(statusbar,btn_remove_edge->tooltip());
  btn_remove_edge->callback((Fl_Callback*)btn_remove_edge_cb);
  btn_vertex_properties = new SButton(2+50*5,2,50,h0-30);
  btn_vertex_properties->image(
    new Fl_Bitmap(btn_vertex_properties_bits,btn_vertex_properties_width,
                  btn_vertex_properties_height)
    );
  btn_vertex_properties->type(102);
  btn_vertex_properties->selection_color(FL_YELLOW);
  btn_vertex_properties->tooltip("Vertex properties");
  btn_vertex_properties->set_hint(statusbar,btn_vertex_properties->tooltip());
  btn_vertex_properties->callback((Fl_Callback*)btn_vertex_properties_cb);
  btn_edge_properties = new SButton(2+50*6,2,50,h0-30);
  btn_edge_properties->image(
    new Fl_Bitmap(btn_edge_properties_bits,btn_edge_properties_width,
                  btn_edge_properties_height)
    );
  btn_edge_properties->type(102);
  btn_edge_properties->selection_color(FL_YELLOW);
  btn_edge_properties->tooltip("Edge properties");
  btn_edge_properties->set_hint(statusbar,btn_edge_properties->tooltip());
  btn_edge_properties->callback((Fl_Callback*)btn_edge_properties_cb);
  btn_add_vertex_to_selection = new SButton(2+50*7,2,50,h0-30);
  btn_add_vertex_to_selection->image(
    new Fl_Bitmap(btn_add_vertex_to_sel_bits,
                  btn_add_vertex_to_sel_width,btn_add_vertex_to_sel_height)
    );
  btn_add_vertex_to_selection->type(102);
  btn_add_vertex_to_selection->selection_color(FL_YELLOW);
  btn_add_vertex_to_selection->tooltip("Add vertex to selection");
  btn_add_vertex_to_selection->set_hint(statusbar,btn_add_vertex_to_selection->tooltip());
  btn_add_vertex_to_selection->callback((Fl_Callback*)btn_add_vertex_to_selection_cb);
  btn_add_edge_to_selection = new SButton(2+50*8,2,50,h0-30);
  btn_add_edge_to_selection->image(
    new Fl_Bitmap(btn_add_edge_to_sel_bits,
                  btn_add_edge_to_sel_width,btn_add_edge_to_sel_height)
    );
  btn_add_edge_to_selection->type(102);
  btn_add_edge_to_selection->selection_color(FL_YELLOW);
  btn_add_edge_to_selection->tooltip("Add edge to selection");
  btn_add_edge_to_selection->set_hint(statusbar,btn_add_edge_to_selection->tooltip());
  btn_add_edge_to_selection->callback((Fl_Callback*)btn_add_edge_to_selection_cb);
  btn_group->end();

  end();

  onset(GRCE_EDIT_MOVE_VERTEX);
}

void DocToolbox::setDocWindow(DocWindow* dw)
{
  doc_parent = dw;
  if (dw)
    onset(dw->EditBox()->grce_edit_mode());
  else
    onset(GRCE_EDIT_MOVE_VERTEX);
}

void DocToolbox::onset(GRCE_EDIT_MODE m)
{
  update_label();

  int i;
  for(i = 0; i < btn_group->children();i++)
    ((Fl_Button*)btn_group->child(i))->value(0);
  switch (m)
  {
  case  GRCE_EDIT_MOVE_VERTEX:
    btn_move_vertex->value(1);
    grce_edit_mode_ = GRCE_EDIT_MOVE_VERTEX;
    break;
  case  GRCE_EDIT_CREATE_VERTEX:
    btn_create_vertex->value(1);
    grce_edit_mode_ = GRCE_EDIT_CREATE_VERTEX;
    break;
  case  GRCE_EDIT_REMOVE_VERTEX:
    btn_remove_vertex->value(1);
    grce_edit_mode_ = GRCE_EDIT_REMOVE_VERTEX;
    break;
  case  GRCE_EDIT_CREATE_EDGE:
    btn_create_edge->value(1);
    grce_edit_mode_ = GRCE_EDIT_CREATE_EDGE;
    break;
  case  GRCE_EDIT_REMOVE_EDGE:
    btn_remove_edge->value(1);
    grce_edit_mode_ = GRCE_EDIT_REMOVE_EDGE;
    break;
  case  GRCE_EDIT_VERTEX_PROPERTIES:
    btn_vertex_properties->value(1);
    grce_edit_mode_ = GRCE_EDIT_VERTEX_PROPERTIES;
    break;
  case  GRCE_EDIT_EDGE_PROPERTIES:
    btn_edge_properties->value(1);
    grce_edit_mode_ = GRCE_EDIT_EDGE_PROPERTIES;
    break;
  case  GRCE_EDIT_ADD_VERTEX_TO_SELECTION:
    btn_add_vertex_to_selection->value(1);
    grce_edit_mode_ = GRCE_EDIT_ADD_VERTEX_TO_SELECTION;
    break;
  case  GRCE_EDIT_ADD_EDGE_TO_SELECTION:
    btn_add_edge_to_selection->value(1);
    grce_edit_mode_ = GRCE_EDIT_ADD_EDGE_TO_SELECTION;
    break;
  default:
    fl_alert("Invalid edit mode!");
  }
}

GRCE_EDIT_MODE DocToolbox::grce_edit_mode()
{
  return grce_edit_mode_;
}

void DocToolbox::update_label()
{
  static char _tmp[1024];
  if (doc_parent)
    strcpy(_tmp,doc_parent->label());
  else
    strcpy(_tmp,"No graphs opened");
  strcat(_tmp," - toolbox");
  copy_label(_tmp);
}

int DocToolbox::handle(int e)
{
  DocWindow* adw = DocWindow::activeDocWindow();
  switch(e)
  {
  case FL_HIDE:
    return Fl_Window::handle(e);
    break;
  case FL_ENTER:
    if (adw)
      setDocWindow(adw);
    return Fl_Window::handle(e);
    break;
  default:
    return Fl_Window::handle(e);
  }
}

DocToolbox::~DocToolbox()
{
}

void DocToolbox::btn_move_vertex_cb(Fl_Widget *widget, void *)
{
  DocToolbox* DocToolbox_Ptr;
  DocToolbox_Ptr = (DocToolbox*)(widget->parent()->parent());
  DocToolbox_Ptr->on_grce_edit_mode(GRCE_EDIT_MOVE_VERTEX);
}

void DocToolbox::btn_create_vertex_cb(Fl_Widget *widget, void *)
{
  DocToolbox* DocToolbox_Ptr;
  DocToolbox_Ptr = (DocToolbox*)(widget->parent()->parent());
  DocToolbox_Ptr->on_grce_edit_mode(GRCE_EDIT_CREATE_VERTEX);
}

void DocToolbox::btn_remove_vertex_cb(Fl_Widget *widget, void *)
{
  DocToolbox* DocToolbox_Ptr;
  DocToolbox_Ptr = (DocToolbox*)(widget->parent()->parent());
  DocToolbox_Ptr->on_grce_edit_mode(GRCE_EDIT_REMOVE_VERTEX);
}

void DocToolbox::btn_create_edge_cb(Fl_Widget *widget, void *)
{
  DocToolbox* DocToolbox_Ptr;
  DocToolbox_Ptr = (DocToolbox*)(widget->parent()->parent());
  DocToolbox_Ptr->on_grce_edit_mode(GRCE_EDIT_CREATE_EDGE);
}

void DocToolbox::btn_remove_edge_cb(Fl_Widget *widget, void *)
{
  DocToolbox*   DocToolbox_Ptr;
  DocToolbox_Ptr = (DocToolbox*)(widget->parent()->parent());
  DocToolbox_Ptr->on_grce_edit_mode(GRCE_EDIT_REMOVE_EDGE);
}

void DocToolbox::btn_vertex_properties_cb(Fl_Widget *widget, void *)
{
  DocToolbox* DocToolbox_Ptr;
  DocToolbox_Ptr = (DocToolbox*)(widget->parent()->parent());
  DocToolbox_Ptr->on_grce_edit_mode(GRCE_EDIT_VERTEX_PROPERTIES);
}

void DocToolbox::btn_edge_properties_cb(Fl_Widget *widget, void *)
{
  DocToolbox* DocToolbox_Ptr;
  DocToolbox_Ptr = (DocToolbox*)(widget->parent()->parent());
  DocToolbox_Ptr->on_grce_edit_mode(GRCE_EDIT_EDGE_PROPERTIES);
}

void DocToolbox::btn_add_vertex_to_selection_cb(Fl_Widget *widget, void *)
{
  DocToolbox* DocToolbox_Ptr;
  DocToolbox_Ptr = (DocToolbox*)(widget->parent()->parent());
  DocToolbox_Ptr->on_grce_edit_mode(GRCE_EDIT_ADD_VERTEX_TO_SELECTION);
}

void DocToolbox::btn_add_edge_to_selection_cb(Fl_Widget *widget, void *)
{
  DocToolbox* DocToolbox_Ptr;
  DocToolbox_Ptr = (DocToolbox*)(widget->parent()->parent());
  DocToolbox_Ptr->on_grce_edit_mode(GRCE_EDIT_ADD_EDGE_TO_SELECTION);
}

int DocToolbox::w0 = 454;
int DocToolbox::h0 = 100;
const char* DocToolbox::default_sb_hint = "Hint: this is toolbox window";

void DocToolbox::on_grce_edit_mode(GRCE_EDIT_MODE m)
{
  grce_edit_mode_ = m;
  doc_parent->on_edit_mode_set();
}

}
