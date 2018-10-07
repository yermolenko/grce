/*
  The VisWindow class.

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

#include "VisWindow.hpp"
#include "grce/EnergyChart.hpp"
#include "grce/EnergySpectrum.hpp"
#include "grce/Attributes.hpp"
#include <sstream>
#include <fstream>
#include <ctime>

#define MAX_LOG_BUFFER_LEN 100000

namespace grce
{

using yaatk::VectorXD;

int
VisWindow::handle(int e)
{
  switch(e)
  {
  default:
    return Fl_Window::handle(e);
  }
}

void
VisWindow::current_vertexindex_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr = (VisWindow*)(w->parent()->parent()->parent());
  int new_index = int(((Fl_Counter *)w)->value());
  VisWindow_Ptr->setVertexViewIndex(new_index/*-1*/);
}

void
VisWindow::current_stateindex_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr = (VisWindow*)(w->parent());
  int new_index = int(((Fl_Counter *)w)->value());

  VisWindow_Ptr->loadNewSnapshot(new_index);
}

void
VisWindow::out(std::string s)
{
  const char * LogMsg = s.c_str();

  int dlen = strlen(LogMsg);
  if (log_pos+dlen < MAX_LOG_BUFFER_LEN-1)
  {
    strcat(log_buffer,LogMsg);
    log_pos += strlen(LogMsg);
    vertex_info->value(log_buffer);
    vertex_info->position(log_pos,log_pos);
    vertex_info->redraw();
  }
}

void
VisWindow::clear_out()
{
  log_pos = 0;
  strcpy(log_buffer,"");
  vertex_info->value(log_buffer);
  vertex_info->position(log_pos,log_pos);
  vertex_info->redraw();
}

void
VisWindow::setVertexViewIndex(int index)
{
  renderBox->selectedVertexIndex = index;
  current_vertexindex->value(index/*+1*/);

  grctk::Object v = renderBox->getSelectedObject();

  VectorXD posXD = stateList.getRep(stateIndex)[index];

  vertex_coords_x->value(posXD[0]);
  vertex_coords_y->value(posXD[1]);
  vertex_coords_z->value(posXD[2]);

  clear_out();

  std::ostringstream os;
  os << "Vertex #" << index << ":" << std::endl;

  using yaatk::operator<<;

  os TRACESS(posXD);
  os TRACESS(aColor[v]);
  os TRACESS(aSelected[v]);
  // os TRACESS_NO_ENDL(aLabel[v],"; ");
  os TRACESS(aLabel[v]);
  os TRACESS(aLabelVisible[v]);
  os TRACESS(aInt[v]);
  os TRACESS(aDouble[v]);

  out(os.str());

  renderBox->redraw();
}

