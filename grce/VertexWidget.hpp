/*
  The VertexWidget class (header file).

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

#ifndef grce_VertexWidget_hpp
#define grce_VertexWidget_hpp

#include "grce/Color.hpp"
#include <grctk/Universe.hpp>
#include <yaatk/Vector3D.hpp>
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Box.H>
#include <Fl/Fl_Group.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include <cstring>
#include <string>

namespace grce
{

class VertexWidget: public Fl_Box
{
  grctk::Object vertex;
  std::string tooltipString;
  VertexWidget(const VertexWidget &);
  VertexWidget & operator = (const VertexWidget &);
public:
  VertexWidget(
    int x0 = 0,  int y0 = 0,
    int w0 = 20, int h0 = 20,
    const char* title="V");
  VertexWidget(
    grctk::Object vertexObject,
    int x0 = 0,  int y0 = 0,
    int w0 = 20, int h0 = 20,
    const char* title="V");
  void addToGroup(Fl_Group*);
  void vertexObject(const grctk::Object);
  const grctk::Object vertexObject();

  void SyncRealWithInt();
  void SyncIntWithReal();

  void updateTooltip();

  double GetScaledRX();
  double GetScaledRY();
  double GetScaledRZ();

  void center_position(short x, short y);
  int center_x();
  int center_y();

  void center_x(int x_new);
  void center_y(int y_new);

  int handle(int);

  virtual void draw();

  virtual ~VertexWidget();
protected:
  int drag_x;
  int drag_y;

  int handle_move_vertex(int);
  int handle_create_vertex(int);
  int handle_remove_vertex(int);
  int handle_create_edge(int);
  int handle_remove_edge(int);
  int handle_vertex_properties(int);
  int handle_edge_properties(int);
  int handle_add_vertex_to_selection(int);
  int handle_add_edge_to_selection(int);
};

}

#endif
