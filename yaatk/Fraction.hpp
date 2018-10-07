/*
   The Fraction class.

   Copyright (C) 2001, 2002, 2003, 2004, 2015, 2016, 2017, 2018 Alexander
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

#ifndef yaatk_Fraction_hpp
#define yaatk_Fraction_hpp

#include <iostream>

namespace yaatk
{

template <class T>
class Fraction;

template <class T>
std::istream&
operator>> (std::istream& is, Fraction<T>& vec);

template <class T>
std::ostream&
operator<< (std::ostream& os, const Fraction<T>& vec);

template <class T>
bool
operator==(const Fraction<T>& v1, const Fraction<T>& v2);

template <class T>
bool
operator!=(const Fraction<T>& v1, const Fraction<T>& v2);

template <class T>
bool
operator<(const Fraction<T>& v1, const Fraction<T>& v2);

template <class T>
bool
operator>(const Fraction<T>& v1, const Fraction<T>& v2);

template <class T>
bool
operator>=(const Fraction<T>& v1, const Fraction<T>& v2);

template <class T>
bool
operator<=(const Fraction<T>& v1, const Fraction<T>& v2);

template <class T>
Fraction<T>
operator+(const Fraction<T>& a, const Fraction<T>& b);

template <class T>
void
operator+=(Fraction<T>& v, const Fraction<T>& a);

template <class T>
void
operator-=(Fraction<T>& v, const Fraction<T>& a);

template <class T>
Fraction<T>
operator-(const Fraction<T>& v, const Fraction<T>& b);

template <class T>
Fraction<T>
operator*(const Fraction<T>& v, const Fraction<T>& b);

template <class T>
Fraction<T>
operator/(const Fraction<T>& v, const Fraction<T>& b);

template <class T>
Fraction<T> operator*(const Fraction<T>& v, const T& m);

template <class T>
Fraction<T>
operator/(const Fraction<T>& v, const T& m);

template <class T>
Fraction<T>
operator*(const T& m,const Fraction<T>& v);

template <class T>
void operator*=(Fraction<T>& v, const T& m);

template <class T>
class Fraction
{
public:
  T n,d;
  Fraction(T nc = 0, T dc = 1);
  void     normalize();
  Fraction<T> normalized() const;

//  explicit Fraction(const size_t&);

  double  toDouble() const {return double(n)/double(d);};

  friend Fraction<T> operator+ <> (const Fraction<T>& a,const Fraction<T>& b);

  friend bool     operator== <> (const Fraction<T>& v1, const Fraction<T>& v2);
  friend bool     operator!= <> (const Fraction<T>& v1, const Fraction<T>& v2);
  friend bool     operator< <> (const Fraction<T>& v1, const Fraction<T>& v2);
  friend bool     operator> <> (const Fraction<T>& v1, const Fraction<T>& v2);
  friend bool     operator>= <> (const Fraction<T>& v1, const Fraction<T>& v2);
  friend bool     operator<= <> (const Fraction<T>& v1, const Fraction<T>& v2);

  friend void     operator+= <> (Fraction<T>& v, const Fraction<T>& a);
  friend void     operator-= <> (Fraction<T>& v, const Fraction<T>& a);
  friend Fraction<T> operator- <> (const Fraction<T>& v,const Fraction<T>& b);
  friend Fraction<T> operator* <> (const Fraction<T>& v,const T& m);
  friend Fraction<T> operator/ <> (const Fraction<T>& v,const T& m);
  friend Fraction<T> operator* <> (const T& m,const Fraction<T>& v);
  friend void     operator*= <> (Fraction<T>& v, const T& m);
  Fraction<T> operator-() const;

  friend Fraction<T> operator* <> (const Fraction<T>& a,const Fraction<T>& b);
  friend Fraction<T> operator/ <> (const Fraction<T>& a,const Fraction<T>& b);

  friend std::istream&  operator>>  <> (std::istream& is, Fraction<T>& vec);
  friend std::ostream&  operator<<  <> (std::ostream& os, const Fraction<T>& vec);
};

template <class T>
inline
Fraction<T>::Fraction(T nc, T dc):
  n(nc),d(dc)
{
}

template <class T>
inline
bool
operator==(const Fraction<T>& a, const Fraction<T>& b)
{
  return ((a.n == b.n) && (a.d == b.d));
}

template <class T>
inline
bool
operator!=(const Fraction<T>& a, const Fraction<T>& b)
{
  return ((a.n != b.n) || (a.d != b.d));
}

template <class T>
inline
bool
operator>(const Fraction<T>& a, const Fraction<T>& b)
{
  return (a.n*b.d > b.n*a.d);
}

template <class T>
inline
bool
operator<(const Fraction<T>& a, const Fraction<T>& b)
{
  return (a.n*b.d < b.n*a.d);
}

template <class T>
inline
bool
operator>=(const Fraction<T>& a, const Fraction<T>& b)
{
  return !operator<(a,b);
}

template <class T>
inline
bool
operator<=(const Fraction<T>& a, const Fraction<T>& b)
{
  return !operator>(a,b);
}

template <class T>
inline
Fraction<T>
operator+(const Fraction<T>& a, const Fraction<T>& b)
{
  return Fraction<T>(a.n*b.d+b.n*a.d, a.d*b.d).normalized();
}

template <class T>
inline
Fraction<T>
operator*(const Fraction<T>& a, const Fraction<T>& b)
{
  return Fraction<T>(a.n*b.n, a.d*b.d).normalized();
}

template <class T>
inline
Fraction<T>
operator/(const Fraction<T>& a, const Fraction<T>& b)
{
  return Fraction<T>(a.n*b.d, a.d*b.n).normalized();
}

template <class T>
inline
void
operator+=(Fraction<T>& v, const Fraction<T>& a)
{
  Fraction<T> f(v+a);
  v=f;
}

template <class T>
inline
void
operator-=(Fraction<T>& v, const Fraction<T>& a)
{
  Fraction<T> f(v-a);
  v=f;
}

template <class T>
inline
Fraction<T>
operator-(const Fraction<T>& a, const Fraction<T>& b)
{
  return Fraction<T>(a.n*b.d-b.n*a.d, a.d*b.d).normalized();
}

template <class T>
inline
Fraction<T>
operator*(const Fraction<T>& v, const T& m)
{
  return Fraction<T>(v.n*m, v.d).normalized();
}

template <class T>
inline
Fraction<T>
operator/(const Fraction<T>& v, const T& m)
{
  return Fraction<T>(v.n, v.d*m).normalized();
}

template <class T>
inline
Fraction<T> operator*(const T& m,const Fraction<T>& v)
{
  return Fraction<T>(m*v.n, v.d).normalized();
}

template <class T>
inline
void
operator*=(Fraction<T>& v, const T& m)
{
  Fraction<T> f(Fraction<T>(v.n*m, v.d).normalized());
  v = f;
}

template <class T>
inline
Fraction<T>
Fraction<T>::operator-() const
{
  return Fraction<T>(-n,d);
}

template <class T>
inline
T
NOD(T a, T b)
{
  do
  {
    if (a>b)
    {
      if (a%b == T(0))
      {
        return b;
      }
      else
      {
        a = a-(a/b)*b;
      }
    }
    else
    {
      if (b%a == T(0))
      {
        return a;
      }
      else
      {
        b = b-(b/a)*a;
      }
    }
  }while (a!=b);
  return a;
}

template <class T>
inline
void
Fraction<T>::normalize()
{
  T D(NOD(n,d));
  n/=D; d/=D;
}

template <class T>
inline
Fraction<T>
Fraction<T>::normalized() const
{
  T D(NOD(n,d));
  return Fraction<T>(n/D,d/D);
}

template <class T>
inline
std::istream&
operator>>(std::istream& is,  Fraction<T>& vec)
{
  T u = 0, v = 0;
  is >> u >> v;
  if (is)
    vec = Fraction<T>(u,v);
  else
    std::cerr << "Error in reading Fraction<T>" << std::endl;
  return is;
}

template <class T>
inline
std::ostream&
operator<<(std::ostream& os, const Fraction<T>& vec)
{
  os << vec.n << std::endl
     << vec.d << std::endl;
  return os;
}

} // namespace yaatk

#endif