VisWindow::VisWindow(const grctk::GraphMultiRep& repList, bool instantAnimate,
                     const std::string title):
  Fl_Window(700,535),
  baseId(title),
  stateList(repList),
  stateIndex(0)
{
  copy_label((title + " - 3D View Controls").c_str());

  log_buffer = new char[MAX_LOG_BUFFER_LEN];
  log_buffer[0]='\0';
  log_pos = 0;

  Fl_Tabs*
    tabs = new Fl_Tabs(5,5,w()-10,h()-50);
  Fl_Group*
    graph_view_group = new Fl_Group(
      tabs->x(),tabs->y()+20,tabs->w(),tabs->h()-20,
      "Controls and Info");
  graph_view_group->begin();

  renderBox = new VisBox(stateList.g,15,35,500,500,
                         title + " - 3D View");
  renderBox->parentVisWindow = this;
  graph_view_group->remove(renderBox);
  renderBox->set_non_modal();

  {
    Fl_Light_Button* t
      = new Fl_Light_Button(570,360,115,20,
                            "Auto rescale");
    t->callback((Fl_Callback*)btn_rescale_cb);
    t->value(true);
  }

  {
    Fl_Button* t
      = new Fl_Button(570,320,115,20,
                      "Scale Up");
    t->tooltip(btn_scale_up_tooltip);
    t->callback((Fl_Callback*)btn_scale_up_cb);
  }

  {
    Fl_Button* t
      = new Fl_Button(570,340,115,20,
                      "Scale Down");
    t->tooltip(btn_scale_down_tooltip);
    t->callback((Fl_Callback*)btn_scale_down_cb);
  }

  {
    Fl_Slider* t
      = new Fl_Slider(160, 320, 25, 130,
                      "Render\nquality");
    t->labelsize(12);
    t->minimum(16);
    t->maximum(3);
    t->value(14);
    t->step(1);
    t->type(FL_VERT_NICE_SLIDER);
    t->callback(set_int_and_invalidate_cb,
                &renderBox->verticesQuality);
  }

  {
    Fl_Light_Button* t
      = new Fl_Light_Button(195,320,105,20,
                            "Vertices");
    t->callback(btn_bool_toggle_cb,
                &renderBox->showVertices);
    t->value(renderBox->showVertices);
  }

  {
    Fl_Light_Button* t
      = new Fl_Light_Button(300,320,105,20,
                            "Edges");
    t->callback(btn_bool_toggle_cb,
                &renderBox->showEdges);
    t->value(renderBox->showEdges);
  }

  {
    Fl_Light_Button* t
      = new Fl_Light_Button(195,340,105,20,
                            "Unified");
    t->tooltip(btn_colored_vertices_tooltip);
    t->callback(btn_bool_toggle_cb,
                &renderBox->unifiedVertices);
    t->value(renderBox->unifiedVertices);
  }

  {
    Fl_Light_Button* t
      = new Fl_Light_Button(300,340,105,20,
                            "Unified");
    t->tooltip(btn_colored_edges_tooltip);
    t->callback(btn_bool_toggle_cb,
                &renderBox->unifiedEdges);
    t->value(renderBox->unifiedEdges);
  }

  {
    Fl_Button* t
      = new Fl_Button(195,360,105,20,
                      "Select Color");
    t->tooltip(btn_vertices_color_tooltip);
    t->callback((Fl_Callback*)btn_vertices_color_cb);
  }

  {
    Fl_Button* t
      = new Fl_Button(300,360,105,20,
                      "Select Color");
    t->tooltip(btn_edges_color_tooltip);
    t->callback((Fl_Callback*)btn_edges_color_cb);
  }

  {
    Fl_Button* t
      = new Fl_Button(195,380,210,20,
                      "Background");
    t->tooltip(btn_bg_color_tooltip);
    t->callback((Fl_Callback*)btn_bg_color_cb);
  }

  {
    Fl_Light_Button* t
      = new Fl_Light_Button(195,410,125,20,
                            "Tiny Vertices");
    t->callback(btn_bool_toggle_cb,
                &renderBox->tinyVertices);
    t->value(renderBox->tinyVertices);
  }

  {
    Fl_Light_Button* t
      = new Fl_Light_Button(320,410,85,20,
                            "Axes");
    t->tooltip(btn_show_axes_tooltip);
    t->callback(btn_bool_toggle_cb,
                &renderBox->showAxes);
    t->value(renderBox->showAxes);
  }

  {
    Fl_Box* t
      = new Fl_Box(FL_UP_FRAME,15,320,135,160,
                   "Light Direction");
    t->align(FL_ALIGN_TOP | FL_ALIGN_INSIDE);
  }

  {
    Fl_Slider* t
      = new Fl_Slider(30, 345, 25, 115,"X");
    t->labelsize(12);
    t->minimum(-1.0);
    t->maximum(+1.0);
    t->value(renderBox->light0_dir[0]);
    t->step(0.1);
    t->type(FL_VERT_NICE_SLIDER);
    t->callback(set_float_cb,
                renderBox->light0_dir+0);
  }

  {
    Fl_Slider* t
      = new Fl_Slider(70, 345, 25, 115,"Y");
    t->labelsize(12);
    t->minimum(-1.0);
    t->maximum(+1.0);
    t->value(renderBox->light0_dir[1]);
    t->step(0.1);
    t->type(FL_VERT_NICE_SLIDER);
    t->callback(set_float_cb,
                renderBox->light0_dir+1);
  }

  {
    Fl_Slider* t
      = new Fl_Slider(110, 345, 25, 115,"Z");
    t->labelsize(12);
    t->minimum(-1.0);
    t->maximum(+1.0);
    t->value(renderBox->light0_dir[2]);
    t->step(0.1);
    t->type(FL_VERT_NICE_SLIDER);
    t->callback(set_float_cb,
                renderBox->light0_dir+2);
  }

  {
    Fl_Button* t
      = new Fl_Button(415,320,145,20,
                      "Open Editor (rot)");
    t->tooltip(btn_open_editor_rot_tooltip);
    t->callback((Fl_Callback*)btn_open_in_editor_cb, (void*)0);
  }

  {
    Fl_Button* t
      = new Fl_Button(415,340,145,20,
                      "Open Editor");
    t->tooltip(btn_open_editor_tooltip);
    t->callback((Fl_Callback*)btn_open_in_editor_cb, (void*)1);
  }

  {
    Fl_Button* t
      = new Fl_Button(415,370,145,20,
                      "Save image");
    t->tooltip(btn_save_image_tooltip);
    t->callback((Fl_Callback*)btn_save_image_cb);
  }

  {
    Fl_Button* t
      = new Fl_Button(415,390,145,20,
                      "Save hi-res image");
    t->callback((Fl_Callback*)btn_save_tiled_image_cb);
  }

  {
    Fl_Button* t
      = new Fl_Button(415,420,145,20,
                      "Save video");
    t->tooltip(btn_save_video_tooltip);
    t->callback((Fl_Callback*)btn_save_video_cb);
  }

  {
    Fl_Box* t
      = new Fl_Box(FL_EMBOSSED_FRAME,415,35,270,280,
                   "Rotate and clip");
    t->align(FL_ALIGN_TOP | FL_ALIGN_INSIDE);
  }

  roll_x = new Fl_Roller(455, 80, 20, 215, "X");
  roll_x->tooltip(roll_x_tooltip);
  roll_x->type(0);
  roll_x->labelsize(12);
  roll_x->minimum(0);
  roll_x->maximum(360);
  roll_x->value(0);
  roll_x->step(1);
  roll_x->callback((Fl_Callback*)roll_x_cb);

  val_xmin = new Fl_Slider(430, 80, 25, 215, "min");
  val_xmin->type(FL_VERT_NICE_SLIDER);
  val_xmin->labelsize(12);
  val_xmin->minimum(101);
  val_xmin->maximum(-1);
  val_xmin->value(0);
  val_xmin->step(0.1);
  val_xmin->when(FL_WHEN_RELEASE);
  val_xmin->callback((Fl_Callback*)val_xminmax_cb);

  val_xmax = new Fl_Slider(475, 80, 25, 215, "max");
  val_xmax->type(FL_VERT_NICE_SLIDER);
  val_xmax->labelsize(12);
  val_xmax->minimum(101);
  val_xmax->maximum(-1);
  val_xmax->value(100);
  val_xmax->step(0.1);
  val_xmax->when(FL_WHEN_RELEASE);
  val_xmax->callback((Fl_Callback*)val_xminmax_cb);

  roll_y = new Fl_Roller(540, 80, 20, 215, "Y");
  roll_y->tooltip(roll_y_tooltip);
  roll_y->type(0);
  roll_y->labelsize(12);
  roll_y->minimum(0);
  roll_y->maximum(360);
  roll_y->value(0);
  roll_y->step(1);
  roll_y->callback((Fl_Callback*)roll_y_cb);

  val_ymin = new Fl_Slider(515, 80, 25, 215, "min");
  val_ymin->type(FL_VERT_NICE_SLIDER);
  val_ymin->labelsize(12);
  val_ymin->minimum(101);
  val_ymin->maximum(-1);
  val_ymin->value(0);
  val_ymin->step(0.1);
  val_ymin->when(FL_WHEN_RELEASE);
  val_ymin->callback((Fl_Callback*)val_xminmax_cb);

  val_ymax = new Fl_Slider(560, 80, 25, 215, "max");
  val_ymax->type(FL_VERT_NICE_SLIDER);
  val_ymax->labelsize(12);
  val_ymax->minimum(101);
  val_ymax->maximum(-1);
  val_ymax->value(100);
  val_ymax->step(0.1);
  val_ymax->when(FL_WHEN_RELEASE);
  val_ymax->callback((Fl_Callback*)val_xminmax_cb);

  roll_z = new Fl_Roller(625, 80, 20, 215, "Z");
  roll_z->tooltip(roll_z_tooltip);
  roll_z->type(0);
  roll_z->labelsize(12);
  roll_z->minimum(0);
  roll_z->maximum(360);
  roll_z->value(0);
  roll_z->step(1);
  roll_z->callback((Fl_Callback*)roll_z_cb);

  val_zmin = new Fl_Slider(600, 80, 25, 215, "min");
  val_zmin->type(FL_VERT_NICE_SLIDER);
  val_zmin->labelsize(12);
  val_zmin->minimum(101);
  val_zmin->maximum(-1);
  val_zmin->value(0);
  val_zmin->step(0.1);
  val_zmin->when(FL_WHEN_RELEASE);
  val_zmin->callback((Fl_Callback*)val_xminmax_cb);

  val_zmax = new Fl_Slider(645, 80, 25, 215, "max");
  val_zmax->type(FL_VERT_NICE_SLIDER);
  val_zmax->labelsize(12);
  val_zmax->minimum(101);
  val_zmax->maximum(-1);
  val_zmax->value(100);
  val_zmax->step(0.1);
  val_zmax->when(FL_WHEN_RELEASE);
  val_zmax->callback((Fl_Callback*)val_xminmax_cb);

  {
    Fl_Button* t
      = new Fl_Button(430, 55, 25*2+20, 20,
                      "45\xB0 rot");
    t->callback(btn_view_cb,roll_x);
  }

  {
    Fl_Button* t
      = new Fl_Button(515, 55, 25*2+20, 20,
                      "45\xB0 rot");
    t->callback(btn_view_cb,roll_y);
  }

  {
    Fl_Button* t
      = new Fl_Button(600, 55, 25*2+20, 20,
                      "45\xB0 rot");
    t->callback(btn_view_cb,roll_z);
  }

  {
    new Fl_Box(FL_UP_FRAME,15,35,385+5,235+45,NULL);
  }

  vertex_info = new Fl_Multiline_Output(25,55,370,190,
                                        "Graph element info");
  vertex_info->textcolor(FL_BLUE);
  vertex_info->align(FL_ALIGN_TOP);
  vertex_info->value("");

  vertex_coords_x = new Fl_Value_Input(25+40,255,110,20,"Pos:");
  vertex_coords_x->range(-10000,+10000);
  vertex_coords_x->precision(4);
  vertex_coords_x->align(FL_ALIGN_LEFT);
#ifdef GRCE_NOT_TO_PORT
  vertex_coords_x->callback(set_vertex_properties_cb);
#endif

  vertex_coords_y = new Fl_Value_Input(25+40+110*1,255,110,20,"");
  vertex_coords_y->range(-10000,+10000);
  vertex_coords_y->precision(4);
  vertex_coords_y->align(FL_ALIGN_LEFT);
#ifdef GRCE_NOT_TO_PORT
  vertex_coords_y->callback(set_vertex_properties_cb);
#endif

  vertex_coords_z = new Fl_Value_Input(25+40+110*2,255,110,20,"");
  vertex_coords_z->range(-10000,+10000);
  vertex_coords_z->precision(4);
  vertex_coords_z->align(FL_ALIGN_LEFT);
#ifdef GRCE_NOT_TO_PORT
  vertex_coords_z->callback(set_vertex_properties_cb);
#endif

  current_vertexindex = new Fl_Counter(25,285,220,20,NULL);
  current_vertexindex->align(FL_ALIGN_LEFT);
  current_vertexindex->lstep(100);
  current_vertexindex->step(1);
  current_vertexindex->precision(0);
  current_vertexindex->bounds(0,repList.g.size()-1);
  current_vertexindex->value(0);
  current_vertexindex->callback((Fl_Callback*)current_vertexindex_cb);

  {
    Fl_Light_Button* t
      = new Fl_Light_Button(255,285,140,20,
                            "Highlight");
    t->callback(btn_bool_toggle_cb,
                &renderBox->showSelected);
    t->value(renderBox->showSelected);
  }

  graph_view_group->end();

  {
    std::vector<double> EL = repList.attrArray();

    Fl_Group*
      energy_chart_group = new Fl_Group(
        tabs->x(),tabs->y()+20,tabs->w(),tabs->h()-20,
        "Energy = f(representation)");
    energy_chart_group->begin();
    energy_chart = new EnergyChart(
      energy_chart_group->x()+5,energy_chart_group->y()+5,
      energy_chart_group->w()-10,energy_chart_group->h()-10,EL);
    energy_chart_group->end();

    Fl_Group*
      energy_spectrum_group = new Fl_Group(
        tabs->x(),tabs->y()+20,tabs->w(),tabs->h()-20,
        "dN/dE");
    energy_spectrum_group->begin();

    energy_spectrum = new EnergySpectrum(
      energy_spectrum_group->x()+5,energy_spectrum_group->y()+5,
      energy_spectrum_group->w()-10,energy_spectrum_group->h()-10-35,
      EL,EL.size()*10);

    Fl_Value_Output*
      min_energy = new Fl_Value_Output(
        energy_spectrum->x()+60,energy_spectrum->y()+energy_spectrum->h()+5,
        160,25,
        "Emin = ");
    min_energy->align(FL_ALIGN_LEFT);
    min_energy->precision(20);
    min_energy->value(energy_chart->Emin());

    Fl_Value_Output*
      max_energy = new Fl_Value_Output(
        energy_spectrum->x()+energy_spectrum->w()-160,energy_spectrum->y()+energy_spectrum->h()+5,
        160,25,
        "Emax = ");
    max_energy->align(FL_ALIGN_LEFT);
    max_energy->precision(20);
    max_energy->value(energy_chart->Emax());

    energy_spectrum_group->end();
  }

  tabs->end();

  btn_animate = new Fl_Light_Button(
    tabs->x(),tabs->y()+tabs->h()+10,105,25,
    "Animate");
  btn_animate->callback((Fl_Callback*)btn_animate_cb);
  btn_animate->value(instantAnimate);

  animate_delay = new Fl_Counter(
    btn_animate->x()+btn_animate->w()+40, btn_animate->y(), 85, 25,
    "Delay");
  animate_delay->labelsize(12);
  animate_delay->minimum(0.01);
  animate_delay->maximum(5.0);
  animate_delay->value(1.0);
  animate_delay->lstep(0.5);
  animate_delay->step(0.01);
  animate_delay->align(FL_ALIGN_LEFT);
  // animate_delay->type(FL_SIMPLE_COUNTER);

  current_stateindex = new Fl_Counter(
    tabs->x()+tabs->w()-265,tabs->y()+tabs->h()+10,150,25,
    "Representation #");
  // current_stateindex->align(FL_ALIGN_TOP);
  current_stateindex->align(FL_ALIGN_LEFT);
  current_stateindex->lstep(10);
  current_stateindex->step(1);
  current_stateindex->precision(0);
  if (stateList.size() > 0)
    current_stateindex->bounds(0,stateList.size()-1);
  else
    current_stateindex->bounds(0,0);
  current_stateindex->value(0);
  current_stateindex->callback((Fl_Callback*)current_stateindex_cb);

  {
    Fl_Button* t
      = new Fl_Button(
        current_stateindex->x()+current_stateindex->w()+10,current_stateindex->y(),
        105,25,
        "Go minimum!");
    // t->tooltip(btn_go_minimum_tooltip);
    t->callback((Fl_Callback*)btn_go_minimum_cb);
  }

  end();

  renderBox->show();

  show();
  renderBox->redraw();
  redraw();
  renderBox->invalidate();
  renderBox->hide();
  renderBox->show();

  // renderBox->allowRescale = false;
  renderBox->allowRescale = true;
//  renderBox->reArrange(-1,101,-1,101,-1,101);
  renderBox->redraw();

  callback(window_cb);
  { // rotate around x axis by 4*45=180 degrees
    for(size_t ri = 0; ri < 4; ri++)
    {
      renderBox->make_current();
      btn_view_cb(roll_x,roll_x);
      renderBox->redraw();
    }
  }
  if (btn_animate->value()) Fl::add_timeout(1.0, timer_callback);

  if (stateList.size() > 0)
    loadNewSnapshot(stateList.findRepWithMinEnergy());
}

