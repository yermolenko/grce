/*
  The VisBox class.

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

#include "VisBox.hpp"
#include "grce/VisWindow.hpp"
#include "grce/Attributes.hpp"
#include "grce/GRCE.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <FL/Fl.H>
#ifdef GRCE_PNG
#include <png.h>
#endif
#include <stdexcept>

namespace grce
{

using yaatk::Vector3D;

void
VisBox::loadNewRep(const std::vector<yaatk::Vector3D> &rep)
{
  {
    bool allowRescale_bak = allowRescale;
    // if (old_atoms_count != atoms.size())
    // {
    //   allowRescale = true;
    // }

    setData(rep);

    allowRescale = allowRescale_bak;
  }

  redraw();
}

void
VisBox::loadNewRep_and_size_range(const std::vector<Vector3D> &rep)
{
  setData(rep);
  size_range(100, 100, 5000, 5000, 3*4, 3*4, 1);
}

VisBox::VisBox(const grctk::AdjMatrix& graph,
               int x,int y,int w,int h,const std::string title)
  : Fl_Gl_Window(x,y,w,h),
    parentVisWindow(NULL),
    allowRescale(true),
    vertexColor(Color(255,255,255)),
    edgeColor(Color(255,255,255)),
    bgColor(Color(255,255,255)),
    showAxes(false),
    tinyVertices(false),
    showVertices(true),
    showEdges(true),
    showSelected(false),
    unifiedVertices(false),
    unifiedEdges(false),
    verticesQuality(14),
    verticesQualityInHQMode(50),
    hqMode(false),
    selectedVertexIndex(0),
    nRange(50),
    vertexRadius(1.0), axesRadius(1.0), scale(1.0), maxScale(1.0),
    g(graph),
    R(),Ro(),
    lstBall(0),
    lstBallHQ(0),
    lstStick(0),
    lstStickHQ(0),
    old_rot_x(0.0), old_rot_y(0.0), old_rot_z(0.0),
    tiledMode(false),tileCount(8)
{
  mode(FL_RGB | FL_DOUBLE | FL_ACCUM | FL_ALPHA | FL_DEPTH | FL_MULTISAMPLE);
  end();

  copy_label(title.c_str());

  light0_dir[0] = 1.0;
  light0_dir[1] = -1.0;
  light0_dir[2] = -1.0;
  light0_dir[3] = 0.0;

  callback(window_cb);

  std::vector<Vector3D> Rx;
  Rx.resize(g.size());
  for(size_t i = 0; i < g.size(); ++i)
    Rx[i] = a3D[g[i]];

  // loadNewRep_and_size_range(Rx);
  setData(Rx);
}

void
VisBox::reArrange(double xmin, double xmax,
                  double ymin, double ymax,
                  double zmin, double zmax)
{
  double xmin_ = XMin+(XMax-XMin)*xmin;
  double xmax_ = XMin+(XMax-XMin)*xmax;
  double ymin_ = YMin+(YMax-YMin)*ymin;
  double ymax_ = YMin+(YMax-YMin)*ymax;
  double zmin_ = ZMin+(ZMax-ZMin)*zmin;
  double zmax_ = ZMin+(ZMax-ZMin)*zmax;

  int i;
  int VC = Ro.size();

  R.resize(0);

  for(i=0;i<VC;i++)
  {
    if (Ro[i].x >= xmin_ && Ro[i].x <= xmax_ &&
        Ro[i].y >= ymin_ && Ro[i].y <= ymax_ &&
        Ro[i].z >= zmin_ && Ro[i].z <= zmax_)
    {
      R.push_back(Ro[i]);
    }
  }

  GLdouble /*DistMin,*/DistMax,XMin,XMax,YMin,YMax,ZMin,ZMax;

  VC = R.size();

  XMin=R[0].x;
  XMax=R[0].x;
  YMin=R[0].y;
  YMax=R[0].y;
  ZMin=R[0].z;
  ZMax=R[0].z;
  for(int i=0;i<VC;i++)
  {
    if (R[i].x<XMin)
    {
      XMin=R[i].x;
    }
    else
      if (R[i].x>XMax)
      {
        XMax=R[i].x;
      };
    if (R[i].y<YMin)
    {
      YMin=R[i].y;
    }
    else
      if (R[i].y>YMax)
      {
        YMax=R[i].y;
      };
    if (R[i].z<ZMin)
    {
      ZMin=R[i].z;
    }
    else
      if (R[i].z>ZMax)
      {
        ZMax=R[i].z;
      };
  };

  // TRACE(XMin);
  // TRACE(XMax);
  if (allowRescale)
  {
    XCenter=(XMin+XMax)/2;
    YCenter=(YMin+YMax)/2;
    ZCenter=(ZMin+ZMax)/2;
  }
  /*DistMin=*/DistMax=sqrt(yaatk::SQR(XMax-XMin)+yaatk::SQR(YMax-YMin)+yaatk::SQR(ZMax-ZMin));
  GLdouble DistMin = DistMax;
  for(int j=0;j<R.size();j++)
    for(int i=0;i<j;i++)
      if (yaatk::distance(R[i],R[j])<DistMin)
        DistMin=yaatk::distance(R[i],R[j]);
  // vertexRadius = 2.57*1.0/2.0/3.0/2.0;
  vertexRadius = DistMin/10;
  axesRadius = vertexRadius*3;
  // TRACE(scale);
  if (allowRescale)
  {
    scale=(2.0*nRange)/(DistMax+2.0*vertexRadius);
  }
  // TRACE(scale);
  maxScale=2.0*(scale);
  redraw();
}

