/*
  The Environment class.

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

#include "Environment.hpp"
#include <iostream>
#include <map>
#include <set>

namespace grctk
{

std::ostream&
saveToStream(std::ostream& os, const AdjMatrix& g,
             std::map<Object,size_t>& idTransform)
{
  for(size_t j = 0; j < g.size(); ++j)
    os << idTransform[g[j]] << ((j != g.size()-1)?"\t":"");
  os << "\n";

  for(size_t j = 0; j < g.size(); j++)
  {
    for(size_t i = 0; i < j; i++)
      os << "" << ((i != g.size()-1)?"\t":"");
    for(size_t i = j; i < g.size(); i++)
      os << (g(j,i)?idTransform[g(j,i)]:0) << ((i != g.size()-1)?"\t":"");
    os << "\n";
  }

  return os;
}

std::ostream&
saveToStream(std::ostream& os, const AdjMatrix& g)
{
  for(size_t j = 0; j < g.size(); ++j)
    os << g[j].id() << ((j != g.size()-1)?"\t":"");
  os << "\n";

  for(size_t j = 0; j < g.size(); j++)
  {
    for(size_t i = 0; i < j; i++)
      os << "" << ((i != g.size()-1)?"\t":"");
    for(size_t i = j; i < g.size(); i++)
      os << g(j,i).id() << ((i != g.size()-1)?"\t":"");
    os << "\n";
  }

  return os;
}

void
saveEnv(const std::vector<GraphInfo>& graphs,
        const std::vector<GenericAttribute*>& attributes,
        const std::string& baseDir,
        const std::string& universeID)
{
  std::set<Object> objects;
  for(size_t gi = 0; gi < graphs.size(); ++gi)
  {
    const AdjMatrix& g = graphs[gi].g;
    for(size_t i = 0; i < g.size(); ++i)
    {
      objects.insert(g[i]);
      for(size_t j = i+1; j < g.size(); ++j)
        if (g(i,j))
          objects.insert(g(i,j));
    }
  }

  std::map<Object,size_t> obj2newID;
  {
    size_t newObjID = 1;
    for(std::set<Object>::const_iterator obj = objects.begin();
        obj != objects.end(); ++obj)
    {
      obj2newID[*obj] = newObjID;
      ++newObjID;
    }
  }

  yaatk::ChDir cdBaseDir(baseDir,false);
  {
    std::ofstream ofu((universeID + ".ids").c_str());
    for(std::set<Object>::const_iterator obj = objects.begin();
        obj != objects.end(); ++obj)
      ofu << obj2newID[*obj] << "\n";
  }
  {
    for(size_t ai = 0; ai < attributes.size(); ++ai)
    {
      std::ostringstream ofa_name;
      REQUIRE(attributes[ai]->idString() != "");
      ofa_name << universeID << ".attr." << attributes[ai]->idString();
      std::ofstream ofa(ofa_name.str().c_str());
      for(std::set<Object>::const_iterator obj = objects.begin();
          obj != objects.end(); ++obj)
      {
        attributes[ai]->saveToStream(ofa,*obj);
        ofa << "\n";
      }
    }
  }
  for(size_t gi = 0; gi < graphs.size(); ++gi)
  {
    const AdjMatrix& g = graphs[gi].g;
    std::ostringstream ofg_name;
    if (graphs[gi].idstr == "untitled" || graphs[gi].idstr == "")
      ofg_name << universeID << ".graph." << gi/* << ".am"*/;
    else
      ofg_name << universeID << ".graph." << graphs[gi].idstr/* << ".am"*/;
    std::ofstream ofg(ofg_name.str().c_str());
    saveToStream(ofg,g,obj2newID);
    ofg << graphs[gi].desc << "\n";
  }
}

