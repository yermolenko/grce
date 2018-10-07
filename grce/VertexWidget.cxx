/*
  The VertexWidget class.

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

#include "VertexWidget.hpp"
#include "grce/GraphEdit.hpp"
#include "grce/GRCE.hpp"
#include <iostream>
#include <iomanip>

namespace grce
{

VertexWidget::VertexWidget(
  int x0, int y0, int w0, int h0, const char* title):
  Fl_Box(x0, y0, w0, h0, title),
  vertex(),
  tooltipString()
{
  align(FL_ALIGN_WRAP);
  box(FL_NO_BOX);
}

VertexWidget::VertexWidget(
  grctk::Object vertexObject,
  int x0, int y0, int w0, int h0, const char* title):
  Fl_Box(x0, y0, w0, h0, title),
  vertex(vertexObject),
  tooltipString()
{
  align(FL_ALIGN_WRAP);
  box(FL_NO_BOX);
  updateTooltip();
}

VertexWidget::~VertexWidget()
{
}

void
VertexWidget::vertexObject(const grctk::Object o)
{
  vertex = o;
  updateTooltip();
}

const grctk::Object
VertexWidget::vertexObject()
{
  return vertex;
}

void
VertexWidget::addToGroup(Fl_Group* g)
{
  g->add(this);
  copy_label(aLabel[vertex].c_str());
  // updateTooltip();
}

double
VertexWidget::GetScaledRX()
{
  double ScaledRX = 0.0;

  GraphEdit* DataHolder = (GraphEdit*)parent();
  ScaledRX = (a3D[vertex].x + DataHolder->GetXShift())*DataHolder->GetScale();
  ScaledRX += DataHolder->origin_x();
  ScaledRX += DataHolder->x();

  return ScaledRX;
}

double
VertexWidget::GetScaledRY()
{
  double ScaledRY = 0.0;

  GraphEdit* DataHolder = (GraphEdit*)parent();
  ScaledRY = (a3D[vertex].y + DataHolder->GetYShift())*DataHolder->GetScale();
  ScaledRY += DataHolder->origin_y();
  ScaledRY += DataHolder->y();

  return ScaledRY;
}

double
VertexWidget::GetScaledRZ()
{
  double ScaledRZ = 0.0;

  GraphEdit* DataHolder = (GraphEdit*)parent();
  ScaledRZ = (a3D[vertex].z + DataHolder->GetZShift())*DataHolder->GetScale();

  return ScaledRZ;
}

void
VertexWidget::SyncRealWithInt()
{
  GraphEdit* DataHolder = (GraphEdit*)parent();
  a3D[vertex].x = double(center_x() - DataHolder->origin_x() - DataHolder->x())
    /DataHolder->GetScale() - DataHolder->GetXShift();
  a3D[vertex].y = double(center_y() - DataHolder->origin_y() - DataHolder->y())
    /DataHolder->GetScale() - DataHolder->GetYShift();

  std::set<grctk::Object> vSet;
  vSet.insert(vertexObject());
  grce->mainWindow->docControl->invalidateAttrs(vSet);

  updateTooltip();
}

void
VertexWidget::updateTooltip()
{
  copy_label(aLabel[vertex].c_str());

  std::ostringstream oss;
  oss << "("
      << a3D[vertex].x << ", "
      << a3D[vertex].y << ", "
      << a3D[vertex].z << ")" << "\n";
  oss << "W: " << aDouble[vertex] << "\n";
  oss << "IntW: " << aInt[vertex] << "\n";
  if (aLabel[vertex] != "" && !aLabelVisible[vertex])
    oss << "Label:" << aLabel[vertex] << "\n";
  tooltipString = oss.str();
  tooltip(tooltipString.c_str());
}

void
VertexWidget::SyncIntWithReal()
{
  double ScaledRX,ScaledRY;
  ScaledRX = GetScaledRX();
  ScaledRY = GetScaledRY();
  center_position(int(ScaledRX),int(ScaledRY));
}

void VertexWidget::draw()
{
  Fl_Color buc = fl_color();
  if (!aSelected[vertex])
  {
    unsigned char r,g,b;
    aColor[vertex].analyseRGB(r,g,b);
    draw_box(FL_OVAL_BOX, fl_rgb_color(r,g,b));
    if (aLabelVisible[vertex])
      draw_label();
  }
  else
  {
    unsigned char r,g,b;
    aColor[vertex].analyseRGB(r,g,b);
    draw_box(FL_OSHADOW_BOX, fl_rgb_color(255-r,255-g,255-b));
    if (aLabelVisible[vertex])
      draw_label();
  }
  fl_color(buc);
}

void VertexWidget::center_position(short x, short y)
{
  Fl_Box::position(x - w()/2, y - h()/2);
}

int VertexWidget::center_x()
{
  return (x() + w()/2);
}

int VertexWidget::center_y()
{
  return (y() + h()/2);
}

void VertexWidget::center_x(int x_new)
{
  center_position(x_new,center_x());
}

void VertexWidget::center_y(int y_new)
{
  center_position(y_new,center_y());
}

int VertexWidget::handle(int e)
{
  GraphEdit* doc_wnd_ptr;
  doc_wnd_ptr = (GraphEdit*) parent();
  switch(doc_wnd_ptr->grce_edit_mode())
  {
  case GRCE_EDIT_MOVE_VERTEX:
    return handle_move_vertex(e);
    break;
  case GRCE_EDIT_CREATE_VERTEX:
    return handle_create_vertex(e);
    break;
  case GRCE_EDIT_REMOVE_VERTEX:
    return handle_remove_vertex(e);
    break;
  case GRCE_EDIT_CREATE_EDGE:
    return handle_create_edge(e);
    break;
  case GRCE_EDIT_REMOVE_EDGE:
    return handle_remove_edge(e);
    break;
  case GRCE_EDIT_VERTEX_PROPERTIES:
    return handle_vertex_properties(e);
    break;
  case GRCE_EDIT_EDGE_PROPERTIES:
    return handle_edge_properties(e);
    break;
  case GRCE_EDIT_ADD_VERTEX_TO_SELECTION:
    return handle_add_vertex_to_selection(e);
    break;
  case GRCE_EDIT_ADD_EDGE_TO_SELECTION:
    return handle_add_edge_to_selection(e);
    break;
  default:
    fl_alert("INVALID EDIT MODE !");
    return Fl_Box::handle(e);
  }
}

int VertexWidget::handle_move_vertex(int e)
{
  GraphEdit* doc_wnd_ptr;
  doc_wnd_ptr = (GraphEdit*) parent();
  int drag_x_tmp, drag_y_tmp;
  switch(e)
  {
  case FL_PUSH:
    drag_x = Fl::event_x();
    drag_y = Fl::event_y();
/*
    if (Fl::event_button()==3)
    fl_alert("Vertex Properties");
*/
    Fl_Box::handle(e);
    return 1;
  case FL_RELEASE:
    drag_x = 1;
    drag_y = 1;
    doc_wnd_ptr = (GraphEdit*) parent();
    doc_wnd_ptr->UpdateEdges();
    parent()->parent()->damage(FL_DAMAGE_ALL);
    SyncRealWithInt();
    Fl_Box::handle(e);
    doc_wnd_ptr->next();
    return 1;
  case FL_DRAG:
    drag_x_tmp = Fl::event_x();
    drag_y_tmp = Fl::event_y();
    center_position(this->center_x()+ (drag_x_tmp - drag_x),
                    this->center_y()+ (drag_y_tmp - drag_y));
    drag_x = drag_x_tmp;
    drag_y = drag_y_tmp;
    doc_wnd_ptr = (GraphEdit*) parent();
    doc_wnd_ptr->UpdateEdges();
    parent()->parent()->damage(FL_DAMAGE_ALL);
    Fl_Box::handle(e);
    return 1;
  case FL_MOVE:
    return 1;
  case FL_MOUSEWHEEL:
    return 1;
  default:
    return Fl_Box::handle(e);
  }
}

