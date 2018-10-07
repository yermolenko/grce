/*
  The AdjMatrix class (header file).

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

#ifndef grctk_AdjMatrix_hpp
#define grctk_AdjMatrix_hpp

#include <yaatk/SquareMatrix.hpp>
#include "grctk/Universe.hpp"
#include <vector>
#include <iostream>

namespace grctk
{

class AdjMatrix
{
  std::vector<Object> vertices;
  yaatk::TriangularSquareMatrix<Object> am;
  void acquireOwnership();
  void discardOwnership();
public:
  AdjMatrix();
  AdjMatrix(size_t vertexCount);
  AdjMatrix(const AdjMatrix&);
  AdjMatrix& operator=(const AdjMatrix&);
  AdjMatrix& assign(const AdjMatrix&);
  AdjMatrix& cloneFrom(const AdjMatrix&);
  AdjMatrix clone() const;
  virtual ~AdjMatrix();

  size_t size() const { return vertices.size(); }
  size_t vertexCount() const { return size(); }

  const Object& getVertex(const size_t vertexIndex) const
    {
      return vertices[vertexIndex];
    }
  const Object& vertex(const size_t vertexIndex) const
    {
      return vertices[vertexIndex];
    }
  const Object& operator[](const size_t vertexIndex) const
    {
      return vertices[vertexIndex];
    }

  const Object& edge(size_t vi1, size_t vi2) const
    {
      return am(vi1,vi2);
    }
  bool s(size_t vi1, size_t vi2) const
    {
      return bool(am(vi1,vi2));
    }
  const Object& operator()(size_t vi1, size_t vi2) const
    {
      return am(vi1,vi2);
    }
  const Object& edge(size_t vi1, size_t vi2, const Object& e)
    {
      assert(vi1 != vi2 || !bool(e));
      if (am(vi1,vi2))
        am(vi1,vi2).removeOwner();
      am(vi1,vi2) = e;
      if (e)
        e.addOwner();
      return e;
    }

  bool hasVertex(const Object&);
  size_t vertexIndex(const Object&);
  size_t vertexDegree(size_t index) const;

  void operator+=(const Object& vertex);
  void addVertex(const Object& vertex) { operator+=(vertex); }
  void operator-=(size_t vertexIndex);
  void removeVertex(size_t vertexIndex) { operator-=(vertexIndex); }

  void clear();

  friend std::ostream& operator<<(std::ostream& os, const AdjMatrix& m);
  // friend std::istream& operator>>(std::istream& is, AdjMatrix& m);
  // friend std::istream& rescue(std::istream& is, AdjMatrix& m);
};

std::ostream& operator<<(std::ostream& os, const AdjMatrix& m);
// std::istream& operator>>(std::istream& is, AdjMatrix& m);
// std::istream& rescue(std::istream& is, AdjMatrix& m);

}

#endif
