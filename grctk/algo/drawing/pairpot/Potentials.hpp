/*
  The optimal geometric graph representation building: interaction
  potential.
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

#ifndef grctk_Potentials_hpp
#define grctk_Potentials_hpp

#include <yaatk/VectorXD.hpp>
#include <sstream>
#include <string>

namespace grctk
{

class PPhi
{
  double alpha0;
  double alpha1;
  double k;
  double r0;
  // interaction in case of vertex adjacency
  double phi0(double r) const
    {
      return (k*exp(alpha0*(r0-r)));
    }
  double phi0der(double r) const
    {
      return (-alpha0*k*exp(alpha0*(r0-r)));
    }
  // interaction in case of no vertex adjacency
  double phi1(double r) const
    {
      return (exp(2.0*alpha1*(r0-r))-2.0*exp(alpha1*(r0-r)));
    }
  double phi1der(double r) const
    {
      double tmp = exp(alpha1*(r0-r));
      return (-2.0*alpha1*tmp*(tmp-1.0));
    }
public:
  double operator()(const yaatk::VectorXD &v1, const yaatk::VectorXD &v2,
                    const int s) const
    {
      yaatk::VectorXD r(v1-v2);
      double R = yaatk::module(r);
      return (1-s)*phi0(R) + s*phi1(R);
    }
  yaatk::VectorXD grad(const yaatk::VectorXD &v1, const yaatk::VectorXD &v2,
                       const int s) const
    {
      yaatk::VectorXD r(v1 - v2);
      double R = yaatk::module(r);
      double Der = (1-s)*phi0der(R) + s*phi1der(R);
      return ((yaatk::normalized(r))*=Der);
    }
  PPhi(double alpha0_ = 6.0,
       double alpha1_ = 2.0,
       double k_ = 0.03,
       double r0_ = 0.12):
    alpha0(alpha0_),
    alpha1(alpha1_),
    k(k_),
    r0(r0_)
    {
    }
};

}

#endif
