/*
  The FindOrbitsEdgeTensions algorithm (header file).

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

#ifndef grctk_FindOrbitsEdgeTensions_hpp
#define grctk_FindOrbitsEdgeTensions_hpp

#include "grctk/algo/AlgBase.hpp"
#include "grctk/Universe.hpp"
#include "grctk/AdjMatrix.hpp"
#include <yaatk/Rational.hpp>
#include <yaatk/LongInteger.hpp>

namespace grctk
{

class FindOrbitsEdgeTensions : public AlgBase
{
  yaatk::TriangularSquareMatrix<yaatk::LongInteger> eW;
  Attribute<yaatk::Rational> eRational;
  void f(size_t b, size_t A, yaatk::LongInteger divider, const AdjMatrix&);
  void vertexInvariants_Sum(const AdjMatrix&, Attribute<yaatk::Rational>& aOrbit);
  template <typename edgeset>
  void vertexInvariants_Set(const AdjMatrix&, Attribute<yaatk::Rational>& aOrbit);
public:
  void operator()(const AdjMatrix &g, Attribute<yaatk::Rational>& aOrbit);
  FindOrbitsEdgeTensions(Log& setlog = nullLog):
    AlgBase(setlog),
    eW(),
    eRational(Universe::singleton(),"Temporarily rational attribute for edges")
    {}
};

} //namespace grctk

#endif