VisWindow::~VisWindow()
{
  if (btn_animate->value() && this == VisWindow_GlobalPtr)
    VisWindow_GlobalPtr = NULL;
  delete renderBox;
}

std::string
VisWindow::proposedId() const
{
  std::ostringstream ossNewId;
  ossNewId << baseId;
  if (stateList.size() > 1)
  {
    ossNewId << "-" << current_stateindex->value();
    if (current_stateindex->value() == stateList.findRepWithMinEnergy())
      ossNewId << "-min";
  }
  return ossNewId.str();
}

void
VisWindow::redrawGL()
{
  renderBox->redraw();
}

void
VisWindow::btn_save_image_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  VisBox* VisBox_Ptr;
  VisBox_Ptr = VisWindow_Ptr->renderBox;

  std::string idGuess = VisWindow_Ptr->proposedId();

  char fname[1000];
#ifdef GRCE_PNG
  sprintf(fname,"%.50s-small.png", idGuess.c_str());
  char *tmp_filename = fl_file_chooser
    (
      "Choose a file to save image to ...",
      "PNG files (*.png)",
      fname,0
      );
#else
  sprintf(fname,"%.50s-small.bmp", idGuess.c_str());
  char *tmp_filename = fl_file_chooser
    (
      "Choose a file to save image to ...",
      "Windows Bitmap Files (*.bmp)",
      fname,0
      );
