/*
  The FindOrbitsSubgraphIso algorithm.

  Copyright (C) 2003, 2004, 2011, 2015, 2017, 2018 Alexander
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

#include "FindOrbitsSubgraphIso.hpp"
#include "grctk/algo/isomorphism/CMR.hpp"
#include <limits>
#include <vector>
#include <algorithm>
#include <set>
#include <vector>

namespace grctk
{

void
FindOrbitsSubgraphIso::operator()(
  const AdjMatrix &g, Attribute<int>& aOrbit)
{
  logStream() << "\nFindOrbitsSubgraphIso started\n";
  flushLogStreams();

  std::vector<std::set<size_t> > orbits;
  size_t i,j,VC = g.vertexCount();
  orbits.resize(VC);

  for(i = 0;i<VC;i++)
  {
    AdjMatrix gi(g);
    gi.removeVertex(i);
    for(j = 0;j<VC;j++)
    {
      checkAborted();

      AdjMatrix gj(g);
      gj.removeVertex(j);

      CMR alg;
      if (alg(gi,gj))
        orbits[i].insert(j);
    }
  }

  for(int i = VC-1; i >=0; i--)
    for(std::set<size_t>::iterator k=orbits[i].begin();k!=orbits[i].end();k++)
      aOrbit[g[*k]] = i+1;

  logStream() << "FindOrbitsSubgraphIso finished.\n" ;
  flushLogStreams();
}

} //namespace grctk
