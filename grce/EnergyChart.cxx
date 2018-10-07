/*
  The EnergyChart class.

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

#include "EnergyChart.hpp"
#include <yaatk/yaatk.hpp>

namespace grce
{

EnergyChart::EnergyChart(
  int x, int y, int w, int h,
  const std::vector<double>& energyValues):
  Fl_Chart(x,y,w,h),
  old_index(-1),
  E_max(), E_min(),
  E_min_ind(),
  energyList(energyValues)
{
  type(FL_SPIKE_CHART);
  box(FL_EMBOSSED_BOX);

  REQUIRE(energyList.size() > 0);

  E_max = energyList[0];
  E_min = energyList[0];
  E_min_ind = 0;

  for(size_t i = 0; i < energyList.size(); i++)
  {
    if (energyList[i] > E_max)
      E_max = energyList[i];
    if (energyList[i] < energyList[E_min_ind])
    {
      E_min_ind = i;
      E_min = energyList[i];
    }
  }

  for(size_t i = 0; i < energyList.size(); i++)
    add(energyList[i]-E_max,NULL,FL_RED);

//  set_current_index(0);
}

void EnergyChart::set_current_index(int index)
{
  if (old_index == index) return;
  if (old_index != -1)
  {
    replace(index+1,energyList[index] - E_max,NULL,FL_GREEN);
    replace(old_index+1,energyList[old_index] - E_max,NULL,FL_RED);
    old_index = index;
  }
  else
  {
    replace(index+1,energyList[index] - E_max,NULL,FL_GREEN);
    old_index = index;
  }
}

}
