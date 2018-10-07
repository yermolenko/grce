/*
   The VectorXD class.

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

#ifndef yaatk_VectorXD_hpp
#define yaatk_VectorXD_hpp

#include "yaatk/tools.hpp"
#include "yaatk/Vector3D.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <valarray>
#include <vector>

namespace yaatk
{

typedef std::valarray<double> VectorXD;
typedef std::vector<VectorXD> VectorVectorXD;
typedef std::vector<VectorVectorXD> VectorVectorVectorXD;

template <class T>
T
module(const std::valarray<T>& v)
{
  T tmp(0.0);
  for(size_t i=0;i<v.size();i++)
    tmp += SQR(v[i]);
  return std::sqrt(tmp);
}

template <class T>
void
normalize(std::valarray<T>& v)
{
  T L(module(v));
  for(size_t i=0;i<v.size();i++) v[i]/=L;
}

template <class T>
std::valarray<T>
normalized(const std::valarray<T>& v)
{
  std::valarray<T> nv(v);
  normalize(nv);
  return nv;
}

template<class T>
std::istream&
operator>>(std::istream& is, std::valarray<T>& vec)
{
  std::string line;
  std::getline(is, line, '\n');
  if (!is) return is;
  {
    std::istringstream iss(line);
    T v;
    size_t size = 0;
    while (iss >> v)
      ++size;
    vec.resize(size);
  }
  {
    std::istringstream iss(line);
    T v;
    size_t i = 0;
    while (iss >> v)
    {
      vec[i] = v;
      ++i;
    }
  }

  return is;
}

template<class T>
std::istream&
operator>>(std::istream& is, std::vector<T>& vec)
{
  std::string line;
  std::getline(is, line, '\n');
  if (!is) return is;
  {
    std::istringstream iss(line);
    T v;
    size_t size = 0;
    while (iss >> v)
      ++size;
    vec.resize(size);
  }
  {
    std::istringstream iss(line);
    T v;
    size_t i = 0;
    while (iss >> v)
    {
      vec[i] = v;
      ++i;
    }
  }

  return is;
}

template<class T>
std::ostream&
operator<<(std::ostream& os, const std::valarray<T>& vec)
{
  for(size_t i=0;i<vec.size();i++)
  {
    os << vec[i] << " ";
  }

  return os;
}

template<class T>
std::ostream&
operator<<(std::ostream& os, const std::vector<T>& vec)
{
  for(size_t i=0;i<vec.size();i++)
    os << vec[i] << " ";

  return os;
}

template <class T>
void
resize(std::valarray<T>& v, size_t newSize, bool saveStored)
{
  if (saveStored)
  {
    v.resize(newSize);
  }
  else
  {
    v.resize(newSize);
    v = T();
  }
}

template <class T>
T
distance(std::valarray<T> &v1,std::valarray<T> &v2)
{
  return module(std::valarray<T>(v1-v2));
}

}

#endif
