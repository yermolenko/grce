/*
  The GraphEdit class (header file).

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

#ifndef grce_GraphEdit_hpp
#define grce_GraphEdit_hpp

#include "grce/DocToolbox.hpp"
#include "grce/Attributes.hpp"
#include "grce/VertexWidget.hpp"
#include "grce/EdgeWidget.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Box.H>
#include <ostream>
#include <grctk/AdjMatrix.hpp>
#include <grctk/algo/formats/BinCode.hpp>

namespace grce
{

class GraphEdit : public Fl_Scroll
{
  static char default_sb_hint[1024];
public:
  struct MODE_INFO
  {
    const char *hint;
    GRCE_EDIT_MODE grce_edit_mode_;
    int mode_step_;
    Fl_Box* statusbar;

    grce::VertexWidget *v1, *v2;
    int ix, iy;
  }mode_info;

  GRCE_EDIT_MODE grce_edit_mode();
  void grce_edit_mode(GRCE_EDIT_MODE m);
  int  grce_edit_mode_step();
  void grce_edit_mode_step(int m);
  void mode_info_init();
  void next();
private:
  double Scale, XShift, YShift, ZShift;
  Fl_Box* boxOrigin;

  std::set<grctk::Object> objectsWithInvalidatedAttrs;
public:
  const grctk::AdjMatrix graphAsAdjMatrix() const;
  const grctk::AdjMatrix selectionAsAdjMatrix() const;
  int origin_x() const { return boxOrigin->x() + boxOrigin->w()/2; };
  int origin_y() const { return boxOrigin->y() + boxOrigin->h()/2 - 30 /*menuBar*/; };

  void Repair();

  void RemoveVisuals();
  void AddVisuals(const grctk::AdjMatrix&);
  void RemoveAddVisuals(const grctk::AdjMatrix&);

  void UpdateScale(bool fitToWindow = true);

  void UpdateVertexes();
  void UpdateVertexes(const std::vector<VertexWidget*>&);
  void UpdateEdges();
  void UpdateEdges(const std::vector<EdgeWidget*>&);
  void UpdateAll();

  void invalidateAttrs(const std::set<grctk::Object>&);
  void invalidateAttrs(const grctk::AdjMatrix&);
  void redrawInvalidated();

  double GetScale();
  void SetScale(double new_val);

  double GetXShift();
  void SetXShift(double new_val);

  double GetYShift();
  void SetYShift(double new_val);

  double GetZShift();
  void SetZShift(double new_val);

  int handle(int);

  int handle_move_vertex(int);
  int handle_create_vertex(int);
  int handle_remove_vertex(int);
  int handle_create_edge(int);
  int handle_remove_edge(int);
  int handle_vertex_properties(int);
  int handle_edge_properties(int);
  int handle_add_vertex_to_selection(int);
  int handle_add_edge_to_selection(int);

  void add_vertex();
  void remove_vertex();
  void add_edge();
  void delete_edge();

  void vertex_properties();
  void edge_properties();

  void selection_properties();

  void add_vertex_to_selection();
  void add_edge_to_selection();

  void SelectFromGraph(const grctk::AdjMatrix&);
  void SelectAllVertexes();
  void DeselectAllVertexes();
  void SelectAllEdges();
  void DeselectAllEdges();
  void SelectAll();
  void DeselectAll();

  void LabelAllVertexes();

  void ResetAllVertexWeights();
  void ResetAllEdgeWeights();
  void ResetAllWeights();

  GraphEdit(int, int, int, int,
            const char *title = 0,
            grctk::AdjMatrix = grctk::AdjMatrix());
  ~GraphEdit();

  void LoadFromGraph(const grctk::AdjMatrix&);
  void LoadFromGraphEdit(GraphEdit&);

  void set_statusbar(Fl_Box*);
};

}

#endif
