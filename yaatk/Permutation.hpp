/*
   Permutations generator (header file).

   Copyright (C) 2001, 2002, 2003, 2004, 2015, 2017, 2018 Alexander
   Yermolenko <yaa.mbox@gmail.com>

   This file is part of YAATK, Yet another auxiliary toolkit.

   YAATK is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   YAATK is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with YAATK.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef yaatk_Permutation_hpp
#define yaatk_Permutation_hpp

namespace yaatk
{

class Permutation
{
private:
  int* a;
  int  n;
public:
  Permutation(int size);
  ~Permutation();
  void gen_first();
  bool gen_next();
  bool is_last();
  const int& operator [](int i) const{ return a[i]; };
};

}

#endif
