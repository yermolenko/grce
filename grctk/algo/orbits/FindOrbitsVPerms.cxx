/*
  The FindOrbitsVPerms algorithm.

  Copyright (C) 2002, 2003, 2004, 2011, 2015, 2017, 2018 Alexander
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

#include "FindOrbitsVPerms.hpp"
#include <yaatk/Permutation.hpp>

namespace grctk
{

void
FindOrbitsVPerms::operator()(const AdjMatrix &g, Attribute<int>& aOrbit)
{
  logStream() << "\nFindOrbitsVPerms started\n";
  flushLogStreams();

  std::vector<std::set<size_t> > orbits;
  size_t i,k,VC = g.vertexCount();
  orbits.resize(VC);

  yaatk::Permutation j(VC);
  j.gen_first();

  bool am = true;

  do
  {
    am = true;

    for(i=0;i<VC && am;i++)
      for(k=0;k<VC && am;k++)
        if (g.s(i,k)!=g.s(j[i],j[k]))
          am = false;

    if (am)
      for(i = 0;i<VC;i++)
        orbits[i].insert(j[i]);

    checkAborted();

    if (!(j.gen_next())) break;
  } while (1);

  for(int i = VC-1; i >=0; i--)
    for(std::set<size_t>::iterator k=orbits[i].begin();k!=orbits[i].end();k++)
      aOrbit[g[*k]] = i+1;

  logStream() << "FindOrbitsVPerms finished\n" ;
  flushLogStreams();
}

} //namespace grctk
