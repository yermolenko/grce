/*
  The VisWindow class (header file).

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

#ifndef grce_VisWindow_hpp
#define grce_VisWindow_hpp

#include "grce/VisBox.hpp"
#include "grce/EnergyChart.hpp"
#include "grce/EnergySpectrum.hpp"
#include "grce/bmpImage.hpp"
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Roller.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Chart.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Radio_Light_Button.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Enumerations.H>
#include <FL/gl.h>
#include <FL/fl_ask.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_File_Chooser.H>
#include <grctk/algo/drawing/pairpot/PairPot.hpp>
#include <vector>

namespace grce
{

class VisBox;

class VisWindow : public Fl_Window
{
  Fl_Light_Button* btn_animate;
  Fl_Counter *animate_delay;

  Fl_Counter* current_vertexindex;
  Fl_Counter* current_stateindex;

  VisBox *renderBox;

  EnergyChart *energy_chart;
  EnergySpectrum *energy_spectrum;

  Fl_Multiline_Output* vertex_info;

  Fl_Roller *roll_x;
  Fl_Roller *roll_y;
  Fl_Roller *roll_z;

  Fl_Slider *val_xmin;
  Fl_Slider *val_xmax;
  Fl_Slider *val_ymin;
  Fl_Slider *val_ymax;
  Fl_Slider *val_zmin;
  Fl_Slider *val_zmax;

  static void btn_bool_toggle_cb(Fl_Widget *, void *);
  static void set_double_cb(Fl_Widget *, void *);
  static void set_float_cb(Fl_Widget *, void *);
  static void set_int_cb(Fl_Widget *, void *);
  static void set_int_and_invalidate_cb(Fl_Widget *, void *);

  static void current_vertexindex_cb(Fl_Widget *, void *);
  static void current_stateindex_cb(Fl_Widget *, void *);

  Fl_Value_Input* vertex_coords_x;
  Fl_Value_Input* vertex_coords_y;
  Fl_Value_Input* vertex_coords_z;
  static void set_vertex_properties_cb(Fl_Widget *, void *);

  static void roll_x_cb(Fl_Widget *, void *);
  static void roll_y_cb(Fl_Widget *, void *);
  static void roll_z_cb(Fl_Widget *, void *);

  static void btn_view_cb(Fl_Widget *, void *);

  static void val_xminmax_cb(Fl_Widget *, void *);

  static void btn_rescale_cb(Fl_Widget *, void *);
  static void timer_callback(void *);
  static void btn_animate_cb(Fl_Widget *, void *);

  static void btn_bg_color_cb(Fl_Widget *, void *);
  static void btn_vertices_color_cb(Fl_Widget *, void *);
  static void btn_edges_color_cb(Fl_Widget *, void *);

  static void btn_save_image_cb(Fl_Widget *, void *);
  static void btn_save_tiled_image_cb(Fl_Widget *, void *);
  static void btn_save_video_cb(Fl_Widget *, void *);
  static void btn_open_in_editor_cb(Fl_Widget *, void *);

  static void btn_scale_up_cb(Fl_Widget *, void *);
  static void btn_scale_down_cb(Fl_Widget *, void *);

  static void btn_go_minimum_cb(Fl_Widget *, void *);

  static void window_cb(Fl_Widget *, void *);

  void quickSaveBitmap();

  std::string baseId;
public:
  int  handle(int);
  VisWindow(const grctk::GraphMultiRep&, bool instantAnimate,
            const std::string title = "Graph");
  void updateStateList();
  ~VisWindow();
  std::string proposedId() const;
  void redrawGL();
  void setVertexViewIndex(int index);

  grctk::GraphMultiRep stateList;
  int stateIndex;
  void loadNewSnapshot(int index);

  char *log_buffer;
  int log_pos;

  void out(std::string);
  void clear_out();

  static const char
  *btn_show_axes_tooltip,
    *btn_colored_vertices_tooltip,
    *btn_colored_edges_tooltip,
    *roll_x_tooltip,
    *roll_y_tooltip,
    *roll_z_tooltip,
    *btn_vertices_color_tooltip,
    *btn_edges_color_tooltip,
    *btn_bg_color_tooltip,
    *btn_save_image_tooltip,
    *btn_scale_up_tooltip,
    *btn_scale_down_tooltip,
    *btn_open_editor_rot_tooltip,
    *btn_open_editor_tooltip,
    *btn_save_video_tooltip;
};

}

extern grce::VisWindow* VisWindow_GlobalPtr;

#endif