void
VisBox::setData(const std::vector<Vector3D> &rep)
{
  Ro = rep;

  int VC = Ro.size();

  XMin=Ro[0].x;
  XMax=Ro[0].x;
  YMin=Ro[0].y;
  YMax=Ro[0].y;
  ZMin=Ro[0].z;
  ZMax=Ro[0].z;
  for(int i=0;i<VC;i++)
  {
    if (Ro[i].x<XMin)
    {
      XMin=Ro[i].x;
    }
    else
      if (Ro[i].x>XMax)
      {
        XMax=Ro[i].x;
      };
    if (Ro[i].y<YMin)
    {
      YMin=Ro[i].y;
    }
    else
      if (Ro[i].y>YMax)
      {
        YMax=Ro[i].y;
      };
    if (Ro[i].z<ZMin)
    {
      ZMin=Ro[i].z;
    }
    else
      if (Ro[i].z>ZMax)
      {
        ZMax=Ro[i].z;
      };
  };

  reArrange(-1,101,-1,101,-1,101);
}

void
VisBox::drawObjects()
{
  glInitNames();

  glPushMatrix();

  glLightfv(GL_LIGHT0, GL_POSITION, light0_dir);

  glScaled(scale,scale,scale);
  glTranslated(-XCenter, -YCenter, -ZCenter);
  glEnable(GL_LIGHTING);

  if (showVertices)
    listVertices();

  if (showEdges)
    listEdges();

  if (showAxes)
  {
//    glDisable(GL_LIGHTING);
    listAxes();
//    glEnable(GL_LIGHTING);
  }

  glPopMatrix();
}

void
VisBox::listAxes()
{
  Vector3D xyz000(0,0,0);
  Vector3D x(XMax,0,0);
  Vector3D y(0,YMax,0);
  Vector3D z(0,0,ZMax);

  drawArrow(x,xyz000,0x0000FF,
            axesRadius,1.0/20);
  drawArrow(y,xyz000,0x00FF00,
            axesRadius,1.0/20);
  drawArrow(z,xyz000,0xFF0000,
            axesRadius,1.0/20);
}

void
VisBox::listVertices()
{
  size_t i;
  Color c;
  for(i=0;i<R.size();i++)
  {
    glPushName(i);
    glPushMatrix();
    if (unifiedVertices)
    {
      c = vertexColor;
    }
    else
    {
      c = (0x00FF00);
      c = aColor[g[i]];

      if (showSelected)
      {
        if (i!=selectedVertexIndex)
        {
          if (g(i,selectedVertexIndex))
          {
            c = (0x777777);
          }
        }
        else
          c = (0xFFFFFF);
      }
    }
    myglColor(c);

    if (verticesQuality > 2)
    {
      glTranslated(R[i].x,R[i].y,R[i].z);
      double scale = 1.0*vertexRadius;
      if (tinyVertices) scale /= 5;
      glScaled(scale,scale,scale);
      glCallList(hqMode?lstBallHQ:lstBall);
    }
    else
    {
      glPointSize(vertexRadius*scale);
      glBegin(GL_POINTS);
      glVertex3d(R[i].x,R[i].y,R[i].z);
      glEnd();
    }
    glPopMatrix();
    glPopName();
  }
}

