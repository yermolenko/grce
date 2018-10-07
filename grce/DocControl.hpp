/*
  The DocControl class (header file).

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

#ifndef grce_DocControl_hpp
#define grce_DocControl_hpp

#include "grce/DocWindow.hpp"
#include "grce/DocToolbox.hpp"
#include "grce/Wizard.hpp"
#include "grce/ParamsDialog.hpp"
#include "grce/algo/color/ResetColors.hpp"
#include <FL/Fl.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Light_Button.H>
#include <vector>
#include <grctk/AdjMatrix.hpp>
#include <grctk/algo/drawing/random/RandomizePositions.hpp>

namespace grce
{

class DocControlItem : public Fl_Light_Button
{
  DocWindow* doc_item;
  static void state_cb(Fl_Widget *w, void *);
public:
  DocWindow* docItem() {return doc_item;};
  void visible_state(bool);
  void closedoc();

  DocControlItem(int,int,int,int,DocWindow*);
  ~DocControlItem();

  void on_visible_state();
  void on_remove();

  void updateTitle();
};

class DocControl : public Fl_Scroll
{
  Fl_Pack* pack;
  std::string universeId;
  std::string universeDir;
public:
  const std::string universeName();
  void universeName(const std::string&);
  const std::string universeDirectory();
  void universeDirectory(const std::string&);

  DocToolbox* toolbox;

  void add_doc_item(DocWindow*);
  void closeall();

  const std::vector<DocWindow*> listItems() const;
  DocWindow* getItem(int i);

  DocControl(int,int,int,int,const char *title = 0);
  ~DocControl();

  void invalidateAttrs(const std::set<grctk::Object>&);
  void invalidateAttrs(const grctk::AdjMatrix&);
  void redrawInvalidated();

  DocWindow* createNew();
  DocWindow* createNewFromGraph(const grctk::AdjMatrix&,
                                const std::string idString = "untitled",
                                const std::string description = "");

  template <typename MulAlg>
  void multiply(DocWindow* docWindow1, DocWindow* docWindow2);

  void testIsoCMR(DocWindow* docWindow1, DocWindow* docWindow2);

  std::pair<DocWindow*,DocWindow*> chooseTwoDocs(
    const std::vector<DocWindow*>& docWindows,
    DocWindow* curDocWindow = NULL);
  std::pair<DocWindow*,DocWindow*> chooseTwoDocs(
    DocWindow* curDocWindow = NULL);

  static void redraw_invalidated_cb(void *);
};

template <typename MulAlg>
void
DocControl::multiply(DocWindow* docWindow1, DocWindow* docWindow2)
{
  std::vector<ParamsTab> paramsTabs;
  paramsTabs.push_back(ParamsTab("Main parameters",std::vector<BaseParam*>()));
  DocWindowParam graph1(listItems(),docWindow1,"Graph 1");
  paramsTabs[0].second.push_back(&graph1);
  DocWindowParam graph2(listItems(),docWindow2,"Graph 2");
  paramsTabs[0].second.push_back(&graph2);
  ParamsDialog params("Choose graphs to multiply", paramsTabs);
  params.show();
  while (params.shown())
    Fl::wait();
  if (!params())
    return;

  LogExecutor logger;
  typedef AlgThreeParams<
    MulAlg,
    const grctk::AdjMatrix, const grctk::AdjMatrix,
    grctk::Attribute<yaatk::Vector3D>&,
    grctk::AdjMatrix> R;
  grctk::AdjMatrix g;
  R* r = new R(graph1.value()->EditBox()->graphAsAdjMatrix(),
               graph2.value()->EditBox()->graphAsAdjMatrix(),
               a3D,
               g,logger);
  SimpleWizard wiz(r,"Multiply graphs");

  if (logger() && wiz())
  {
    std::ostringstream ossTitle;
    ossTitle << graph1.value()->idString() << "_x_" << graph2.value()->idString();
    DocWindow* docWindow = createNewFromGraph(g,ossTitle.str());
    grce::ResetColors algResetColors;
    algResetColors(docWindow->EditBox()->graphAsAdjMatrix(),aColor,
                   false,true);
    docWindow->EditBox()->UpdateAll();
    docWindow->EditBox()->redraw();
  }
}

}

#endif