#endif
  if (tmp_filename && (!fl_filename_isdir(tmp_filename)))
  {
    VisBox_Ptr->saveImageToFile(tmp_filename);

    VisBox_Ptr->make_current();
  }
}

void
VisWindow::btn_save_tiled_image_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  VisBox* VisBox_Ptr;
  VisBox_Ptr = VisWindow_Ptr->renderBox;

  std::string idGuess = VisWindow_Ptr->proposedId();

  char fname[1000];
#ifdef GRCE_PNG
  sprintf(fname,"%.50s.png", idGuess.c_str());
  char *tmp_filename = fl_file_chooser
    (
      "Choose a file to save image to ...",
      "PNG files (*.png)",
      fname,0
      );
#else
  sprintf(fname,"%.50s.bmp", idGuess.c_str());
  char *tmp_filename = fl_file_chooser
    (
      "Choose a file to save image to ...",
      "Windows Bitmap Files (*.bmp)",
      fname,0
      );
#endif
  if (tmp_filename && (!fl_filename_isdir(tmp_filename)))
  {
    VisBox_Ptr->saveTiledImageToFile(tmp_filename);

    VisBox_Ptr->make_current();
  }
}

void
VisWindow::quickSaveBitmap()
{
  char tmp_filename[4096];

#ifdef GRCE_PNG
  sprintf(tmp_filename,"%010d.png",stateIndex);
#else
  sprintf(tmp_filename,"%010d.bmp",stateIndex);
#endif

  if (!fl_filename_isdir(tmp_filename))
  {
    renderBox->saveImageToFile(tmp_filename);
  }
}

