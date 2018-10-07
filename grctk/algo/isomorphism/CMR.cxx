/*
  The Consecutive Matching with Returns (CMR) algorithm for the graph
  isomorphism problem.
  See the following article for the reference implementation:
  Pinchuk V.P. Raspoznavaniye izomorfnosti grafov: PNV-algoritm
  [Recognition of Graph Isomorphism: CMR-algorithm].
  Skladni sistemi i procesi [Complex Systems and Processes],
  2002, No. 1, pp. 4-11. [In Russian]
  <https://elibrary.ru/item.asp?id=11737054>

  Copyright (C) 2004, 2011, 2015, 2017, 2018 Alexander Yermolenko
  <yaa.mbox@gmail.com>

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

#include "CMR.hpp"
#include <stdexcept>
#include <algorithm>

namespace grctk
{

CMR::CMR(Log& setlog):
  AlgBase(setlog),
  n(0),Av(),Bv(),Adegs(),Bdegs(),st()
{
}

void
CMR::allocateMemory(size_t nv)
{
  if (nv <= n)
    return;

  Av.resize(nv);
  Bv.resize(nv);

  Adegs.resize(nv);
  Bdegs.resize(nv);

  st.resize(nv);
}

void
CMR::presort(const AdjMatrix& A, const AdjMatrix& B,
             const PresortOptions& presortOptions)
{
  if (presortOptions.useRule1)
  {
    logStream() << "Using presorting fragment 1\n";
    flushLogStreams();
    std::sort(Av.begin(),Av.end(),greaterAdeg(*this));
  }

  if (presortOptions.useRule2)
  {
    logStream() << "Using presorting fragment 2\n";
    flushLogStreams();
    std::sort(Bv.begin(),Bv.end(),greaterBdeg(*this));
  }

  if (presortOptions.useRule3)
  {
    logStream() << "Using presorting fragment 3\n";
    flushLogStreams();
    // add vertices one by one
    // on each step add the one that will add the most edges possible
    for(size_t vertex = 1; vertex < n; vertex++)
    {
      checkAborted();
      size_t maxDeg = 0;
      size_t maxDegVertex = vertex;
      for(size_t i = vertex; i < n; i++)
      {
        size_t deg = 0;
        for(size_t j = 0; j < vertex; j++)
          deg += A.s(Av[i],Av[j]);
        if (maxDeg < deg)
        {
          maxDeg = deg;
          maxDegVertex = i;
        }
      }
      std::swap(Av[vertex],Av[maxDegVertex]);
    }
  }
}

bool
CMR::operator()(const AdjMatrix& A, const AdjMatrix& B,
                const PresortOptions& presortOptions)
{
  logStream() << "\nCMR started\n";
  flushLogStreams();

  size_t n1 = A.size();
  size_t n2 = B.size();

  logStream() << "n1="<< n1 << " n2=" << n2 << "\n";
  flushLogStreams();

  if (n1 != n2)
  {
    logStream() << "CMR algorithm finished.\n";
    flushLogStreams();
    return false;
  }

  if (n1 > n)
    allocateMemory(n1);
  n = n1;

  for(size_t i = 0; i < n; i++)
  {
    // vertex degrees cache
    Adegs[i] = A.vertexDegree(i);
    Bdegs[i] = B.vertexDegree(i);
    // vertex mapping indices
    Av[i] = Bv[i] = i;
    // back tracking array
    st[i] = 0;
  }

  presort(A,B,presortOptions);

  using std::swap;

  bool maybeIsomorphic = true;
  size_t mapIndex = 0;
  for(; maybeIsomorphic && mapIndex < n; mapIndex++)
  {
    size_t mapCandidatesStart = mapIndex;
    while (1)
    {
      size_t i = mapCandidatesStart;
      for(; i < n; i++)
      {
        if (Adegs[Av[mapIndex]] == Bdegs[Bv[i]])
        {
          size_t j = 0;
          for(; j < mapIndex; j++)
            if (B.s(Bv[i],Bv[j]) != A.s(Av[mapIndex],Av[j]))
              break;
          if (j == mapIndex) // adjacencies for i-th vertex are sufficient
          {
            swap(Bv[mapIndex],Bv[i]);
            st[mapIndex] = i; // save mapping for backtracking
            break;
          }
        }
      }
      if (i == n) // if no vertrices left to try
      {
        if (mapIndex == 0) // if there is no way back
        {
          maybeIsomorphic = false; // request to finish mapping
          break; // stop finding the current match immediately
        }
        mapIndex--; // try to step back in mapping
        swap(Bv[mapIndex],Bv[st[mapIndex]]); // restore previous mapping
        mapCandidatesStart = st[mapIndex] + 1; // try mapping another vertex
      }
      else
        break; // match for the current mapIndex was found
    }
  }

  REQUIRE(maybeIsomorphic == (mapIndex == n));

  logStream() << "CMR algorithm finished.\n";
  flushLogStreams();

  return (mapIndex == n)?1:0;
}

}
