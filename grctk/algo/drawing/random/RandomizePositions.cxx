/*
  The RandomizePositions algorithm.

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

#include "RandomizePositions.hpp"
#include <gsl/gsl_rng.h>
#include <cstdlib>

namespace grctk
{

using yaatk::Vector3D;

void
RandomizePositions::operator()(const AdjMatrix &g, Attribute<Vector3D>& a3D)
{
  logStream() << "\nRandomizePositions started\n";
  flushLogStreams();

  gsl_rng *rng = gsl_rng_alloc(gsl_rng_ranlxd2);
  REQUIRE(rng != NULL);
  gsl_rng_set(rng, (unsigned long int) time(NULL));
  REQUIRE(gsl_rng_min(rng) == 0);
  REQUIRE(gsl_rng_max(rng) > 1000);

  for(size_t i = 0; i < g.size(); ++i)
    a3D[g[i]] = Vector3D(gsl_rng_uniform(rng),
                         gsl_rng_uniform(rng),
                         gsl_rng_uniform(rng));

  gsl_rng_free(rng);

  logStream() << "RandomizePositions finished\n" ;
  flushLogStreams();
}

} //namespace grctk
