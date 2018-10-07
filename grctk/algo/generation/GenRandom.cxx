/*
  The GenRandom class.

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

#include "GenRandom.hpp"
#include "grctk/algo/connectivity/ConComp.hpp"
#include <gsl/gsl_rng.h>
#include <stdexcept>

namespace grctk
{

AdjMatrix
GenRandom::operator()(size_t vc, size_t ec, bool connected_only)
{
  logStream() << "\nGenRandom started\n";
  logStream() << "size = " << vc << "\n";
  flushLogStreams();

  if (ec > vc*(vc-1)/2)
    throw std::logic_error("Number of edges is too high");

  if (connected_only && ec < vc-1)
    throw std::logic_error("Number of edges is too low to generate a connected graph");

  gsl_rng *rng = gsl_rng_alloc(gsl_rng_ranlxd2);
  REQUIRE(rng != NULL);
  gsl_rng_set(rng, (unsigned long int) time(NULL));
  REQUIRE(gsl_rng_min(rng) == 0);
  REQUIRE(gsl_rng_max(rng) > 1000);

  AdjMatrix g;

  bool connected = true;

  do{
    connected = true;

    g.clear();
    for(size_t i = 0; i < vc; ++i)
    {
      checkAborted();
      g += grctk::Universe::singleton().create();
    }
    logStream() << "Trying to generate a random graph ...\n";
    flushLogStreams();

    size_t ecount = 0;
    while (ecount < ec)
    {
      size_t j = gsl_rng_uniform_int(rng,vc);
      size_t i = gsl_rng_uniform_int(rng,vc);
      if (j!=i && !g.edge(j,i))
      {
        checkAborted();
        g.edge(j,i,Universe::singleton().create());
        ecount++;
      }
    }

    ConComp conComp;
    size_t c = 0;
    conComp(g,0,c);
    if (c != 1)
      connected = false;

  }while ((connected_only)?(!connected):false);

  logStream() << "GenRandom finished\n";
  flushLogStreams();

  gsl_rng_free(rng);

  return g;
}

} //namespace grctk
