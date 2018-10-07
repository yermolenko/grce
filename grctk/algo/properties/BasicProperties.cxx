/*
  The BasicProperties algorithm.

  Copyright (C) 2001, 2002, 2003, 2004, 2015, 2017, 2018 Alexander
  Yermolenko <yaa.mbox@gmail.com>

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

#include "BasicProperties.hpp"
#include "grctk/algo/drawing/tools.hpp"

namespace grctk
{

void
BasicProperties::operator()(const AdjMatrix& g)
{
  logStream() << "Number of vertices : " << g.size() << "\n";

  size_t numberOfEdges = 0;
  for(size_t i = 0; i < g.size(); ++i)
    for(size_t j = i+1; j < g.size(); ++j)
      if (g.s(i,j))
        ++numberOfEdges;
  logStream() << "Number of edges : " << numberOfEdges << "\n";

  flushLogStreams();
}

} //namespace grctk
