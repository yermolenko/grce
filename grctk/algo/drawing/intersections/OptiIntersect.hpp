/*
  The OptiIntersect algorithm (header file).

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

#ifndef grctk_OptiIntersect_hpp
#define grctk_OptiIntersect_hpp

#include "grctk/algo/AlgBase.hpp"
#include "grctk/AdjMatrix.hpp"
#include <yaatk/Vector2D.hpp>

namespace grctk
{

class OptiIntersect : public AlgBase
{
  size_t intersectionsCount(const AdjMatrix &g,
                            const size_t *p_cur,
                            const std::vector<yaatk::Vector2D>& R) const;
public:
  OptiIntersect(Log& setlog = nullLog) : AlgBase(setlog) {}
  void operator()(const AdjMatrix &g, const int& gc, Attribute<yaatk::Vector2D>& a2D);
};

} //namespace grctk

#endif
