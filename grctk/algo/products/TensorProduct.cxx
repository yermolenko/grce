/*
  The TensorProduct algorithm.

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

#include "TensorProduct.hpp"
#include "grctk/algo/drawing/tools.hpp"

namespace grctk
{

AdjMatrix
TensorProduct::operator()(const AdjMatrix& g1, const AdjMatrix& g2)
{
  logStream() << "\nTensorProduct started\n";
  flushLogStreams();

  AdjMatrix g = graphTemplate(g1,g2);

  size_t n1 = g1.size();
  size_t n2 = g2.size();

  for(size_t i = 0; i < n1; i++)
    for(size_t j = 0; j < n1; j++)
      if (g1.edge(i,j))
        for(size_t p = 0; p < n2; p++)
          for(size_t q = 0; q < n2; q++)
            if (!g.edge(i*n2+p,j*n2+q) && g2.edge(p,q))
            {
              checkAborted();
              g.edge(i*n2+p,j*n2+q,Universe::singleton().create());
            }

  logStream() << "TensorProduct finished.\n" ;
  flushLogStreams();

  return g;
}

AdjMatrix
TensorProduct::operator()(const AdjMatrix& g1, const AdjMatrix& g2,
                          Attribute<yaatk::Vector3D>& a3D)
{
  logStream() << "\nTensorProduct with smart placing started\n";
  flushLogStreams();

  AdjMatrix g = operator()(g1,g2);
  positionVertices(g,g1,g2,a3D);

  logStream() << "TensorProduct with smart placing finished.\n";
  flushLogStreams();

  return g;
}

} //namespace grctk
