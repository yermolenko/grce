/*
   The Vector3D class.

   Copyright (C) 2004, 2005, 2009, 2011, 2016, 2017, 2018 Alexander
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

#ifndef yaatk_Vector3D_hpp
#define yaatk_Vector3D_hpp

#include <yaatk/tools.hpp>
#include <yaatk/Vector2D.hpp>

#include <iostream>
#include <iomanip>
#include <cmath>

namespace yaatk
{

class Vector3D
{
public:
  double x,y,z;
  double& X(size_t i)
  {
        switch (i)
        { 
          case 0: return x;break;
          case 1: return y;break;
          case 2: return z;break;
          default: throw;
        };
  }
  const double& X(size_t i) const
  {
        switch (i)
        { 
          case 0: return x;break;
          case 1: return y;break;
          case 2: return z;break;
          default: throw;
        };
  }
  Vector3D(double xc/* = 0.0*/, double yc/* = 0.0*/, double zc/* = 0.0*/);
  Vector3D(const Vector2D&);
  Vector3D();
  Vector3D& operator=(const double&);
//  Vector3D& operator=(const Vector3D&);
  Vector3D(const double&);

  double    module() const;
  double    module_squared() const;
  void     normalize();
  Vector3D normalized() const;
  friend Vector3D operator+(const Vector3D& a,const Vector3D& b);
  friend bool     operator==(const Vector3D& v1, const Vector3D& v2);
  friend bool     operator!=(const Vector3D& v1, const Vector3D& v2);
  friend void     operator+=(Vector3D& v, const Vector3D& a);
  friend void     operator-=(Vector3D& v, const Vector3D& a);
  friend Vector3D operator-(const Vector3D& v,const Vector3D& b);
  friend Vector3D operator*(const Vector3D& v,const double& m);
  friend Vector3D operator/(const Vector3D& v,const double& m);
  friend Vector3D operator*(const double& m,const Vector3D& v);
  friend void     operator*=(Vector3D& v, const double& m);
  friend void     operator/=(Vector3D& v, const double& m);
  Vector3D operator-() const;

  friend double    distance(const Vector3D &v1,const Vector3D &v2);
  friend Vector3D vectormul(const Vector3D& a,const Vector3D& b);
  friend double    scalarmul(const Vector3D& a,const Vector3D& b);
  friend double    relangle(const Vector3D& a, const Vector3D& b);

  friend std::istream&  operator>> (std::istream& is, Vector3D& vec);
  friend std::ostream&  operator<< (std::ostream& os, const Vector3D& vec);
};

std::istream&
operator>> (std::istream& is, Vector3D& vec);

std::ostream&
operator<< (std::ostream& os, const Vector3D& vec);

bool
operator==(const Vector3D& v1, const Vector3D& v2);

bool
operator!=(const Vector3D& v1, const Vector3D& v2);

Vector3D
operator+(const Vector3D& a, const Vector3D& b);

void
operator+=(Vector3D& v, const Vector3D& a);

Vector3D
operator-(const Vector3D& v, const Vector3D& b);

Vector3D operator*(const Vector3D& v, const double& m);

Vector3D
operator/(const Vector3D& v, const double& m);

Vector3D
operator*(const double& m,const Vector3D& v);

void operator*=(Vector3D& v, const double& m);

// double
// distance(Vector3D &v1,Vector3D &v2);

Vector3D
vectormul(const Vector3D& a, const Vector3D& b);

double
scalarmul(const Vector3D& a, const Vector3D& b);

double
relangle(const Vector3D& a, const Vector3D& b);

using std::istream;
using std::ostream;
using std::cerr;
using std::cout;
using std::endl;
using std::flush;
using std::sqrt;
using std::acos;

inline
Vector3D::Vector3D(double xc, double yc, double zc):
  x(xc),y(yc),z(zc)
{
}

inline
Vector3D::Vector3D(const Vector2D& obj):
  x(obj.x),y(obj.y),z(0.0)
{
}

inline
Vector3D::Vector3D():
  x(0.0),y(0.0),z(0.0)
{
}

inline
Vector3D::Vector3D(const double& obj):
  x(obj),y(obj),z(obj)
{
}

/*
inline
Vector3D&
Vector3D::operator=(const Vector3D& obj)
{
  if (this == &obj)
    return (*this);
  x = obj.x;
  y = obj.y;
  z = obj.z;
  return (*this); 
}
*/

inline
Vector3D&
Vector3D::operator=(const double& obj)
{
  x = obj;
  y = obj;
  z = obj;
  return (*this); 
}