void
VisWindow::btn_save_video_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  // VisBox* VisBox_Ptr;
  // VisBox_Ptr = VisWindow_Ptr->renderBox;

  char *tmp_filename = fl_dir_chooser
    (
      "Choose a directory to save video to ...",
      "",0
      );
  if (tmp_filename && (fl_filename_isdir(tmp_filename)))
  {
    std::string trajdir = yaatk::getcwd();
    yaatk::chdir(tmp_filename);

    char videoDirName[1024];
    bool videoDirExists = true;
    int videoDirIndex = 0;
    while (videoDirExists)
    {
      std::string fname_prefix = VisWindow_Ptr->baseId;
      sprintf(videoDirName,"%s-video%02d",fname_prefix.c_str(),videoDirIndex);
      if (fl_filename_isdir(videoDirName))
        videoDirIndex++;
      else
        videoDirExists = false;
      REQUIRE(videoDirIndex < 100);
    }

    yaatk::mkdir(videoDirName);
    yaatk::chdir(trajdir.c_str());
    for(size_t i = 0; i < VisWindow_Ptr->stateList.size()/*-1*/; i++)
    {
//      VisWindow_Ptr->loadNewState(i);
      VisWindow_Ptr->current_stateindex->value(i);
      current_stateindex_cb(VisWindow_Ptr->current_stateindex,NULL);
      yaatk::chdir(tmp_filename);
      yaatk::chdir(videoDirName);
      while (!Fl::ready()) {};
      Fl::flush();
      while (!Fl::ready()) {};
      VisWindow_Ptr->quickSaveBitmap();
//      sleep(1);
      yaatk::chdir(trajdir.c_str());
    }
  }
}

