/*
   Permutations generator.

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

#include "Permutation.hpp"

namespace yaatk
{

Permutation::Permutation(int size)
{
  a = new int[size];
  n = size;
}

Permutation::~Permutation()
{
  delete [] a;
}

void Permutation::gen_first()
{
  for(int i = 0;i < n;i++) a[i] = i;
}

bool Permutation::gen_next()
{
  int tmp;
  if (!is_last())
  {
    int i,j,k;
    i = n-2;
    while (a[i] >= a[i+1])  i--;

    j = n-1;
    while (a[j] <= a[i])  j--;

    tmp = a[j];
    a[j] = a[i];
    a[i] = tmp;

    for(k = 1;i + k < n-k;k++)
    {
      tmp = a[i+k];
      a[i+k] = a[n-k];
      a[n-k] = tmp;
    }
    return true;
  }
  else
  {
    return false;
  }
}

bool Permutation::is_last()
{
  bool is_lastX = true;
  for(int i = n-1;i>=0;i--)
    if (a[i]!=n-i-1)
    {
      is_lastX = false;
      break;
    }
  return is_lastX;
}

}
