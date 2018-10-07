/*
  The optimal geometric graph representation building: main routines
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

#ifndef grctk_PairPotBase_hpp
#define grctk_PairPotBase_hpp

#include "Potentials.hpp"

#include "grctk/algo/AlgBase.hpp"
#include "grctk/AdjMatrix.hpp"
#include "grctk/Universe.hpp"
#include "grctk/algo/drawing/GraphRep.hpp"
#include <yaatk/Vector3D.hpp>
#include <yaatk/VectorXD.hpp>
#include <sstream>
#include <string>

namespace grctk
{

class PairPotBase : public AlgBase
{
public:
  struct TuneParams
  {
    const double alpha0;
    const double alpha1;
    const double k;
    const double r0;
    TuneParams(
      const double alpha0_def = 6.0,
      const double alpha1_def = 2.0,
      const double k_def = 0.03,
      const double r0_def = 0.12) :
      alpha0(alpha0_def), alpha1(alpha1_def), k(k_def), r0(r0_def)
      {
      }
  };
  struct InputParams
  {
    const double inp_LInit;
    const double inp_Eps;
    const bool inp_UseExisted;
    const Attribute<yaatk::Vector3D>& a3D;
    InputParams(
      const Attribute<yaatk::Vector3D>& a3D_def,
      const double inp_LInit_def = 0.0L,
      const double inp_Eps_def = 0.0001,
      const bool inp_UseExisted_def = false) :
      inp_LInit((inp_LInit_def>0)?inp_LInit_def:(10.0L*rand()/double(RAND_MAX))),
      inp_Eps(inp_Eps_def),
      inp_UseExisted(inp_UseExisted_def),
      a3D(a3D_def)
      {
      }
  };
protected:
  PPhi phi;
  TuneParams tune;
  double L;
  void Normalize(GraphRep&);
protected:
  void buildCurrent(const InputParams &input, GraphRep &rep);
  double Phi(size_t i, GraphRep&);
  yaatk::VectorXD Fi(size_t i, GraphRep&);
  double fullEnergy(GraphRep&);
public:
  void operator()(const InputParams &input, GraphRep &graphRep);
  PairPotBase(TuneParams &tune_params, Log& setlog = nullLog);
private:
  PairPotBase(const PairPotBase &);
  PairPotBase & operator = (const PairPotBase &);
};

}

#endif
