/*
  The ParamsDialog class.

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

#include "ParamsDialog.hpp"
#include "grce/DocWindow.hpp"
#include <FL/Fl_Pack.H>
#include <FL/Fl_Scroll.H>
#include <stdexcept>

namespace grce
{

IntegerParam::IntegerParam(const int defaultValue, const std::string name):
  BaseParam(name),defaultParamValue(defaultValue),widget(NULL)
{
}

Fl_Widget*
IntegerParam::createVisual(int x, int y, int w)
{
  widget = new Fl_Int_Input(x+w/2,y,w/2,25,paramName.c_str());
  loadDefaultValue();
  return widget;
}

void
IntegerParam::loadDefaultValue()
{
  if (widget)
  {
    std::ostringstream s;
    s << defaultParamValue;
    widget->value(s.str().c_str());
  }
}

int
IntegerParam::value()
{
  if (widget)
    return atoi(widget->value());
  return 0;
}

FloatParam::FloatParam(const double defaultValue, const std::string name):
  BaseParam(name),defaultParamValue(defaultValue),widget(NULL)
{
}

Fl_Widget*
FloatParam::createVisual(int x, int y, int w)
{
  widget = new Fl_Float_Input(x+w/2,y,w/2,25,paramName.c_str());
  loadDefaultValue();
  return widget;
}

void
FloatParam::loadDefaultValue()
{
  if (widget)
  {
    std::ostringstream s;
    s << defaultParamValue;
    widget->value(s.str().c_str());
  }
}

double
FloatParam::value()
{
  if (widget)
  {
    std::istringstream iss(widget->value());
    double v = 0.0;
    iss >> v;
    REQUIRE(!iss.fail());
    return v;
  }
  return 0;
}

CheckboxParam::CheckboxParam(const bool defaultValue, const std::string name):
  BaseParam(name),defaultParamValue(defaultValue),widget(NULL)
{
}

Fl_Widget*
CheckboxParam::createVisual(int x, int y, int w)
{
  widget = new Fl_Check_Button(x,y,w,25,paramName.c_str());
  loadDefaultValue();
  return widget;
}

void
CheckboxParam::loadDefaultValue()
{
  if (widget)
    widget->value(defaultParamValue);
}

int
CheckboxParam::value()
{
  if (widget)
    return widget->value();
  return 0;
}

Fl_Widget*
OptionsParam::createVisual(int x, int y, int w)
{
  widgetsGroup = new Fl_Pack(x,y+25,w,15+25+25*optionList.size());
  widgetsGroup->box(FL_DOWN_FRAME);
  widgetsGroup->copy_label(paramName.c_str());
  widgetsGroup->begin();
  int y_cur = widgetsGroup->y();
  for(size_t i = 0; i < optionList.size(); ++i)
  {
    Fl_Round_Button* b = new Fl_Round_Button(x,y_cur,w,25,optionList[i].c_str());
    b->type(FL_RADIO_BUTTON);
    widgetsGroup->add(b);
    y_cur += 25;
  }
  widgetsGroup->end();

  loadDefaultValue();

  return widgetsGroup;
}

void
OptionsParam::loadDefaultValue()
{
  for(int k = 0; k < widgetsGroup->children(); k++)
  {
    Fl_Round_Button *o = dynamic_cast<Fl_Round_Button*>(widgetsGroup->child(k));
    if (o)
    {
      if (k == 0 || defaultOption == k)
        o->setonly();
    }
  }
}

std::string
OptionsParam::value()
{
  for(int k = 0; k < widgetsGroup->children(); k++)
  {
    Fl_Round_Button *o = dynamic_cast<Fl_Round_Button*>(widgetsGroup->child(k));
    if (o)
    {
      if (o->value())
        return o->label();
    }
  }

  throw std::logic_error("Radio button value is not set");

  return std::string();
}

OptionsParam::OptionsParam(
  const std::vector<std::string> options,
  const int defaultOptionIndex,
  const std::string name):
  BaseParam(name),
  optionList(options),
  defaultOption(defaultOptionIndex),
  widgetsGroup(NULL)
{
}

Fl_Widget*
MultiSelectParam::createVisual(int x, int y, int w)
{
  widgetsGroup = new Fl_Pack(x,y+25,w,15+25+25*optionListDefault.size());
  widgetsGroup->box(FL_DOWN_FRAME);
  widgetsGroup->copy_label(paramName.c_str());
  widgetsGroup->begin();
  int y_cur = widgetsGroup->y();
  for(std::map<std::string,bool>::const_iterator it = optionListDefault.begin();
      it != optionListDefault.end(); ++it)
  {
    Fl_Check_Button* b = new Fl_Check_Button(x,y_cur,w,25,it->first.c_str());
    widgetsGroup->add(b);
    y_cur += 25;
  }
  widgetsGroup->end();

  loadDefaultValue();

  return widgetsGroup;
}

void
MultiSelectParam::loadDefaultValue()
{
  for(int k = 0; k < widgetsGroup->children(); k++)
  {
    Fl_Check_Button *o = dynamic_cast<Fl_Check_Button*>(widgetsGroup->child(k));
    if (o)
    {
      std::map<std::string,bool>::const_iterator it
        = optionListDefault.find(o->label());
      REQUIRE(it != optionListDefault.end());
      if (it->second)
        o->value(1);
      else
        o->value(0);
    }
  }
}

bool
MultiSelectParam::selected(const std::string optionName)
{
  for(int k = 0; k < widgetsGroup->children(); k++)
  {
    Fl_Check_Button *o = dynamic_cast<Fl_Check_Button*>(widgetsGroup->child(k));
    if (o)
    {
      if (optionName == o->label())
      {
        if (o->value())
          return true;
        else
          return false;
      }
    }
  }

  throw std::logic_error("Unknown MultiSelect option");

  return false;
}

MultiSelectParam::MultiSelectParam(
  const std::map<std::string,bool> options,
  const std::string name):
  BaseParam(name),
  optionListDefault(options),
  widgetsGroup(NULL)
{
}

Fl_Widget*
DocWindowParam::createVisual(int x, int y, int w)
{
  widget = new Fl_Choice(x+w/2,y,w/2,25,paramName.c_str());
  for(size_t i = 0; i < valueList.size(); ++i)
  {
    std::string uniqValue = valueList[i]->idString();
    while (widget->find_item(uniqValue.c_str()))
      uniqValue += "+";
    widget->add(uniqValue.c_str());
  }
  loadDefaultValue();
  return widget;
}

void
DocWindowParam::loadDefaultValue()
{
  if (widget)
  {
    for(size_t i = 0; i < valueList.size(); ++i)
      if (valueList[i] == defaultDocWindow)
      {
        widget->value(i);
        return;
      }
    widget->value(0);
    std::cerr << "Warning: DocWindowParam -- default value is out of range" << std::endl;
  }
}

DocWindow*
DocWindowParam::value()
{
  if (widget)
    return valueList[widget->value()];
  return 0;
}

DocWindowParam::DocWindowParam(
  const std::vector<DocWindow*> paramValueList,
  const DocWindow* defaultDocWindowPtr,
  const std::string name):
  BaseParam(name),
  valueList(paramValueList),
  defaultDocWindow(defaultDocWindowPtr),
  widget(NULL)
{
  if (valueList.size() == 0)
    throw std::logic_error("Empty DocWindowParam list");
}

ParamsDialog::ParamsDialog(const std::string& algTitle,
                           std::vector<ParamsTab> parametersTabs):
  Fl_Window(Fl::w()/2-300,Fl::h()/2-200,600,400,""),
  paramsTabs(parametersTabs),
  modalresult(MR_CANCEL)
{
  copy_label((algTitle + " - Parameters").c_str());
  set_modal();

  btn_defaults = new Fl_Button(5,355,110,40,"Load defaults");
  btn_defaults->callback((Fl_Callback*)btn_defaults_cb);

  btn_cancel = new Fl_Button(w()-115,btn_defaults->y(),110,40,"Cancel");
  btn_cancel->callback((Fl_Callback*)btn_cancel_cb);
//  btn_cancel->deactivate();

  btn_ok = new Fl_Return_Button(
    btn_defaults->x()+btn_defaults->w()+10,btn_defaults->y(),
    btn_cancel->x()-btn_defaults->x()-btn_defaults->w()-20,40,
    "Ok, let's start ...");
  btn_ok->callback((Fl_Callback*)btn_ok_cb);
  btn_ok->take_focus();

  tabs = new Fl_Tabs(5,5,w()-10,btn_ok->y()-10);
  // for(size_t dummy1 = 0; dummy1 < 5; ++dummy1)
  for(size_t tabIndex = 0; tabIndex < paramsTabs.size(); ++tabIndex)
  {
    Fl_Group* tab_group =
      new Fl_Group(tabs->x()+5,tabs->y()+5+25,
                   tabs->w()-10,tabs->h()-10-25);
    tab_group->copy_label(paramsTabs[tabIndex].first.c_str());
    tab_group->begin();
    Fl_Scroll* scroll_group =
      new Fl_Scroll(tabs->x()+5+10,tabs->y()+5+25+10,
                    tabs->w()-10-10*2,tabs->h()-10-25-10*2);
    scroll_group->type(Fl_Scroll::VERTICAL);
    scroll_group->begin();
    int y_cur = scroll_group->y();
    // for(size_t dummy2 = 0; dummy2 < 15-dummy1; ++dummy2)
    for(size_t paramIndex = 0;
        paramIndex < paramsTabs[tabIndex].second.size();
        ++paramIndex)
    {
      y_cur += paramsTabs[tabIndex].second[paramIndex]->
        createVisual(scroll_group->x(),y_cur,
                     scroll_group->w()-15)->h();
    }
    scroll_group->end();
    tab_group->end();
  }
  tabs->end();
  end();
}

void
ParamsDialog::loadDefaults()
{
  for(size_t tabIndex = 0; tabIndex < paramsTabs.size(); ++tabIndex)
    for(size_t paramIndex = 0;
        paramIndex < paramsTabs[tabIndex].second.size();
        ++paramIndex)
      paramsTabs[tabIndex].second[paramIndex]->loadDefaultValue();
}

void
ParamsDialog::btn_ok_cb(Fl_Widget *w, void *)
{
  ParamsDialog* dlg = (ParamsDialog*)(w->parent());

  if (dlg->valuesAreCorrect())
  {
    dlg->modalresult = MR_OK;
    dlg->hide();
  }
}

void
ParamsDialog::btn_cancel_cb(Fl_Widget *w, void *)
{
  ParamsDialog* dlg = (ParamsDialog*)(w->parent());

  dlg->modalresult = MR_CANCEL;
  dlg->hide();
}

void
ParamsDialog::btn_defaults_cb(Fl_Widget *w, void *)
{
  ParamsDialog* dlg = (ParamsDialog*)(w->parent());
  dlg->loadDefaults();
}

}
