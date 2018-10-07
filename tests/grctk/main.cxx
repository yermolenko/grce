/*
  The basic test of GRCTK.

  Copyright (C) 2018 Alexander Yermolenko <yaa.mbox@gmail.com>

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

#include <iostream>
#include <sstream>

#include <yaatk/SquareMatrix.hpp>
#include <grctk/AdjMatrix.hpp>
#include <grctk/algo/formats/Environment.hpp>
#include <map>

using namespace grctk;

#define PERFORM_TEST(test)                         \
  {                                                \
    std::cerr << (#test) << " : ";                 \
    if (test)                                      \
      std::cerr << "OK" << std::endl;              \
    else                                           \
      std::cerr << "FAIL" << std::endl;            \
  }

bool
test_square_matrix()
{
  {
    yaatk::SquareMatrix<int> m(0);
    REQUIRE(m.size() == 0);
  }

  {
    yaatk::SquareMatrix<int> m(0);
    size_t n = 4;
    m.resize(n);
    size_t counter = 1;
    for(size_t i = 0; i < n; i++)
      for(size_t j = 0; j < n; j++)
        m(i,j) = counter++;
    REQUIRE(m.size() == 4);
    REQUIRE(m(0,3) == 4);
    REQUIRE(m(1,0) == 5);
    REQUIRE(m(3,3) == 16);

    m.remove(2);
    REQUIRE(m.size() == 3);
    REQUIRE(m(0,2) == 4);
    REQUIRE(m(1,0) == 5);
    REQUIRE(m(2,2) == 16);

    m.resize(2);
    REQUIRE(m.size() == 2);
    REQUIRE(m(0,1) == 2);
    REQUIRE(m(1,0) == 5);
    REQUIRE(m(1,1) == 6);

    m.resize(6);
    REQUIRE(m.size() == 6);
    REQUIRE(m(0,1) == 2);
    REQUIRE(m(1,0) == 5);
    REQUIRE(m(5,5) == 0);
    REQUIRE(m(5,4) == 0);
  }

  return true;
}

bool
test_triangular_square_matrix()
{
  {
    yaatk::TriangularSquareMatrix<int> m(0);
    REQUIRE(m.size() == 0);
  }

  {
    yaatk::TriangularSquareMatrix<int> m(0);
    size_t n = 4;
    m.resize(n);
    size_t counter = 1;
    for(size_t i = 0; i < n; i++)
      for(size_t j = 0; j < n; j++)
        m(i,j) = counter++;
    REQUIRE(m.size() == 4);
    REQUIRE(m(0,3) == 13);
    REQUIRE(m(1,0) == 5);
    REQUIRE(m(3,3) == 16);

    m.remove(2);
    REQUIRE(m.size() == 3);
    REQUIRE(m(0,2) == 13);
    REQUIRE(m(1,0) == 5);
    REQUIRE(m(2,2) == 15);

    m.resize(2);
    REQUIRE(m.size() == 2);
    REQUIRE(m(0,0) == 1);
    REQUIRE(m(0,1) == 5);
    REQUIRE(m(1,0) == 5);
    REQUIRE(m(1,1) == 6);

    m.resize(6);
    REQUIRE(m.size() == 6);
    REQUIRE(m(0,0) == 1);
    REQUIRE(m(0,1) == 5);
    REQUIRE(m(1,0) == 5);
    REQUIRE(m(1,1) == 6);
    REQUIRE(m(5,5) == 0);
    REQUIRE(m(5,4) == 0);
  }

  return true;
}

bool
test_AdjMatrix()
{
  {
    grctk::Universe u;
    grctk::AdjMatrix oam;
    oam += u.create();
    grctk::Object v = u.create();
    oam += v;
    REQUIRE(oam.vertexIndex(v) == 1);
    grctk::AdjMatrix oam1(oam);
    oam -= 0;
    REQUIRE(oam.vertexIndex(v) == 0);
    try
    {
      oam1 -= 1;
      oam1.vertexIndex(v);
      return false;
    }
    catch (std::exception& e)
    {
    }
  }
  {
    grctk::Universe u;

    std::set<Object> objects;
    for(size_t i = 0; i < 7; ++i)
    {
      grctk::Object o = u.create();
      o.addOwner();
      objects.insert(o);
    }

    std::stringstream iss;
    iss << "5 6 7" << std::endl;
    iss << "0 1 0" << std::endl;
    iss << "  0 1" << std::endl;
    iss << "    0" << std::endl;
    grctk::AdjMatrix am;
    grctk::loadFromStream(iss,am,u);
    assert(iss);
    std::stringstream oss;
    oss << am;
    while (oss)
    {
      std::string s;
      std::getline(oss, s);
      // std::cout << s << std::endl;
    }
    REQUIRE(oss.str() ==
            "5\t6\t7\n"
            "0\t1\t0\n"
            "\t0\t1\n"
            "\t\t0\n");

    for(std::set<Object>::const_iterator obj = objects.begin();
        obj != objects.end(); ++obj)
    {
      REQUIRE(u.objectExists(*obj));
      (*obj).removeOwner();
    }
  }

  return true;
}

int main(int argc, char *argv[])
{
  PERFORM_TEST(test_square_matrix());
  PERFORM_TEST(test_triangular_square_matrix());
  PERFORM_TEST(test_AdjMatrix());

  return 0;
}
