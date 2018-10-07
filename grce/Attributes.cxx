/*
  Attributes for GRCE graph elements.

  Copyright (C) 2001, 2002, 2003, 2004, 2017, 2018 Alexander
  Yermolenko <yaa.mbox@gmail.com>

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

#include "Attributes.hpp"

namespace grce
{

grctk::Attribute<yaatk::Vector3D> a3D(
  grctk::Universe::singleton(),
  "Position (3D) attribute of GRCE objects","pos3d");
grctk::Attribute<Color> aColor(
  grctk::Universe::singleton(),
  "Color attribute of GRCE objects","color");
grctk::Attribute<int> aSelected(
  grctk::Universe::singleton(),
  "Selection attribute of GRCE objects","selected");
grctk::Attribute<std::string> aLabel(
  grctk::Universe::singleton(),
  "Labels of GRCE objects","label");
grctk::Attribute<int> aLabelVisible(
  grctk::Universe::singleton(),
  "Label Visibility of GRCE objects","label.visible");
grctk::Attribute<double> aDouble(
  grctk::Universe::singleton(),
  "Double attribute of GRCE objects","double");
grctk::Attribute<int> aInt(
  grctk::Universe::singleton(),
  "Integer attribute of GRCE objects","int");

} //namespace grce
