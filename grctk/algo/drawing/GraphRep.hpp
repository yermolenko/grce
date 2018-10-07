/*
  The GraphRep class.

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

#ifndef grctk_GraphRep_hpp
#define grctk_GraphRep_hpp

#include "grctk/Universe.hpp"
#include "grctk/AdjMatrix.hpp"
#include <yaatk/VectorXD.hpp>
#include <sstream>
#include <string>

namespace grctk
{

struct GraphRep
{
  const AdjMatrix g;
  const size_t dim;
  Attribute<yaatk::VectorXD> aXD;
  double energy;
  GraphRep(
    const AdjMatrix& graph,
    size_t dimensions):
    g(graph),
    dim(dimensions),
    aXD(),
    energy(0.0)
    {
    }
};

}

#endif
