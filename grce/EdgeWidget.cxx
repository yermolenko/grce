/*
  The EdgeWidget class.

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

#include "EdgeWidget.hpp"
#include "grce/Attributes.hpp"
#include <iostream>
#include <iomanip>

namespace grce
{

EdgeWidget::EdgeWidget(
  int x0, int y0, int w0, int h0, const char* title):
  Fl_Widget(x0,y0,w0,h0,title),
  edge(),
  v_start(NULL),v_end(NULL)
{
  align(FL_ALIGN_WRAP);
  box(FL_NO_BOX);
  labelfont(FL_COURIER);
  labelsize(12);
}

EdgeWidget::EdgeWidget(
  grctk::Object edgeObject,
  int x0, int y0, int w0, int h0, const char* title):
  Fl_Widget(x0,y0,w0,h0,title),
  edge(edgeObject),
  v_start(NULL),v_end(NULL)
{
  align(FL_ALIGN_WRAP);
  box(FL_NO_BOX);
  labelfont(FL_COURIER);
  labelsize(12);
}

EdgeWidget::~EdgeWidget()
{
}

void
EdgeWidget::edgeObject(const grctk::Object o)
{
  edge = o;
}

const grctk::Object
EdgeWidget::edgeObject()
{
  return edge;
}

void
EdgeWidget::addToGroup(Fl_Group* g, int n)
{
  g->insert(*this, n);
  copy_label(aLabel[edge].c_str());
}

void
EdgeWidget::draw()
{
  draw_edge_curve();
  if (aLabelVisible[edge])
    draw_label();
}

void
EdgeWidget::update_draw_params()
{
  copy_label(aLabel[edge].c_str());

  int xmax,ymax,xmin,ymin;
  xmax = v_start->center_x()>v_end->center_x()?
    v_start->center_x():v_end->center_x();
  ymax = v_start->center_y()>v_end->center_y()?
    v_start->center_y():v_end->center_y();
  xmin = v_start->center_x()>v_end->center_x()?
    v_end->center_x():v_start->center_x();
  ymin = v_start->center_y()>v_end->center_y()?
    v_end->center_y():v_start->center_y();
  this->resize(xmin-10,ymin-10,xmax-xmin+20,ymax-ymin+20);
  this->show();
  redraw();
}

void
EdgeWidget::set_start_end(VertexWidget * v1, VertexWidget * v2)
{
  if (v_start == NULL && v_end == NULL)
  {
    v_start = v1;
    v_end = v2;
  }
}

void
EdgeWidget::get_start_end(const VertexWidget *&v1, const VertexWidget *&v2)
{
  v1 = v_start;
  v2 = v_end;
}

void EdgeWidget::draw_edge_curve()
{
  Fl_Color buc = fl_color();
  unsigned char r,g,b;
  aColor[edge].analyseRGB(r,g,b);
  fl_push_matrix();
  if (aSelected[edge])
  {
    r = 255-r; g = 255-g; b = 255-b;
    fl_color(fl_rgb_color(r,g,b));
    fl_line_style(FL_DASHDOTDOT);
  }
  else
  {
    fl_color(fl_rgb_color(r,g,b));
    fl_line_style(FL_SOLID);
  }
  fl_line(v_start->center_x(),v_start->center_y(),
          v_end->center_x(),v_end->center_y());
  fl_pop_matrix();
  fl_color(buc);

/*
  fl_draw(label(),(v_end->center_x()-v_start->center_x())/2+v_start->center_x(),
  (v_end->center_y()-v_start->center_y())/2+v_start->center_y());
*/

  fl_line_style(FL_SOLID);
}

}
