/*
  The EdgeWidget class (header file).

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

#ifndef grce_EdgeWidget_hpp
#define grce_EdgeWidget_hpp

#include "grce/VertexWidget.hpp"
#include "grce/Color.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <Fl/Fl_Group.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include <cstring>
#include <string>
#include <grctk/Universe.hpp>

namespace grce
{

class EdgeWidget: public Fl_Widget
{
  grctk::Object edge;
  VertexWidget  *v_start, *v_end;
  EdgeWidget(const EdgeWidget &);
  EdgeWidget & operator = (const EdgeWidget &);
public:
  EdgeWidget(
    int x0 = 0, int y0 = 0,
    int w0 = 10, int h0 = 10,
    const char* title = "");
  EdgeWidget(
    grctk::Object edgeObject,
    int x0 = 0, int y0 = 0,
    int w0 = 10, int h0 = 10,
    const char* title = "");
  virtual ~EdgeWidget();
  void edgeObject(const grctk::Object);
  const grctk::Object edgeObject();
  void addToGroup(Fl_Group*, int);

  virtual void draw();
  void update_draw_params();

  void set_start_end(VertexWidget * v1, VertexWidget * v2);
  void get_start_end(const VertexWidget *&v1, const VertexWidget *&v2);
  VertexWidget* sourceWidget() {return v_start;}
  VertexWidget* targetWidget() {return v_end;}

  void draw_edge_curve();
protected:
  int drag_x;
  int drag_y;
};

}

#endif
