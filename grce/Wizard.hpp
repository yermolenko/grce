/*
  The Wizard classes (header file).

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

#ifndef grce_Wizard_hpp
#define grce_Wizard_hpp

#include "grce/ParamsDialog.hpp"
#include "grce/LogExecutor.hpp"
#include "grce/Runnable.hpp"
#include <grctk/AdjMatrix.hpp>
#include <grctk/Universe.hpp>
#include <yaatk/Vector3D.hpp>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Tabs.H>

namespace grce
{

class SimpleWizard
{
public:
  Runnable* runnable;
  bool operator()();
  SimpleWizard(Runnable* r, const std::string& algTitle = "SomeAlg");
  virtual ~SimpleWizard();
};

template <typename Alg,
          typename Arg1,
          typename RetValue>
class AlgOneParam : public Runnable
{
public:
  Alg alg;
  RetValue& retValue;
  Arg1 arg1;
  virtual void execute() { retValue = alg(arg1); }
  AlgOneParam(
    Arg1 arg1Val,
    RetValue& retVal,
    LogExecutor& log):
    Runnable(log),
    alg(log),
    retValue(retVal),
    arg1(arg1Val)
    {}
};

template <typename Alg,
          typename Arg1>
class AlgOneParamNoRet : public Runnable
{
public:
  Alg alg;
  Arg1 arg1;
  virtual void execute() { alg(arg1); }
  AlgOneParamNoRet(
    Arg1 arg1Val,
    LogExecutor& log):
    Runnable(log),
    alg(log),
    arg1(arg1Val)
    {}
};

template <typename Alg,
          typename Arg1, typename Arg2,
          typename RetValue>
class AlgTwoParams : public Runnable
{
public:
  Alg alg;
  RetValue& retValue;
  Arg1 arg1;
  Arg2 arg2;
  virtual void execute() { retValue = alg(arg1,arg2); }
  AlgTwoParams(
    Arg1 arg1Val,
    Arg2 arg2Val,
    RetValue& retVal,
    LogExecutor& log):
    Runnable(log),
    alg(log),
    retValue(retVal),
    arg1(arg1Val),
    arg2(arg2Val)
    {}
};

template <typename Alg,
          typename Arg1,
          typename Arg2>
class AlgTwoParamsNoRet : public Runnable
{
public:
  Alg alg;
  Arg1 arg1;
  Arg2 arg2;
  virtual void execute() { alg(arg1,arg2); }
  AlgTwoParamsNoRet(
    Arg1 arg1Val,
    Arg2 arg2Val,
    LogExecutor& log):
    Runnable(log),
    alg(log),
    arg1(arg1Val),
    arg2(arg2Val)
    {}
};

template <typename Alg,
          typename Arg1, typename Arg2, typename Arg3,
          typename RetValue>
class AlgThreeParams : public Runnable
{
public:
  Alg alg;
  RetValue& retValue;
  Arg1 arg1;
  Arg2 arg2;
  Arg3 arg3;
  virtual void execute() { retValue = alg(arg1,arg2,arg3); }
  AlgThreeParams(
    Arg1 arg1Val,
    Arg2 arg2Val,
    Arg3 arg3Val,
    RetValue& retVal,
    LogExecutor& log):
    Runnable(log),
    alg(log),
    retValue(retVal),
    arg1(arg1Val),
    arg2(arg2Val),
    arg3(arg3Val)
    {}
};

template <typename Alg,
          typename Arg1, typename Arg2, typename Arg3>
class AlgThreeParamsNoRet : public Runnable
{
public:
  Alg alg;
  Arg1 arg1;
  Arg2 arg2;
  Arg3 arg3;
  virtual void execute() { alg(arg1,arg2,arg3); }
  AlgThreeParamsNoRet(
    Arg1 arg1Val,
    Arg2 arg2Val,
    Arg3 arg3Val,
    LogExecutor& log):
    Runnable(log),
    alg(log),
    arg1(arg1Val),
    arg2(arg2Val),
    arg3(arg3Val)
    {}
};

template <typename Alg, typename Tune,
          typename Arg1, typename Arg2, typename Arg3,
          typename G = grctk::AdjMatrix>
class RunOptimizeWithOneTuneTwoConstAndOneNoncostParams : public Runnable
{
public:
  Alg alg;
  // G g;
  const Arg1 arg1;
  const Arg2 arg2;
  Arg3& arg3;
  virtual void execute() { /*g = */alg(arg1,arg2,arg3); }
  RunOptimizeWithOneTuneTwoConstAndOneNoncostParams(
    const Tune& tune,
    const Arg1& arg1Val,
    const Arg2& arg2Val,
    Arg3& arg3Val,
    LogExecutor& log):
    Runnable(log),
    alg(tune,log),
    // g(),
    arg1(arg1Val),
    arg2(arg2Val),
    arg3(arg3Val)
    {}
};

}

#endif
