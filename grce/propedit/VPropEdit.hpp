/*
  The VPropEdit class (header file).

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

#ifndef grce_VPropEdit_hpp
#define grce_VPropEdit_hpp

#include "grce/Attributes.hpp"
#include "grce/VertexWidget.hpp"
#include "grce/EdgeWidget.hpp"
#include "grce/GraphEdit.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Int_Input.H>

namespace grce
{

class VPropEdit : public Fl_Group
{
  GraphEdit* sample;
  Fl_Button* btn_choose_color;
  Fl_Check_Button* btn_label_visible;
  Fl_Input* inp_label;
  Fl_Value_Input* inp_weight;
  Fl_Int_Input* inp_int_weight;
public:
  bool colorAltered;
  static void btn_choose_color_cb(Fl_Widget *, void *);
  bool labelVisibleAltered;
  static void btn_label_visible_cb(Fl_Widget *, void *);
  bool labelAltered;
  static void inp_label_cb(Fl_Widget *, void *);
  bool weightAltered;
  static void inp_weight_cb(Fl_Widget *, void *);
  bool intWeightAltered;
  static void inp_int_weight_cb(Fl_Widget *, void *);

  int handle(int);
  VPropEdit(const grctk::Object);
  ~VPropEdit();

  const grctk::Object vertexSample()
    {
      return sample->graphAsAdjMatrix()[0];
    }
};

}

#endif
