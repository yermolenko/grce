/*
  The GraphMultiRep class (header file).

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

#ifndef grctk_GraphMultiRep_hpp
#define grctk_GraphMultiRep_hpp

#include "grctk/Universe.hpp"
#include "grctk/AdjMatrix.hpp"
#include "grctk/algo/drawing/GraphRep.hpp"
#include <yaatk/VectorXD.hpp>
#include <yaatk/Vector3D.hpp>
#include <sstream>
#include <string>

namespace grctk
{

struct GraphMultiRep
{
  const AdjMatrix g;
  const size_t dim;
  Attribute<std::vector<yaatk::VectorXD> > aMultiXD;
  std::vector<double> energies;
  GraphMultiRep(const AdjMatrix& graph, size_t dimensions);
  GraphMultiRep(const AdjMatrix& graph, size_t dimensions,
                Attribute<yaatk::Vector3D> a3D);
  void addRep(const GraphRep& rep);
  size_t findRepWithMinEnergy() const;
  std::vector<double> attrArray() const { return energies; }
  size_t size() const;
  const std::vector<yaatk::VectorXD> getRep(const size_t index) const;
  const std::vector<yaatk::Vector3D> getRep3DProj(const size_t index) const;
  static std::vector<yaatk::Vector3D> RepXDtoRep3D(const std::vector<yaatk::VectorXD>& rXD);
  static std::vector<yaatk::VectorXD> Rep3DtoRepXD(const std::vector<yaatk::Vector3D>& r3D, size_t dim);
};

}

#endif
