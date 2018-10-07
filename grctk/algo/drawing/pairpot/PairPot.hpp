/*
  The optimal geometric graph representation building: stochastic part
  (header file).
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

#ifndef grctk_PairPot_hpp
#define grctk_PairPot_hpp

#include "PairPotBase.hpp"
#include "grctk/algo/drawing/GraphMultiRep.hpp"
#include <yaatk/VectorXD.hpp>
#include <sstream>
#include <string>

namespace grctk
{

class PairPot : public AlgBase
{
private:
  PairPot(const PairPot &);
  PairPot & operator = (const PairPot &);
public:
  PairPotBase::TuneParams tune;
  struct InputParams
  {
    long inp_MaxIterations;
    bool inp_SaveEvery;
    InputParams(
      long inp_MaxIterations_def = 1,
      bool inp_SaveEvery_def = false):
      inp_MaxIterations(inp_MaxIterations_def),
      inp_SaveEvery(inp_SaveEvery_def)
      {
      }
  };
  void operator()(const PairPotBase::InputParams &inputBase,
                  const InputParams &input, GraphMultiRep &graphMultiRep);
  PairPot(const PairPotBase::TuneParams &tune_params, Log& setlog = nullLog);
};

}

#endif
