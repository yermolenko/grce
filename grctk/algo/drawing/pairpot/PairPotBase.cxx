/*
  The optimal geometric graph representation building: main routines.
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

#include "PairPotBase.hpp"
#include "grctk/algo/drawing/transform/MovePositions.hpp"
#include <cstdlib>
#include <ctime>

namespace grctk
{

using yaatk::VectorXD;

void
PairPotBase::Normalize(GraphRep &rep)
{
  for(size_t i = 0; i < rep.g.size(); ++i)
    yaatk::normalize(rep.aXD[rep.g[i]]);
}

double
PairPotBase::Phi(size_t i, GraphRep &rep)
{
  double result = 0;
  for(size_t j = 0; j < rep.g.size(); ++j)
    if (j != i)
      result += phi(rep.aXD[rep.g[j]],rep.aXD[rep.g[i]],rep.g(j,i));
  return result;
}

double
PairPotBase::fullEnergy(GraphRep &rep)
{
  double result = 0;
  for(size_t i = 0; i < rep.g.size(); ++i)
    result += Phi(i,rep);
  return result;
}

VectorXD
PairPotBase::Fi(size_t i, GraphRep &rep)
{
  VectorXD tmp(rep.dim);
  size_t sz = rep.g.size();
  for (size_t j = 0; j < sz; ++j)
    if (j != i)
      tmp += phi.grad(rep.aXD[rep.g[i]],rep.aXD[rep.g[j]],rep.g(j,i));
  return tmp;
}

void
PairPotBase::buildCurrent(const InputParams &input, GraphRep &rep)
{
  L = input.inp_LInit;

  VectorXD oldPos(rep.dim), FiVec(rep.dim);

  while (1)
  {
    bool moreOptimal = false;
    for(size_t i = 0; i < rep.g.size(); i++)
    {
      checkAborted();

      double PhiOld = Phi(i,rep);
      FiVec = Fi(i,rep);
      oldPos = rep.aXD[rep.g[i]];

      if (yaatk::module(FiVec) == 0)
      {
        errStream() << "Gradient == 0.0" << "\n";
        flushLogStreams();
        throw std::logic_error("Gradient == 0.0");
      }

      rep.aXD[rep.g[i]] = oldPos - L*FiVec/yaatk::module(FiVec);
      double PhiNew = Phi(i,rep);

      if (PhiNew >= PhiOld)
        rep.aXD[rep.g[i]] = oldPos;
      else
        moreOptimal = true;
    }
    if (!moreOptimal)
    {
      if (L < input.inp_Eps)
        break;
      else
        L = L/2;
    }
  }

  rep.energy = fullEnergy(rep);
}

PairPotBase::PairPotBase(TuneParams &tune_params, Log& setlog)
  :AlgBase(setlog),
   tune(tune_params)
{
  phi = PPhi(tune.alpha0, tune.alpha1, tune.k, tune.r0);
}

void
PairPotBase::operator()(const InputParams &input, GraphRep &rep)
{
  logStream() << "\nPairPotBase started\n";
  flushLogStreams();

  for(size_t i = 0; i < rep.g.size(); i++)
    rep.aXD[rep.g[i]].resize(rep.dim);

  if (input.inp_UseExisted)
  {
    for(size_t i = 0; i < rep.g.size(); i++)
    {
      for(size_t j = 0; j < rep.aXD[rep.g[i]].size(); j++)
        rep.aXD[rep.g[i]][j] = 0;
      size_t tXD = (3>rep.dim)?rep.dim:3;
      for(size_t j = 0; j < tXD; j++)
        rep.aXD[rep.g[i]][j] = input.a3D[rep.g[i]].X(j);
    }
  }
  else
  {
    for(size_t j = 0; j < rep.g.size(); j++)
    {
      for(size_t coord = 0; coord < rep.dim; coord++)
        rep.aXD[rep.g[j]][coord] = rand()/double(RAND_MAX);
    }
  }

  Normalize(rep);

  buildCurrent(input,rep);

  MovePositions move;
  move.moveMassCenterToOrigin(rep.g,rep.aXD);

  logStream() << "PairPotBase finished\n" ;
  flushLogStreams();
}

}