std::istream&
loadFromStream(std::istream& is, AdjMatrix& g, Universe& universe,
               std::map<size_t,Object>& idTransform)
{
  std::string line;
  std::getline(is, line, '\n');
  REQUIRE(!is.fail());
  std::istringstream iss(line);
  size_t id;
  while (iss >> id)
  {
    REQUIRE(universe.objectExists(idTransform[id]));
    // g += universe.getObject(idTransform[id]); // should not compile, just a test
    g += idTransform[id];
  }

  size_t vc = g.size();
  yaatk::TriangularSquareMatrix<size_t> matrix(vc);

  REQUIRE(!is.fail());

  is >> matrix;
  REQUIRE(!is.fail());

  for(size_t i = 0; i < vc; ++i)
    for(size_t j = i+1; j < vc; ++j)
    {
      if (!matrix(i,j)) continue;
      size_t id = matrix(i,j);
      REQUIRE((id == 0 && idTransform[id].id() == 0) || universe.objectExists(idTransform[id]));
      g.edge(i,j,idTransform[id]);
    }

  return is;
}

std::istream&
loadFromStream(std::istream& is, AdjMatrix& g, Universe& universe)
{
  std::string line;
  std::getline(is, line, '\n');
  REQUIRE(!is.fail());
  std::istringstream iss(line);
  size_t id;
  while (iss >> id)
  {
    REQUIRE(universe.objectExists(id));
    g += universe.getObject(id);
  }

  size_t vc = g.size();
  yaatk::TriangularSquareMatrix<size_t> matrix(vc);

  REQUIRE(!is.fail());

  is >> matrix;
  REQUIRE(!is.fail());

  for(size_t i = 0; i < vc; ++i)
    for(size_t j = i+1; j < vc; ++j)
    {
      if (!matrix(i,j)) continue;
      size_t id = matrix(i,j);
      REQUIRE((id == 0) || universe.objectExists(id));
      g.edge(i,j,universe.getObject(id));
    }

  return is;
}

void
loadEnv(Universe& universe,
        std::vector<GraphInfo>& graphs,
        std::vector<GenericAttribute*>& attributes,
        const std::string& baseDir,
        const std::string& universeID)
{
  yaatk::ChDir cdBaseDir(baseDir,false);

  std::map<size_t,Object> idTransform;
  idTransform[0] = universe.nullObject();

  std::set<Object> objects;
  {
    std::ifstream ifu((universeID + ".ids").c_str());
    REQUIRE(ifu);
    size_t id;
    while (ifu >> id)
    {
      grctk::Object o = universe.create();
      o.addOwner();
      idTransform[id] = o;
      objects.insert(idTransform[id]);
    }
  }
  REQUIRE(objects.size() == idTransform.size()-1);

  for(size_t gi = 0; gi < graphs.size(); ++gi)
  {
    AdjMatrix& g = graphs[gi].g;
    std::ostringstream ifg_name;
    ifg_name << universeID << ".graph." << graphs[gi].idstr;
    std::ifstream ifg(ifg_name.str().c_str());
    REQUIRE(ifg);
    loadFromStream(ifg,g,universe,idTransform);
    REQUIRE(!ifg.fail());
    { std::string eatToNextLine; std::getline(ifg, eatToNextLine, '\n'); }
    std::getline(ifg, graphs[gi].desc, '\n');
  }

  {
    for(size_t ai = 0; ai < attributes.size(); ++ai)
    {
      std::ostringstream ifa_name;
      REQUIRE(attributes[ai]->idString() != "");
      ifa_name << universeID << ".attr." << attributes[ai]->idString();
      std::ifstream ifa(ifa_name.str().c_str());
      REQUIRE(ifa);
      {
        std::ifstream ifu((universeID + ".ids").c_str());
        REQUIRE(ifu);
        size_t id;
        while (ifu >> id)
        {
          REQUIRE(universe.objectExists(idTransform[id]));
          attributes[ai]->loadFromStream(ifa,idTransform[id]);
        }
      }
    }
  }

  REQUIRE(objects.size() == idTransform.size()-1);
  {
    for(std::set<Object>::const_iterator obj = objects.begin();
        obj != objects.end(); ++obj)
    {
      REQUIRE(universe.objectExists(*obj));
      (*obj).removeOwner();
    }
  }
}

}
