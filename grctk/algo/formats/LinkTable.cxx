/*
  The LinkTable class.

  Copyright (C) 2001, 2002, 2003, 2004, 2015, 2018 Alexander
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

#include "LinkTable.hpp"
#include <sstream>

namespace grctk
{

void
LinkTable::exportTo(const AdjMatrix& g, std::ostream& os)
{
  logStream() << "\nLinkTable::export() started\n";
  flushLogStreams();

  size_t vc = g.size();
  os << vc << "\n";
  for(size_t j = 0;j < vc;j++)
  {
    os << (j+1) << " ";
    os << std::setw(10) << g.vertexDegree(j);
    for(size_t i = 0;i < vc;i++)
      if (g(j,i))
        os << " " << std::setw(10) << (i+1);
    os << "\n";
  }

  logStream() << "LinkTable::export() finished\n";
  flushLogStreams();
}

void
LinkTable::importFrom(AdjMatrix& g, std::istream& is)
{
  logStream() << "\nLinkTable::import() started\n";
  flushLogStreams();

  size_t vc;
  is >> vc;
  for(size_t j = 0; j < vc; j++)
    g.addVertex(grctk::Universe::singleton().create());
  for(size_t j = 0; j < vc; j++)
  {
    size_t tmp;
    is >> tmp;
    if ((tmp-1) != j)
    {
      errStream() << "Incorrect file format!";
      flushLogStreams();
      return;
    }
    size_t deg;
    is >> deg;
    for(size_t inddeg = 0; inddeg < deg; inddeg++)
    {
      size_t x;
      is >> x;
      g.edge(j,x-1,grctk::Universe::singleton().create());
    }
  }

  logStream() << "LinkTable::import() finished\n";
  flushLogStreams();
}

}
