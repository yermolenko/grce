/*
  The Attr2Color algorithm.

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

#include "Attr2Color.hpp"
#include <limits>

namespace grce
{

template <typename T>
void
Attr2Color::attr2colorLinear(const grctk::AdjMatrix &g,
                             const grctk::Attribute<T>& a,
                             grctk::Attribute<Color>& aColor)
{
  T minVal = (g.size()>0) ? a[g[0]] : std::numeric_limits<T>::max();
  T maxVal = (g.size()>0) ? a[g[0]] : std::numeric_limits<T>::min();
  for(size_t i = 0; i < g.size(); ++i)
  {
    if (a[g[i]] > maxVal)
      maxVal = a[g[i]];
    if (a[g[i]] < minVal)
      minVal = a[g[i]];
  }

  Color maxColor(0,0,0xFF);
  Color minColor(0xFF,0,0);

  for(size_t i = 0; i < g.size(); ++i)
    aColor[g[i]] = ((maxVal == minVal)?0:
                    (double(a[g[i]] - minVal)*(maxColor - minColor)/(maxVal - minVal)))
      + minColor;
}

template <typename T>
void
Attr2Color::attr2color(const grctk::AdjMatrix &g,
                       const grctk::Attribute<T>& a,
                       grctk::Attribute<Color>& aColor)
{
  std::set<T> attrValues;

  for(size_t i = 0; i < g.size(); ++i)
    attrValues.insert(a[g[i]]);

  ColorGenerator colorGen;

  size_t attrValueIndex = 0;
  for(typename std::set<T>::const_iterator itAttr = attrValues.begin();
      itAttr != attrValues.end();
      ++itAttr)
  {
    Color c = colorGen();
    for(size_t i = 0; i < g.size(); ++i)
      if (a[g[i]] == *itAttr)
        aColor[g[i]] = c;
    attrValueIndex++;
  }
}

void
Attr2Color::operator()(const grctk::AdjMatrix &g,
                       const grctk::Attribute<int>& a,
                       grctk::Attribute<Color>& aColor,
                       bool linearMapping)
{
  {
    logStream() << "\nAttr2Color started\n";
    flushLogStreams();
  }

  if (linearMapping)
    attr2colorLinear<int>(g,a,aColor);
  else
    attr2color<int>(g,a,aColor);

  {
    logStream() << "Attr2Color finished\n" ;
    flushLogStreams();
  }
}

void
Attr2Color::operator()(const grctk::AdjMatrix &g,
                       const grctk::Attribute<yaatk::Rational>& a,
                       grctk::Attribute<Color>& aColor)
{
  {
    logStream() << "\nAttr2Color started\n";
    flushLogStreams();
  }

  attr2color<yaatk::Rational>(g,a,aColor);

  {
    logStream() << "Attr2Color finished\n" ;
    flushLogStreams();
  }
}

} //namespace grce