int VertexWidget::handle_create_vertex(int e)
{
  switch(e)
  {
  default:
    return Fl_Box::handle(e);
  }
}

int VertexWidget::handle_remove_vertex(int e)
{
  GraphEdit* doc_wnd_ptr;
  doc_wnd_ptr = (GraphEdit*) parent();
  switch(e)
  {
  case FL_PUSH:
    switch (doc_wnd_ptr->grce_edit_mode_step())
    {
    case 1:
      doc_wnd_ptr->mode_info.v1 = this;
      break;
    };
    doc_wnd_ptr->next();
    return 1;
  default:
    return Fl_Box::handle(e);
  }
}

int VertexWidget::handle_create_edge(int e)
{
  GraphEdit* doc_wnd_ptr;
  doc_wnd_ptr = (GraphEdit*) parent();
  switch(e)
  {
  case FL_PUSH:
    switch (doc_wnd_ptr->grce_edit_mode_step())
    {
    case 1:
      doc_wnd_ptr->mode_info.v1 = this;
      break;
    case 2:
      doc_wnd_ptr->mode_info.v2 = this;
      break;
    };
    doc_wnd_ptr->next();
    return 1;
  default:
    return Fl_Box::handle(e);
  }
}

int VertexWidget::handle_remove_edge(int e)
{
  GraphEdit* doc_wnd_ptr;
  doc_wnd_ptr = (GraphEdit*) parent();
  switch(e)
  {
  case FL_PUSH:
    switch (doc_wnd_ptr->grce_edit_mode_step())
    {
    case 1:
      doc_wnd_ptr->mode_info.v1 = this;
      break;
    case 2:
      doc_wnd_ptr->mode_info.v2 = this;
      break;
    };
    doc_wnd_ptr->next();
    return 1;
  default:
    return Fl_Box::handle(e);
  }
}