Vector3D
_vectorMul(const Vector3D& a, const Vector3D& b)
{
  return Vector3D(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);
}

double
_scalarMul(const Vector3D& a, const Vector3D& b)
{
  return (a.x*b.x+a.y*b.y+a.z*b.z);
}

double
_relAngle(const Vector3D& a, const Vector3D& b)
{
  return acos(_scalarMul(a,b)/(a.module()*b.module()));
}

void
VisBox::drawEdge(const Vector3D& vi, const Vector3D& vj,
                 unsigned int color, double radius, GLubyte alpha)
{
  Vector3D TempRotVector;
  double TempRotAngle;

  glPushMatrix();
  myglColor(color, alpha);
  glTranslated(vi.x,vi.y,vi.z);
  TempRotVector=_vectorMul(Vector3D(0,0,1.0L),vj-vi);
  TempRotAngle=(_relAngle(Vector3D(0,0,1.0L),vj-vi)/M_PI)*180.0L;
  glRotated(TempRotAngle,TempRotVector.x,TempRotVector.y,TempRotVector.z);
  glScaled(radius,radius,(vi-vj).module());
  glCallList(hqMode?lstStickHQ:lstStick);
  glPopMatrix();
}

void
VisBox::drawArrow(const Vector3D& vi, const Vector3D& vj,
                  unsigned int color, double radius, double arrowPart)
{
  Vector3D TempRotVector;
  double TempRotAngle;

  GLUquadricObj *quadObj;

  glPushMatrix();
  quadObj = gluNewQuadric ();
  gluQuadricDrawStyle (quadObj, GLU_FILL);
  myglColor(color);
  Vector3D vish = vi+(vj-vi)*arrowPart;
  glTranslated(vish.x,vish.y,vish.z);
  TempRotVector=_vectorMul(Vector3D(0,0,1.0L),vj-vi);
  TempRotAngle=(_relAngle(Vector3D(0,0,1.0L),vj-vi)/M_PI)*180.0L;
  glRotated(TempRotAngle,TempRotVector.x,TempRotVector.y,TempRotVector.z);
  gluCylinder (quadObj,
               radius/3.0,
               radius/3.0,
               (vi-vj).module()*(1.0-arrowPart),
               hqMode?verticesQualityInHQMode:verticesQuality,
               hqMode?verticesQualityInHQMode:verticesQuality);
  gluDeleteQuadric(quadObj);
  glPopMatrix();

  glPushMatrix();
  quadObj = gluNewQuadric ();
  gluQuadricDrawStyle (quadObj, GLU_FILL);
  myglColor(color);
  glTranslated(vi.x,vi.y,vi.z);
  TempRotVector=_vectorMul(Vector3D(0,0,1.0L),vj-vi);
  TempRotAngle=(_relAngle(Vector3D(0,0,1.0L),vj-vi)/M_PI)*180.0L;
  glRotated(TempRotAngle,TempRotVector.x,TempRotVector.y,TempRotVector.z);
  gluCylinder (quadObj,
               0,
               radius,
               (vi-vj).module()*arrowPart,
               hqMode?verticesQualityInHQMode:verticesQuality,
               hqMode?verticesQualityInHQMode:verticesQuality);
  gluDeleteQuadric(quadObj);
  glPopMatrix();
}

void
VisBox::listEdges()
{
  Color c = 0x00FF00;
  for(size_t i = 0; i < R.size(); i++)
  {
    for(size_t j = i+1; j < R.size(); j++)
    {
      glPushMatrix();
      double strength = 1.0;
      if (strength > 0.0 && g(i,j))
      {
        if (unifiedEdges)
          c = edgeColor;
        else
          c = aColor[g(i,j)];
        drawEdge(R[i],R[j], c, vertexRadius/3, GLubyte(strength*255));
      }
      glPopMatrix();
    }
  }
}

