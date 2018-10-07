/*
  The Consecutive Matching with Returns (CMR) algorithm for the graph
  isomorphism problem (header file).
  See the following article for the reference implementation:
  Pinchuk V.P. Raspoznavaniye izomorfnosti grafov: PNV-algoritm
  [Recognition of Graph Isomorphism: CMR-algorithm].
  Skladni sistemi i procesi [Complex Systems and Processes],
  2002, No. 1, pp. 4-11. [In Russian]
  <https://elibrary.ru/item.asp?id=11737054>

  Copyright (C) 2004, 2011, 2015, 2017, 2018 Alexander Yermolenko
  <yaa.mbox@gmail.com>

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

#ifndef grctk_CMR_hpp
#define grctk_CMR_hpp

#include "grctk/algo/AlgBase.hpp"
#include "grctk/AdjMatrix.hpp"

namespace grctk
{

class CMR : public AlgBase
{
public:
  struct PresortOptions
  {
    bool useRule1;
    bool useRule2;
    bool useRule3;
    PresortOptions():useRule1(true),useRule2(true),useRule3(true) {}
  };
  bool operator()(const AdjMatrix&, const AdjMatrix&,
                  const PresortOptions& presortOptions = PresortOptions());
  CMR(Log& setlog = nullLog);
  virtual ~CMR() {}
private:
  size_t n;
  std::vector<size_t> Av;
  std::vector<size_t> Bv;
  std::vector<size_t> Adegs;
  std::vector<size_t> Bdegs;
  std::vector<size_t> st;
  struct greaterAdeg
   {
       greaterAdeg(const CMR& objCMR):cmr(objCMR) {}
       const CMR& cmr;
       bool operator()(const size_t& i, const size_t& j)
       {
         return cmr.Adegs[i] > cmr.Adegs[j];
       }
   };
  struct greaterBdeg
   {
       greaterBdeg(const CMR& objCMR):cmr(objCMR) {}
       const CMR& cmr;
       bool operator()(const size_t& i, const size_t& j)
       {
         return cmr.Bdegs[i] > cmr.Bdegs[j];
       }
   };
  void allocateMemory(size_t nv);
  void presort(const AdjMatrix&, const AdjMatrix&,
               const PresortOptions& presortOptions);
};

}

#endif