inline
bool
operator==(const Vector3D& a, const Vector3D& b)
{
  return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

inline
bool
operator!=(const Vector3D& a, const Vector3D& b)
{
  return ((a.x != b.x) || (a.y != b.y) || (a.z != b.z));
}

inline
Vector3D
operator+(const Vector3D& a, const Vector3D& b)
{
  return Vector3D(a.x+b.x, a.y+b.y, a.z+b.z);
}

inline
void
operator+=(Vector3D& v, const Vector3D& a)
{
  v.x+=a.x; v.y+=a.y; v.z+=a.z;
}

inline
void
operator-=(Vector3D& v, const Vector3D& a)
{
  v.x-=a.x; v.y-=a.y; v.z-=a.z;
}


inline
Vector3D
operator-(const Vector3D& v, const Vector3D& b)
{
  return Vector3D(v.x-b.x, v.y-b.y ,v.z-b.z);
}


inline
Vector3D
operator*(const Vector3D& v, const double& m)
{
  return Vector3D(v.x*m, v.y*m, v.z*m);
}

inline
Vector3D
operator/(const Vector3D& v, const double& m)
{
  return Vector3D(v.x/m, v.y/m, v.z/m);
}

inline
Vector3D operator*(const double& m,const Vector3D& v)
{
  return Vector3D(v.x*m, v.y*m, v.z*m);
}

inline
void
operator*=(Vector3D& v, const double& m)
{
  v.x*=m; v.y*=m;; v.z*=m;
}

inline
void
operator/=(Vector3D& v, const double& m)
{
  v.x/=m; v.y/=m;; v.z/=m;
}

inline
double
distance(const Vector3D &v1, const Vector3D &v2)
{
  return sqrt(SQR(v1.x-v2.x)+SQR(v1.y-v2.y)+SQR(v1.z-v2.z));
}

inline
Vector3D
vectormul(const Vector3D& a, const Vector3D& b)
{
  return Vector3D(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);
}

inline
double
scalarmul(const Vector3D& a, const Vector3D& b)
{
  return (a.x*b.x+a.y*b.y+a.z*b.z);
}

inline
double
relangle(const Vector3D& a, const Vector3D& b)
{
  return acos(scalarmul(a,b)/(a.module()*b.module()));
}

inline
double
Vector3D::module() const
{
  return sqrt(x*x+y*y+z*z);
}

inline
double
Vector3D::module_squared() const
{
  return x*x+y*y+z*z;
}

inline
void
Vector3D::normalize()
{
  double L=module();
  x/=L; y/=L; z/=L;
}

inline
Vector3D
Vector3D::normalized() const
{
  double L=module();
  return Vector3D(x/L, y/L, z/L);
}

inline
Vector3D
Vector3D::operator-() const
{
  return Vector3D(-x,-y,-z);
}

inline
istream&
operator>>(istream& is,  Vector3D& vec)
{
  double u = 0, v = 0, w = 0;
  is >> u >> v >> w;
  if (is)
    vec = Vector3D(u,v,w);
  else
    cerr << " Error in reading Vector3D " << endl << flush;
  return is;
}

inline
ostream&
operator<<(ostream& os, const Vector3D& vec)
{
  os << vec.x << " "
     << vec.y << " "
     << vec.z << " ";
  return os;
}

class IntVector3D
{
public:
  int x,y,z;
  int& X(size_t i)
  {
        switch (i)
        { 
          case 0: return x;break;
          case 1: return y;break;
          case 2: return z;break;
          default: throw;
        };
  }
  const int& X(size_t i) const
  {
        switch (i)
        { 
          case 0: return x;break;
          case 1: return y;break;
          case 2: return z;break;
          default: throw;
        };
  }
  IntVector3D(int xc, int yc, int zc);
  IntVector3D();
  IntVector3D& operator=(const int&);
  IntVector3D(const int&);

  friend std::istream&  operator>> (std::istream& is, IntVector3D& vec);
  friend std::ostream&  operator<< (std::ostream& os, const IntVector3D& vec);
};

inline
IntVector3D::IntVector3D(int xc, int yc, int zc):
  x(xc),y(yc),z(zc)
{
}

inline
IntVector3D::IntVector3D():
  x(0),y(0),z(0)
{
}

inline
IntVector3D::IntVector3D(const int& obj):
  x(obj),y(obj),z(obj)
{
}

inline
IntVector3D&
IntVector3D::operator=(const int& obj)
{
  x = obj;
  y = obj;
  z = obj;
  return (*this); 
}

inline
istream&
operator>>(istream& is,  IntVector3D& vec)
{
  int u = 0, v = 0, w = 0;
  is >> u >> v >> w;
  if (is)
    vec = IntVector3D(u,v,w);
  else
    cerr << " Error in reading IntVector3D " << endl << flush;
  return is;
}

inline
ostream&
operator<<(ostream& os, const IntVector3D& vec)
{
  os << vec.x << " "
     << vec.y << " "
     << vec.z << " ";
  return os;
}

} // namespace yaatk

#endif