void
VisBox::setupLighting()
{
  GLfloat MaterialAmbient[] = {0.5, 0.5, 0.5, 1.0};
  GLfloat MaterialDiffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat MaterialSpecular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat MaterialShininess[] = {100.0};

  glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmbient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialDiffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, MaterialSpecular);
  glMaterialfv(GL_FRONT, GL_SHININESS, MaterialShininess);

  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);

  GLfloat global_ambient[] = {0.2, 0.2, 0.2, 1.0};

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

  GLfloat light0_diffuse[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat light0_ambient[] = {0.0, 0.0, 0.0, 1.0};
  GLfloat light0_specular[] = {0.0, 0.0, 0.0, 1.0};

  glLightfv(GL_LIGHT0, GL_POSITION, light0_dir);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  //  glShadeModel(GL_SMOOTH);

  glEnable(GL_DEPTH_TEST);
  //  glEnable(GL_CULL_FACE);
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_NORMALIZE);

  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_POINT_SMOOTH_HINT, GL_DONT_CARE);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
}

void
VisBox::prepareBasicLists()
{
  glNewList(lstBall,GL_COMPILE);
  {
    GLUquadricObj *quadObj;
    quadObj = gluNewQuadric ();
    gluQuadricDrawStyle (quadObj, GLU_FILL);
    gluSphere (quadObj,
               1.0,
               verticesQuality,
               verticesQuality);
    gluDeleteQuadric(quadObj);
  }
  glEndList();

  glNewList(lstBallHQ,GL_COMPILE);
  {
    GLUquadricObj *quadObj;
    quadObj = gluNewQuadric ();
    gluQuadricDrawStyle (quadObj, GLU_FILL);
    gluSphere (quadObj,
               1.0,
               verticesQualityInHQMode,
               verticesQualityInHQMode);
    gluDeleteQuadric(quadObj);
  }
  glEndList();

  glNewList(lstStick,GL_COMPILE);
  {
    GLUquadricObj *quadObj;
    quadObj = gluNewQuadric ();
    gluQuadricDrawStyle (quadObj, GLU_FILL);
    gluCylinder (quadObj,
                 1.0,
                 1.0,
                 1.0,
                 verticesQuality,
                 verticesQuality);
    gluDeleteQuadric(quadObj);
  }
  glEndList();

  glNewList(lstStickHQ,GL_COMPILE);
  {
    GLUquadricObj *quadObj;
    quadObj = gluNewQuadric ();
    gluQuadricDrawStyle (quadObj, GLU_FILL);
    gluCylinder (quadObj,
                 1.0,
                 1.0,
                 1.0,
                 verticesQualityInHQMode,
                 verticesQualityInHQMode);
    gluDeleteQuadric(quadObj);
  }
  glEndList();
}

void
VisBox::onResizeGL()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0,0,(w()>h())?h():w(),(w()>h())?h():w());
//  glOrtho (-nRange, nRange, -nRange, nRange, -nRange, nRange);
  GLfloat xr[3][2];

  if (!tiledMode)
  {
    xr[0][0] = -nRange;
    xr[0][1] = +nRange;
    xr[1][0] = -nRange;
    xr[1][1] = +nRange;
    xr[2][0] = -nRange;
    xr[2][1] = +nRange;
  }
  else
  {
    xr[0][0] = -nRange+2.0*nRange/tileCount*(tileIndex[0]);
    xr[0][1] = -nRange+2.0*nRange/tileCount*(tileIndex[0]+1);
    xr[1][0] = -nRange+2.0*nRange/tileCount*(tileIndex[1]);
    xr[1][1] = -nRange+2.0*nRange/tileCount*(tileIndex[1]+1);
    xr[2][0] = -nRange;
    xr[2][1] = +nRange;
  }
  glOrtho (xr[0][0], xr[0][1], xr[1][0], xr[1][1], xr[2][0], xr[2][1]);
}

void
VisBox::draw()
{
  if (!valid())
  {
    onResizeGL();

    setupLighting();

    if (!lstBall) lstBall = glGenLists(1);
    REQUIRE(lstBall);

    if (!lstBallHQ) lstBallHQ = glGenLists(1);
    REQUIRE(lstBallHQ);

    if (!lstStick) lstStick = glGenLists(1);
    REQUIRE(lstStick);

    if (!lstStickHQ) lstStickHQ = glGenLists(1);
    REQUIRE(lstStickHQ);

    prepareBasicLists();
  }

  setupLighting();

  glClearColor((bgColor%0x100)/255.0,
               ((bgColor/0x100)%0x100)/255.0,
               (bgColor/0x10000)/255.0,1.0f);
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  drawObjects();
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
}

