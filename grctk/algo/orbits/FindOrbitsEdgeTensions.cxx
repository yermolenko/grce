/*
  The FindOrbitsEdgeTensions algorithm.

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

#include "FindOrbitsEdgeTensions.hpp"
#include <limits>
#include <vector>

namespace grctk
{

void
FindOrbitsEdgeTensions::f(size_t A, size_t b, yaatk::LongInteger divider,
                          const AdjMatrix& g)
{
  checkAborted();

  std::vector<size_t> vec; vec.reserve(100);
  for(size_t i = 0; i < g.size(); i++)
  {
    if (eW(A,i) + yaatk::LongInteger(g.s(i,b)?1:0) == eW(A,b) && g.s(i,b))
      vec.push_back(i);
  }

  yaatk::LongInteger newDivider = divider*yaatk::LongInteger(vec.size());

  for(size_t k = 0; k < vec.size();k++)
  {
    const grctk::Object& edge = g(vec[k],b);
    eRational[edge] = eRational[edge] + yaatk::Rational(1,newDivider);
    if (vec[k] != A)
      f(A,vec[k],newDivider, g);
  }
}

void
FindOrbitsEdgeTensions::vertexInvariants_Sum(
  const AdjMatrix& gr, Attribute<yaatk::Rational>& aOrbit)
{
  checkAborted();

  for(size_t i = 0; i < gr.size(); i++)
  {
    yaatk::Rational sum(yaatk::LongInteger(0));

    for(size_t j = 0; j < gr.size(); j++)
      if (gr.s(i,j))
        sum = sum + eRational[gr(i,j)];

    aOrbit[gr[i]] = sum;
  }
}

template <typename edgeset>
void
FindOrbitsEdgeTensions::vertexInvariants_Set(
  const AdjMatrix& gr, Attribute<yaatk::Rational>& aOrbit)
{
  checkAborted();

  std::vector<edgeset> setList;
  std::set<edgeset> s;

  for(size_t i = 0; i < gr.size(); i++)
  {
    edgeset sum;
    for(size_t j = 0; j < gr.size(); j++)
      if (gr.s(i,j))
        sum.insert(eRational[gr(i,j)]);
    setList.push_back(sum);
    s.insert(sum);
  }

  size_t index = 1;
  for(typename std::set<edgeset>::iterator it = s.begin(); it != s.end(); it++)
  {
    for(size_t i = 0; i < gr.size(); i++)
      if (setList[i] == *it)
        aOrbit[gr[i]] = yaatk::LongInteger(index);
    index++;
  }
}

void
FindOrbitsEdgeTensions::operator()(const AdjMatrix &g,
                                   Attribute<yaatk::Rational>& aOrbit)
{
  logStream() << "\nFindOrbitsEdgeTensions start\n";
  flushLogStreams();

  size_t VC = g.size();

  yaatk::LongInteger max = yaatk::veryBigLongInteger();
  eW.resize(VC);
  size_t i,j;
  for(j = 0;j < VC;j++)
    for(i = 0;i < VC;i++)
      eW(j,i) = (g.s(j,i)?yaatk::LongInteger(1):max);

  size_t k;
  for(k = 0;k < VC;k++)
    for(i = 0;i < VC;i++)
      for(j = 0;j < VC;j++)
        if ((eW(i,k) != max)&&
            (eW(k,j) != max))
        {
          checkAborted();

          yaatk::LongInteger tmp = eW(i,k) + eW(k,j);
          if (tmp < eW(i,j))
            eW(i,j) = tmp;
        }

  for(size_t i = 0; i < VC; i++)
    for(size_t j = 0; j < VC; j++)
      if (g.s(i,j))
        eRational[g(i,j)] = yaatk::LongInteger(0);

  for(j = 0;j < VC;j++)
    eW(j,j) = 0;

  for(size_t i = 0; i < VC; i++)
  {
    logStream() << "Processing from vertex " << i << "\n" ;
    flushLogStreams();
    for(size_t j = 0/*i+1*/; j < VC; j++)
      f(i,j,1,g);
  }

  // vertexInvariants_Sum(g,aOrbit);
  // vertexInvariants_Set<std::set<yaatk::Rational> >(g,aOrbit);
  vertexInvariants_Set<std::multiset<yaatk::Rational> >(g,aOrbit);

  logStream() << "FindOrbitsEdgeTensions finished\n" ;
  flushLogStreams();
}

} //namespace grctk
