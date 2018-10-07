/*
  The Universe and the Attribute classes.

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

#include "Universe.hpp"

#include <iostream>

namespace grctk
{

Object::Object(const Object& obj):
  oid(obj.oid),u(obj.u)
{
}

Object&
Object::operator=(const Object& obj)
{
  assert(u == obj.u || oid == 0 || obj.oid == 0);

  if (this == &obj)
    return (*this);

  oid = obj.oid;
  u = obj.u;

  return (*this);
}

void
Object::addOwner() const
{
  assert(oid != 0);
  assert(u);
  u->refCounters[oid]++;
}

void
Object::removeOwner() const
{
  assert(oid != 0);
  assert(u);
  assert(u->refCounters[oid] != 0);
  u->refCounters[oid]--;
  if (u->refCounters[oid] == 0)
  {
    u->resetAttrs(oid);
    u->freeId(oid);
  }
}

Object
Object::clone() const
{
  return u->clone(*this);
}

GenericAttribute::GenericAttribute(const GenericAttribute& obj):
  gu(obj.gu),desc(obj.desc),idstr(obj.idstr)
{
}

GenericAttribute&
GenericAttribute::operator=(const GenericAttribute& obj)
{
  if (this == &obj)
    return (*this);

  REQUIRE(&gu == &obj.gu);
  desc = obj.desc;
  idstr = obj.idstr;

  return (*this);
}

std::string
GenericAttribute::genId() const
{
  std::ostringstream oss;
  oss << "id" << gu.attrsCount();
  return oss.str();
}

size_t
Universe::allocateId()
{
  ids.insert(0);
  size_t nextId = 0;
  REQUIRE(*ids.begin() == 0);
  for(std::set<size_t>::iterator
        idi = ids.begin(), idiPrev = idi++;
      idi != ids.end() && !nextId;
      idiPrev = idi, ++idi)
  {
    if (*idi != *idiPrev + 1)
      nextId = *idiPrev + 1;
  }
  while(!nextId)
  {
    nextId = (*ids.rbegin() + 1)%dataSize;
    if (ids.find(nextId) == ids.end())
      break;
    REQUIRE(dataSize < 50000);
    dataSize++;
  }
  ids.insert(nextId);
  return nextId;
}

void
Universe::freeId(size_t id)
{
  if (id != 0)
    ids.erase(id);
}

void
Universe::addAttr(GenericAttribute* attr)
{
  attrs.insert(attr);
  // PRINT("Universe::addAttr() : ");
  // PRINT(typeid(*attr).name()); PRINT(" : ");
  // PRINT(attr->description()); PRINT("\n");
}

void
Universe::removeAttr(GenericAttribute* attr)
{
  attrs.erase(attr);
  // PRINT("Universe::removeAttr() : ");
  // PRINT(typeid(*attr).name()); PRINT(" : ");
  // PRINT(attr->description()); PRINT("\n");
}

bool
Universe::objectExists(const size_t id) const
{
  return (id != 0) && (ids.find(id) != ids.end());
}

Universe::Universe()
  :ids(),dataSize(1),refCounters(1),attrs()
{
  // PRINT("Universe::Universe()\n");
  ids.insert(0);
}

Universe::~Universe()
{
  // PRINT("Universe::~Universe()\n");
  // TRACE(ids.size());
  assert(ids.size() == 1);
}

std::ostream&
operator<<(std::ostream& os, const Object& m)
{
  os << m.id();
  return os;
}

std::istream&
operator>>(std::istream& is, Object& m)
{
  is >> m.oid;
  return is;
}

Universe&
Universe::singleton() {
  static Universe instance;
  return instance;
}

}