void
VisBox::myglColor(Color color, GLubyte alpha)
{
  glColor4ub(color.r(), color.g(), color.b(), alpha);
}

Vector3D getPosition()
{
  GLdouble m[16];
  glGetDoublev(GL_MODELVIEW_MATRIX,m);
  return Vector3D(m[3*4+0],m[3*4+1],m[3*4+2]);
}

void
VisBox::openInEditor(std::string id, bool discardRotation)
{
  grctk::AdjMatrix g4edit = g.clone();

  if (!discardRotation)
  {
    glMatrixMode(GL_MODELVIEW);

    for(size_t i = 0; i < R.size(); i++)
    {
      glPushMatrix();

      glTranslated(R[i].x,R[i].y,R[i].z);

      a3D[g4edit[i]] = getPosition();
      a3D[g4edit[i]].y = -a3D[g4edit[i]].y;

      glPopMatrix();
    }
  }
  else
  {
    for(size_t i = 0; i < R.size(); i++)
    {
      a3D[g4edit[i]] = R[i];
    }
  }

  grce->mainWindow->docControl->createNewFromGraph(g4edit, id);
}

#ifdef GRCE_PNG

int writePNGImage(char* filename, int width, int height,
                  unsigned char *buffer, char* title)
{
  int retcode = 0;
  FILE *fp = NULL;
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  png_bytep row = NULL;

  try
  {
    fp = fopen(filename, "wb");
    if (fp == NULL)
      throw std::runtime_error("Error opening output file.");

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
      throw std::runtime_error("Could not allocate png write struct.");

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
      throw std::runtime_error("Could not allocate png info struct.");

    if (setjmp(png_jmpbuf(png_ptr)))
      throw std::runtime_error("Error during png creation.");

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    if (title != NULL)
    {
      png_text title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key = "Title";
      title_text.text = title;
      png_set_text(png_ptr, info_ptr, &title_text, 1);
    }

    png_write_info(png_ptr, info_ptr);

    row = (png_bytep) malloc(3 * width * sizeof(png_byte));

    if (!row)
      throw std::runtime_error("Memory allocation error");

//    for(int y = 0; y < height; y++)
    for(int y = height-1; y >= 0; y--)
    {
      for (int x = 0; x < width; x++)
      {
        memcpy(&(row[x*3]),&(buffer[(y*width + x)*3]),3);
      }
      png_write_row(png_ptr, row);
    }

    png_write_end(png_ptr, NULL);
  }
  catch (std::exception& e)
  {
    std::cerr << "Caught exception: " << e.what() << std::endl;
    retcode = -1;
  }

  if (row != NULL) free(row);
  if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
  if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
  if (fp != NULL) fclose(fp);

  return retcode;
}

#endif

void
VisBox::saveImageToFile(char* filename)
{
  hqMode = true;

  unsigned long width = w(); unsigned long height = h();

  unsigned char *d = new unsigned char[width*height*3];
  REQUIRE(d!=NULL);

  glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,d);
#ifdef GRCE_PNG
  writePNGImage(filename,width,height,d,filename);
#else
  for(unsigned long i = 0;i < width*height*3; i++)
  {
    if (i % 3 == 0)
    {
      unsigned char t;
      t = d[i];
      d[i] = d[i+2];
      d[i+2] = t;
    }
  }

  grce::bmpImage* bmp = new grce::bmpImage(width,height,d);
  bmp->SaveToFile(filename);
  delete bmp;
#endif

  delete [] d;

  hqMode = false;
}

