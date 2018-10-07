/*
  The AdjMatrix class.

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

#include "AdjMatrix.hpp"
#include "grctk/algo/formats/Environment.hpp"
#include <algorithm>
#include <stdexcept>

namespace grctk
{

void AdjMatrix::acquireOwnership()
{
  size_t i, j, size = vertices.size();
  for(i = 0; i < size; ++i)
    for(j = i; j < size; ++j)
      if (am(i,j))
        am(i,j).addOwner();
  for(i = 0; i < size; ++i)
    vertices[i].addOwner();
}

void AdjMatrix::discardOwnership()
{
  size_t i, j, size = vertices.size();
  for(i = 0; i < size; ++i)
    for(j = i; j < size; ++j)
      if (am(i,j))
        am(i,j).removeOwner();
  for(i = 0; i < size; ++i)
    vertices[i].removeOwner();
}

AdjMatrix::AdjMatrix():
  vertices(),am()
{
  acquireOwnership();
}

AdjMatrix::AdjMatrix(size_t vertexCount):
  vertices(),am(vertexCount)
{
  vertices.reserve(vertexCount);
  for(size_t i = 0; i < vertexCount; ++i)
  {
    vertices.push_back(Universe::singleton().create());
    vertices.back().addOwner();
  }
}

AdjMatrix::AdjMatrix(const AdjMatrix& obj):
  vertices(obj.vertices),am(obj.am)
{
  acquireOwnership();
}

AdjMatrix::~AdjMatrix()
{
  discardOwnership();
}

AdjMatrix&
AdjMatrix::operator=(const AdjMatrix& obj)
{
  return assign(obj);
}

AdjMatrix&
AdjMatrix::assign(const AdjMatrix& obj)
{
  if (this == &obj)
    return (*this);

  discardOwnership();

  vertices = obj.vertices;
  am = obj.am;

  acquireOwnership();

  return (*this);
}

AdjMatrix&
AdjMatrix::cloneFrom(const AdjMatrix& obj)
{
  if (this == &obj)
    return (*this);

  discardOwnership();

  vertices.clear();

  size_t i,j,vc = obj.vertices.size();
  vertices.reserve(vc);
  for(i = 0; i < vc; ++i)
    vertices.push_back(obj.vertices[i].clone());

  am = yaatk::TriangularSquareMatrix<Object>(vc);
  for(i = 0; i < vc; ++i)
    for(j = i+1; j < vc; ++j)
      if (obj.am(i,j))
        am(i,j) = obj.am(i,j).clone();

  acquireOwnership();

  return (*this);
}

AdjMatrix
AdjMatrix::clone() const
{
  AdjMatrix g;
  g.cloneFrom(*this);
  return g;
}

bool
AdjMatrix::hasVertex(const Object& v)
{
  std::vector<Object>::const_iterator i
    = std::find(vertices.begin(),vertices.end(),v);
  if (i == vertices.end())
    return false;
  return true;
}

size_t
AdjMatrix::vertexIndex(const Object& v)
{
  std::vector<Object>::const_iterator i
    = std::find(vertices.begin(),vertices.end(),v);
  if (i == vertices.end())
    throw std::logic_error("AdjMatrix does not contain the vertex");
  return i - vertices.begin();
}

size_t
AdjMatrix::vertexDegree(size_t index) const
{
  size_t deg = 0, vc = vertices.size();
  for(size_t i = 0; i < vc; ++i)
    deg += am(index,i)?1:0;
  return deg;
}

void
AdjMatrix::operator+=(const Object& vertex)
{
  vertices.push_back(vertex);
  vertex.addOwner();
  am.resize(vertices.size());
}

void
AdjMatrix::operator-=(size_t vertexIndex)
{
  size_t size = vertices.size();
  for(size_t i = 0; i < size; ++i)
    if (am(i,vertexIndex))
    {
      am(i,vertexIndex).removeOwner();
      am(i,vertexIndex) = Object();
    }
  am.remove(vertexIndex);

  vertices[vertexIndex].removeOwner();
  vertices.erase(vertices.begin() + vertexIndex);
}

void
AdjMatrix::clear()
{
  discardOwnership();
  am.resize(0);
  vertices.clear();
}

std::ostream&
operator<<(std::ostream& os, const AdjMatrix& m)
{
  saveToStream(os, m);
  return os;
}

}