void
VisWindow::btn_open_in_editor_cb(Fl_Widget *w, void *p)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  VisBox* VisBox_Ptr;
  VisBox_Ptr = VisWindow_Ptr->renderBox;

  bool discardRotation = (bool) p;

  std::string idGuess = VisWindow_Ptr->proposedId();
  const char *id = fl_input("Id of the new state:", idGuess.c_str());
  if (id)
    VisBox_Ptr->openInEditor(id, discardRotation);
}

void
VisWindow::btn_view_cb(Fl_Widget *, void *p)
{
  Fl_Roller* roll = (Fl_Roller*) p;
  double v = roll->value();
  v -= 45; if (v <= 0) v = 360+v;
  roll->value(v);
  roll->callback()(roll,NULL);
}

void
VisWindow::btn_go_minimum_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent());
  VisBox* VisBox_Ptr
    = VisWindow_Ptr->renderBox;

  // if (VisWindow_Ptr->stateList.size() > 0)
  VisWindow_Ptr->loadNewSnapshot(VisWindow_Ptr->stateList.findRepWithMinEnergy());

  VisBox_Ptr->redraw();
}

void
VisWindow::btn_scale_up_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());
  VisBox* VisBox_Ptr
    = VisWindow_Ptr->renderBox;

  if (1.2*VisBox_Ptr->scale<=VisBox_Ptr->maxScale)
    VisBox_Ptr->scale = 1.2*VisBox_Ptr->scale;

  VisBox_Ptr->redraw();
}

