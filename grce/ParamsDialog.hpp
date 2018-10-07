/*
  The ParamsDialog class (header file).

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

#ifndef grce_ParamsDialog_hpp
#define grce_ParamsDialog_hpp

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Choice.H>
#include <string>
#include <vector>
#include <map>

namespace grce
{

class ParamsDialog;
class Saver;
class Loader;

class BaseParam
{
  virtual Fl_Widget* createVisual(int x, int y, int w) = 0;
  virtual void loadDefaultValue() = 0;
  // virtual bool valueIsAcceptable() = 0;
  BaseParam(const BaseParam&);
  BaseParam& operator=(const BaseParam&);
protected:
  const std::string paramName;
public:
  BaseParam(const std::string name):paramName(name) {}
  virtual ~BaseParam() {}
  friend class ParamsDialog;
  friend class Saver;
  friend class Loader;
};

typedef std::pair<std::string, std::vector<BaseParam*> > ParamsTab;

class IntegerParam : public BaseParam
{
  const int defaultParamValue;
  Fl_Int_Input* widget;
  Fl_Widget* createVisual(int x, int y, int w);
  void loadDefaultValue();
public:
  IntegerParam(const int defaultValue, const std::string name);
  int value();
};

class FloatParam : public BaseParam
{
  const double defaultParamValue;
  Fl_Float_Input* widget;
  Fl_Widget* createVisual(int x, int y, int w);
  void loadDefaultValue();
public:
  FloatParam(const double defaultValue, const std::string name);
  double value();
};

class CheckboxParam : public BaseParam
{
  const bool defaultParamValue;
  Fl_Check_Button* widget;
  Fl_Widget* createVisual(int x, int y, int w);
  void loadDefaultValue();
public:
  CheckboxParam(const bool defaultValue, const std::string name);
  int value();
  friend class Saver;
  friend class Loader;
};

class OptionsParam : public BaseParam
{
  const std::vector<std::string> optionList;
  const int defaultOption;
  Fl_Group* widgetsGroup;
  Fl_Widget* createVisual(int x, int y, int w);
  void loadDefaultValue();
public:
  OptionsParam(const std::vector<std::string> options,
               const int defaultOptionIndex = 0,
               const std::string name = "");
  std::string value();
};

class MultiSelectParam : public BaseParam
{
  const std::map<std::string,bool> optionListDefault;
  Fl_Group* widgetsGroup;
  Fl_Widget* createVisual(int x, int y, int w);
  void loadDefaultValue();
public:
  MultiSelectParam(const std::map<std::string,bool> options,
                   const std::string name = "");
  bool selected(const std::string);
};

class DocWindow;

class DocWindowParam : public BaseParam
{
  const std::vector<DocWindow*> valueList;
  const DocWindow* defaultDocWindow;
  Fl_Choice* widget;
  Fl_Widget* createVisual(int x, int y, int w);
  void loadDefaultValue();
public:
  DocWindowParam(const std::vector<DocWindow*> paramValueList,
                 const DocWindow* defaultDocWindowPtr = NULL,
                 const std::string name = "Graph");
  DocWindow* value();
};

class ParamsDialog : public Fl_Window
{
  Fl_Return_Button* btn_ok;
  Fl_Button* btn_defaults;
  Fl_Button* btn_cancel;

  std::vector<ParamsTab> paramsTabs;
public:
  enum ModalResult {MR_CANCEL = 0, MR_OK = 1};
private:
  ModalResult modalresult;
protected:
  Fl_Tabs* tabs;
public:
  ModalResult modalResult() const { return modalresult; }
  bool operator()() { return modalResult() == MR_OK && valuesAreCorrect(); }
  ParamsDialog(const std::string& algTitle,
               std::vector<ParamsTab> = std::vector<ParamsTab>());
  ~ParamsDialog() {}
  static void btn_ok_cb(Fl_Widget *, void *);
  static void btn_defaults_cb(Fl_Widget *, void *);
  static void btn_cancel_cb(Fl_Widget *, void *);

  virtual void loadDefaults();
  virtual bool valuesAreCorrect() { return true; };
};

}

#endif
