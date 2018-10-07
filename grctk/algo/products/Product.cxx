/*
  The Product algorithm.

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

#include "Product.hpp"
#include "grctk/algo/drawing/tools.hpp"

namespace grctk
{

AdjMatrix
Product::graphTemplate(const AdjMatrix& g1, const AdjMatrix& g2) const
{
  AdjMatrix g;

  for(size_t i = 0; i < g1.size(); ++i)
    for(size_t j = 0; j < g2.size(); ++j)
    {
      checkAborted();
      g += g1[i].clone();
    }

  return g;
}

void
Product::positionVertices(AdjMatrix& g,
                          const AdjMatrix& g1, const AdjMatrix& g2,
                          Attribute<yaatk::Vector3D>& a3D) const
{
  yaatk::Vector3D g1dims = dimensions(g1,a3D);
  yaatk::Vector3D g2dims = dimensions(g2,a3D);

  double dim12ratio = (g1dims.x/g2dims.x < g1dims.y/g2dims.y)?
    (g1dims.x/g2dims.x):(g1dims.y/g2dims.y);

  for(size_t i = 0; i < g1.size(); ++i)
    for(size_t j = 0; j < g2.size(); ++j)
    {
      if (g2dims.x/g2dims.y > 1.0)
      {
        a3D[g[i*g2.size() + j]].x -= 1.5*dimensions(g1,a3D).x*j;
        a3D[g[i*g2.size() + j]].y -= 1.5*(a3D[g2[j]].y - massCenter(g2,a3D).y)*dim12ratio;
      }
      else
      {
        a3D[g[i*g2.size() + j]].x += 1.5*(a3D[g2[j]].x - massCenter(g2,a3D).x)*dim12ratio;
        a3D[g[i*g2.size() + j]].y -= 1.5*dimensions(g1,a3D).y*j;
      }
    }
}

} //namespace grctk
