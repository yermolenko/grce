/*
   The SquareMatrix and TriangularSquareMatrix classes.

   Copyright (C) 2001, 2002, 2003, 2004, 2015, 2016, 2017, 2018
   Alexander Yermolenko <yaa.mbox@gmail.com>

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

#ifndef yaatk_SquareMatrix_hpp
#define yaatk_SquareMatrix_hpp

#include "Exception.hpp"

#include <yaatk/yaatk.hpp>

#include <iostream>
#include <iomanip>
#include <vector>

namespace yaatk
{

template <class T>
class SquareMatrix
{
protected:
  size_t n;
  std::vector<T> data;
public:
  SquareMatrix()
    : n(0), data() {}
  explicit SquareMatrix(size_t size)
    : n(size), data(size*size) {}
  SquareMatrix(const SquareMatrix<T> &obj)
    : n(obj.n), data(obj.data) {}
  SquareMatrix<T>& operator=(const SquareMatrix<T> &obj)
    {
      if (this == &obj) return *this;

      n = obj.n;
      data = obj.data;

      return *this;
    }
  virtual ~SquareMatrix() {}
  size_t size() const { return n; }
  T& operator ()(size_t i, size_t j)
    {
      return data[i*n + j];
    }
  const T& operator ()(size_t i, size_t j) const
    {
      return data[i*n + j];
    }
  // T* operator [](size_t i)
  //   {
  //     return data + i*n;
  //   }
  // const T* operator [](size_t i) const
  //   {
  //     return data + i*n;
  //   }
  void resize(size_t newSize)
    {
      if (n == newSize)
        return;

      if (n < newSize)
      {
        data.resize(newSize*newSize);
        if (n > 0)
          for(size_t i = newSize - 1;; i--)
          {
            for(size_t j = newSize - 1;; j--)
            {
              if (i < n && j < n)
                data[i*newSize + j] = data[i*n + j];
              else
                data[i*newSize + j] = T();
              if (j == 0)
                break;
            }
            if (i == 0)
              break;
          }
      }
      else
      {
        for(size_t i = 0; i < newSize; i++)
          for(size_t j = 0; j < newSize; j++)
            data[i*newSize + j] = data[i*n + j];
        data.resize(newSize*newSize);
      }

      n = newSize;
    }
  void remove(size_t row)
    {
      for(size_t i = 0; i < n; i++)
        for(size_t j = 0; j < n; j++)
        {
          size_t new_i = (i > row)?(i-1):i;
          size_t new_j = (j > row)?(j-1):j;
          data[new_i*(n-1) + new_j] = data[i*n + j];
        }
      n--;
      data.resize(n*n);
    }
};

template<class T>
inline
std::ostream&
operator<<(std::ostream& os, const SquareMatrix<T>& m)
{
  for(size_t j = 0; j < m.size(); j++)
  {
    for(size_t i = 0; i < m.size(); i++)
      os << m(j,i) << ((i != m.size()-1)?"\t":"");
    os << std::endl;
  }
  return os;
}

template<class T>
inline
std::istream&
operator>>(std::istream& is, SquareMatrix<T>& m)
{
  std::string line;
  std::getline(is, line, '\n');
  if (!is)
    throw std::runtime_error("Error reading SquareMatrix");
  std::istringstream iss(line);
  T v;
  std::vector<T> row;
  while (iss >> v)
  {
    row.push_back(v);
    if (!iss)
      throw std::runtime_error("Error reading SquareMatrix");
  }
  m.resize(row.size());
  for(size_t j = 0; j < m.size(); j++)
  {
    for(size_t i = 0; i < m.size(); i++)
      if (j == 0)
        m(j,i) = row[i];
      else
      {
        is >> m(j,i);
        if (!is)
          throw std::runtime_error("Error reading SquareMatrix");
      }
  }
  return is;
}

template <class T>
class TriangularSquareMatrix
{
protected:
  size_t n;
  std::vector<T> data;
public:
  TriangularSquareMatrix()
    : n(0), data() {}
  explicit TriangularSquareMatrix(size_t size)
    : n(size), data(size*(size+1)/2) {}
  TriangularSquareMatrix(const TriangularSquareMatrix<T> &obj)
    : n(obj.n), data(obj.data) {}
  TriangularSquareMatrix<T>& operator=(const TriangularSquareMatrix<T> &obj)
    {
      if (this == &obj) return *this;

      n = obj.n;
      data = obj.data;

      return *this;
    }
  virtual ~TriangularSquareMatrix() {}
  size_t size() const { return n; }
  T& operator ()(size_t i, size_t j)
    {
      if (j > i)
      {
        size_t t = i;
        i = j;
        j = t;
      }
      return data[(i+1)*i/2 + j];
    }
  const T& operator ()(size_t i, size_t j) const
    {
      if (j > i)
      {
        size_t t = i;
        i = j;
        j = t;
      }
      return data[(i+1)*i/2 + j];
    }
  void resize(size_t newSize)
    {
      if (n == newSize)
        return;

      data.resize(newSize*(newSize+1)/2);

      n = newSize;
    }
  void remove(size_t row)
    {
      for(size_t i = row + 1; i < n; i++)
        for(size_t j = 0; j < i; j++)
        {
          size_t new_i = i-1;
          size_t new_j = (j > row)?(j-1):j;
          data[new_i*(new_i+1)/2 + new_j] = data[i*(i+1)/2 + j];
        }
      n--;
      data.resize(n*(n+1)/2);
    }
};

template<class T>
inline
std::ostream&
operator<<(std::ostream& os, const TriangularSquareMatrix<T>& m)
{
  for(size_t j = 0; j < m.size(); j++)
  {
    for(size_t i = 0; i < j; i++)
      os << "" << ((i != m.size()-1)?"\t":"");
    for(size_t i = j; i < m.size(); i++)
      os << m(j,i) << ((i != m.size()-1)?"\t":"");
    os << std::endl;
  }
  return os;
}

template<class T>
inline
std::istream&
operator>>(std::istream& is, TriangularSquareMatrix<T>& m)
{
  std::string line;
  std::getline(is, line);
  if (!is)
    throw std::runtime_error("Error reading SquareMatrix");
  std::istringstream iss(line);
  T v;
  std::vector<T> row;
  while (iss >> v)
  {
    row.push_back(v);
    if (!iss)
      throw std::runtime_error("Error reading SquareMatrix");
  }
  m.resize(row.size());
  for(size_t j = 0; j < m.size(); j++)
  {
    for(size_t i = j; i < m.size(); i++)
      if (j == 0)
        m(j,i) = row[i];
      else
      {
        is >> m(j,i);
        if (!is)
          throw std::runtime_error("Error reading SquareMatrix");
      }
  }
  return is;
}

} // namespace yaatk

#endif
