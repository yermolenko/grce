/*
  The MovePositions algorithm.

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

#include "MovePositions.hpp"

namespace grctk
{

using yaatk::VectorXD;

VectorXD
MovePositions::massCenter(const AdjMatrix &g, Attribute<yaatk::VectorXD>& aXD)
{
  REQUIRE(g.size() > 0);
  VectorXD sum(aXD[g[0]].size());
  for(size_t i = 0; i < g.size(); i++)
    sum += aXD[g[i]];
  for(size_t si = 0; si < sum.size(); si++)
    sum[si] /= g.size();
  return sum;
}

void
MovePositions::moveMassCenter(const AdjMatrix &g, Attribute<yaatk::VectorXD>& aXD, const VectorXD& shift)
{
  for(size_t i = 0; i < g.size(); ++i)
    aXD[g[i]] += shift;
}

void
MovePositions::moveMassCenterToOrigin(const AdjMatrix &g, Attribute<yaatk::VectorXD>& aXD)
{
  moveMassCenter(g,aXD,-massCenter(g,aXD));
}

void
MovePositions::operator()(const AdjMatrix &g, Attribute<VectorXD>& aXD, const VectorXD& shift)
{
  logStream() << "\nMovePositions started\n";
  flushLogStreams();

  moveMassCenter(g, aXD, shift);

  logStream() << "MovePositions finished\n" ;
  flushLogStreams();
}

} //namespace grctk
