/*
  The BinCode class (header file).

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

#ifndef grctk_BinCode_hpp
#define grctk_BinCode_hpp

#include "grctk/AdjMatrix.hpp"
#include <vector>
#include <fstream>

namespace grctk
{

class BinCode
{
  typedef unsigned short int BaseType;
  std::vector<BaseType> bic;
  unsigned short int vc;
public:
  size_t getLen() { return bic.size()*sizeof(BaseType); }
  BinCode(unsigned short int vcount);
  virtual ~BinCode();
  BinCode(const BinCode&);
  BinCode& operator = (const BinCode&);

  AdjMatrix getGraph(std::ifstream &);
  void writeGraph(std::ofstream &, const AdjMatrix& g);

  unsigned long checksum();
};

}

#endif
