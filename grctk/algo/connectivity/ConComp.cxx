/*
  The ConComp algorithm.

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

#include "ConComp.hpp"

#include <limits>
#include <vector>
#include <stdexcept>

namespace grctk
{

void
ConComp::Comp(size_t x, size_t count, const AdjMatrix& g, std::vector<size_t> &mark)
{
  const size_t n = g.size();
  mark[x] = count;
  for(size_t i = 0; i < n; i++)
  {
    if (g.edge(x,i))
      if (mark[i] == 0)
        Comp(i,count,g,mark);
  }
}

AdjMatrix
ConComp::operator()(const AdjMatrix& g, const size_t s, size_t &count)
{
  logStream() << "\nConComp started\n";
  flushLogStreams();

  if (s >= g.size())
    throw std::logic_error("Starting vertex does not exist in the graph");

  const size_t n = g.size();
  std::vector<size_t> mark(n);

  count = 0;
  for(size_t i = 0; i < n; i++)
    mark[i] = 0;

  for(size_t i = 0; i < n; i++)
  {
    if (mark[i] == 0)
    {
      count += 1;
      Comp(i,count,g,mark);
    }
  }

  AdjMatrix result = g;

  for(size_t i = 0; i < n; i++)
    if (mark[i] != mark[s])
      result -= result.vertexIndex(g[i]);

  logStream() << "ConComp finished\n";
  flushLogStreams();

  return result;
}

} //namespace grctk
