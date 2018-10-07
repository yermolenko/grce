/*
  The OptiIntersect algorithm.

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

#include "OptiIntersect.hpp"
#include <yaatk/Permutation.hpp>
#include <cstring>

namespace grctk
{

using yaatk::Vector2D;

size_t
OptiIntersect::intersectionsCount(
  const AdjMatrix &g,
  const size_t *p_cur,
  const std::vector<Vector2D>& R) const
{
  size_t VC = g.vertexCount();

  size_t is_count = 0;

  for(size_t i1 = 0; i1 < VC; i1++)
    for(size_t j1 = i1+1; j1 < VC; j1++)
      for(size_t i2 = 0; i2 < VC; i2++)
        for(size_t j2 = i2+1; j2 < VC; j2++)
        {
          checkAborted();
          if ((i1 != i2) && (j1 != j2) && (i2 != j1) && (j2 != i1))
          {
            if (g(i1,j1) &&
                g(i2,j2) &&
                areIntersecting(
                  R[p_cur[i1]],R[p_cur[j1]],
                  R[p_cur[i2]],R[p_cur[j2]]
                  )
              )
            {
              is_count++;
            }
          }
        }

  return is_count;
}

void
OptiIntersect::operator()(const AdjMatrix &g, const int& gc, Attribute<Vector2D>& a2D)
{
  logStream() << "\nOptiIntersect started\n";
  flushLogStreams();

  size_t VC = g.vertexCount();
  if (gc > VC) return;

  std::vector<Vector2D> R(VC);

  double radius = 1.0;
  double da = 2*M_PI/double(VC);
  for(size_t q = 0; q < VC; q++)
  {
    R[q].x = radius*cos(da*q);
    R[q].y = radius*sin(da*q);
  }

  size_t gnum = VC/gc;
  size_t *p_best = new size_t[VC];
  size_t *p_cur  = new size_t[VC];

  for(size_t i1 = 0; i1 < VC; i1++)
  {
    p_best[i1] = i1;
    p_cur[i1] = i1;
  }
  size_t min_is_count = (VC)*(VC-1)/2;
  for(size_t gi = 0; gi < gnum; gi++)
  {
    logStream() << "Group #" << gi+1 << "\n";
    flushLogStreams();

    yaatk::Permutation p(gc);
    p.gen_first();
    do
    {
      for(size_t k = 0; k < gc; k++)
        p_cur[gi*gc+k] =  p[k] + gi*gc;
      size_t is_count = intersectionsCount(g,p_cur,R);
      if (is_count < min_is_count)
      {
        for(size_t w = 0; w < VC; w++)
          p_best[w] = p_cur[w];
        min_is_count = is_count;
      }
      if (!(p.gen_next())) break;
    }while (1);
  }
  if (VC%gc != 0)
  {
    logStream() << "Group #" << gnum+1 << "\n";
    flushLogStreams();
    size_t gr = VC%gnum;
    yaatk::Permutation p(gr);
    p.gen_first();
    do
    {
      for(size_t k = 0; k < gr; k++)
        p_cur[gnum*gc+k] =  p[k] + gnum*gc;
      size_t is_count = intersectionsCount(g,p_cur,R);
      if (is_count < min_is_count)
      {
        for(size_t w = 0; w < VC; w++)
          p_best[w] = p_cur[w];
        min_is_count = is_count;
      }
      if (!(p.gen_next())) break;
    }while (1);
  }

  for(size_t i = 0; i < VC; i++)
    a2D[g[i]] = R[p_best[i]];

  delete [] p_best;
  delete [] p_cur;

  logStream() << "Number of intersections: " << min_is_count/2 << "\n";
  flushLogStreams();

  logStream() << "\nOptiIntersect finished\n";
  flushLogStreams();
}

} //namespace grctk
