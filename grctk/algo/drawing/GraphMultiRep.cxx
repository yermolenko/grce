/*
  The GraphMultiRep class.

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

#include "GraphMultiRep.hpp"

namespace grctk
{

GraphMultiRep::GraphMultiRep(
  const AdjMatrix& graph,
  size_t dimensions):
  g(graph),
  dim(dimensions),
  aMultiXD(),
  energies()
{
}

GraphMultiRep::GraphMultiRep(
  const AdjMatrix& graph,
  size_t dimensions,
  Attribute<yaatk::Vector3D> a3D):
  g(graph),
  dim(dimensions),
  aMultiXD(),
  energies()
{
  for(size_t i = 0; i < g.size(); ++i)
  {
    yaatk::VectorXD vxd(dim);
    for(size_t idim = 0; idim < 3; ++idim)
      vxd[idim] = a3D[g[i]].X(idim);
    aMultiXD[g[i]].push_back(vxd);
  }
  energies.push_back(0.0);
}

void
GraphMultiRep::addRep(const GraphRep& rep)
{
  REQUIRE(g.size() == rep.g.size());
  for(size_t i = 0; i < g.size(); ++i)
  {
    REQUIRE(aMultiXD[g[i]].size() == 0 || aMultiXD[g[i]].back().size() == rep.aXD[rep.g[i]].size());
    REQUIRE(i == 0 || rep.aXD[rep.g[i]].size() == rep.aXD[rep.g[i-1]].size());
    aMultiXD[g[i]].push_back(rep.aXD[rep.g[i]]);
  }
  energies.push_back(rep.energy);
}

size_t
GraphMultiRep::findRepWithMinEnergy() const
{
  REQUIRE(energies.size() > 0);
  size_t indexOfMin = 0;
  for(size_t i = 0; i < energies.size(); ++i)
    if (energies[i] < energies[indexOfMin])
      indexOfMin = i;
  return indexOfMin;
}

size_t
GraphMultiRep::size() const
{
  REQUIRE(g.size() > 0);
  if (g.size() > 0)
    return aMultiXD[g[0]].size();
  else
    return 0;
}

const std::vector<yaatk::VectorXD>
GraphMultiRep::getRep(const size_t index) const
{
  std::vector<yaatk::VectorXD> rep(g.size());
  for(size_t i = 0; i < g.size(); ++i)
  {
    REQUIRE(aMultiXD[g[i]][index].size() == dim);
    rep[i].resize(dim);
    rep[i] = aMultiXD[g[i]][index];
  }
  return rep;
}

const std::vector<yaatk::Vector3D>
GraphMultiRep::getRep3DProj(const size_t index) const
{
  return RepXDtoRep3D(getRep(index));
}

std::vector<yaatk::Vector3D>
GraphMultiRep::RepXDtoRep3D(const std::vector<yaatk::VectorXD>& rXD)
{
  std::vector<yaatk::Vector3D> r3D(rXD.size());
  for(size_t i = 0; i < rXD.size(); ++i)
  {
    size_t dim2copy = (rXD[i].size() <= 3)?rXD[i].size():3;
    for(size_t idim = 0; idim < dim2copy; ++idim)
      r3D[i].X(idim) = rXD[i][idim];
  }
  return r3D;
}

std::vector<yaatk::VectorXD>
GraphMultiRep::Rep3DtoRepXD(const std::vector<yaatk::Vector3D>& r3D, size_t dim)
{
  std::vector<yaatk::VectorXD> rXD(r3D.size());
  for(size_t i = 0; i < rXD.size(); ++i)
  {
    rXD[i].resize(dim);
    for(size_t idim = 0; idim < 3; ++idim)
      rXD[i][idim] = r3D[i].X(idim);
  }
  return rXD;
}

}
