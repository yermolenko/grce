/*
  The Map2Int algorithm.

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

#ifndef grctk_Map2Int_hpp
#define grctk_Map2Int_hpp

#include "grctk/algo/AlgBase.hpp"
#include "grctk/AdjMatrix.hpp"

namespace grctk
{

template <typename T>
class Map2Int : public AlgBase
{
public:
  void operator()(const AdjMatrix &g, const Attribute<T>& a, Attribute<int>& aInt);
  Map2Int(Log& setlog = nullLog): AlgBase(setlog) {}
};

template <typename T>
void
Map2Int<T>::operator()(const AdjMatrix &g, const Attribute<T>& a, Attribute<int>& aInt)
{
  {
    logStream() << "\nMap2Int started\n";
    flushLogStreams();
  }

  std::set<T> attrValues;

  for(size_t i = 0; i < g.size(); ++i)
    attrValues.insert(a[g[i]]);

  size_t attrValueIndex = 1;
  for(typename std::set<T>::const_iterator itAttr = attrValues.begin();
      itAttr != attrValues.end();
      ++itAttr)
  {
    for(size_t i = 0; i < g.size(); ++i)
      if (a[g[i]] == *itAttr)
        aInt[g[i]] = attrValueIndex;
    attrValueIndex++;
  }

  {
    logStream() << "Map2Int finished\n" ;
    flushLogStreams();
  }
}

} //namespace grctk

#endif
