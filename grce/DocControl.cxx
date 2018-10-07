/*
  The DocControl class.

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

#include "DocControl.hpp"
#include "DocWindow.hpp"
#include "GRCE.hpp"
#include <grctk/algo/isomorphism/CMR.hpp>
#include <yaatk/yaatk.hpp>
#include <stdexcept>

namespace grce
{

DocControlItem::DocControlItem(int x0, int y0, int w0, int h0,
                               DocWindow* dw)
  :Fl_Light_Button(x0,y0,w0,h0,0),doc_item(dw)
{
  dw->control_item = this;
//  type(FL_RADIO_BUTTON);
  callback((Fl_Callback*)state_cb);
  updateTitle();
}

void DocControlItem::updateTitle()
{
  copy_label(doc_item->label());
  tooltip(doc_item->label());
}

void DocControlItem::visible_state(bool v)
{
  value(v);
}

void DocControlItem::on_remove()
{
  hide();
  parent()->remove(this);
}

void DocControlItem::closedoc()
{
  doc_item->close();
  delete doc_item;
}

void DocControlItem::on_visible_state()
{
  if (value())
  {
    doc_item->show();
  }
  else
  {
    doc_item->hide();
  }
}

void DocControlItem::state_cb(Fl_Widget *w, void *)
{
  DocControlItem* DocControlItem_Ptr;
  DocControlItem_Ptr = (DocControlItem*)w;
  DocControlItem_Ptr->on_visible_state();
}

DocControlItem::~DocControlItem()
{
}

DocControl::DocControl(int x0, int y0, int w0, int h0,
                       const char *title)
  :Fl_Scroll(x0,y0,w0,h0,title),
   universeId("universe00"),
   universeDir(yaatk::getcwd()),
   toolbox(NULL)
{
  box(FL_NO_BOX);
  pack = new Fl_Pack(x0,y0,w0,h0);
  pack->end();
  end();

  toolbox = new DocToolbox(NULL);
  toolbox->set_non_modal();
  // toolbox->show();

  redraw();

  Fl::add_timeout(1.0,redraw_invalidated_cb);
}

DocControl::~DocControl()
{
}

const std::string
DocControl::universeName()
{
  return universeId;
}

void
DocControl::universeName(const std::string& id)
{
  universeId = id;
}

const std::string
DocControl::universeDirectory()
{
  return universeDir;
}

void
DocControl::universeDirectory(const std::string& dir)
{
  universeDir = dir;
}

void DocControl::add_doc_item(DocWindow* dw)
{
  DocControlItem* ci =
    new DocControlItem(10, 10, 20, 25, dw);
  pack->add(ci);
  ci->visible_state(1);
  ci->on_visible_state();

  toolbox->set_non_modal();
  toolbox->show();
}

void DocControl::closeall()
{
  int i;
  int chld_count = pack->children();
  for(i = 0; i < chld_count;i++)
  {
    DocControlItem* cur_ci = (DocControlItem*)pack->child(0);
    // std::cout << "closing document ..." << std::endl;
    cur_ci->closedoc();
  }
}

const std::vector<DocWindow*>
DocControl::listItems() const
{
  std::vector<DocWindow*> v;
  int i;
  int chld_count = pack->children();
  for(i = 0; i < chld_count;i++)
  {
    DocControlItem* cur_ci = (DocControlItem*)pack->child(i);
    v.push_back(cur_ci->docItem());
  }
  return v;
}

DocWindow*
DocControl::getItem(int i)
{
  DocControlItem* cur_ci = (DocControlItem*)pack->child(i);
  return cur_ci->docItem();
}

void
DocControl::invalidateAttrs(const std::set<grctk::Object>& objs)
{
  DocWindow* adw = DocWindow::activeDocWindow();

  int i, chld_count = pack->children();
  for(i = 0; i < chld_count;i++)
  {
    DocControlItem* cur_ci = (DocControlItem*)pack->child(i);
    if (cur_ci->docItem() != adw)
      cur_ci->docItem()->EditBox()->invalidateAttrs(objs);
  }
}

void
DocControl::invalidateAttrs(const grctk::AdjMatrix&g)
{
  DocWindow* adw = DocWindow::activeDocWindow();

  int i, chld_count = pack->children();
  for(i = 0; i < chld_count;i++)
  {
    DocControlItem* cur_ci = (DocControlItem*)pack->child(i);
    if (cur_ci->docItem() != adw)
      cur_ci->docItem()->EditBox()->invalidateAttrs(g);
  }
}

void
DocControl::redrawInvalidated()
{
  DocWindow* adw = DocWindow::activeDocWindow();

  int i, chld_count = pack->children();
  for(i = 0; i < chld_count;i++)
  {
    DocControlItem* cur_ci = (DocControlItem*)pack->child(i);
    if (cur_ci->docItem() != adw)
      cur_ci->docItem()->EditBox()->redrawInvalidated();
  }
}

void
DocControl::redraw_invalidated_cb(void *)
{
  grce->mainWindow->docControl->redrawInvalidated();
  Fl::repeat_timeout(1.0, redraw_invalidated_cb);
}

DocWindow*
DocControl::createNew()
{
  DocWindow* grce_doc_window =
    new DocWindow(0,0,700,550,"untitled",this);
  add_doc_item(grce_doc_window);

  return grce_doc_window;
}

DocWindow*
DocControl::createNewFromGraph(const grctk::AdjMatrix &g,
                               const std::string idString,
                               const std::string description)
{
  DocWindow* grce_doc_window =
    new DocWindow(0,0,700,550,idString.c_str(),this);
  grce_doc_window->LoadFromGraph(g);
  grce_doc_window->idString(idString);
  grce_doc_window->description(description);
  add_doc_item(grce_doc_window);

  return grce_doc_window;
}

std::pair<DocWindow*,DocWindow*>
DocControl::chooseTwoDocs(
  const std::vector<DocWindow*>& docWindows,
  DocWindow* curDocWindow)
{
  if (docWindows.size() == 0)
    throw std::runtime_error("You should have some graph(s) opened first");

  std::pair<DocWindow*,DocWindow*> docWindowPair(
    docWindows[(docWindows.size() >= 2) ? (docWindows.size()-2) : 0],
    docWindows[(docWindows.size() >= 1) ? (docWindows.size()-1) : 0]);

  for(size_t i = 0; i < docWindows.size(); ++i)
    if (docWindows[i] == curDocWindow)
    {
      docWindowPair.first = docWindows[i];
      if (i+1 < docWindows.size())
        docWindowPair.second = docWindows[i+1];
      else if (i > 0)
        docWindowPair.second = docWindows[i-1];
      break;
    }

  return docWindowPair;
}

std::pair<DocWindow*,DocWindow*>
DocControl::chooseTwoDocs(DocWindow* curDocWindow)
{
  return chooseTwoDocs(listItems(), curDocWindow);
}

void
DocControl::testIsoCMR(DocWindow* docWindow1, DocWindow* docWindow2)
{
  std::vector<ParamsTab> paramsTabs;
  paramsTabs.push_back(ParamsTab("Main parameters",std::vector<BaseParam*>()));
  DocWindowParam graph1(listItems(),docWindow1,"Graph 1");
  paramsTabs[0].second.push_back(&graph1);
  DocWindowParam graph2(listItems(),docWindow2,"Graph 2");
  paramsTabs[0].second.push_back(&graph2);
  std::map<std::string,bool> presortRulesDefaults;
  presortRulesDefaults["Presort rule 1"] = true;
  presortRulesDefaults["Presort rule 2"] = true;
  presortRulesDefaults["Presort rule 3"] = true;
  MultiSelectParam presortRules(presortRulesDefaults,"Presort vertices according to:");
  paramsTabs[0].second.push_back(&presortRules);
  ParamsDialog params("Choose graphs for isomorphism test", paramsTabs);
  params.show();
  while (params.shown())
    Fl::wait();
  if (!params())
    return;

  grctk::CMR::PresortOptions presortOptions;
  presortOptions.useRule1 = presortRules.selected("Presort rule 1");
  presortOptions.useRule2 = presortRules.selected("Presort rule 2");
  presortOptions.useRule3 = presortRules.selected("Presort rule 3");

  LogExecutor logger;
  typedef AlgThreeParams<grctk::CMR,
                         const grctk::AdjMatrix,
                         const grctk::AdjMatrix,
                         const grctk::CMR::PresortOptions&,
                         bool> R;
  bool testResult;
  R* r = new R(graph1.value()->EditBox()->graphAsAdjMatrix(),
               graph2.value()->EditBox()->graphAsAdjMatrix(),
               presortOptions,
               testResult,
               logger);
  SimpleWizard wiz(r,"Testing for isomorphism using CMR");

  if (logger() && wiz())
  {
    if (testResult)
      fl_message("Graphs are isomorphic");
    else
      fl_message("Graphs are NOT isomorphic");
  }
}

}
