/*
  The Color and the ColorGenerator classes.

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

#include "Color.hpp"

namespace grce
{

Color ColorGenerator::operator()()
{
  r += 1;
  if (r == gradations)
  {
    r = 0;
    g += 1;
    if (g == gradations)
    {
      g = 0;
      b += 1;
      if (b == gradations)
      {
        gradations++;
        r = 1; g = b = 0;
      }
    }
  }

  if (r == gradations-1 && g == gradations-1 && b == gradations-1)
  {
    gradations++;
    r = 1; g = b = 0;
  }

  return Color(r*255/(gradations-1),g*255/(gradations-1),b*255/(gradations-1));
}

}
