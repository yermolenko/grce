/*
  The Loader class.

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

#include "Loader.hpp"
#include <FL/Fl_File_Chooser.H>
#include <yaatk/yaatk.hpp>
#include <grctk/algo/formats/Environment.hpp>

namespace grce
{

Loader::Loader(grctk::Universe& destUniverse, DocControl* docCtrl):
  Fl_Window(Fl::w()/2-300,Fl::h()/2-200,600,400,""),
  universe(destUniverse),
  docControl(docCtrl),
  universeIds(),
  graphs(),
  graphItems(),
  attrs(),
  attrItems(),
  modalresult(MR_CANCEL)
{
  copy_label("Load The Environment");
  set_modal();

  btn_reset_choices = new Fl_Button(5,355,110,40,"Reset choices");
  btn_reset_choices->callback((Fl_Callback*)btn_reset_choices_cb);

  btn_cancel = new Fl_Button(w()-115,btn_reset_choices->y(),110,40,"Cancel");
  btn_cancel->callback((Fl_Callback*)btn_cancel_cb);
//  btn_cancel->deactivate();

  btn_ok = new Fl_Return_Button(
    btn_reset_choices->x()+btn_reset_choices->w()+10,btn_reset_choices->y(),
    btn_cancel->x()-btn_reset_choices->x()-btn_reset_choices->w()-20,40,
    "Ok, let's start ...");
  btn_ok->callback((Fl_Callback*)btn_ok_cb);
  btn_ok->take_focus();

  inp_dir = new Fl_File_Input(5+120,10,w()-10-120-70-10,35,"Base Directory:");
  inp_dir->when(FL_WHEN_CHANGED);
  inp_dir->callback((Fl_Callback*)inp_dir_cb);
  btn_dir_browse = new Fl_Button(
    inp_dir->x()+inp_dir->w()+10,inp_dir->y(),
    70,35,"Browse");
  btn_dir_browse->callback((Fl_Callback*)btn_dir_browse_cb);
  inp_universe_id = new Fl_Choice(
    inp_dir->x(),inp_dir->y()+inp_dir->h()+10,
    w()-10-120,25,"Universe ID:");
  inp_universe_id->callback((Fl_Callback*)inp_universe_cb);

  tabs = new Fl_Tabs(5,inp_universe_id->y()+inp_universe_id->h()+10,
                     w()-10,btn_ok->y()-100);
  {
    Fl_Group* tab_group =
      new Fl_Group(tabs->x()+5,tabs->y()+5+25,
                   tabs->w()-10,tabs->h()-10-25);
    tab_group->copy_label("Graphs To Load");
    tab_group->begin();
    graphs_scroll_group =
      new Fl_Scroll(tabs->x()+5+10,tabs->y()+5+25+10,
                    tabs->w()-10-10*2,tabs->h()-10-25-10*2);
    graphs_scroll_group->type(Fl_Scroll::VERTICAL);
    graphs_scroll_group->begin();
    // int y_cur = graphs_scroll_group->y();
    graphs_scroll_group->end();
    tab_group->end();
  }
  {
    Fl_Group* tab_group =
      new Fl_Group(tabs->x()+5,tabs->y()+5+25,
                   tabs->w()-10,tabs->h()-10-25);
    tab_group->copy_label("Attributes To Load");
    tab_group->begin();
    attrs_scroll_group =
      new Fl_Scroll(tabs->x()+5+10,tabs->y()+5+25+10,
                    tabs->w()-10-10*2,tabs->h()-10-25-10*2);
    attrs_scroll_group->type(Fl_Scroll::VERTICAL);
    attrs_scroll_group->begin();
    // int y_cur = attrs_scroll_group->y();
    attrs_scroll_group->end();
    tab_group->end();
  }
  tabs->end();

  end();

  inp_dir->value(docControl->universeDirectory().c_str());
  updateSavedUniversesList();
  // updateGraphAndAttrLists();
  resetChoices();
}

Loader::~Loader()
{
  for(size_t i = 0; i < graphs.size(); ++i)
  {
    graphs_scroll_group->remove(graphs[i]->widget);
    Fl::delete_widget(graphs[i]->widget);
    delete graphs[i];
  }
  for(size_t i = 0; i < attrs.size(); ++i)
  {
    attrs_scroll_group->remove(attrs[i]->widget);
    Fl::delete_widget(attrs[i]->widget);
    delete attrs[i];
  }
}

void
Loader::resetChoices()
{
  for(size_t gi = 0; gi < graphs.size(); ++gi)
    graphs[gi]->loadDefaultValue();

  for(size_t ai = 0; ai < attrs.size(); ++ai)
    attrs[ai]->loadDefaultValue();
}

void
Loader::btn_ok_cb(Fl_Widget *w, void *)
{
  Loader* dlg = (Loader*)(w->parent());

  if (!fl_filename_isdir(dlg->inp_dir->value()))
  {
    fl_alert("Destination directory does not exist");
    return;
  }

  dlg->modalresult = MR_OK;

  std::vector<grctk::GraphInfo> graphs;
  REQUIRE(dlg->graphs.size() == dlg->graphItems.size());
  for(size_t gi = 0; gi < dlg->graphs.size(); ++gi)
    if (dlg->graphs[gi]->value())
      graphs.push_back(dlg->graphItems[gi]);
  std::vector<grctk::GenericAttribute*> attributes;
  REQUIRE(dlg->attrs.size() == dlg->attrItems.size());
  for(size_t ai = 0; ai < dlg->attrs.size(); ++ai)
    if (dlg->attrs[ai]->value())
      attributes.push_back(dlg->attrItems[ai]);
  bool problems = false;
  try
  {
    grctk::loadEnv(dlg->universe,
                   graphs,attributes,
                   dlg->inp_dir->value(),
                   dlg->universeIds[dlg->inp_universe_id->value()]);
  }
  catch (std::exception& e)
  {
    problems = true;
    fl_alert("%s",(std::string("Error reading the environment: ") + e.what()).c_str());
  }
  catch (...)
  {
    problems = true;
    fl_alert("Unknown error while reading the environment");
  }

  for(size_t gi = 0; gi < graphs.size(); ++gi)
    dlg->docControl->createNewFromGraph(
      graphs[gi].g,
      graphs[gi].idstr,
      graphs[gi].desc + (problems?"may be corrupted":""));

  dlg->docControl->universeName(dlg->universeIds[dlg->inp_universe_id->value()]);
  dlg->docControl->universeDirectory(dlg->inp_dir->value());

  dlg->hide();
}

void
Loader::btn_cancel_cb(Fl_Widget *w, void *)
{
  Loader* dlg = (Loader*)(w->parent());
  dlg->modalresult = MR_CANCEL;
  dlg->hide();
}

void
Loader::btn_reset_choices_cb(Fl_Widget *w, void *)
{
  Loader* dlg = (Loader*)(w->parent());
  dlg->resetChoices();
}

void
Loader::updateSavedUniversesList()
{
  inp_universe_id->clear();
  std::vector<std::string> files = yaatk::listFiles(inp_dir->value());
  universeIds.clear();
  for(size_t i = 0; i < files.size(); ++i)
  {
    std::string suffix = ".ids";
    size_t idEnd = files[i].find(suffix);
    if (idEnd != std::string::npos && idEnd == files[i].size() - suffix.size())
    {
      std::string id = files[i].substr(0,idEnd);
      universeIds.push_back(id);
    }
  }
  if (universeIds.size() > 0)
  {
    for(size_t i = 0; i < universeIds.size(); ++i)
      inp_universe_id->add(universeIds[i].c_str());
  }
  else
  {
    inp_universe_id->add("There are no saves in the directory");
  }
  inp_universe_id->value(0);
  updateGraphAndAttrLists();
}

void
Loader::inp_dir_cb(Fl_Widget *w, void *)
{
  Loader* dlg = (Loader*)(w->parent());
  dlg->updateSavedUniversesList();
}

void
Loader::btn_dir_browse_cb(Fl_Widget *w, void *)
{
  Loader* dlg = (Loader*)(w->parent());

  Fl_File_Chooser fc(
    fl_filename_isdir(dlg->inp_dir->value())?(dlg->inp_dir->value()):".",
    "*", Fl_File_Chooser::DIRECTORY,
    "Choose a directory");
  fc.show();
  while (fc.visible())
    Fl::wait();
  if (fc.value())
    dlg->inp_dir->value(fc.value());

  dlg->updateSavedUniversesList();
}

void
Loader::updateGraphAndAttrLists()
{
  for(size_t i = 0; i < graphs.size(); ++i)
  {
    graphs_scroll_group->remove(graphs[i]->widget);
    Fl::delete_widget(graphs[i]->widget);
    delete graphs[i];
  }
  graphs.clear();
  graphItems.clear();
  for(size_t i = 0; i < attrs.size(); ++i)
  {
    attrs_scroll_group->remove(attrs[i]->widget);
    Fl::delete_widget(attrs[i]->widget);
    delete attrs[i];
  }
  attrs.clear();
  attrItems.clear();

  graphs_scroll_group->redraw();
  attrs_scroll_group->redraw();

  if (universeIds.size() == 0)
    return;

  const std::string universeId = universeIds[inp_universe_id->value()];

  yaatk::ChDir baseDirCD(inp_dir->value(),false);

  {
    std::vector<std::string> files = yaatk::listFiles(inp_dir->value());
    for(size_t i = 0; i < files.size(); ++i)
    {
      std::string mgraph = universeId + ".graph.";
      size_t uidStart = files[i].find(mgraph);
      if (uidStart != std::string::npos && uidStart == 0)
      {
        REQUIRE(mgraph.size() < files[i].size());
        std::string graphId = files[i].substr(mgraph.size());
        graphs.push_back(new CheckboxParam(true,graphId));
        graphItems.push_back(grctk::GraphInfo(grctk::AdjMatrix(),graphId));
      }
    }

    int y_cur = graphs_scroll_group->y();

    for(size_t gi = 0; gi < graphs.size(); ++gi)
    {
      Fl_Widget* w = graphs[gi]->createVisual(
        graphs_scroll_group->x(),y_cur,
        graphs_scroll_group->w()-15);
      y_cur += w->h();
      graphs_scroll_group->add(w);
    }

    graphs_scroll_group->redraw();
  }

  {
    const std::set<grctk::GenericAttribute*> attrsList = universe.listAttrs();
    for(std::set<grctk::GenericAttribute*>::iterator ait = attrsList.begin();
        ait != attrsList.end(); ++ait)
      if (yaatk::exists(universeId + ".attr." + (*ait)->idString()))
      {
        attrs.push_back(new CheckboxParam(true/*(*ait)->hasNonDefaultValues()*/,
                                          (*ait)->description()));
        attrItems.push_back(*ait);
      }

    int y_cur = attrs_scroll_group->y();

    for(size_t ai = 0; ai < attrs.size(); ++ai)
    {
      Fl_Widget* w = attrs[ai]->createVisual(
        attrs_scroll_group->x(),y_cur,
        attrs_scroll_group->w()-15);
      y_cur += w->h();
      attrs_scroll_group->add(w);
    }

    attrs_scroll_group->redraw();
  }
}

void
Loader::inp_universe_cb(Fl_Widget *w, void *)
{
  Loader* dlg = (Loader*)(w->parent());
  dlg->updateGraphAndAttrLists();
}

} //namespace grce
