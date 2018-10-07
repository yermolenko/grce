/*
  The CmpSubgraphIsoAndEdgeTensions algorithm (header file).

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

#ifndef grctk_CmpSubgraphIsoAndEdgeTensions_hpp
#define grctk_CmpSubgraphIsoAndEdgeTensions_hpp

#include "grctk/algo/AlgBase.hpp"
#include "grctk/Universe.hpp"
#include "grctk/AdjMatrix.hpp"
#include <yaatk/Rational.hpp>
#include <set>

namespace grctk
{

class CmpSubgraphIsoAndEdgeTensions : public AlgBase
{
public:
  bool operator()(const AdjMatrix &g,
                  Attribute<int>& aOrbitSubgraphIso,
                  Attribute<yaatk::Rational>& aOrbitEdgeTensions);
  CmpSubgraphIsoAndEdgeTensions(Log& setlog = nullLog): AlgBase(setlog) {}
  template <typename T>
  static size_t getOrbitsCount(const AdjMatrix& g, const Attribute<T>& a)
    {
      std::set<T> s;
      for(size_t i = 0; i < g.size(); i++)
        s.insert(a[g[i]]);
      return s.size();
    }
};

} //namespace grctk

#endif