void
VisWindow::btn_scale_down_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());
  VisBox* VisBox_Ptr = VisWindow_Ptr->renderBox;

  VisBox_Ptr->scale = VisBox_Ptr->scale/1.2;
  VisBox_Ptr->redraw();
}

void
VisWindow::btn_bool_toggle_cb(Fl_Widget *w, void *pbool)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  bool v = ((Fl_Light_Button *)w)->value();
  *((bool*)pbool) = v;
  VisWindow_Ptr->renderBox->redraw();
}

void
VisWindow::set_double_cb(Fl_Widget *w, void *pdouble)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  double v = ((Fl_Slider *)w)->value();
  *((double*)pdouble) = v;

  VisWindow_Ptr->renderBox->redraw();
}

void
VisWindow::set_float_cb(Fl_Widget *w, void *pfloat)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  float v = ((Fl_Slider *)w)->value();
  *((float*)pfloat) = v;

  VisWindow_Ptr->renderBox->redraw();
}

void
VisWindow::set_int_cb(Fl_Widget *w, void *pint)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  int v = ((Fl_Slider *)w)->value();
  *((int*)pint) = v;

  VisWindow_Ptr->renderBox->redraw();
}

void
VisWindow::set_int_and_invalidate_cb(Fl_Widget *w, void *pint)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  int v = ((Fl_Slider *)w)->value();
  *((int*)pint) = v;

  VisWindow_Ptr->renderBox->invalidate();
  VisWindow_Ptr->renderBox->redraw();
}

void
VisWindow::roll_x_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  double new_rot_x,old_rot_x;
  new_rot_x = ((Fl_Roller *)w)->value();
  old_rot_x = VisWindow_Ptr->renderBox->old_rot_x;

  VisWindow_Ptr->renderBox->
    rollAround(new_rot_x - old_rot_x,1,0,0);
}

void
VisWindow::roll_y_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  double new_rot_y,old_rot_y;
  new_rot_y = ((Fl_Roller *)w)->value();
  old_rot_y = VisWindow_Ptr->renderBox->old_rot_y;

  VisWindow_Ptr->renderBox->
    rollAround(new_rot_y - old_rot_y,0,1,0);
}

void
VisWindow::roll_z_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  double new_rot_z,old_rot_z;
  new_rot_z = ((Fl_Roller *)w)->value();
  old_rot_z = VisWindow_Ptr->renderBox->old_rot_z;

  VisWindow_Ptr->renderBox->
    rollAround(new_rot_z - old_rot_z,0,0,1);
}

void
VisWindow::val_xminmax_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  VisWindow_Ptr->renderBox->
    reArrange(
      VisWindow_Ptr->val_xmin->value()/100.0,
      VisWindow_Ptr->val_xmax->value()/100.0,
      VisWindow_Ptr->val_ymin->value()/100.0,
      VisWindow_Ptr->val_ymax->value()/100.0,
      VisWindow_Ptr->val_zmin->value()/100.0,
      VisWindow_Ptr->val_zmax->value()/100.0
      );
}

void
VisWindow::btn_rescale_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());

  bool v = ((Fl_Light_Button *)w)->value();
  VisWindow_Ptr->renderBox->allowRescale = v;
  VisWindow_Ptr->renderBox->reArrange(-1,101,-1,101,-1,101);
  VisWindow_Ptr->renderBox->redraw();
}

void
VisWindow::btn_animate_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent());

  bool v = ((Fl_Light_Button *)w)->value();
  if (v)
  {
    if (VisWindow_GlobalPtr)
      VisWindow_GlobalPtr->btn_animate->value(false);
    VisWindow_GlobalPtr = VisWindow_Ptr;
  }

  if (v) Fl::repeat_timeout(0.5, timer_callback);
}

