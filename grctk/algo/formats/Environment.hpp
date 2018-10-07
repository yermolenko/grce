/*
  The Environment class (header file).

  Copyright (C) 2001, 2002, 2003, 2004, 2015, 2018 Alexander
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

#ifndef grctk_Environment_hpp
#define grctk_Environment_hpp

#include <grctk/Universe.hpp>
#include <grctk/AdjMatrix.hpp>
#include <string>
#include <vector>
#include <map>

namespace grctk
{

struct GraphInfo
{
  AdjMatrix g;
  std::string idstr;
  std::string desc;
  GraphInfo(const AdjMatrix& graph,
            std::string idString = "untitled",
            std::string description = "")
    : g(graph), idstr(idString), desc(description) {}
};

std::ostream&
saveToStream(std::ostream& os, const AdjMatrix& g,
             std::map<Object,size_t>& idTransform);

std::ostream&
saveToStream(std::ostream& os, const AdjMatrix& g);

void
saveEnv(const std::vector<GraphInfo>& graphs,
        const std::vector<GenericAttribute*>& attributes,
        const std::string& baseDir,
        const std::string& universeID);

std::istream&
loadFromStream(std::istream& is, AdjMatrix& g, Universe& universe,
               std::map<size_t,Object>& idTransform);

std::istream&
loadFromStream(std::istream& is, AdjMatrix& g, Universe& universe);

void
loadEnv(Universe&,
        std::vector<GraphInfo>& graphs,
        std::vector<GenericAttribute*>& attributes,
        const std::string& baseDir,
        const std::string& universeID);

}

#endif
