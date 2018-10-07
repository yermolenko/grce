/*
   LongInteger definition.

   Copyright (C) 2004, 2018 Alexander Yermolenko <yaa.mbox@gmail.com>

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

#ifndef yaatk_LongInteger_hpp
#define yaatk_LongInteger_hpp

#include "gmp.h"
#include "gmpxx.h"

#include <sstream>

namespace yaatk
{

typedef mpz_class LongInteger;

inline
mpz_class veryBigLongInteger()
{
  mpz_class result(0);
  std::istringstream is("99999999999999999999999999999999999999999999999999999999999999999");
  is >> result;
  return result;
}

}

#endif
