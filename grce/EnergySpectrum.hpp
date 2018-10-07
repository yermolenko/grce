/*
  The EnergySpectrum class (header file).

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

#ifndef grce_EnergySpectrum_hpp
#define grce_EnergySpectrum_hpp

#include <FL/Fl_Chart.H>
#include <FL/Fl.H>
#include <vector>
#include <cstdlib>

namespace grce
{

class EnergySpectrum : public Fl_Chart
{
  int old_index;
  double E_max, E_min, dE;
  size_t N;
  std::vector<double> bins;
  std::vector<double> energyList;
public:
  EnergySpectrum(int x, int y, int w, int h,
                 const std::vector<double>& energyValues,
                 size_t numberOfIntervals);
  void set_current_index(int);
};

}

#endif