void
VisWindow::timer_callback(void*)
{
  puts("TICK");

  if (VisWindow_GlobalPtr && VisWindow_GlobalPtr->btn_animate->value())
  {
    grce::VisWindow* VisWindow_Ptr;
    VisWindow_Ptr = VisWindow_GlobalPtr;

    int oldval = VisWindow_Ptr->current_stateindex->value();
    int maxval = VisWindow_Ptr->current_stateindex->maximum();
    VisWindow_Ptr->current_stateindex->value((oldval+1)%(maxval+1));
    current_stateindex_cb(VisWindow_Ptr->current_stateindex,NULL);

    double delay = VisWindow_Ptr->animate_delay->value();

    if (VisWindow_Ptr->btn_animate->value()) Fl::repeat_timeout(delay, timer_callback);
  }
}

void
VisWindow::btn_bg_color_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());
  Color c = VisWindow_Ptr->renderBox->bgColor;

  unsigned char r,g,b;
  c.analyseRGB(r,g,b);

  if (fl_color_chooser("Background Color",r,g,b))
  {
    c = Color(r,g,b);
    VisWindow_Ptr->renderBox->bgColor = c;
    VisWindow_Ptr->renderBox->redraw();
  }
}

void
VisWindow::btn_vertices_color_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());
  Color c = VisWindow_Ptr->renderBox->vertexColor;

  unsigned char r,g,b;
  c.analyseRGB(r,g,b);

  if (fl_color_chooser("Vertex Color",r,g,b))
  {
    c = Color(r,g,b);
    VisWindow_Ptr->renderBox->vertexColor = c;
    VisWindow_Ptr->renderBox->redraw();
  }
}

void
VisWindow::btn_edges_color_cb(Fl_Widget *w, void *)
{
  VisWindow* VisWindow_Ptr;
  VisWindow_Ptr =
    (VisWindow*)(w->parent()->parent()->parent());
  Color c = VisWindow_Ptr->renderBox->edgeColor;

  unsigned char r,g,b;
  c.analyseRGB(r,g,b);

  if (fl_color_chooser("Edge Color",r,g,b))
  {
    c = Color(r,g,b);
    VisWindow_Ptr->renderBox->edgeColor = c;
    VisWindow_Ptr->renderBox->redraw();
  }
}

void
VisWindow::window_cb(Fl_Widget* widget, void*)
{
  ((Fl_Window*)widget)->hide();
  delete widget;
}

const char* VisWindow::
btn_show_axes_tooltip = "Show/Hide Axes";
const char* VisWindow::
btn_colored_vertices_tooltip = "Toggle Colored/Monochrome Vertices";
const char* VisWindow::
btn_colored_edges_tooltip = "Toggle Colored/Monochrome Edges";
const char* VisWindow::
roll_x_tooltip = "Rolls around X-axis";
const char* VisWindow::
roll_y_tooltip = "Rolls around Y-axis";
const char* VisWindow::
roll_z_tooltip = "Rolls around Z-axis";

const char* VisWindow::
btn_bg_color_tooltip = "Choose Background Color";
const char* VisWindow::
btn_vertices_color_tooltip = "Choose Unified Vertices Color";
const char* VisWindow::
btn_edges_color_tooltip = "Choose Unified Edges Color";

const char* VisWindow::
btn_save_image_tooltip = "Save image to a file";

const char* VisWindow::
btn_scale_up_tooltip = "Scale = Scale*1.2";
const char* VisWindow::
btn_scale_down_tooltip = "Scale = Scale/1.2";

const char* VisWindow::
btn_open_editor_rot_tooltip = "Open in Graph Editor (taking to account rotation in this window)";
const char* VisWindow::
btn_open_editor_tooltip = "Open in Graph Editor";

const char* VisWindow::
btn_save_video_tooltip = "Save representaions as a sequence of images";

void
VisWindow::loadNewSnapshot(int index)
{
  stateIndex = index;

  renderBox->loadNewRep(stateList.getRep3DProj(stateIndex));

  energy_chart->set_current_index(stateIndex);
  energy_spectrum->set_current_index(stateIndex);

  if (stateIndex != current_stateindex->value())
    current_stateindex->value(stateIndex);

  setVertexViewIndex(int(current_vertexindex->value())/*-1*/);
}

}

grce::VisWindow* VisWindow_GlobalPtr = NULL;
