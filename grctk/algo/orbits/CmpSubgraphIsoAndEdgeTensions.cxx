/*
  The CmpSubgraphIsoAndEdgeTensions algorithm.

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

#include "CmpSubgraphIsoAndEdgeTensions.hpp"

#include <limits>
#include <vector>
#include <algorithm>

#include "grctk/algo/orbits/FindOrbitsSubgraphIso.hpp"
#include "grctk/algo/orbits/FindOrbitsEdgeTensions.hpp"

namespace grctk
{

bool
CmpSubgraphIsoAndEdgeTensions::operator()(
  const AdjMatrix &g,
  Attribute<int>& aOrbitSubgraphIso,
  Attribute<yaatk::Rational>& aOrbitEdgeTensions)
{
  logStream() << "\nCmpSubgraphIsoAndEdgeTensions started\n";
  flushLogStreams();

  size_t VC = g.vertexCount();

  checkAborted();
  FindOrbitsSubgraphIso alg1;
  alg1(g,aOrbitSubgraphIso);

  checkAborted();
  FindOrbitsEdgeTensions alg2;
  alg2(g,aOrbitEdgeTensions);

  bool correct = true;
  for(size_t i1 = 0; i1 < VC && correct; i1++)
    for(size_t i2 = 0; i2 < VC && correct; i2++)
      if (
        (aOrbitSubgraphIso[g[i1]] != aOrbitSubgraphIso[g[i2]])
        &&
        (aOrbitEdgeTensions[g[i1]] == aOrbitEdgeTensions[g[i2]])
        )
        correct = false;

  logStream() << "FindOrbitsSubgraphIso and FindOrbitsEdgeTensions produced ";
  if (correct)
    logStream() << "the same results\n";
  else
    logStream() << "*different* results\n";

  logStream() << "CmpSubgraphIsoAndEdgeTensions finished\n";
  flushLogStreams();

  return correct;
}

} //namespace grctk
