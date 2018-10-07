/*
  The ResetColors algorithm.

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

#include "ResetColors.hpp"

namespace grce
{
void
ResetColors::operator()(const grctk::AdjMatrix &g, grctk::Attribute<Color>& aColor,
                        bool resetVertexColors, bool resetEdgeColors)
{
  {
    logStream() << "\nResetColors started\n";
    flushLogStreams();
  }

  time_t t;
  srand((unsigned)time(&t));

  if (resetVertexColors)
    for(size_t i = 0; i < g.size(); ++i)
      aColor[g[i]] = Color(255,0,0);

  if (resetEdgeColors)
    for(size_t i = 0; i < g.size(); ++i)
      for(size_t j = i+1; j < g.size(); ++j)
        if (g(i,j))
          aColor[g(i,j)] = Color(0,127,0);

  {
    logStream() << "ResetColors finished\n" ;
    flushLogStreams();
  }
}

} //namespace grce
