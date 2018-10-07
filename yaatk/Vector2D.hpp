/*
   The Vector2D class.

   Copyright (C) 2001, 2002, 2003, 2004, 2015, 2017, 2018 Alexander
   Yermolenko <yaa.mbox@gmail.com>

   This file is part of YAATK, Yet another auxiliary toolkit.

   YAATK is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   YAATK is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with YAATK.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef yaatk_Vector2D_hpp
#define yaatk_Vector2D_hpp

#include "yaatk/tools.hpp"
#include <iostream>
#include <cstdio>

namespace yaatk
{

class Vector2D
{
public:
  double x,y;
  Vector2D(double xc = 0.0, double yc = 0.0);
  double    module() const;
  void     normalize();
  Vector2D normalized() const;
  friend Vector2D operator+(const Vector2D& a, const Vector2D& b);
  friend bool     operator==(const Vector2D& v1, const Vector2D& v2);
  friend bool     operator!=(const Vector2D& v1, const Vector2D& v2);
  friend void     operator+=(Vector2D& v, const Vector2D& a);
  friend void     operator-=(Vector2D& v, const Vector2D& a);
  friend Vector2D operator-(const Vector2D& v,const Vector2D& b);
  friend Vector2D operator*(const Vector2D& v,const double& m);
  friend Vector2D operator/(const Vector2D& v,const double& m);
  friend Vector2D operator*(const double& m,const Vector2D& v);
  friend void     operator*=(Vector2D& v, const double& m);
  Vector2D operator-() const;

  friend double    distance(Vector2D &v1,Vector2D &v2);

  friend std::istream& operator>>(std::istream& is, Vector2D& vec);
  friend std::ostream& operator<<(std::ostream& os, const Vector2D& vec);

  Vector2D operator<<(std::FILE* f);
  Vector2D operator>>(std::FILE* f);
};

bool areIntersecting(const Vector2D&, const Vector2D&,
                     const Vector2D&, const Vector2D&);

inline
Vector2D::Vector2D(double xc, double yc):
  x(xc),y(yc)
{
}

inline
bool
operator==(const Vector2D& a, const Vector2D& b)
{
  return ((a.x == b.x) && (a.y == b.y));
}

inline
bool
operator!=(const Vector2D& a, const Vector2D& b)
{
  return ((a.x != b.x) || (a.y != b.y));
}

inline
Vector2D
operator+(const Vector2D& a, const Vector2D& b)
{
  return Vector2D(a.x+b.x, a.y+b.y);
}

inline
void
operator+=(Vector2D& v, const Vector2D& a)
{
  v.x+=a.x; v.y+=a.y;
}

inline
void
operator-=(Vector2D& v, const Vector2D& a)
{
  v.x-=a.x; v.y-=a.y;
}

inline
Vector2D
operator-(const Vector2D& v, const Vector2D& b)
{
  return Vector2D(v.x-b.x, v.y-b.y);
}

inline
Vector2D
operator*(const Vector2D& v, const double& m)
{
  return Vector2D(v.x*m, v.y*m);
}

inline
Vector2D
operator/(const Vector2D& v, const double& m)
{
  return Vector2D(v.x/m, v.y/m);
}

inline
Vector2D operator*(const double& m,const Vector2D& v)
{
  return Vector2D(v.x*m, v.y*m);
}

inline
void
operator*=(Vector2D& v, const double& m)
{
  v.x*=m; v.y*=m;
}

inline
double
distance(Vector2D &v1,Vector2D &v2)
{
  return sqrt(SQR(v1.x-v2.x)+SQR(v1.y-v2.y));
}

inline
Vector2D
vectorMul(const Vector2D& a, const Vector2D& b)
{
  return Vector2D(a.x*b.y-a.y*b.x);
}

inline
double
scalarMul(const Vector2D& a, const Vector2D& b)
{
  return (a.x*b.x+a.y*b.y);
}

inline
double
Vector2D::module() const
{
  return sqrt(x*x+y*y);
}

inline
void
Vector2D::normalize()
{
  double L(module());
  x/=L; y/=L;
}

inline
Vector2D
Vector2D::normalized() const
{
  double L(module());
  return Vector2D(x/L, y/L);
}

inline
Vector2D
Vector2D::operator-() const
{
  return Vector2D(-x,-y);
}

inline
std::istream&
operator>>(std::istream& is, Vector2D& vec)
{
  double u = 0, v = 0;
  is >> u >> v;
  if (is)
    vec = Vector2D(u,v);
  else
    std::cerr << "Error in reading Vector2D" << std::endl;
  return is;
}

inline
std::ostream&
operator<<(std::ostream& os, const Vector2D& vec)
{
  os << vec.x << std::endl
     << vec.y << std::endl;
  return os;
}

inline
Vector2D
Vector2D::operator<<(FILE* f)
{
  fscanf(f,"%lf %lf", &x,&y);
  return *this;
}

inline
Vector2D
Vector2D::operator>>(FILE* f)
{
  fprintf(f,"%9f %9f\n", x,y);
  return *this;
}

inline
bool areIntersecting(const Vector2D& p1, const Vector2D& p2,
                     const Vector2D& p3, const Vector2D& p4)
{
  double cx1, cx2, cx3, cx4,
    cy1, cy2, cy3, cy4;
  bool cond1,cond2,cond3;

  cx1 = yaatk::min(p1.x,p2.x);
  cy1 = yaatk::min(p1.y,p2.y);
  cx2 = yaatk::max(p1.x,p2.x);
  cy2 = yaatk::max(p1.y,p2.y);

  cx3 = yaatk::min(p3.x,p4.x);
  cy3 = yaatk::min(p3.y,p4.y);
  cx4 = yaatk::max(p3.x,p4.x);
  cy4 = yaatk::max(p3.y,p4.y);

  cond1 = (cx2 >= cx3)&&(cx4 >= cx1)&&(cy2 >= cy3)&&(cy4 >= cy1);
  if (!cond1) return false;

  cond2 = ( scalarMul(vectorMul(p3 - p1,p2 - p1),vectorMul(p4 - p1,p2 - p1)) <= 0.0);
  if (!cond2) return false;

  cond3 = ( scalarMul(vectorMul(p1 - p3,p4 - p3),vectorMul(p2 - p3,p4 - p3)) <= 0.0);
  if (!cond3) return false;

  return true;
}

}

#endif
