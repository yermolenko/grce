/*
  The GraphEdit class.

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

#include "GraphEdit.hpp"
#include "grce/DocWindow.hpp"
#include "grce/VisWindow.hpp"
#include "grce/GRCE.hpp"
#include "grce/propedit/PropEdit.hpp"
#include "grce/algo/color/ResetColors.hpp"
#include <grctk/algo/drawing/GraphMultiRep.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

namespace grce
{

void GraphEdit::set_statusbar(Fl_Box* sb)
{
  mode_info.statusbar = sb;
}

GRCE_EDIT_MODE GraphEdit::grce_edit_mode()
{
  return mode_info.grce_edit_mode_;
}

void GraphEdit::grce_edit_mode(GRCE_EDIT_MODE m)
{
  mode_info.grce_edit_mode_ = m;
//  mode_info.mode_step_=-1;
  mode_info.mode_step_=0;
  next();
}

int GraphEdit::grce_edit_mode_step()
{
  return mode_info.mode_step_;
}

void GraphEdit::grce_edit_mode_step(int m)
{
  mode_info.mode_step_ = m;
}

void GraphEdit::mode_info_init()
{
//  mode_info.mode_step_ = -1;
  mode_info.statusbar = 0;
  mode_info.v1 = mode_info.v2 = NULL;
  mode_info.ix = mode_info.iy = 0;
  mode_info.hint = "";
  grce_edit_mode(GRCE_EDIT_MOVE_VERTEX);
//  next();
}

void GraphEdit::next()
{
//  cout << "mode_info.mode_step_=" << mode_info.mode_step_ << "\n";

  switch (mode_info.grce_edit_mode_)
  {
  case GRCE_EDIT_MOVE_VERTEX:
    switch (mode_info.mode_step_)
    {
//    case -1:
//      mode_info.mode_step_++;
    case 0:
      mode_info.hint = "Drag any VERTEX";
      mode_info.mode_step_=1;
      break;
    case 1:
      mode_info.hint = "Drag any VERTEX";
      mode_info.mode_step_=1;
      break;
    }
    break;
  case GRCE_EDIT_CREATE_VERTEX:
    switch (mode_info.mode_step_)
    {
//    case -1:
//      mode_info.mode_step_++;
    case 0:
      mode_info.hint = "Click to place VERTEX";
      mode_info.mode_step_=1;
      break;
    case 1:
      mode_info.hint = "Click to place VERTEX";
      add_vertex();
      mode_info.mode_step_=1;
      break;
    }
    break;
  case GRCE_EDIT_REMOVE_VERTEX:
    switch (mode_info.mode_step_)
    {
//    case -1:
//      mode_info.mode_step_++;
    case 0:
      mode_info.hint = "Click on VERTEX to delete it";
      mode_info.mode_step_=1;
      break;
    case 1:
      mode_info.hint = "Click on VERTEX to delete it";
      remove_vertex();
      mode_info.mode_step_=1;
      break;
    }
    break;
  case GRCE_EDIT_CREATE_EDGE:
    switch (mode_info.mode_step_)
    {
//    case -1:
//      mode_info.mode_step_++;
    case 0:
      mode_info.hint = "Click FIRST VERTEX (start of the edge)";
      mode_info.mode_step_=1;
      break;
    case 1:
      mode_info.hint = "Click SECOND VERTEX (end of the edge)";
      mode_info.mode_step_=2;
      break;
    case 2:
      add_edge();
      mode_info.hint = "Click FIRST VERTEX (start of the edge)";
      mode_info.mode_step_=1;
      break;
    }
    break;
  case GRCE_EDIT_REMOVE_EDGE:
    switch (mode_info.mode_step_)
    {
//    case -1:
//      mode_info.mode_step_++;
    case 0:
      mode_info.hint = "Click FIRST VERTEX (start of the edge)";
      mode_info.mode_step_=1;
      break;
    case 1:
      mode_info.hint = "Click SECOND VERTEX (end of the edge)";
      mode_info.mode_step_=2;
      break;
    case 2:
      delete_edge();
      mode_info.hint = "Click FIRST VERTEX (start of the edge)";
      mode_info.mode_step_=1;
      break;
    }
    break;
  case GRCE_EDIT_VERTEX_PROPERTIES:
    switch (mode_info.mode_step_)
    {
//    case -1:
//      mode_info.mode_step_++;
    case 0:
      mode_info.hint = "Click on VERTEX to edit it's properties";
      mode_info.mode_step_=1;
      break;
    case 1:
      vertex_properties();
      mode_info.hint = "Click on VERTEX to edit it's properties";
      mode_info.mode_step_=1;
      break;
    }
    break;
  case GRCE_EDIT_EDGE_PROPERTIES:
    switch (mode_info.mode_step_)
    {
//    case -1:
//      mode_info.mode_step_++;
    case 0:
      mode_info.hint = "Click FIRST VERTEX (start of the edge)";
      mode_info.mode_step_=1;
      break;
    case 1:
      mode_info.hint = "Click SECOND VERTEX (end of the edge)";
      mode_info.mode_step_=2;
      break;
    case 2:
      edge_properties();
      mode_info.hint = "Click FIRST VERTEX (start of the edge)";
      mode_info.mode_step_=1;
      break;
    }
    break;
  case GRCE_EDIT_ADD_VERTEX_TO_SELECTION:
    switch (mode_info.mode_step_)
    {
//    case -1:
//      mode_info.mode_step_++;
    case 0:
      mode_info.hint = "Click on VERTEX to {add to,remove from} selection";
      mode_info.mode_step_=1;
      break;
    case 1:
      add_vertex_to_selection();
      mode_info.hint = "Click on VERTEX to {add to,remove from} selection";
      mode_info.mode_step_=1;
      break;
    }
    break;
  case GRCE_EDIT_ADD_EDGE_TO_SELECTION:
    switch (mode_info.mode_step_)
    {
//    case -1:
//      mode_info.mode_step_++;
    case 0:
      mode_info.hint = "Click FIRST VERTEX (start of the edge) to {add to,remove from} selection";
      mode_info.mode_step_=1;
      break;
    case 1:
      mode_info.hint = "Click SECOND VERTEX (end of the edge) to {add to,remove from} selection";
      mode_info.mode_step_=2;
      break;
    case 2:
      add_edge_to_selection();
      mode_info.hint = "Click FIRST VERTEX (start of the edge) to {add to,remove from} selection";
      mode_info.mode_step_=1;
      break;
    }
    break;
  default:
    mode_info.hint = default_sb_hint;
    fl_alert("error !");
  }
//  mode_info.mode_step_++;
  if (mode_info.statusbar)
  {
    mode_info.statusbar->label(mode_info.hint);
  }
}

double GraphEdit::GetScale() {return Scale;}
void GraphEdit::SetScale(double new_val) {Scale = new_val;}

double GraphEdit::GetXShift() {return XShift;}
void GraphEdit::SetXShift(double new_val) {XShift = new_val;}

double GraphEdit::GetYShift() {return YShift;}
void GraphEdit::SetYShift(double new_val) {YShift = new_val;}

double GraphEdit::GetZShift() {return ZShift;}
void GraphEdit::SetZShift(double new_val) {ZShift = new_val;}

void GraphEdit::LoadFromGraph(const grctk::AdjMatrix& g)
{
  RemoveAddVisuals(g);
  UpdateScale();
  UpdateVertexes();
  UpdateEdges();
}

void GraphEdit::LoadFromGraphEdit(GraphEdit& ge)
{
  LoadFromGraph(ge.graphAsAdjMatrix());
}

void
GraphEdit::Repair()
{
  for(int k = 0; k < children(); k++)
  {
    VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
    if (pV)
    {
      grctk::Object v = pV->vertexObject();
      aColor[v] = Color(255,0,0);
      aDouble[v] = 1.0;
      a3D[v] = yaatk::Vector3D(rand()/double(RAND_MAX),
                               rand()/double(RAND_MAX),
                               0);
    }
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE)
    {
      grctk::Object e = pE->edgeObject();
      aColor[e] = Color(0,127,0);
      aDouble[e] = 1.0;
    }
  }

  UpdateScale();
  UpdateVertexes();
  UpdateEdges();
}

void GraphEdit::UpdateScale(bool fitToWindow)
{
  double w_win,h_win;

  if (fitToWindow)
  {
    w_win = w();
    h_win = h();
  }
  else
  {
    int x_int_min, x_int_max, y_int_min, y_int_max, x_int_cur, y_int_cur;
    size_t vCount = 0;
    for(int k = 0; k < children(); k++)
    {
      VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
      if (pV)
      {
        if (vCount == 0)
        {
          x_int_max = x_int_min = pV->center_x();// - origin_x() - x();
          y_int_max = y_int_min = pV->center_y();// - origin_y() - y();
        }
        vCount++;
        {
          x_int_cur = pV->center_x();// - origin_x() - x();
          y_int_cur = pV->center_y();// - origin_y() - y();
          if (x_int_cur > x_int_max) x_int_max = x_int_cur;
          if (x_int_cur < x_int_min) x_int_min = x_int_cur;
          if (y_int_cur > y_int_max) y_int_max = y_int_cur;
          if (y_int_cur < y_int_min) y_int_min = y_int_cur;
        }
      }
    }
    if (vCount < 2 || x_int_max == x_int_min || y_int_max == y_int_min)
    {
      w_win = w();
      h_win = h();
    }
    else
    {
      w_win = x_int_max - x_int_min;
      h_win = y_int_max - y_int_min;
    }
  }

  double w_real,h_real;

  int max_diam = 15;
  int sb_width = 16;

  double x_max = 0.0, x_min = 0.0, y_max = 0.0, y_min = 0.0;

  size_t vertexCount = 0;
  {
    double x_cur,y_cur,
      new_x_shift = 0, new_y_shift = 0;

    for(int k = 0; k < children(); k++)
    {
      VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
      if (pV)
      {
        grctk::Object v = pV->vertexObject();

        if (vertexCount == 0)
        {
          x_max = x_min = a3D[v].x;
          y_max = y_min = a3D[v].y;
        }
        vertexCount++;

        {
          x_cur = a3D[v].x;
          y_cur = a3D[v].y;
          if (x_cur > x_max) x_max = x_cur;
          if (x_cur < x_min) x_min = x_cur;
          if (y_cur > y_max) y_max = y_cur;
          if (y_cur < y_min) y_min = y_cur;
        }
      }
    }
    // new_y_shift = -y_min + double(max_diam)/(new_scale);
    // new_x_shift = -x_min + double(max_diam)/(new_scale);

    if (vertexCount < 2 || x_max == x_min || y_max == y_min)
    {
      w_real = 2.0*fabs(x_max);
      h_real = 2.0*fabs(y_max);
    }
    else
    {
      w_real = x_max - x_min;
      h_real = y_max - y_min;
    }
  }

  double new_scale = 1.0, new_x_scale, new_y_scale;
  {
    const double minRealSize = 1e-30;
    if (fabs(w_real) < minRealSize && fabs(h_real) < minRealSize)
    {
      w_real = 1.0;
      h_real = 1.0;
    }
    else
    {
      if (fabs(w_real) < minRealSize)
        w_real = minRealSize;
      if (fabs(h_real) < minRealSize)
        h_real = minRealSize;
    }
    new_x_scale = (w_win-2.0*max_diam-sb_width)/w_real;
    new_y_scale = (h_win-2.0*max_diam-sb_width)/h_real;
    // if (x_max != x_min && y_max != y_min)
    new_scale = (new_x_scale > new_y_scale)?new_y_scale:new_x_scale;
  }

  // if (new_x_scale > new_y_scale)
  // new_y_shift = -y_min;// + double(max_diam)/(new_scale);
  // else
  // new_x_shift = -x_min;// + double(max_diam)/(new_scale);

  if (vertexCount == 0)
  {
    new_scale = 1.0;
    // new_x_shift = 0.0;
    // new_y_shift = 0.0;
  }

  SetScale(new_scale);
  // SetXShift(new_x_shift);
  // SetYShift(new_y_shift);

  if (vertexCount > 0)
    position((x_min + GetXShift())*GetScale() + origin_x() + xposition() - max_diam,
             (y_min + GetYShift())*GetScale() + origin_y() + yposition() - max_diam);
}

void
GraphEdit::UpdateAll()
{
  // RemoveAddVisuals();
  UpdateScale();
  UpdateVertexes();
  UpdateEdges();
  // position(0,0);
}

void
GraphEdit::invalidateAttrs(const std::set<grctk::Object>& objs)
{
  objectsWithInvalidatedAttrs.insert(objs.begin(), objs.end());
}

void
GraphEdit::invalidateAttrs(const grctk::AdjMatrix& g)
{
  size_t n = g.size();
  for(size_t i = 0; i < n; ++i)
    objectsWithInvalidatedAttrs.insert(g[i]);
  for(size_t i = 0; i < n; i++)
    for(size_t j = 0; j < n; j++)
      if (g.edge(i,j))
        objectsWithInvalidatedAttrs.insert(g.edge(i,j));
}

void
GraphEdit::redrawInvalidated()
{
  if (!objectsWithInvalidatedAttrs.empty())
  {
    std::vector<VertexWidget*> vlist;
    std::vector<EdgeWidget*> elist;
    for(int k = 0; k < children(); k++)
    {
      EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
      if (pE)
      {
        grctk::Object e = pE->edgeObject();
        VertexWidget *pV1 = pE->sourceWidget();
        grctk::Object v1 = pV1->vertexObject();
        VertexWidget *pV2 = pE->targetWidget();
        grctk::Object v2 = pV2->vertexObject();
        if (objectsWithInvalidatedAttrs.find(e) != objectsWithInvalidatedAttrs.end() ||
            objectsWithInvalidatedAttrs.find(v1) != objectsWithInvalidatedAttrs.end() ||
            objectsWithInvalidatedAttrs.find(v2) != objectsWithInvalidatedAttrs.end())
          elist.push_back(pE);
      }
      else
      {
        VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
        if (pV)
        {
          grctk::Object v = pV->vertexObject();
          if (objectsWithInvalidatedAttrs.find(v) != objectsWithInvalidatedAttrs.end())
            vlist.push_back(pV);
        }
      }
    }
    // UpdateScale(false);
    UpdateVertexes(vlist);
    UpdateEdges(elist);
    redraw();
    objectsWithInvalidatedAttrs.clear();
  }
}

int GraphEdit::handle(int e)
{
  switch(grce_edit_mode())
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
    return Fl_Widget::handle(e);
  }
}

int GraphEdit::handle_move_vertex(int e)
{
  switch(e)
  {
  default:
    return Fl_Scroll::handle(e);
  }
}

int GraphEdit::handle_create_vertex(int e)
{
  int x_tmp, y_tmp;
  switch(e)
  {
  case FL_PUSH:
    x_tmp = Fl::event_x();
    y_tmp = Fl::event_y();
    if (x_tmp < x() + w() - 20 && y_tmp < y() + h() - 20)
    {
      // mode_info.ix = x_tmp - x();
      // mode_info.iy = y_tmp - y();
      mode_info.ix = x_tmp;
      mode_info.iy = y_tmp;
      next();
    }
    Fl_Scroll::handle(e);
    return 1;
  default:
    return Fl_Scroll::handle(e);
  }
}

int GraphEdit::handle_remove_vertex(int e)
{
  switch(e)
  {
  default:
    return Fl_Scroll::handle(e);
  }
}

int GraphEdit::handle_create_edge(int e)
{
  switch(e)
  {
  default:
    return Fl_Scroll::handle(e);
  }
}

int GraphEdit::handle_remove_edge(int e)
{
  switch(e)
  {
  default:
    return Fl_Scroll::handle(e);
  }
}

int GraphEdit::handle_vertex_properties(int e)
{
  switch(e)
  {
  default:
    return Fl_Scroll::handle(e);
  }
}

int GraphEdit::handle_edge_properties(int e)
{
  switch(e)
  {
  default:
    return Fl_Scroll::handle(e);
  }
}

int GraphEdit::handle_add_vertex_to_selection(int e)
{
  switch(e)
  {
  default:
    return Fl_Scroll::handle(e);
  }
}

int GraphEdit::handle_add_edge_to_selection(int e)
{
  switch(e)
  {
  default:
    return Fl_Scroll::handle(e);
  }
}

GraphEdit::GraphEdit(int x0, int y0, int w0, int h0,
                     const char *title, grctk::AdjMatrix initialGraph):
  Fl_Scroll(x0,y0,w0,h0,title),
  mode_info(),
  Scale(1.0),XShift(0),YShift(0.0),ZShift(0.0),
  objectsWithInvalidatedAttrs()
{
  box(FL_DOWN_BOX);
  boxOrigin = new Fl_Box(w()/2+x(),h()/2+y(),10,10,"");
  boxOrigin->box(FL_BORDER_BOX);
  boxOrigin->tooltip("(0,0)");
  // boxOrigin = new Fl_Box(1,31,10,10,"o");
  // boxOrigin->hide();
  end();
  mode_info_init();
  // UpdateScale();
  LoadFromGraph(initialGraph);
  this->show();
}

void GraphEdit::RemoveVisuals()
{
  std::vector<Fl_Widget*> widgetsToRemove;

  for(int k = 0; k < children(); k++)
  {
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE)
    {
      grctk::Object e = pE->edgeObject();
      e.removeOwner();
      widgetsToRemove.push_back(pE);
    }
  }

  for(int k = 0; k < children(); k++)
  {
    VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
    if (pV)
    {
      grctk::Object v = pV->vertexObject();
      v.removeOwner();
      widgetsToRemove.push_back(pV);
    }
  }

  for(size_t i = 0; i < widgetsToRemove.size(); ++i)
  {
    remove(widgetsToRemove[i]);
    Fl::delete_widget(widgetsToRemove[i]);
  }

  // PRINT("GraphEdit::RemoveVisuals() : ");
  // TRACE(children());
}

void GraphEdit::AddVisuals(const grctk::AdjMatrix& g)
{
  size_t v0 = children();
  size_t VC = g.vertexCount();
  for(size_t i = 0; i < VC; i++)
  {
    VertexWidget* pV = new VertexWidget(g.getVertex(VC-i-1));
    pV->vertexObject().addOwner();
    pV->addToGroup(this);
//  pV->SyncIntWithReal();
  }

  REQUIRE(children() == VC + v0);

  for(size_t i = 0; i < VC; i++)
  {
    for(size_t j = i+1; j < VC; j++)
    {
      if (g(j,i))
      {
        EdgeWidget* pE = new EdgeWidget(g(j,i));
        pE->edgeObject().addOwner();
        VertexWidget *pV1 = dynamic_cast<VertexWidget*>(child(children()-j-1));
        REQUIRE(pV1 != NULL);
        VertexWidget *pV2 = dynamic_cast<VertexWidget*>(child(children()-i-1));
        REQUIRE(pV2 != NULL);
        pE->set_start_end(pV1, pV2);
        pE->addToGroup(this, v0);
      }
    }
  }

#ifdef GRCE_JUST_A_TEST
  Fl_Box*
    bo = new Fl_Box(w()/3+x(),h()/3+y(),10,10,"");
  bo->box(FL_BORDER_BOX);
  bo->tooltip("(1,1)");
  this->add(bo);
  Fl_Widget*
    wo = new Fl_Box(w()/4+x(),h()/4+y(),10,10,"");
  wo->box(FL_BORDER_BOX);
  wo->tooltip("(2,2)");
  this->add(wo);
#endif
}

void GraphEdit::RemoveAddVisuals(const grctk::AdjMatrix& g)
{
  RemoveVisuals();
  AddVisuals(g);
}

void GraphEdit::UpdateVertexes()
{
  for(int k = 0; k < children(); k++)
  {
    VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
    if (pV)
    {
      int x_int = int(pV->GetScaledRX());
      int y_int = int(pV->GetScaledRY());
      pV->center_position(x_int,y_int);
      pV->updateTooltip();
      pV->show();
    }
  }
}

void GraphEdit::UpdateVertexes(const std::vector<VertexWidget*>& childs)
{
  for(size_t k = 0; k < childs.size(); k++)
  {
    VertexWidget *pV = childs[k];
    // if (pV)
    {
      int x_int = int(pV->GetScaledRX());
      int y_int = int(pV->GetScaledRY());
      pV->center_position(x_int,y_int);
      pV->updateTooltip();
      pV->show();
    }
  }
}

void GraphEdit::UpdateEdges()
{
  for(int k = 0; k < children(); k++)
  {
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE)
    {
      pE->update_draw_params();
      pE->show();
    }
  }
}

void GraphEdit::UpdateEdges(const std::vector<EdgeWidget*>& childs)
{
  for(size_t k = 0; k < childs.size(); k++)
  {
    EdgeWidget *pE = childs[k];
    // if (pE)
    {
      pE->update_draw_params();
      pE->show();
    }
  }
}

GraphEdit::~GraphEdit()
{
  // TRACE("GraphEdit::~GraphEdit()");
  RemoveVisuals();
  // TRACE(grctk::Universe::singleton().size());
}

void GraphEdit::add_vertex()
{
  grctk::Object v = grctk::Universe::singleton().create();
  v.addOwner();
  VertexWidget* pV = new VertexWidget(v);
  pV->addToGroup(this);
//  pV->SyncIntWithReal();

  aDouble[v] = 1.0;
  aColor[v] = Color(255,0,0);
//  pV->SetIXY(mode_info.ix,mode_info.iy);

  int center_x = mode_info.ix;
  int center_y = mode_info.iy;

  a3D[v].x = double(center_x - origin_x() - x())
    /GetScale() - GetXShift();
  a3D[v].y = double(center_y - origin_y() - y())
    /GetScale() - GetYShift();

  // TRACE(a3D[v]);

//  pV->SyncRealWithInt();
  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void GraphEdit::remove_vertex()
{
  std::vector<Fl_Widget*> widgetsToRemove;

  grctk::Object v = mode_info.v1->vertexObject();
  for(int k = 0; k < children(); k++)
  {
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE)
    {
      if (pE->sourceWidget()->vertexObject() == v ||
          pE->targetWidget()->vertexObject() == v)
      {
        grctk::Object e = pE->edgeObject();
        e.removeOwner();
        widgetsToRemove.push_back(pE);
      }
    }
  }

  v.removeOwner();
  widgetsToRemove.push_back(mode_info.v1);

  for(size_t i = 0; i < widgetsToRemove.size(); ++i)
  {
    remove(widgetsToRemove[i]);
    Fl::delete_widget(widgetsToRemove[i]);
  }

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void GraphEdit::add_edge()
{
  if (mode_info.v1 == mode_info.v2) return;

  for(int k = 0; k < children(); k++)
  {
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE)
    {
      if ((pE->sourceWidget() == mode_info.v1 &&
           pE->targetWidget() == mode_info.v2) ||
          (pE->sourceWidget() == mode_info.v2 &&
           pE->targetWidget() == mode_info.v1))
      {
        return;
      }
    }
  }

  grctk::Object e = grctk::Universe::singleton().create();
  e.addOwner();
  EdgeWidget* pE = new EdgeWidget(e);
  pE->set_start_end(mode_info.v1, mode_info.v2);
  pE->addToGroup(this, 0);

  aDouble[e] = 1.0;
  aColor[e] = Color(0,127,0);

  // TRACE(mode_info.v1->center_x());
  // TRACE(mode_info.v1->center_y());
  // TRACE(mode_info.v2->center_x());
  // TRACE(mode_info.v2->center_y());

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void GraphEdit::delete_edge()
{
  if (mode_info.v1 == mode_info.v2) return;

  std::vector<Fl_Widget*> widgetsToRemove;

  for(int k = 0; k < children(); k++)
  {
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE)
    {
      if ((pE->sourceWidget() == mode_info.v1 &&
           pE->targetWidget() == mode_info.v2) ||
          (pE->sourceWidget() == mode_info.v2 &&
           pE->targetWidget() == mode_info.v1))
      {
        pE->edgeObject().removeOwner();
        widgetsToRemove.push_back(pE);
      }
    }
  }

  for(size_t i = 0; i < widgetsToRemove.size(); ++i)
  {
    remove(widgetsToRemove[i]);
    Fl::delete_widget(widgetsToRemove[i]);
  }

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void GraphEdit::vertex_properties()
{
  std::set<grctk::Object> vSet;
  vSet.insert(mode_info.v1->vertexObject());
  std::set<grctk::Object> eSet;
  PropEdit pe(vSet, eSet);
  while (pe.shown())
    Fl::wait();

  grce->mainWindow->docControl->invalidateAttrs(vSet);

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void GraphEdit::edge_properties()
{
  if (mode_info.v1 == mode_info.v2) return;

  std::set<grctk::Object> vSet;
  std::set<grctk::Object> eSet;

  for(int k = 0; k < children(); k++)
  {
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE)
    {
      if ((pE->sourceWidget() == mode_info.v1 &&
           pE->targetWidget() == mode_info.v2) ||
          (pE->sourceWidget() == mode_info.v2 &&
           pE->targetWidget() == mode_info.v1))
      {
        eSet.insert(pE->edgeObject());
      }
    }
  }

  PropEdit pe(vSet, eSet);
  while (pe.shown())
    Fl::wait();

  grce->mainWindow->docControl->invalidateAttrs(eSet);

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void GraphEdit::selection_properties()
{
  std::set<grctk::Object> vSet;
  std::set<grctk::Object> eSet;

  for(int k = 0; k < children(); k++)
  {
    VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
    if (pV && aSelected[pV->vertexObject()])
      vSet.insert(pV->vertexObject());
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE && aSelected[pE->edgeObject()])
      eSet.insert(pE->edgeObject());
  }

  PropEdit pe(vSet, eSet);
  while (pe.shown())
    Fl::wait();

  grce->mainWindow->docControl->invalidateAttrs(vSet);
  grce->mainWindow->docControl->invalidateAttrs(eSet);

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

const grctk::AdjMatrix
GraphEdit::graphAsAdjMatrix() const
{
  grctk::AdjMatrix g;
  for(int k = children()-1; k >=0; k--)
  {
    VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
    if (pV && !g.hasVertex(pV->vertexObject()))
      g += pV->vertexObject();
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE)
    {
      if (!g.hasVertex(pE->sourceWidget()->vertexObject()))
        g += pE->sourceWidget()->vertexObject();
      if (!g.hasVertex(pE->targetWidget()->vertexObject()))
        g += pE->targetWidget()->vertexObject();
      g.edge(g.vertexIndex(pE->sourceWidget()->vertexObject()),
             g.vertexIndex(pE->targetWidget()->vertexObject()),
             pE->edgeObject());
    }
  }

  return g;
}

const grctk::AdjMatrix
GraphEdit::selectionAsAdjMatrix() const
{
  grctk::AdjMatrix gSel;
  for(int k = children()-1; k >=0; k--)
  {
    VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
    if (pV && aSelected[pV->vertexObject()] &&
        !gSel.hasVertex(pV->vertexObject()))
      gSel += pV->vertexObject();
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE && aSelected[pE->edgeObject()])
    {
      if (!gSel.hasVertex(pE->sourceWidget()->vertexObject()))
        gSel += pE->sourceWidget()->vertexObject();
      if (!gSel.hasVertex(pE->targetWidget()->vertexObject()))
        gSel += pE->targetWidget()->vertexObject();
      gSel.edge(gSel.vertexIndex(pE->sourceWidget()->vertexObject()),
                gSel.vertexIndex(pE->targetWidget()->vertexObject()),
                pE->edgeObject());
    }
  }

  return gSel;
}

void GraphEdit::add_vertex_to_selection()
{
  VertexWidget *pV = mode_info.v1;

  aSelected[pV->vertexObject()] = !aSelected[pV->vertexObject()];

  std::set<grctk::Object> vSet;
  vSet.insert(pV->vertexObject());
  grce->mainWindow->docControl->invalidateAttrs(vSet);

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void GraphEdit::add_edge_to_selection()
{
  if (mode_info.v1 == mode_info.v2) return;

  std::set<grctk::Object> eSet;

  for(int k = 0; k < children(); k++)
  {
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE)
    {
      if ((pE->sourceWidget() == mode_info.v1 &&
           pE->targetWidget() == mode_info.v2) ||
          (pE->sourceWidget() == mode_info.v2 &&
           pE->targetWidget() == mode_info.v1))
      {
        aSelected[pE->edgeObject()] = !aSelected[pE->edgeObject()];
        eSet.insert(pE->edgeObject());
      }
    }
  }

  grce->mainWindow->docControl->invalidateAttrs(eSet);

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void
GraphEdit::SelectFromGraph(const grctk::AdjMatrix& g)
{
  std::set<grctk::Object> vSet;
  std::set<grctk::Object> eSet;

  size_t VC = g.vertexCount();
  for(size_t i = 0; i < VC; i++)
  {
    aSelected[g[i]] = true;
    vSet.insert(g[i]);
  }
  for(size_t i = 0; i < VC; i++)
  {
    for(size_t j = i+1; j < VC; j++)
    {
      if (g(j,i))
      {
        aSelected[g(j,i)] = true;
        eSet.insert(g(j,i));
      }
    }
  }

  grce->mainWindow->docControl->invalidateAttrs(vSet);
  grce->mainWindow->docControl->invalidateAttrs(eSet);

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void
GraphEdit::SelectAllVertexes()
{
  std::set<grctk::Object> vSet;
  std::set<grctk::Object> eSet;

  for(int k = 0; k < children(); k++)
  {
    VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
    if (pV)
    {
      aSelected[pV->vertexObject()] = true;
      vSet.insert(pV->vertexObject());
    }
  }

  grce->mainWindow->docControl->invalidateAttrs(vSet);
  grce->mainWindow->docControl->invalidateAttrs(eSet);

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void
GraphEdit::SelectAllEdges()
{
  std::set<grctk::Object> vSet;
  std::set<grctk::Object> eSet;

  for(int k = 0; k < children(); k++)
  {
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE)
    {
      aSelected[pE->edgeObject()] = true;
      eSet.insert(pE->edgeObject());
    }
  }

  grce->mainWindow->docControl->invalidateAttrs(vSet);
  grce->mainWindow->docControl->invalidateAttrs(eSet);

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void
GraphEdit::DeselectAllVertexes()
{
  std::set<grctk::Object> vSet;
  std::set<grctk::Object> eSet;

  for(int k = 0; k < children(); k++)
  {
    VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
    if (pV)
    {
      aSelected[pV->vertexObject()] = false;
      vSet.insert(pV->vertexObject());
    }
  }

  grce->mainWindow->docControl->invalidateAttrs(vSet);
  grce->mainWindow->docControl->invalidateAttrs(eSet);

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void
GraphEdit::DeselectAllEdges()
{
  std::set<grctk::Object> vSet;
  std::set<grctk::Object> eSet;

  for(int k = 0; k < children(); k++)
  {
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE)
    {
      aSelected[pE->edgeObject()] = false;
      eSet.insert(pE->edgeObject());
    }
  }

  grce->mainWindow->docControl->invalidateAttrs(vSet);
  grce->mainWindow->docControl->invalidateAttrs(eSet);

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void
GraphEdit::LabelAllVertexes()
{
  std::set<grctk::Object> vSet;
  std::set<grctk::Object> eSet;

  size_t i = 0;
  // for(int k = 0; k < children(); k++)
  for(int k = children()-1; k >=0; k--)
  {
    VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
    if (pV)
    {
      const grctk::Object& v = pV->vertexObject();
      if (aLabel[v] == "" || aLabel[v] == "VERTEX")
      {
        std::ostringstream os;
        os << i;
        i++;
        aLabel[v] = os.str();
        aLabelVisible[v] = true;
      }
      vSet.insert(pV->vertexObject());
    }
  }

  grce->mainWindow->docControl->invalidateAttrs(vSet);
  grce->mainWindow->docControl->invalidateAttrs(eSet);

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void
GraphEdit::ResetAllVertexWeights()
{
  for(int k = 0; k < children(); k++)
  {
    VertexWidget *pV = dynamic_cast<VertexWidget*>(child(k));
    if (pV)
    {
      const grctk::Object& v = pV->vertexObject();
      aDouble[v] = 1.0;
    }
  }

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void
GraphEdit::ResetAllEdgeWeights()
{
  for(int k = 0; k < children(); k++)
  {
    EdgeWidget *pE = dynamic_cast<EdgeWidget*>(child(k));
    if (pE)
      aDouble[pE->edgeObject()] = 1.0;
  }

  UpdateVertexes();
  UpdateEdges();
  redraw();
}

void
GraphEdit::ResetAllWeights()
{
  ResetAllVertexWeights();
  ResetAllEdgeWeights();
}

void
GraphEdit::DeselectAll()
{
  DeselectAllEdges();
  DeselectAllVertexes();
}

void
GraphEdit::SelectAll()
{
  SelectAllEdges();
  SelectAllVertexes();
}

char GraphEdit::default_sb_hint[1024] = "Hint: press <CTRL>+<R> to refresh,"
  " if screen become damaged or runs too slow";

}
