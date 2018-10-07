/*
  The Saver class.

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

#include "Saver.hpp"
#include <FL/Fl_File_Chooser.H>
#include <grctk/algo/formats/Environment.hpp>

namespace grce
{

Saver::Saver(const grctk::Universe& universe,
             const std::vector<DocWindow*>& docs,
             const std::string& prefix,
             const std::string& dir):
  Fl_Window(Fl::w()/2-300,Fl::h()/2-200,600,400,""),
  defaultPrefix(prefix),
  defaultDir(dir),
  graphs(),
  graphItems(docs),
  attrs(),
  attrItems(),
  modalresult(MR_CANCEL)
{
  copy_label("Save The Environment");
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

  tabs = new Fl_Tabs(5,5,w()-10,btn_ok->y()-100);
  {
    Fl_Group* tab_group =
      new Fl_Group(tabs->x()+5,tabs->y()+5+25,
                   tabs->w()-10,tabs->h()-10-25);
    tab_group->copy_label("Graphs To Save");
    tab_group->begin();
    Fl_Scroll* scroll_group =
      new Fl_Scroll(tabs->x()+5+10,tabs->y()+5+25+10,
                    tabs->w()-10-10*2,tabs->h()-10-25-10*2);
    scroll_group->type(Fl_Scroll::VERTICAL);
    scroll_group->begin();
    int y_cur = scroll_group->y();

    for(size_t docIndex = 0; docIndex < docs.size(); ++docIndex)
      graphs.push_back(new CheckboxParam(true,docs[docIndex]->label()));

    for(size_t gi = 0; gi < graphs.size(); ++gi)
    {
      y_cur += graphs[gi]->createVisual(
        scroll_group->x(),y_cur,
        scroll_group->w()-15)->h();
    }

    scroll_group->end();
    tab_group->end();
  }
  {
    Fl_Group* tab_group =
      new Fl_Group(tabs->x()+5,tabs->y()+5+25,
                   tabs->w()-10,tabs->h()-10-25);
    tab_group->copy_label("Attributes To Save");
    tab_group->begin();
    Fl_Scroll* scroll_group =
      new Fl_Scroll(tabs->x()+5+10,tabs->y()+5+25+10,
                    tabs->w()-10-10*2,tabs->h()-10-25-10*2);
    scroll_group->type(Fl_Scroll::VERTICAL);
    scroll_group->begin();
    int y_cur = scroll_group->y();

    const std::set<grctk::GenericAttribute*> attrsList = universe.listAttrs();
    for(std::set<grctk::GenericAttribute*>::iterator ait = attrsList.begin();
        ait != attrsList.end(); ++ait)
    {
      attrs.push_back(new CheckboxParam(true/*(*ait)->hasNonDefaultValues()*/,
                                        (*ait)->description()));
      attrItems.push_back(*ait);
    }

    for(size_t ai = 0; ai < attrs.size(); ++ai)
    {
      y_cur += attrs[ai]->createVisual(
        scroll_group->x(),y_cur,
        scroll_group->w()-15)->h();
    }

    scroll_group->end();
    tab_group->end();
  }
  tabs->end();

  inp_universe_id = new Fl_Input(tabs->x()+120,tabs->y()+tabs->h()+10,
                                 tabs->w()-120,25,"Universe ID:");
  inp_dir = new Fl_File_Input(
    inp_universe_id->x(),inp_universe_id->y()+inp_universe_id->h()+10,
    inp_universe_id->w()-70-10,35,"Base Directory:");
  btn_dir_browse = new Fl_Button(
    inp_dir->x()+inp_dir->w()+10,inp_dir->y(),
    70,35,"Browse");
  btn_dir_browse->callback((Fl_Callback*)btn_dir_browse_cb);

  end();

  loadDefaults();
}

Saver::~Saver()
{
  for(size_t gi = 0; gi < graphs.size(); ++gi)
    delete graphs[gi];
  for(size_t ai = 0; ai < attrs.size(); ++ai)
    delete attrs[ai];
}

void
Saver::loadDefaults()
{
  for(size_t gi = 0; gi < graphs.size(); ++gi)
    graphs[gi]->loadDefaultValue();

  for(size_t ai = 0; ai < attrs.size(); ++ai)
    attrs[ai]->loadDefaultValue();

  inp_universe_id->value(defaultPrefix.c_str());
  if (defaultDir == "")
    inp_dir->value(yaatk::getcwd().c_str());
  else
    inp_dir->value(defaultDir.c_str());
}

void
Saver::btn_ok_cb(Fl_Widget *w, void *)
{
  Saver* dlg = (Saver*)(w->parent());

  if (!fl_filename_isdir(dlg->inp_dir->value()))
  {
    fl_alert("Destination directory does not exist");
    return;
  }

  {
    yaatk::ChDir cdBaseDir(dlg->inp_dir->value(),false);
    if (yaatk::exists(std::string(dlg->inp_universe_id->value()) + ".ids"))
      if (fl_choice(
            "The destination directory already contains data with specified universe ID. Continue with overwriting?",
            "No","Yes",NULL) != 1)
        return;
  }

  dlg->modalresult = MR_OK;

  std::vector<grctk::GraphInfo> graphs;
  for(size_t gi = 0; gi < dlg->graphs.size(); ++gi)
    if (dlg->graphs[gi]->value())
      graphs.push_back(
        grctk::GraphInfo(dlg->graphItems[gi]->EditBox()->graphAsAdjMatrix(),
                         dlg->graphItems[gi]->idString(),
                         dlg->graphItems[gi]->description()));
  std::vector<grctk::GenericAttribute*> attributes;
  for(size_t ai = 0; ai < dlg->attrs.size(); ++ai)
    if (dlg->attrs[ai]->value())
      attributes.push_back(dlg->attrItems[ai]);
  grctk::saveEnv(graphs,attributes,
                 dlg->inp_dir->value(),dlg->inp_universe_id->value());

  dlg->hide();
}

void
Saver::btn_cancel_cb(Fl_Widget *w, void *)
{
  Saver* dlg = (Saver*)(w->parent());
  dlg->modalresult = MR_CANCEL;
  dlg->hide();
}

void
Saver::btn_defaults_cb(Fl_Widget *w, void *)
{
  Saver* dlg = (Saver*)(w->parent());
  dlg->loadDefaults();
}

void
Saver::btn_dir_browse_cb(Fl_Widget *w, void *)
{
  Saver* dlg = (Saver*)(w->parent());
  Fl_File_Chooser fc(
    fl_filename_isdir(dlg->inp_dir->value())?(dlg->inp_dir->value()):".",
    "*", Fl_File_Chooser::DIRECTORY,
    "Choose a directory");
  fc.newButton->activate();
  fc.show();
  while (fc.visible())
    Fl::wait();
  if (fc.value())
    dlg->inp_dir->value(fc.value());
}

} //namespace grce
