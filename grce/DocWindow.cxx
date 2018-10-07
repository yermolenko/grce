/*
  The DocWindow class.

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

#include "DocWindow.hpp"
#include "grce/Wizard.hpp"
#include "grce/Saver.hpp"
#include "grce/MainWindow.hpp"
#include "grce/VisWindow.hpp"
#include "grce/algo/color/ResetColors.hpp"
#include "grce/algo/color/Attr2Color.hpp"
#include <grctk/algo/attrconv/Map2Int.hpp>
#include <grctk/algo/products/CartesianProduct.hpp>
#include <grctk/algo/products/TensorProduct.hpp>
#include <grctk/algo/products/LexicographicalProduct.hpp>
#include <grctk/algo/products/StrongProduct.hpp>
#include <grctk/algo/connectivity/ConComp.hpp>
#include <grctk/algo/drawing/random/RandomizePositions.hpp>
#include <grctk/algo/drawing/pairpot/PairPot.hpp>
#include <grctk/algo/drawing/intersections/OptiIntersect.hpp>
#include <grctk/algo/orbits/FindOrbitsSubgraphIso.hpp>
#include <grctk/algo/orbits/FindOrbitsVPerms.hpp>
#include <grctk/algo/orbits/FindOrbitsEdgeTensions.hpp>
#include <grctk/algo/orbits/CmpSubgraphIsoAndEdgeTensions.hpp>
#include <grctk/algo/properties/BasicProperties.hpp>
#include <FL/fl_ask.H>

namespace grce
{

void DocWindow::mnu_edit_mode_cb(Fl_Widget *widget, void *data)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(widget->parent());
  GRCE_EDIT_MODE m;
  switch (size_t(data))
  {
  case 1:
    m = GRCE_EDIT_MOVE_VERTEX;
    break;
  case 2:
    m = GRCE_EDIT_CREATE_VERTEX;
    break;
  case 3:
    m = GRCE_EDIT_REMOVE_VERTEX;
    break;
  case 4:
    m = GRCE_EDIT_CREATE_EDGE;
    break;
  case 5:
    m = GRCE_EDIT_REMOVE_EDGE;
    break;
  case 6:
    m = GRCE_EDIT_VERTEX_PROPERTIES;
    break;
  case 7:
    m = GRCE_EDIT_EDGE_PROPERTIES;
    break;
  case 8:
    m = GRCE_EDIT_ADD_VERTEX_TO_SELECTION;
    break;
  case 9:
    m = GRCE_EDIT_ADD_EDGE_TO_SELECTION;
    break;
  };
  DocWindow_Ptr->edit_mode_set(m);
}

void DocWindow::mnu_save_cb(Fl_Widget *widget, void *)
{
  DocWindow* docWindow = (DocWindow*)(widget->parent());

  std::vector<DocWindow*> docs;
  docs.push_back(docWindow);
  Saver saver(grctk::Universe::singleton(),
              docs,
              "univ_with_the_graph");
  saver.show();
  while (saver.shown())
    Fl::wait();
  // if (!saver())
  //   return;
}

void DocWindow::mnu_rename_cb(Fl_Widget *widget, void *data)
{
  DocWindow* docWindow = (DocWindow*)(widget->parent());
  char *c = (char*)data;

  if (*c == 'i')
  {
    const char *rval = fl_input("Change graph name:",docWindow->idString().c_str());
    if (rval != NULL)
      docWindow->idString(rval);
  }
  else if (*c == 'd')
  {
    const char *rval = fl_input("Change graph description:",docWindow->description().c_str());
    if (rval != NULL)
      docWindow->description(rval);
  }
}

void DocWindow::mnu_refresh_cb(Fl_Widget *widget, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(widget->parent());
  int x,y,w,h;
  x = DocWindow_Ptr->x();
  y = DocWindow_Ptr->y();
  w = DocWindow_Ptr->w();
  h = DocWindow_Ptr->h();
  DocWindow_Ptr->resize(x,y,w+1,h+1);
  DocWindow_Ptr->edit_box->redraw();
  DocWindow_Ptr->redraw();
  DocWindow_Ptr->resize(x,y,w,h);
  DocWindow_Ptr->edit_box->redraw();
  DocWindow_Ptr->redraw();
}

void DocWindow::mnu_scale_to_window_cb(Fl_Widget *widget, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(widget->parent());
  DocWindow_Ptr->edit_box->UpdateAll();
  DocWindow_Ptr->edit_box->redraw();
  DocWindow_Ptr->redraw();
}

void DocWindow::mnu_basic_properties_cb(Fl_Widget *w, void *)
{
  DocWindow* dw = (DocWindow*)(w->parent());

  LogExecutor logger;
  typedef AlgOneParamNoRet<
    grctk::BasicProperties,
    const grctk::AdjMatrix> R;
  R* r = new R(dw->edit_box->graphAsAdjMatrix(), logger);
  SimpleWizard wiz(r,"Basic graph properties");

  if (logger() && wiz()) { }
}

void DocWindow::mnu_concomp_cb(Fl_Widget *w, void *)
{
  DocWindow* dw = (DocWindow*)(w->parent());

  std::vector<ParamsTab> paramsTabs;
  paramsTabs.push_back(ParamsTab("Main parameters",std::vector<BaseParam*>()));
  IntegerParam vstart(0,"Initial vertex");
  paramsTabs[0].second.push_back(&vstart);
  std::vector<std::string> destinations;
  destinations.push_back("New graph");
  destinations.push_back("New view");
  destinations.push_back("Selection");
  destinations.push_back("Add to Selection");
  OptionsParam dest(destinations,0,"Present connectivity component as:");
  paramsTabs[0].second.push_back(&dest);
  ParamsDialog params("Set parameters", paramsTabs);
  params.show();
  while (params.shown())
    Fl::wait();
  if (!params())
    return;

  LogExecutor logger;
  typedef AlgThreeParams<
    grctk::ConComp,
    const grctk::AdjMatrix, const size_t, size_t&,
    grctk::AdjMatrix> R;
  size_t tmp_concount;
  grctk::AdjMatrix g;
  R* r = new R(dw->edit_box->graphAsAdjMatrix(), vstart.value(), tmp_concount, g, logger);
  SimpleWizard wiz(r,"Generate connectivity component");

  if (logger() && wiz())
  {
    TRACE(tmp_concount);
    std::ostringstream ossTitle;
    ossTitle << "ConComp_" << dw->idString() << "_" << vstart.value();
    if (dest.value() == "New graph")
      dw->docControl->createNewFromGraph(g.clone(),ossTitle.str());
    else if (dest.value() == "New view")
      dw->docControl->createNewFromGraph(g,ossTitle.str());
    else if (dest.value() == "Selection")
    {
      dw->edit_box->DeselectAll();
      dw->edit_box->SelectFromGraph(g);
    }
    else if (dest.value() == "Add to Selection")
      dw->edit_box->SelectFromGraph(g);
    else
      fl_message("Warning: unknown destination");
  }
}

template <typename FindOrbitsAlg, typename OrbitsAttribute>
void DocWindow::mnu_find_orbits_cb(Fl_Widget *w, void *)
{
  DocWindow* dw = (DocWindow*)(w->parent());
  dw->findOrbits<FindOrbitsAlg, OrbitsAttribute>();
}

template <typename FindOrbitsAlg, typename OrbitsAttribute>
void DocWindow::findOrbits()
{
  std::vector<ParamsTab> paramsTabs;
  paramsTabs.push_back(ParamsTab("Main parameters",std::vector<BaseParam*>()));
  std::vector<std::string> destinations;
  destinations.push_back("The copy of the graph");
  destinations.push_back("The original graph");
  OptionsParam dest(destinations,0,"Assign orbit marks to:");
  paramsTabs[0].second.push_back(&dest);
  CheckboxParam colorize(true,"Colorize vertices");
  paramsTabs[0].second.push_back(&colorize);
  CheckboxParam orbit2int(true,"Store orbit index in integer vertex weight");
  paramsTabs[0].second.push_back(&orbit2int);
  ParamsDialog params("Find orbits", paramsTabs);
  params.show();
  while (params.shown())
    Fl::wait();
  if (!params())
    return;

  LogExecutor logger;
  typedef AlgTwoParamsNoRet<
    FindOrbitsAlg,
    const grctk::AdjMatrix, OrbitsAttribute&> R;
  OrbitsAttribute aOrbits;
  const grctk::AdjMatrix& g = edit_box->graphAsAdjMatrix();
  R* r = new R(g,aOrbits,logger);
  SimpleWizard wiz(r,"Find orbits");

  if (logger() && wiz())
  {
    if (dest.value() == "The copy of the graph")
    {
      std::ostringstream ossTitle;
      ossTitle << idString() << "_Orbits_" << "using_CMR";
      DocWindow* docWindow = docControl->createNewFromGraph(g.clone(),ossTitle.str());
      grctk::AdjMatrix newg = docWindow->edit_box->graphAsAdjMatrix();
      if (orbit2int.value())
      {
        grctk::Attribute<int> aOrbitsIndex;
        grctk::Map2Int<typename OrbitsAttribute::AttrType> m2i;
        m2i(newg,aOrbits,aOrbitsIndex);
        for(size_t i = 0; i < newg.size(); ++i)
          aInt[newg[i]] = aOrbitsIndex[newg[i]];
      }
      if (colorize.value())
      {
        Attr2Color a2c;
        a2c(newg,aOrbits,aColor);
      }
      docWindow->edit_box->UpdateAll();
      docWindow->edit_box->redraw();
    }
    else if (dest.value() == "The original graph")
    {
      if (orbit2int.value())
      {
        grctk::Attribute<int> aOrbitsIndex;
        grctk::Map2Int<typename OrbitsAttribute::AttrType> m2i;
        m2i(g,aOrbits,aOrbitsIndex);
        for(size_t i = 0; i < g.size(); ++i)
          aInt[g[i]] = aOrbitsIndex[g[i]];
      }
      if (colorize.value())
      {
        Attr2Color a2c;
        a2c(g,aOrbits,aColor);
      }
      docControl->invalidateAttrs(g);
      edit_box->UpdateAll();
      edit_box->redraw();
    }
    else
      fl_message("Warning: unknown destination");
  }
}

void DocWindow::mnu_cmp_find_orbits_cb(Fl_Widget *w, void *)
{
  DocWindow* dw = (DocWindow*)(w->parent());

  LogExecutor logger;
  typedef AlgThreeParams<
    grctk::CmpSubgraphIsoAndEdgeTensions,
    const grctk::AdjMatrix,
    grctk::Attribute<int>&,
    grctk::Attribute<yaatk::Rational>&,
    bool> R;
  grctk::Attribute<int> aOrbitSubgraphIso;
  grctk::Attribute<yaatk::Rational> aOrbitEdgeTensions;
  const grctk::AdjMatrix& g = dw->edit_box->graphAsAdjMatrix();
  bool cmpResult = false;
  R* r = new R(g,aOrbitSubgraphIso,aOrbitEdgeTensions,cmpResult,logger);
  SimpleWizard wiz(r,"Compare algorithms for finding orbits");

  if (logger() && wiz())
  {
    if (!cmpResult)
    {
      {
        std::ostringstream ossTitle;
        ossTitle << dw->idString() << "_Orbits_" << "SubgraphIso";
        DocWindow* docWindow = dw->docControl->createNewFromGraph(g.clone(),ossTitle.str());
        grctk::AdjMatrix newg = docWindow->edit_box->graphAsAdjMatrix();
        {
          grctk::Attribute<int> aOrbitsIndex;
          grctk::Map2Int<int> m2i;
          m2i(newg,aOrbitSubgraphIso,aOrbitsIndex);
          for(size_t i = 0; i < newg.size(); ++i)
            aInt[newg[i]] = aOrbitsIndex[newg[i]];
        }
        {
          Attr2Color a2c;
          a2c(newg,aOrbitSubgraphIso,aColor);
        }
        docWindow->edit_box->UpdateAll();
        docWindow->edit_box->redraw();
      }
      {
        std::ostringstream ossTitle;
        ossTitle << dw->idString() << "_Orbits_" << "EdgeTensions";
        DocWindow* docWindow = dw->docControl->createNewFromGraph(g.clone(),ossTitle.str());
        grctk::AdjMatrix newg = docWindow->edit_box->graphAsAdjMatrix();
        // {
        //   grctk::Attribute<int> aOrbitsIndex;
        //   grctk::Map2Int<int> m2i;
        //   m2i(newg,aOrbitEdgeTensions,aOrbitsIndex);
        //   for(size_t i = 0; i < newg.size(); ++i)
        //     aInt[newg[i]] = aOrbitsIndex[newg[i]];
        // }
        {
          Attr2Color a2c;
          a2c(newg,aOrbitEdgeTensions,aColor);
        }
        docWindow->edit_box->UpdateAll();
        docWindow->edit_box->redraw();
      }
    }
  }
}

void DocWindow::mnu_optimize_expsimp_cb(Fl_Widget *w, void *)
{
  DocWindow* dw = (DocWindow*)(w->parent());

  std::vector<ParamsTab> paramsTabs;

  paramsTabs.push_back(ParamsTab("Main parameters",std::vector<BaseParam*>()));
  IntegerParam dim(2,"Dimensions");
  paramsTabs[0].second.push_back(&dim);
  FloatParam epsilon(0.0001,"Epsilon");
  paramsTabs[0].second.push_back(&epsilon);
  IntegerParam stocha(10,"Stochastic loads");
  paramsTabs[0].second.push_back(&stocha);
  FloatParam initL(1.0,"Initial L");
  paramsTabs[0].second.push_back(&initL);
  CheckboxParam use_existed(false,"Use existed representation");
  paramsTabs[0].second.push_back(&use_existed);
  CheckboxParam save_every(true,"Save information about all stages");
  paramsTabs[0].second.push_back(&save_every);
  std::vector<std::string> destinations;
  destinations.push_back("The copy of the graph");
  destinations.push_back("The original graph");
  destinations.push_back("Visualization");
  OptionsParam dest(destinations,2,"Apply new vertex positions to:");
  paramsTabs[0].second.push_back(&dest);

  paramsTabs.push_back(ParamsTab("Potential parameters", std::vector<BaseParam*>()));
  FloatParam alpha0(6.0,"alpha_0");
  paramsTabs[1].second.push_back(&alpha0);
  FloatParam alpha1(2.0,"alpha_1");
  paramsTabs[1].second.push_back(&alpha1);
  FloatParam k(0.03,"k");
  paramsTabs[1].second.push_back(&k);
  FloatParam r_0(0.12,"r_0");
  paramsTabs[1].second.push_back(&r_0);

  ParamsDialog params("Set parameters", paramsTabs);
  params.show();
  while (params.shown())
    Fl::wait();
  if (!params())
    return;

  LogExecutor logger;
  typedef RunOptimizeWithOneTuneTwoConstAndOneNoncostParams<
    grctk::PairPot, grctk::PairPotBase::TuneParams,
    grctk::PairPotBase::InputParams, grctk::PairPot::InputParams, grctk::GraphMultiRep,
    grctk::AdjMatrix> R;
  grctk::PairPotBase::TuneParams tune(
    alpha0.value(),alpha1.value(),k.value(),r_0.value());
  grctk::PairPotBase::InputParams inputBase(
    a3D,initL.value(),epsilon.value(),use_existed.value());
  grctk::PairPot::InputParams input(
    stocha.value(),save_every.value());
  grctk::GraphMultiRep tmp_multiRep(
    dw->edit_box->graphAsAdjMatrix(),dim.value());
  R* r = new R(tune,inputBase,input,tmp_multiRep,logger);
  SimpleWizard wiz(r,"Generate optimized representations");

  if (logger() && wiz())
  {
    if (dest.value() == "Visualization")
    {
      std::ostringstream ossTitle;
      ossTitle << dw->idString() << " - Opti" << dim.value() << "D";
      VisWindow* grce_vis_window
        = new VisWindow(tmp_multiRep,false,ossTitle.str());
      grce_vis_window->show();
    }
    else if (dest.value() == "The copy of the graph")
    {
      std::ostringstream ossTitle;
      ossTitle << dw->idString() << "_Optimized";
      DocWindow* docWindow = dw->docControl->createNewFromGraph(tmp_multiRep.g.clone(),ossTitle.str());
      grctk::AdjMatrix newg = docWindow->edit_box->graphAsAdjMatrix();
      const std::vector<yaatk::Vector3D> rep3D = tmp_multiRep.getRep3DProj(tmp_multiRep.findRepWithMinEnergy());
      for(size_t i = 0; i < newg.size(); ++i)
        a3D[newg[i]] = rep3D[i];
      docWindow->edit_box->UpdateAll();
      docWindow->edit_box->redraw();
    }
    else if (dest.value() == "The original graph")
    {
      const std::vector<yaatk::Vector3D> rep3D = tmp_multiRep.getRep3DProj(tmp_multiRep.findRepWithMinEnergy());
      for(size_t i = 0; i < tmp_multiRep.g.size(); ++i)
        a3D[tmp_multiRep.g[i]] = rep3D[i];
      dw->docControl->invalidateAttrs(tmp_multiRep.g);
      dw->edit_box->UpdateAll();
      dw->edit_box->redraw();
    }
    else
      fl_message("Warning: unknown destination");
  }
}

void DocWindow::mnu_opti_intersect_cb(Fl_Widget *w, void *)
{
  DocWindow* dw = (DocWindow*)(w->parent());

  std::vector<ParamsTab> paramsTabs;
  paramsTabs.push_back(ParamsTab("Main parameters",std::vector<BaseParam*>()));
  IntegerParam vGroupSize(5,"Number of vertices in a group");
  paramsTabs[0].second.push_back(&vGroupSize);
  std::vector<std::string> destinations;
  destinations.push_back("The copy of the graph");
  destinations.push_back("The original graph");
  OptionsParam dest(destinations,0,"Apply new vertex positions to:");
  paramsTabs[0].second.push_back(&dest);
  ParamsDialog params("Set parameters", paramsTabs);
  params.show();
  while (params.shown())
    Fl::wait();
  if (!params())
    return;

  LogExecutor logger;
  typedef AlgThreeParamsNoRet<
    grctk::OptiIntersect,
    const grctk::AdjMatrix, const int,
    grctk::Attribute<yaatk::Vector2D>& > R;
  grctk::Attribute<yaatk::Vector2D> a2D;
  const grctk::AdjMatrix& g = dw->edit_box->graphAsAdjMatrix();
  R* r = new R(g,vGroupSize.value(),a2D,logger);
  SimpleWizard wiz(r,"Minimizing edge intersections");

  if (logger() && wiz())
  {
    if (dest.value() == "The copy of the graph")
    {
      std::ostringstream ossTitle;
      ossTitle << dw->idString() << "_MinIntersect_" << vGroupSize.value();
      DocWindow* docWindow = dw->docControl->createNewFromGraph(g.clone(),ossTitle.str());
      grctk::AdjMatrix newg = docWindow->edit_box->graphAsAdjMatrix();
      for(size_t i = 0; i < newg.size(); ++i)
        a3D[newg[i]] = a2D[newg[i]];
      docWindow->edit_box->UpdateAll();
      docWindow->edit_box->redraw();
    }
    else if (dest.value() == "The original graph")
    {
      for(size_t i = 0; i < g.size(); ++i)
        a3D[g[i]] = a2D[g[i]];
      dw->docControl->invalidateAttrs(g);
      dw->edit_box->UpdateAll();
      dw->edit_box->redraw();
    }
    else
      fl_message("Warning: unknown destination");
  }
}

void DocWindow::mnu_visualize_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());

  VisWindow* grce_vis_window = new VisWindow(
    grctk::GraphMultiRep(DocWindow_Ptr->EditBox()->graphAsAdjMatrix(),3,a3D),
    false, DocWindow_Ptr->idString());
  grce_vis_window->show();
}

void DocWindow::mnu_select_all_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow_Ptr->edit_box->SelectAll();
}

void DocWindow::mnu_deselect_all_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow_Ptr->edit_box->DeselectAll();
}

void DocWindow::mnu_select_all_vertexes_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow_Ptr->edit_box->SelectAllVertexes();
}

void DocWindow::mnu_deselect_all_vertexes_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow_Ptr->edit_box->DeselectAllVertexes();
}
void DocWindow::mnu_select_all_edges_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow_Ptr->edit_box->SelectAllEdges();
}

void DocWindow::mnu_deselect_all_edges_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow_Ptr->edit_box->DeselectAllEdges();
}

void DocWindow::mnu_reset_all_edge_weights_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow_Ptr->edit_box->ResetAllEdgeWeights();
}

void DocWindow::mnu_reset_all_vertex_weights_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow_Ptr->edit_box->ResetAllVertexWeights();
}

void DocWindow::mnu_label_vertexes_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow_Ptr->edit_box->LabelAllVertexes();
}

void DocWindow::mnu_reset_all_weights_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow_Ptr->edit_box->ResetAllWeights();
}

void DocWindow::mnu_open_selection_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow* dw = DocWindow_Ptr->docControl->createNew();
  dw->edit_box->LoadFromGraph(DocWindow_Ptr->edit_box->selectionAsAdjMatrix());
  dw->idString(std::string("sel of ") + DocWindow_Ptr->idString());
}

void DocWindow::mnu_duplicate_selection_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow* dw = DocWindow_Ptr->docControl->createNew();
  dw->edit_box->LoadFromGraph(DocWindow_Ptr->edit_box->selectionAsAdjMatrix().clone());
  dw->idString(std::string("clone of sel of ") + DocWindow_Ptr->idString());
}

void DocWindow::mnu_edit_selection_prop_cb(Fl_Widget *w, void *)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow_Ptr->edit_box->selection_properties();
}

void DocWindow::mnu_duplicate_cb(Fl_Widget *w, void *)
{
  DocWindow*    DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow* dw = DocWindow_Ptr->docControl->createNew();
  dw->edit_box->LoadFromGraph(DocWindow_Ptr->edit_box->graphAsAdjMatrix().clone());
  dw->idString(std::string("clone of ") + DocWindow_Ptr->idString());
}

template <typename ProductAlg>
void DocWindow::mnu_product_cb(Fl_Widget *w, void *)
{
  DocWindow* docWindow = (DocWindow*)(w->parent());
  DocControl* docControl = ((DocWindow*)(w->parent()))->docControl;

  std::pair<DocWindow*,DocWindow*> docWindowPair;
  try { docWindowPair = docControl->chooseTwoDocs(docWindow); }
  catch(...) { return; }
  docControl->multiply<ProductAlg>(docWindowPair.first, docWindowPair.second);
}

void DocWindow::mnu_iso_CMR_cb(Fl_Widget *w, void *)
{
  DocWindow* docWindow = (DocWindow*)(w->parent());
  DocControl* docControl = ((DocWindow*)(w->parent()))->docControl;

  std::pair<DocWindow*,DocWindow*> docWindowPair;
  try { docWindowPair = docControl->chooseTwoDocs(docWindow); }
  catch(...) { return; }
  docControl->testIsoCMR(docWindowPair.first, docWindowPair.second);
}

void DocWindow::test_cb(Fl_Widget* w, void*)
{
  Fl_Menu_* mw = (Fl_Menu_*)w;
  const Fl_Menu_Item* m = mw->mvalue();
  if (m && m->submenu())
    return;
  if (!m)
    fl_message("NULL\n Not Implemented yet :-( \n");
  else if (m->shortcut())
    fl_message("%s - %s\n Not Implemented yet :-( \n",
               m->label(), fl_shortcut_label(m->shortcut()));
  else
    fl_message("%s\n Not Implemented yet :-( \n", m->label());
}

void DocWindow::close_cb(Fl_Widget* w, void*)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocWindow_Ptr->close();
  delete DocWindow_Ptr;
}

void DocWindow::close()
{
  control_item->on_remove();
  hide();
  doc_toolbox->setDocWindow(NULL);
}

void DocWindow::mnu_toolbox_cb(Fl_Widget* w, void*)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocToolbox* DocToolbox_Ptr;
  DocToolbox_Ptr = DocWindow_Ptr->doc_toolbox;
  if (DocToolbox_Ptr->shown())
  {
    DocToolbox_Ptr->hide();
  }
  else
  {
    DocToolbox_Ptr->show();
  }
}

void DocWindow::mnu_mainwindow_cb(Fl_Widget* w, void*)
{
  DocWindow* DocWindow_Ptr;
  DocWindow_Ptr = (DocWindow*)(w->parent());
  DocControl* DocControl_Ptr;
  DocControl_Ptr = DocWindow_Ptr->docControl;
  MainWindow* MainWindow_Ptr;
  MainWindow_Ptr = (MainWindow*)DocControl_Ptr->parent();
  if (MainWindow_Ptr->shown())
  {
    MainWindow_Ptr->hide();
  }
  else
  {
    MainWindow_Ptr->show();
  }
}

DocWindow*
DocWindow::activeDocWindow()
{
  Fl_Widget* w = Fl::focus();
  DocWindow *activeDocWindow = NULL;
  while (w)
  {
    activeDocWindow = dynamic_cast<DocWindow*>(w);
    w = w->parent();
  }
  return activeDocWindow;
}

int DocWindow::handle(int e)
{
  switch(e)
  {
  case FL_HIDE:
    if (control_item != NULL)
    {
      control_item->value(0);
    };
    // doc_toolbox->hide();
    return Fl_Double_Window::handle(e);
    break;
  case FL_SHOW:
    // doc_toolbox->show();
    return Fl_Double_Window::handle(e);
    break;
  case FL_FOCUS:
  case FL_ENTER:
    doc_toolbox->setDocWindow(this);
    return Fl_Double_Window::handle(e);
    break;
  default:
    return Fl_Double_Window::handle(e);
  }
//  doc_toolbox->show();
}

DocWindow::DocWindow(int x0, int y0, int w0, int h0,
                     const std::string idStr, DocControl* dc):
  Fl_Double_Window(x0,y0,w0,h0,""),
  doc_toolbox(dc->toolbox),
  control_item(0),
  docControl(dc),
  idstr(idStr),
  desc("")
{
  updateLabel();

  menubar = new Fl_Menu_Bar(0,0,w0,30);
  menubar->menu(menutable);
  menubar->callback(test_cb);

  statusbar = new Fl_Box(0,h0-25,w0,25);
  statusbar->box(FL_THIN_UP_BOX);
  statusbar->labelcolor(FL_BLUE);
  statusbar->labelfont(FL_TIMES_BOLD);
  statusbar->align(FL_ALIGN_INSIDE | FL_ALIGN_LEFT);
  statusbar->label("Hint: press <CTRL>+<R> to refresh,"
                   " if screen become damaged or runs too slow");

  size_range(100,100);
  edit_box = new GraphEdit(0,30,w0,h0-55);
  edit_box->set_statusbar(statusbar);
  resizable(edit_box);

  // doc_toolbox = new DocToolbox(this);

  end();
  doc_toolbox->setDocWindow(this);
  this->show();
  // doc_toolbox->set_non_modal();
  // doc_toolbox->hide();
  // doc_toolbox->show();
}

void DocWindow::LoadFromGraph(const grctk::AdjMatrix& gc)
{
  edit_box->LoadFromGraph(gc);
}

void
DocWindow::updateLabel()
{
  if (desc != "")
  {
    std::string cutFilename = desc;
    size_t maxShowedLen = 200;
    if (desc.size() > maxShowedLen)
      cutFilename = "..." +
        desc.substr(desc.size() - maxShowedLen);
    copy_label((idstr + " - [" + cutFilename + "]").c_str());
  }
  else
    copy_label(idstr.c_str());

  if (control_item)
    control_item->updateTitle();
  if (doc_toolbox)
    doc_toolbox->update_label();
}

void
DocWindow::idString(const std::string& idStr)
{
  // copy_label(newTitle.c_str());
  idstr = idStr;
  updateLabel();
}

void
DocWindow::description(const std::string& descr)
{
  // copy_label(newTitle.c_str());
  desc = descr;
  updateLabel();
}

DocWindow::~DocWindow()
{
}

void DocWindow::on_edit_mode_set()
{
  edit_box->grce_edit_mode(doc_toolbox->grce_edit_mode());
//  statusbar->label(edit_box->mode_info.hint);
}

void DocWindow::edit_mode_set(GRCE_EDIT_MODE m)
{
  edit_box->grce_edit_mode(m);
  doc_toolbox->onset(m);
//  statusbar->label(edit_box->mode_info.hint);
}

Fl_Menu_Item DocWindow::menutable[] = {
  {"&Graph", 0, 0, 0, FL_SUBMENU},
  {"Open 3D &View", FL_SHIFT+'R', mnu_visualize_cb, 0, 0},
  {"&Clone", FL_SHIFT+'N', mnu_duplicate_cb, 0, FL_MENU_DIVIDER},
  {"&Save ...", FL_CTRL + 'S', mnu_save_cb, 0, FL_MENU_DIVIDER},
  {"Re&name ...", 0, mnu_rename_cb, (void*)"i", 0},
  {"Change &description ...", 0, mnu_rename_cb, (void*)"d", FL_MENU_DIVIDER},
  {"Purge", FL_ALT+'q', close_cb, 0},
  {0},
  {"&Edit", 0, 0, 0, FL_SUBMENU},
  {"Mode", 0, 0, 0, FL_SUBMENU | FL_MENU_DIVIDER},
  {"Move vertex", FL_CTRL+'1', mnu_edit_mode_cb, (void*)1, 0},
  {"Create vertex", FL_CTRL+'2', mnu_edit_mode_cb, (void*)2, 0},
  {"Remove vertex", FL_CTRL+'3', mnu_edit_mode_cb, (void*)3, 0},
  {"Create Edge", FL_CTRL+'4', mnu_edit_mode_cb, (void*)4, 0},
  {"Remove Edge", FL_CTRL+'5', mnu_edit_mode_cb, (void*)5, 0},
  {"Vertex Properties", FL_CTRL+'6', mnu_edit_mode_cb, (void*)6, 0},
  {"Edge Properties", FL_CTRL+'7', mnu_edit_mode_cb, (void*)7, 0},
  {"Add vertex to selection subgraph", FL_CTRL+'8', mnu_edit_mode_cb, (void*)8, 0},
  {"Add edge to selection subgraph", FL_CTRL+'9', mnu_edit_mode_cb, (void*)9, 0},
  {0},
  {"Vertexes", 0, 0, 0, FL_SUBMENU},
  {"Select All", 0, mnu_select_all_vertexes_cb, 0, 0},
  {"Deselect All", 0, mnu_deselect_all_vertexes_cb, 0, FL_MENU_DIVIDER},
  {"Reset All weights", 0, mnu_reset_all_vertex_weights_cb, 0, 0},
  {"Label with Numbers", 0, mnu_label_vertexes_cb, 0, 0},
  {0},
  {"Edges", 0, 0, 0, FL_SUBMENU},
  {"Select All",0, mnu_select_all_edges_cb, 0, 0},
  {"Deselect All",0, mnu_deselect_all_edges_cb, 0, FL_MENU_DIVIDER},
  {"Reset All weights",0, mnu_reset_all_edge_weights_cb, 0, 0},
  {0},
  {"Select All", FL_CTRL+'A', mnu_select_all_cb, 0, 0},
  {"Deselect All", 0, mnu_deselect_all_cb, 0, FL_MENU_DIVIDER},
  {"Reset All Weights", 0, mnu_reset_all_weights_cb, 0, FL_MENU_DIVIDER},
  {"Selection", 0, 0, 0, FL_SUBMENU},
  {"Edit properties", 0, mnu_edit_selection_prop_cb, 0, 0},
  {"Open selection subgraph in another window", 0, mnu_open_selection_cb, 0, 0},
  {"Clone selection subgraph and open it", 0, mnu_duplicate_selection_cb, 0, 0},
  {0},
  {0},
  {"&View", 0, 0, 0, FL_SUBMENU},
  {"Scale to fit in window", 0, mnu_scale_to_window_cb, 0, 0},
  {"Refresh", FL_CTRL+'R', mnu_refresh_cb, 0, FL_MENU_DIVIDER},
  {"Show/hide toolbox", FL_CTRL+'T', mnu_toolbox_cb, 0, 0},
  {"Show/hide main window", FL_CTRL+'M', mnu_mainwindow_cb, 0, 0},
  {0},
  {"&Algorithms", FL_F+3, 0, 0, FL_SUBMENU},
  // {"Shortest paths", 0, 0, 0, FL_SUBMENU},
  // {"&Dijkstra...",0, mnu_dijkstra_cb, 0, 0},
  // {0},
  {"Connectedness", 0, 0, 0, FL_SUBMENU},
  {"&Component...", 0, mnu_concomp_cb, 0, 0},
  {0},
  {"Orbits", 0, 0, 0, FL_SUBMENU},
  {"Find &orbits using edge tensions...", 0, mnu_find_orbits_cb<grctk::FindOrbitsEdgeTensions,grctk::Attribute<yaatk::Rational> >, 0, 0},
  {"Find &orbits using subgraph isomorphism check ...", 0, mnu_find_orbits_cb<grctk::FindOrbitsSubgraphIso,grctk::Attribute<int> >, 0, 0},
  {"Find &orbits using vertex permutations (slow) ...", 0, mnu_find_orbits_cb<grctk::FindOrbitsVPerms,grctk::Attribute<int> >, 0, FL_MENU_DIVIDER},
  {"&Check Orbits (Subgraph Iso vs Edge Tensions) ...", 0, mnu_cmp_find_orbits_cb, 0, 0},
  {0},
  {"Optimization", 0, 0, 0, FL_SUBMENU},
  {"Pair potential (simplified)...", 0, mnu_optimize_expsimp_cb, 0, FL_MENU_DIVIDER},
  {"Minimize &Intersections...", FL_CTRL+'I', mnu_opti_intersect_cb, 0, FL_MENU_DIVIDER},
  {0},
  {"Products", 0, 0, 0, FL_SUBMENU},
  {"Cartesian (A2)...", 0, mnu_product_cb<grctk::CartesianProduct>, 0, 0},
  {"Tensor product...", 0, mnu_product_cb<grctk::TensorProduct>, 0, 0},
  {"Lexicographical (A3)...", 0, mnu_product_cb<grctk::LexicographicalProduct>, 0, 0},
  {"Strong product...", 0, mnu_product_cb<grctk::StrongProduct>, 0, 0},
  {0},
  {"Iso", 0, 0, 0, FL_SUBMENU},
  {"Test Isomorphism using CMR...", 0, mnu_iso_CMR_cb, 0, 0},
  {0},
  {"Properties", 0, 0, 0, FL_SUBMENU},
  {"&Basic properties report...", 0, mnu_basic_properties_cb, 0, 0},
  {0},
  {0},
  // {"&Help", 0, 0, 0, FL_SUBMENU},
  // {"Table of contents", FL_F+1, help_cb, (void *)1, 0},
  // {"Help on document window", FL_CTRL+FL_F+1, manual_cb, (void *)2, 0},
  // {0},
  {0}
};

}
