/*
  The Attr2Color algorithm (header file).

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

#ifndef grce_Attr2Color_hpp
#define grce_Attr2Color_hpp

#include "grce/Color.hpp"
#include <grctk/algo/AlgBase.hpp>
#include <grctk/AdjMatrix.hpp>
#include <yaatk/Rational.hpp>

namespace grce
{

class Attr2Color : public grctk::AlgBase
{
  template <typename T>
  void attr2colorLinear(const grctk::AdjMatrix &g,
                        const grctk::Attribute<T>& aInt,
                        grctk::Attribute<Color>& aColor);
  template <typename T>
  void attr2color(const grctk::AdjMatrix &g,
                  const grctk::Attribute<T>& aInt,
                  grctk::Attribute<Color>& aColor);
public:
  void operator()(const grctk::AdjMatrix &g,
                  const grctk::Attribute<int>& aInt,
                  grctk::Attribute<Color>& aColor,
                  bool linearMapping = false);
  void operator()(const grctk::AdjMatrix &g,
                  const grctk::Attribute<yaatk::Rational>& a,
                  grctk::Attribute<Color>& aColor);
  Attr2Color(grctk::Log& setlog = grctk::nullLog): AlgBase(setlog) {}
};

} //namespace grce

#endif
