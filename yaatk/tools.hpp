/*
   Some YAATK auxiliary functions.

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

#ifndef yaatk_tools_hpp
#define yaatk_tools_hpp

#include <cmath>

namespace yaatk
{

template <class T>
double SIGN2FLOAT(T x)
{
  return ((x<0)?(-1.0):(1.0));
}

template <class T>
double BOOL2FLOAT(T x)
{
  return ((x)?(1.0):(0.0));
}

template <class T>
inline
T min(T x,T y) { return ((x)>(y))?(y):(x);}

template <class T>
inline
T max(T x,T y) { return ((x)>(y))?(x):(y); }

template <class T>
inline
T SQR(T x) { return ((x)*(x)); }

template <class T>
inline
T SQR3(T x) { return ((x)*(x)*(x)); }

inline
double
academic_round(double enr)
{
  int   dig;
  dig = int(fabs(enr)*100)%10;
  if (enr < 0)
  {
    if (dig >= 5)
      enr = floor(enr*10.0)/10.0;
    else
      enr = ceil(enr*10.0)/10.0;
  }
  else
  {
    if (dig >= 5)
      enr = ceil(enr*10.0)/10.0;
    else
      enr = floor(enr*10.0)/10.0;
  }
  return enr;
}

inline
double
intpow(double a, int n)
{
  switch (n)
  {
  case 0 : return 1.0;
  case 1 : return a;
  case -1 : return 1.0/(a);
  case 2 : return a*a;
  case -2 : return 1.0/(a*a);
  case 3 : return a*a*a;
  case -3 : return 1.0/(a*a*a);
  case 4 : return a*a*a*a;
  case -4 : return 1.0/(a*a*a*a);
  case 5 : return a*a*a*a*a;
  case -5 : return 1.0/(a*a*a*a*a);
  case 6 : return a*a*a*a*a*a;
  case -6 : return 1.0/(a*a*a*a*a*a);
  default: {
    double v = 1.0;
    int abs_n = ((n >= 0)?(n):(-n));
    for(int i = 1; i <= abs_n; i++)
      v *= a;
    if (n < 0) v = 1.0/v;
    return v;
  }
  }
}

} // namespace yaatk

#endif
