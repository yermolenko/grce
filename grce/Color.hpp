/*
  The Color and the ColorGenerator classes (header file).

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

#ifndef grce_Color_hpp
#define grce_Color_hpp

#include <cstdlib>
#include <iostream>

namespace grce
{

class Color
{
  unsigned int c;
public:
  operator unsigned int() { return c; }
  Color(unsigned int color) : c(color) {}
  Color()
    : c(0*0x1000000+0*0x10000+255*0x100+0) {}
  Color(unsigned char r, unsigned char g, unsigned char b,
        unsigned char a = 0)
    : c(a*0x1000000+b*0x10000+g*0x100+r) {}
  unsigned char r() const { return c%0x100; }
  unsigned char g() const { return (c/0x100)%0x100; }
  unsigned char b() const { return (c/0x10000)%0x100; }
  unsigned char a() const { return (c/0x1000000)%0x100; }
  void analyseRGB(unsigned char &r, unsigned char &g, unsigned char &b)
    {
      r = c%0x100;
      g = (c/0x100)%0x100;
      b = (c/0x10000)%0x100;
    }

  void analyseRGBA(unsigned char &r, unsigned char &g, unsigned char &b,
                   unsigned char &a)
    {
      r = c%0x100;
      g = (c/0x100)%0x100;
      b = (c/0x10000)%0x100;
      a = (c/0x1000000)%0x100;
    }
  friend std::istream& operator>>(std::istream& is, Color&);
  friend std::ostream& operator<<(std::ostream& os, const Color&);
};

inline
std::istream&
operator>>(std::istream& is, Color& c)
{
  unsigned int r,g,b,a;
  is >> r >> g >> b >> a;
  c = Color(r,g,b,a);
  return is;
}

inline
std::ostream&
operator<<(std::ostream& os, const Color& c)
{
  os << (unsigned int)c.r() << " "
     << (unsigned int)c.g() << " "
     << (unsigned int)c.b() << " "
     << (unsigned int)c.a();
  return os;
}

class ColorGenerator
{
  size_t r;
  size_t g;
  size_t b;
  size_t gradations;
public:
  ColorGenerator():r(),g(),b(),gradations(2){}
  Color operator()();
};

}

#endif
