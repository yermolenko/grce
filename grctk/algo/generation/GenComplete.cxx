/*
  The GenComplete class.

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

#include "GenComplete.hpp"

namespace grctk
{

AdjMatrix
GenComplete::operator()(size_t size)
{
  logStream() << "\nGenComplete started\n";
  logStream() << "size = " << size << "\n";
  flushLogStreams();

  AdjMatrix g;

  for(size_t i = 0; i < size; ++i)
  {
    checkAborted();
    g += grctk::Universe::singleton().create();
  }

  for(size_t j = 0; j < size; ++j)
    for(size_t i = j; i < size; ++i)
      if (j != i)
        g.edge(j,i,Universe::singleton().create());

  logStream() << "GenComplete finished\n" ;
  flushLogStreams();

  return g;
}

} //namespace grctk
