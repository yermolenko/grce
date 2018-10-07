/*
  The optimal geometric graph representation building: stochastic part
  See the following article:
  Pinchuk V.P. Postroyeniye optimalnogo geometricheskogo obraza
  neoriyentirovannogo grafa [Building the optimal geometric image of
  an undirected graph]. Radioelektronika, informatika, upravlinna
  [Radio Electronics, Computer Science, Control], 1999, No. 1,
  pp. 89-92. [In Russian]
  <http://ric.zntu.edu.ua/issue/view/1578>

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

#include "PairPot.hpp"
#include <cstdlib>
#include <ctime>

namespace grctk
{

PairPot::PairPot(const PairPotBase::TuneParams &tune_params, Log& setlog):
  AlgBase(setlog),
  tune(tune_params)
{
}

void
PairPot::operator()(const PairPotBase::InputParams &inputBase,
                    const InputParams &input, GraphMultiRep &graphMultiRep)
{
  logStream() << "\nPairPot started\n";
  flushLogStreams();

  time_t t;
  srand((unsigned)time(&t));

  for(long i = 0; i < input.inp_MaxIterations; i++)
  {
    checkAborted();

    logStream() << "Building representation number " << i+1 << "...\n";
    flushLogStreams();

    bool useExisted = inputBase.inp_UseExisted;
    if (i > 0)
      useExisted = false;
    PairPotBase::InputParams inputBaseMod(
      inputBase.a3D,
      inputBase.inp_LInit,
      inputBase.inp_Eps,
      useExisted);
    GraphRep rep(graphMultiRep.g, graphMultiRep.dim);

    PairPotBase alg(tune);
    alg(inputBaseMod,rep);
    graphMultiRep.addRep(rep);
  }

  {
    logStream() << "PairPot finished\n" ;
    flushLogStreams();
  }
}

}
