/*
  The VPropEdit class.

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

#include "VPropEdit.hpp"
#include <FL/Fl_Color_Chooser.H>

namespace grce
{

int
VPropEdit::handle(int e)
{
  switch(e)
  {
  default:
    return Fl_Group::handle(e);
  }
}

VPropEdit::VPropEdit(const grctk::Object vertexExample):
  Fl_Group(30,30,300,250,"Vertex Properties"),
  colorAltered(false),
  labelVisibleAltered(false),
  labelAltered(false),
  weightAltered(false),
  intWeightAltered(false)
{
  grctk::AdjMatrix g;
  g += vertexExample.clone();
  a3D[g[0]] = yaatk::Vector3D(100,100,0);
  aSelected[g[0]] = false;

  sample = new GraphEdit(20,40,150,150,"",g);
  sample->box(FL_DOWN_BOX);
  sample->end();

  btn_choose_color = new Fl_Button(200,50,150,30,"Choose color");
  btn_choose_color->callback((Fl_Callback*)btn_choose_color_cb);

  btn_label_visible = new Fl_Check_Button(200,90,150,30,"Show label:");
  btn_label_visible->callback((Fl_Callback*)btn_label_visible_cb);
  btn_label_visible->value(aLabelVisible[vertexExample]);

  inp_label = new Fl_Input(200,120,150,30);
  inp_label->value(aLabel[vertexExample].c_str());
  inp_label->callback((Fl_Callback*)inp_label_cb);
  inp_label->when(FL_WHEN_CHANGED);

  inp_weight = new Fl_Value_Input(200,170,150,30,"Weight:");
  inp_weight->align(FL_ALIGN_TOP);
  inp_weight->value(aDouble[vertexExample]);
  inp_weight->callback((Fl_Callback*)inp_weight_cb);
  inp_weight->when(FL_WHEN_CHANGED);

  inp_int_weight = new Fl_Int_Input(200,220,150,30,"Integer Weight:");
  inp_int_weight->align(FL_ALIGN_TOP);
  {
    std::ostringstream oss; oss << aInt[vertexExample]; REQUIRE(!oss.fail());
    inp_int_weight->value(oss.str().c_str());
  }
  inp_int_weight->callback((Fl_Callback*)inp_int_weight_cb);
  inp_int_weight->when(FL_WHEN_CHANGED);

  end();
}

VPropEdit::~VPropEdit()
{
}

void
VPropEdit::btn_label_visible_cb(Fl_Widget *w, void *)
{
  VPropEdit* VPropEdit_Ptr;
  VPropEdit_Ptr = (VPropEdit*)(w->parent());

  bool v = VPropEdit_Ptr->btn_label_visible->value();
  aLabelVisible[VPropEdit_Ptr->sample->graphAsAdjMatrix()[0]] = v;
  VPropEdit_Ptr->labelVisibleAltered = true;

  VPropEdit_Ptr->sample->redraw();
}

void
VPropEdit::btn_choose_color_cb(Fl_Widget *w, void *)
{
  VPropEdit* VPropEdit_Ptr;
  VPropEdit_Ptr = (VPropEdit*)(w->parent());

  Color c = aColor[VPropEdit_Ptr->sample->graphAsAdjMatrix()[0]];
  unsigned char r,g,b;
  c.analyseRGB(r,g,b);

  if (fl_color_chooser("Choose Background Color",r,g,b))
  {
    c = Color(r,g,b);
    aColor[VPropEdit_Ptr->sample->graphAsAdjMatrix()[0]] = c;
    VPropEdit_Ptr->colorAltered = true;
  }

  VPropEdit_Ptr->sample->redraw();
}

void
VPropEdit::inp_label_cb(Fl_Widget *w, void *)
{
  VPropEdit* VPropEdit_Ptr;
  VPropEdit_Ptr = (VPropEdit*)(w->parent());

  const char *s = VPropEdit_Ptr->inp_label->value();
  aLabel[VPropEdit_Ptr->sample->graphAsAdjMatrix()[0]] = std::string(s);
  VPropEdit_Ptr->labelAltered = true;

  VPropEdit_Ptr->sample->redraw();
}

void
VPropEdit::inp_weight_cb(Fl_Widget *w, void *)
{
  VPropEdit* VPropEdit_Ptr;
  VPropEdit_Ptr = (VPropEdit*)(w->parent());

  double s = VPropEdit_Ptr->inp_weight->value();
  aDouble[VPropEdit_Ptr->sample->graphAsAdjMatrix()[0]] = s;
  VPropEdit_Ptr->weightAltered = true;

  VPropEdit_Ptr->sample->redraw();
}

void
VPropEdit::inp_int_weight_cb(Fl_Widget *w, void *)
{
  VPropEdit* VPropEdit_Ptr;
  VPropEdit_Ptr = (VPropEdit*)(w->parent());

  int s = 0;
  {
    std::istringstream iss(VPropEdit_Ptr->inp_int_weight->value());
    iss >> s; REQUIRE(!iss.fail());
  }
  aInt[VPropEdit_Ptr->sample->graphAsAdjMatrix()[0]] = s;
  VPropEdit_Ptr->intWeightAltered = true;

  VPropEdit_Ptr->sample->redraw();
}

}
