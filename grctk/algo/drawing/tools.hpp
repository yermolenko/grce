/*
  Auxiliary graph drawing functions.

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

#ifndef grctk_drawing_tools_hpp
#define grctk_drawing_tools_hpp

#include "grctk/Universe.hpp"
#include "grctk/AdjMatrix.hpp"
#include <yaatk/Vector3D.hpp>
#include <yaatk/VectorXD.hpp>

namespace grctk
{

inline
yaatk::Vector3D
dimensions(const AdjMatrix& g, Attribute<yaatk::Vector3D>& a3D)
{
  if (g.size() == 0)
    return yaatk::Vector3D(0.0,0.0,0.0);

  yaatk::Vector3D maxPos = a3D[g[0]];
  yaatk::Vector3D minPos = a3D[g[0]];
  for(size_t i = 0; i < g.size(); ++i)
  {
    if (a3D[g[i]].x > maxPos.x)
      maxPos.x = a3D[g[i]].x;
    if (a3D[g[i]].x < minPos.x)
      minPos.x = a3D[g[i]].x;
    if (a3D[g[i]].y > maxPos.y)
      maxPos.y = a3D[g[i]].y;
    if (a3D[g[i]].y < minPos.y)
      minPos.y = a3D[g[i]].y;
    if (a3D[g[i]].z > maxPos.z)
      maxPos.z = a3D[g[i]].z;
    if (a3D[g[i]].z < minPos.z)
      minPos.z = a3D[g[i]].z;
  }

  return maxPos - minPos;
}

inline
yaatk::Vector3D
massCenter(const AdjMatrix &g, Attribute<yaatk::Vector3D>& a3D)
{
  REQUIRE(g.size() > 0);
  yaatk::Vector3D sum = 0.0;
  for(size_t i = 0; i < g.size(); i++)
    sum += a3D[g[i]];
  sum /= g.size();
  return sum;
}

}

#endif