int VertexWidget::handle_vertex_properties(int e)
{
  GraphEdit* doc_wnd_ptr;
  doc_wnd_ptr = (GraphEdit*) parent();
  switch(e)
  {
  case FL_PUSH:
    switch (doc_wnd_ptr->grce_edit_mode_step())
    {
    case 1:
      doc_wnd_ptr->mode_info.v1 = this;
      break;
    };
    doc_wnd_ptr->next();
    return 1;
  default:
    return Fl_Box::handle(e);
  }
}

int VertexWidget::handle_edge_properties(int e)
{
  GraphEdit* doc_wnd_ptr;
  doc_wnd_ptr = (GraphEdit*) parent();
  switch(e)
  {
  case FL_PUSH:
    switch (doc_wnd_ptr->grce_edit_mode_step())
    {
    case 1:
      doc_wnd_ptr->mode_info.v1 = this;
      break;
    case 2:
      doc_wnd_ptr->mode_info.v2 = this;
      break;
    };
    doc_wnd_ptr->next();
    return 1;
  default:
    return Fl_Box::handle(e);
  }
}

int VertexWidget::handle_add_vertex_to_selection(int e)
{
  GraphEdit* doc_wnd_ptr;
  doc_wnd_ptr = (GraphEdit*) parent();
  switch(e)
  {
  case FL_PUSH:
    switch (doc_wnd_ptr->grce_edit_mode_step())
    {
    case 1:
      doc_wnd_ptr->mode_info.v1 = this;
      break;
    };
    doc_wnd_ptr->next();
    return 1;
  default:
    return Fl_Box::handle(e);
  }
}

int VertexWidget::handle_add_edge_to_selection(int e)
{
  GraphEdit* doc_wnd_ptr;
  doc_wnd_ptr = (GraphEdit*) parent();
  switch(e)
  {
  case FL_PUSH:
    switch (doc_wnd_ptr->grce_edit_mode_step())
    {
    case 1:
      doc_wnd_ptr->mode_info.v1 = this;
      break;
    case 2:
      doc_wnd_ptr->mode_info.v2 = this;
      break;
    };
    doc_wnd_ptr->next();
    return 1;
  default:
    return Fl_Box::handle(e);
  }
}

}