void
VisBox::saveTiledImageToFile(char* filename)
{
  REQUIRE(w()==h());
  tiledMode = true;
  hqMode = true;

  unsigned long width = w()*tileCount; unsigned long height = h()*tileCount;

  unsigned char *d = new unsigned char[width*height*3];
  REQUIRE(d!=NULL);
  unsigned char *dtile = new unsigned char[w()*h()*3];
  REQUIRE(dtile!=NULL);

  grce::bmpImage* bmp = new grce::bmpImage(width,height,d);

  for(tileIndex[0] = 0; tileIndex[0] < tileCount; tileIndex[0]++)
  {
    for(tileIndex[1] = 0; tileIndex[1] < tileCount; tileIndex[1]++)
    {
      onResizeGL();
      redraw();
      while (!Fl::ready()) {};
      Fl::flush();
      while (!Fl::ready()) {};

      glReadPixels(0,0,w(),h(),GL_RGB,GL_UNSIGNED_BYTE,dtile);

#ifdef GRCE_PNG
#else
      for(unsigned long i = 0;i < ((unsigned long) w())*h()*3; i++)
      {
        if (i % 3 == 0)
        {
          unsigned char t;
          t = dtile[i];
          dtile[i] = dtile[i+2];
          dtile[i+2] = t;
        }
      }
#endif

      grce::bmpImage* tilebmp = new grce::bmpImage(w(),h(),dtile);
      for(unsigned long i = 0;i < (unsigned long) h(); i++)
        for(unsigned long j = 0;j < (unsigned long) w(); j++)
          bmp->setPixel(i+tileIndex[1]*w(),j+tileIndex[0]*h(),
                        tilebmp->getPixel(i,j));
    }
  }

#ifdef GRCE_PNG
  writePNGImage(filename,width,height,bmp->getRawDataPtr(),filename);
#else
  bmp->SaveToFile(filename);
#endif

  delete [] dtile;
  delete [] d;
  delete bmp;

  hqMode = false;
  tiledMode = false;

  onResizeGL();
  redraw();
}

void
VisBox::rollAround(double angle,double x, double y,double z)
{
  glMatrixMode(GL_MODELVIEW);

  glRotated(angle,x,y,z);

  if (x == 1.0) old_rot_x += angle;
  if (y == 1.0) old_rot_y += angle;
  if (z == 1.0) old_rot_z += angle;

  redraw();
}

void
VisBox::window_cb(Fl_Widget* widget, void*)
{
  ((Fl_Window*)widget)->hide();
  // exit(0);
  VisBox* VisBox_Ptr;
  VisBox_Ptr = (VisBox*)widget;
  if (VisBox_Ptr->parentVisWindow)
  {
    delete VisBox_Ptr->parentVisWindow;
  }
}

int
VisBox::handle(int event)
{
  int x,y;
  int atom2select;
  switch(event)
  {
  case FL_PUSH:
    x = Fl::event_x();
    y = Fl::event_y();
    atom2select = pickVertex(x,y);
    if (atom2select >=0)
      if (parentVisWindow)
        parentVisWindow->setVertexViewIndex(atom2select);
    return 1;
  default:
    return Fl_Widget::handle(event);
  }
}

int
VisBox::pickVertex(int x, int y)
{
  int pickedVertex = -1;

  const int BUFSIZE = 4*R.size();
  GLuint* selectBuf = new GLuint[BUFSIZE];

  {
    glSelectBuffer(BUFSIZE,selectBuf);
    glRenderMode(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT,viewport);
    gluPickMatrix(x,viewport[3]-y,
                  5,5,viewport);
    GLfloat xr[3][2];
    {
      xr[0][0] = -nRange;
      xr[0][1] = +nRange;
      xr[1][0] = -nRange;
      xr[1][1] = +nRange;
      xr[2][0] = -nRange;
      xr[2][1] = +nRange;
    }
    glOrtho (xr[0][0], xr[0][1], xr[1][0], xr[1][1], xr[2][0], xr[2][1]);
  }

  draw();

  {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glFlush();
  }

  GLint hits = glRenderMode(GL_RENDER);

  {
    GLint numberOfNames = 0;
    GLuint *ptrNames = NULL;
    GLuint *ptr = (GLuint *) selectBuf;
    GLuint minZ = 0xffffffff;
    for (GLint i = 0; i < hits; i++)
    {
      GLuint names = *ptr;
      ptr++;
      if (*ptr < minZ)
      {
        numberOfNames = names;
        minZ = *ptr;
        ptrNames = ptr+2;
      }

      ptr += names+2;
    }
    if (numberOfNames > 0)
    {
      ptr = ptrNames;
      for (GLint j = 0; j < numberOfNames; j++,ptr++)
        pickedVertex = *ptr;
    }
  }

  delete [] selectBuf;

  return pickedVertex;
}

}
