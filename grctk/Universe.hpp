/*
  The Universe and the Attribute classes (header file).

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

#ifndef grctk_Universe_hpp
#define grctk_Universe_hpp

#include <yaatk/yaatk.hpp>
#include <yaatk/VectorXD.hpp>
#include <set>
#include <vector>
#include <cassert>
#include <typeinfo>

namespace grctk
{

class Universe;

class Object
{
  size_t oid;
  Universe* u;
public:
  Object(size_t objId = 0, Universe* universe = NULL):
    oid(objId),u(universe) {}
  Object(bool objId, Universe* universe = NULL); // = delete
  Object(const Object&);
  Object& operator=(const Object&);
  size_t id() const { return oid; }
  /*explicit*/ operator bool() const { return oid != 0; }
  bool operator==(const Object& obj) const
    { assert(u == obj.u); return oid == obj.oid /* && u = obj.u*/; }
  bool operator<(const Object& obj) const
    { assert(u == obj.u); return oid < obj.oid; }
  void addOwner() const;
  void removeOwner() const;
  Object clone() const;
  // Object clone(Universe& destUniverse) const;
  friend std::ostream& operator<<(std::ostream& os, const Object& m);
  friend std::istream& operator>>(std::istream& is, Object& m);
  friend class Universe;
  friend class GenericAttribute;
  template <class T> friend class Attribute;
};

class GenericAttribute
{
  std::string genId() const;
protected:
  Universe& gu;
  std::string desc;
  std::string idstr;
  GenericAttribute(const GenericAttribute&);
  GenericAttribute& operator=(const GenericAttribute&);
public:
  const std::string description() const { return desc; }
  const std::string idString() const { return idstr; }
  virtual void onCreate() = 0;
  virtual void onClone(const Object& srcEl, const Object& destEl) = 0;
  virtual void resetValue(size_t id) = 0;
  virtual void saveToStream(std::ostream& os, const Object&) = 0;
  virtual void loadFromStream(std::istream& is, const Object&) = 0;
  // virtual bool hasNonDefaultValues() const = 0;
  // virtual Object onClone(const Object& srcEl, const Universe& srcUniv) = 0;
  GenericAttribute(Universe& universe,
                   const std::string description,
                   const std::string idString)
    :gu(universe),
     desc(description),
     idstr(idString) { if (idString == "") idstr = genId(); }
  ~GenericAttribute() {}
};

class Universe
{
  Universe(const Object&);
  Universe(const Universe&);
  Universe& operator=(const Universe&);
  friend class Object;
  std::set<size_t> ids;
public:
  size_t size() { return ids.size(); }
protected:
  size_t dataSize;
private:
  std::vector<size_t> refCounters;
  size_t allocateId();
  void freeId(size_t id);
  std::set<GenericAttribute*> attrs;
public:
  const std::set<GenericAttribute*> listAttrs() const { return attrs; }
  size_t attrsCount() const { return attrs.size(); }
  void addAttr(GenericAttribute*);
  void removeAttr(GenericAttribute*);
  bool objectExists(const size_t id) const;
  bool objectExists(const Object& o) const { return objectExists(o.id()); }
  bool objectExists(const bool) const; // = delete
public:
  Universe();
  virtual ~Universe();
  bool operator == (const Universe& obj) const
    {
      return (this == &obj);
    }
  Object nullObject()
    {
      return Object();
    }
public:
  Object getObject(size_t id)
    {
      assert(objectExists(id));
      Object obj(id, this);
      return obj;
    }
  Object getObject(bool); // = delete
  virtual Object create()
    {
      Object e(allocateId(), this);
      refCounters.resize(dataSize + 1);
      refCounters[e.oid] = 0;
      for(std::set<GenericAttribute*>::iterator
            attrIt = attrs.begin();
          attrIt != attrs.end(); ++attrIt)
        (*attrIt)->onCreate();
      return e;
    }
  // virtual void reserveVector(size_t length)
  // virtual std::vector<Object> createVector(size_t length)
  virtual Object clone(const Object& srcEl)
    {
      Object destEl = create();
      for(std::set<GenericAttribute*>::iterator
            attrIt = attrs.begin();
          attrIt != attrs.end(); ++attrIt)
        (*attrIt)->onClone(srcEl,destEl);
      return destEl;
    }
  virtual void resetAttrs(size_t id)
    {
      for(std::set<GenericAttribute*>::iterator
            attrIt = attrs.begin();
          attrIt != attrs.end(); ++attrIt)
        (*attrIt)->resetValue(id);
    }
  // virtual void collectGarbage() {}
  std::ostream& saveToStream(std::ostream& os) const;
  std::istream& loadFromStream(std::istream& is);
  friend class GenericAttribute;
  template <class T> friend class Attribute;
  static Universe& singleton();
  static Universe& null();
};

using yaatk::operator<<;
using yaatk::operator>>;

template <class T>
class Attribute : public GenericAttribute
{
  std::vector<T> r;
public:
  typedef T AttrType;
  T& operator[](const size_t id)
    {
      assert(id != 0);
      assert(id < r.size());
      return r[id];
    }
  T& operator[](const Object& obj)
    {
      assert(obj.id() != 0);
      assert(obj.id() < r.size());
      return r[obj.id()];
    }
  const T& operator[](const size_t id) const
    {
      assert(id != 0);
      assert(id < r.size());
      return r[id];
    }
  const T& operator[](const Object& obj) const
    {
      assert(obj.id() != 0);
      assert(obj.id() < r.size());
      return r[obj.id()];
    }
  virtual void onCreate()
    {
      r.resize(gu.dataSize + 1);
    }
  virtual void onClone(const Object& srcEl, const Object& destEl)
    {
      r[destEl.id()] = r[srcEl.id()];
    }
  virtual void resetValue(size_t id)
    {
      r[id] = T();
    }
  virtual void saveToStream(std::ostream& os, const Object& obj)
    {
      REQUIRE(*obj.u == gu);
      os << r[obj.id()];
    }
  virtual void loadFromStream(std::istream& is, const Object& obj)
    {
      REQUIRE(*obj.u == gu);
      is >> r[obj.id()];
    }
  // virtual bool hasNonDefaultValues() const
  //   {
  //     const T dval;
  //     for(size_t i = 0; i < r.size(); ++i)
  //       if (r[i] != dval)
  //         return true;
  //     return false;
  //   }
  Attribute(Universe& universe = grctk::Universe::singleton(),
            std::string description = "", const std::string idString = "")
    :GenericAttribute(universe,description,idString),r(universe.dataSize)
    {
      gu.addAttr(this);
    }
  Attribute(const Attribute<T>& obj):
    GenericAttribute(obj),
    r(obj.r)
    {
      gu.addAttr(this);
    }
  Attribute& operator=(const Attribute<T>& obj)
    {
      if (this == &obj)
        return (*this);

      GenericAttribute::operator=(obj);

      r = obj.r;

      return (*this);
    }
  ~Attribute()
    {
      gu.removeAttr(this);
    }
};

// template <>
// bool
// Attribute<std::valarray<double> >::hasNonDefaultValues() const
// {
//   const std::valarray<double> dval;
//   for(size_t i = 0; i < r.size(); ++i)
//   {
//     if (r[i].size() != dval.size())
//       return true;
//     for(size_t j = 0; j < r[i].size(); ++j)
//       if (r[i][j] != dval[j])
//         return true;
//   }
//   return false;
// }

}

#endif
