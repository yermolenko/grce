/*
  The VisBox class (header file).

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

#ifndef grce_VisBox_hpp
#define grce_VisBox_hpp

#include "grce/Color.hpp"
#include "grce/bmpImage.hpp"
#include <grctk/Universe.hpp>
#include <grctk/AdjMatrix.hpp>
#include <yaatk/Vector3D.hpp>
#include <yaatk/VectorXD.hpp>
#include <FL/fl_ask.H>
#ifdef __WIN32__
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <FL/Fl_Gl_Window.H>

namespace grce
{

class VisWindow;

class VisBox : public Fl_Gl_Window
{
public:
  VisWindow* parentVisWindow;
  bool allowRescale;
  unsigned long vertexColor,edgeColor,bgColor;
  bool showAxes;
  bool tinyVertices;
  bool showVertices;
  bool showEdges;
  bool showSelected;
  bool unifiedVertices;
  bool unifiedEdges;
  int  verticesQuality;
  int  verticesQualityInHQMode;
  bool hqMode;
  size_t selectedVertexIndex;
  grctk::Object getSelectedObject() {return g[selectedVertexIndex];}

  GLfloat nRange;
  double vertexRadius, axesRadius, scale, maxScale,
    XMin,XMax,YMin,YMax,ZMin,ZMax, XCenter, YCenter, ZCenter;

  GLfloat  light0_dir[4];
public:
  size_t getVertexCount(){return Ro.size();};
private:
  const grctk::AdjMatrix g;
  std::vector<yaatk::Vector3D> R,Ro;
private:
  void draw();
  void drawcube(int wire);
  void drawObjects();
  void setupLighting();
  void myglColor(Color, GLubyte alpha = 0xFF);

  void onResizeGL();

  GLuint lstBall;
  GLuint lstBallHQ;
  GLuint lstStick;
  GLuint lstStickHQ;
  void prepareBasicLists();

  void listVertices();
  void listAxes();
  void listEdges();
  void drawEdge(const yaatk::Vector3D& vi, const yaatk::Vector3D& vj,
                unsigned int color, double radius, GLubyte alpha = 255);
  void drawArrow(const yaatk::Vector3D& vi, const yaatk::Vector3D& vj,
                 unsigned int color, double radius,
                 double arrowPart = 0.2);

public:
  double old_rot_x;
  double old_rot_y;
  double old_rot_z;

  void  reArrange(double xmin, double xmax,
                  double ymin, double ymax,
                  double zmin, double zmax);

  void  rollAround(double,double,double,double);

  double RX(int i) {return R[i].x;}
  double RY(int i) {return R[i].y;}
  double RZ(int i) {return R[i].z;}

public:
  VisBox(const grctk::AdjMatrix& graph,int x,int y,int w,int h,
         const std::string title = "3D View");
  virtual ~VisBox() {}

  void loadNewRep(const std::vector<yaatk::Vector3D> &);
  void loadNewRep_and_size_range(const std::vector<yaatk::Vector3D> &);
  void setData(const std::vector<yaatk::Vector3D> &);

  void saveImageToFile(char* filename);
  void saveTiledImageToFile(char* filename);
  void openInEditor(std::string id, bool discardRotation = false);

  static void window_cb(Fl_Widget *, void *);

  bool tiledMode;
  int  tileCount;
  int  tileIndex[2];

  int handle(int event);
  int pickVertex(int x, int y);
};

}

#endif
