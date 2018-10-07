/*
  The BinCode class.

  Copyright (C) 2003, 2004, 2015, 2018 Alexander Yermolenko
  <yaa.mbox@gmail.com>

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

#include "BinCode.hpp"
#include <iostream>
#include <stdexcept>

namespace grctk
{

BinCode::BinCode(unsigned short int vcount):
  bic(size_t(0)),
  vc(vcount)
{
  unsigned long n = vcount;
  bic.resize((((n*(n-1))/2-1)/(8*sizeof(BaseType))+1));
}

BinCode::~BinCode()
{
}

BinCode::BinCode(const BinCode &obj):
  bic(obj.bic),vc(obj.vc)
{
}

BinCode&
BinCode::operator=(const BinCode &obj)
{
  if (this == &obj)
    return (*this);

  bic = obj.bic;
  vc = obj.vc;

  return (*this);
}

AdjMatrix
BinCode::getGraph(std::ifstream& in)
{
  AdjMatrix g(vc);

  for(size_t i = 0; i < bic.size(); i++)
    in.read((char*)&(bic[i]), sizeof(BaseType));

  std::vector<bool> a;
  for(size_t i = 0; i < bic.size(); i++)
  {
    for(int inn = sizeof(BaseType)*8-1; inn >= 0; inn--)
    {
      unsigned long uvc = vc;
      if (a.size() < (uvc*(uvc-1))/2)
        a.push_back(bic[i] & (1<<inn));
    }
  }

  size_t x = 0, y = 0;
  bool ok = true;
  for(size_t i = 0; i < a.size() && ok; i++)
  {
    if (x == y) { y++; x = 0; }
    if (x > vc-1 || y > vc-1) { ok = false; break; };
    if (a[i])
      g.edge(y, x, Universe::singleton().create());
    x++;
  }

  if (!ok)
    throw std::runtime_error("Error parsing BinCode format");

  return g;
}

void
BinCode::writeGraph(std::ofstream& out, const AdjMatrix& g)
{
  if (g.vertexCount() != vc)
    throw std::logic_error("BinCode::writeGraph failed !");

  std::vector<bool> a;

  for(size_t y = 1; y < vc; y++)
    for(size_t x = 0; x < y; x++)
    {
      if (g(y,x))
        a.push_back(true);
      else
        a.push_back(false);
    }

  if (a.size() != (vc*(vc-1))/2)
    throw std::logic_error("BinCode::writeGraph failed !");

  for(size_t i = 0; i < bic.size(); i++)
    bic[i] = 0;
  for(size_t i = 0; i < a.size(); i++)
  {
    size_t ibic = i/(sizeof(BaseType)*8);
    if (i == a.size()-1 && ibic != bic.size()-1)
      throw std::logic_error("BinCode::writeGraph failed !");
    int inn = sizeof(BaseType)*8 - 1 - i%(sizeof(BaseType)*8);
    bic[ibic] |= (BaseType(a[i])) << inn;
  }

  for(size_t i = 0; i < bic.size(); i++)
    out.write((char*)&(bic[i]), sizeof(BaseType));
}

unsigned long
BinCode::checksum()
{
  unsigned long nChecksum = 0;
  for(size_t i = 0; i < bic.size(); i++)
    nChecksum += (unsigned long)(bic[i]);
  return nChecksum;
}

}
