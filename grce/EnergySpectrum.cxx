/*
  The EnergySpectrum class.

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

#include "EnergySpectrum.hpp"
#include <cmath>
#include <yaatk/yaatk.hpp>

namespace grce
{
EnergySpectrum::EnergySpectrum(
  int x, int y, int w, int h,
  const std::vector<double>& energyValues, size_t numberOfIntervals):
  Fl_Chart(x,y,w,h),
  old_index(-1),
  E_max(),
  E_min(),
  dE(),
  N(numberOfIntervals),
  bins(N),
  energyList(energyValues)
{
  type(FL_BAR_CHART);
  box(FL_EMBOSSED_BOX);

  REQUIRE(energyList.size() > 0);

  if (N <= 480)
    type(FL_BAR_CHART);
  else
    type(FL_SPIKE_CHART);

  for(size_t k = 0; k < N; k++)
    bins[k] = 0.0;

  E_max = energyList[0];
  E_min = energyList[0];

  for(size_t i = 0; i < energyList.size(); i++)
  {
    if (energyList[i] > E_max)
      E_max = energyList[i];
    if (energyList[i] < E_min)
      E_min = energyList[i];
  }

  dE = (E_max - E_min)/double(N);

  if (dE == 0.0) dE = 1e-6;

  E_max += 1e-3*dE;
  E_min -= 1e-3*dE;

  dE = (E_max - E_min)/double(N);

  for(size_t i = 0; i < energyList.size(); i++)
  {
    int ind = int(floor((energyList[i] - E_min)/dE));

    if (ind == -1) ind = 0;
    if (ind == N) ind = N-1;

    bins[ind] += 1.0;
  }

  for(size_t i = 0; i < N; i++)
    add(bins[i],NULL,FL_RED);
}

void EnergySpectrum::set_current_index(int index)
{
  int ind,old_ind;
  if (old_index == index) return;
  if (old_index != -1)
  {
    ind = int(floor((energyList[index] - E_min)/dE));
    old_ind = int(floor((energyList[old_index] - E_min)/dE));

    if (ind == -1) ind = 0;
    if (ind == N) ind = N-1;
    if (old_ind == -1) ind = 0;
    if (old_ind == N) ind = N-1;

    replace(old_ind+1,bins[old_ind],NULL,FL_RED);
    replace(ind+1,bins[ind],NULL,FL_GREEN);
    old_index = index;
  }
  else
  {
    ind = int(floor((energyList[index] - E_min)/dE));

    if (ind == -1) ind = 0;
    if (ind == N) ind = N-1;

    replace(ind+1,bins[ind],NULL,FL_GREEN);
    old_index = index;
  }
}

}
