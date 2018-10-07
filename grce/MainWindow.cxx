/*
  The MainWindow class.

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

#include "MainWindow.hpp"
#include "grce/Wizard.hpp"
#include "grce/SelectOneDlg.hpp"
#include "grce/Saver.hpp"
#include "grce/Loader.hpp"
#include "grce/algo/color/ResetColors.hpp"
#include <grctk/algo/generation/GenComplete.hpp>
#include <grctk/algo/generation/GenRandom.hpp>
#include <grctk/algo/drawing/random/RandomizePositions.hpp>
#include <grctk/algo/products/CartesianProduct.hpp>
#include <grctk/algo/products/TensorProduct.hpp>
#include <grctk/algo/products/LexicographicalProduct.hpp>
#include <grctk/algo/products/StrongProduct.hpp>
#include <grctk/algo/formats/Environment.hpp>
#include <grctk/algo/formats/LinkTable.hpp>
#include <grctk/algo/formats/BinCodeFile.hpp>

namespace grce
{

MainWindow::MainWindow(const char *title)
  :Fl_Window(0,0,250,297,title), menubar(0,0,250,28)
{
  menubar.menu(menuitems);
  menubar.callback(menu_placeholder_test_cb);

  docControl = new DocControl(0,30,249,265);
  end();
}

MainWindow::~MainWindow()
{
  exit(0);
}

int MainWindow::handle(int e)
{
  switch(e)
  {
  case FL_HIDE:
    if (docControl->listItems().size() == 0)
      quit();
    break;
  }
  return Fl_Window::handle(e);
}

void MainWindow::new_graph_cb(Fl_Widget *w, void *)
{
  MainWindow* MainWindow_Ptr;
  MainWindow_Ptr = (MainWindow*)(w->parent());
  MainWindow_Ptr->docControl->createNew();
}

void MainWindow::load_env_cb(Fl_Widget *w, void *)
{
  MainWindow* MainWindow_Ptr;
  MainWindow_Ptr = (MainWindow*)(w->parent());

  if (MainWindow_Ptr->docControl->listItems().size())
    if (fl_choice(
          "The current environment will be replaced by the one being loaded. Continue",
          "No","Yes",NULL) != 1)
      return;

  MainWindow_Ptr->docControl->closeall();

  Loader loader(grctk::Universe::singleton(),
                MainWindow_Ptr->docControl);
  loader.show();
  while (loader.shown())
    Fl::wait();
  // if (!loader())
  //   return;
}

void MainWindow::append_env_cb(Fl_Widget *w, void *)
{
  MainWindow* MainWindow_Ptr;
  MainWindow_Ptr = (MainWindow*)(w->parent());

  std::string universeNameBackup = MainWindow_Ptr->docControl->universeName();

  Loader loader(grctk::Universe::singleton(),
                MainWindow_Ptr->docControl);
  loader.show();
  while (loader.shown())
    Fl::wait();
  // if (!loader())
  //   return;

  MainWindow_Ptr->docControl->universeName(universeNameBackup);
}

void MainWindow::save_env_cb(Fl_Widget *w, void *)
{
  MainWindow* MainWindow_Ptr;
  MainWindow_Ptr = (MainWindow*)(w->parent());

  Saver saver(grctk::Universe::singleton(),
              MainWindow_Ptr->docControl->listItems(),
              MainWindow_Ptr->docControl->universeName(),
              MainWindow_Ptr->docControl->universeDirectory());
  saver.show();
  while (saver.shown())
    Fl::wait();
  if (saver())
  {
    MainWindow_Ptr->docControl->universeName(saver.universeName());
    MainWindow_Ptr->docControl->universeDirectory(saver.universeDirectory());
  }
}

void MainWindow::save_env_as_cb(Fl_Widget *w, void *)
{
  MainWindow* MainWindow_Ptr;
  MainWindow_Ptr = (MainWindow*)(w->parent());

  Saver saver(grctk::Universe::singleton(),
              MainWindow_Ptr->docControl->listItems(),
              std::string("copy-of-") + MainWindow_Ptr->docControl->universeName(),
              MainWindow_Ptr->docControl->universeDirectory());
  saver.show();
  while (saver.shown())
    Fl::wait();
  if (saver())
  {
    MainWindow_Ptr->docControl->universeName(saver.universeName());
    MainWindow_Ptr->docControl->universeDirectory(saver.universeDirectory());
  }
}

void MainWindow::import_cb(Fl_Widget *w, void *)
{
  MainWindow* MainWindow_Ptr;
  MainWindow_Ptr = (MainWindow*)(w->parent());
  char *filename = fl_file_chooser
    (
      "Choose a file to open...",
      "Link tables (*)",
      0,0
      );
  if (filename && (!fl_filename_isdir(filename)))
  {
    try
    {
      std::ifstream fi(filename);
      REQUIRE(fi);

      std::vector<std::string> list;
      char stmp[102];

      int j = 0,i,n = 0;
      while (!fi.eof())
      {
        fi >> stmp;
        if (stmp[0] == '#')
          list.push_back(std::string(stmp));
        n++;
      }
      n--;

      SelectDlg* dlg = new SelectDlg(list);

      dlg->show();
      while(dlg->shown())
      {
        Fl::wait();
      }

      if (dlg->modalResult() != SelectDlg::MR_OK)
      {
        fi.close();
        throw std::runtime_error("Error choosing graph to import");
      }

      i = dlg->value();

      delete dlg;

      fi.close();

      std::ifstream fi1(filename);

      j = 0;
      while (j < i)
      {
        fi1 >> stmp;
        if (stmp[0] == '#')  j++;
      }

      grctk::LinkTable algEI;
      grctk::AdjMatrix g;
      algEI.importFrom(g,fi1);

      fi1.close();

      grctk::RandomizePositions algRandPos;
      algRandPos(g,a3D);

      grce::ResetColors algResetColors;
      algResetColors(g,aColor);

      std::ostringstream oss;
      oss << "-" << i;
      DocWindow* doc_window =
        new DocWindow(0,0,700,550,
                      yaatk::extractLastItem(filename) + oss.str(),
                      MainWindow_Ptr->docControl);
      doc_window->LoadFromGraph(g);
      MainWindow_Ptr->docControl->add_doc_item(doc_window);
    }
    catch(...)
    {
      fl_alert("Error importing the graph");
    }
  }
}

void MainWindow::import_bic_cb(Fl_Widget *w, void *)
{
  MainWindow* MainWindow_Ptr;
  MainWindow_Ptr = (MainWindow*)(w->parent());
  char *filename = fl_file_chooser
    (
      "Choose a file to open...",
      "BinCode Files (*.R)",
      0,0
      );
  if (filename && (!fl_filename_isdir(filename)))
  {
    try
    {
      grctk::BinCodeFileReader bicfile(filename);

      int i;
      unsigned long ng = bicfile.numberOfGraphs();
      {
        std::ostringstream msg;
        msg << "Input graph index [0," << ng << ")";
        const char *rval = fl_input("%s","0",msg.str().c_str());
        if (rval == NULL)
          throw std::runtime_error("Error choosing graph");
        i = atoi(rval);
        if (i >= 0 && i < ng)
          std::cout << "Loading graph number " << i << std::endl;
        else
          throw std::runtime_error("Error choosing graph");
      }

      grctk::AdjMatrix gbic;
      gbic.cloneFrom(bicfile.getGraph(i));

      grctk::RandomizePositions algRandPos;
      algRandPos(gbic,a3D);

      grce::ResetColors algResetColors;
      algResetColors(gbic,aColor);

      std::ostringstream oss;
      oss << "-" << i;
      DocWindow* doc_window =
        new DocWindow(0,0,700,550,
                      yaatk::extractLastItem(filename) + oss.str(),
                      MainWindow_Ptr->docControl);
      doc_window->LoadFromGraph(gbic);
      MainWindow_Ptr->docControl->add_doc_item(doc_window);
    }
    catch(...)
    {
      fl_alert("Error importing the graph");
    }
  }
}

void MainWindow::quit()
{
  docControl->toolbox->hide();
  docControl->closeall();
  hide();
//  exit(0);
}

void MainWindow::quit_cb(Fl_Widget* w, void*)
{
  MainWindow* MainWindow_Ptr;
  MainWindow_Ptr = (MainWindow*)(w->parent());

  if (MainWindow_Ptr->docControl->listItems().size())
  {
    int choice = fl_choice(
      "Save the current environment?",
      "No","Yes","Do not quit now");
    if (choice == 1)
      save_env_cb(w, 0);
    else if (choice == 2)
      return;
  }

  MainWindow_Ptr->quit();
}

template <typename ProductAlg>
void MainWindow::product_cb(Fl_Widget *w, void *)
{
  DocControl* docControl = ((MainWindow*)(w->parent()))->docControl;

  std::pair<DocWindow*,DocWindow*> docWindowPair;
  try { docWindowPair = docControl->chooseTwoDocs(); }
  catch(...) { return; }
  docControl->multiply<ProductAlg>(docWindowPair.first, docWindowPair.second);
}

void MainWindow::iso_CMR_cb(Fl_Widget *w, void *)
{
  DocControl* docControl = ((MainWindow*)(w->parent()))->docControl;
  std::pair<DocWindow*,DocWindow*> docWindowPair;
  try { docWindowPair = docControl->chooseTwoDocs(); }
  catch(...) { return; }
  docControl->testIsoCMR(docWindowPair.first, docWindowPair.second);
}

void MainWindow::iso_opti_cb(Fl_Widget *w, void *)
{
}

void MainWindow::gen_complete_cb(Fl_Widget *w, void *)
{
  DocControl* docControl = ((MainWindow*)(w->parent()))->docControl;

  std::vector<ParamsTab> paramsTabs;
  paramsTabs.push_back(ParamsTab("Main parameters",std::vector<BaseParam*>()));
  IntegerParam vc(5,"Vertex count");
  paramsTabs[0].second.push_back(&vc);
  ParamsDialog params("Set parameters for the new graph", paramsTabs);
  params.show();
  while (params.shown())
    Fl::wait();
  if (!params())
    return;

  LogExecutor logger;
  grctk::AdjMatrix g;
  typedef AlgOneParam<grctk::GenComplete, const size_t, grctk::AdjMatrix> R;
  R* r = new R(vc.value(),g,logger);
  SimpleWizard wiz(r,"Generate a complete graph");

  if (logger() && wiz())
  {
    std::ostringstream ossTitle;
    ossTitle << "K_" << vc.value();
    DocWindow* docWindow = docControl->createNewFromGraph(g,ossTitle.str());
    typedef AlgTwoParamsNoRet<
      grctk::RandomizePositions,
      const grctk::AdjMatrix,
      grctk::Attribute<yaatk::Vector3D>& > RPlace;
    RPlace* rPlace = new RPlace(
      docWindow->EditBox()->graphAsAdjMatrix(),
      a3D, logger);
    SimpleWizard wizPlace(rPlace,"Randomizing vertices positions");
    if (logger() && wizPlace())
    {
      grce::ResetColors algResetColors;
      algResetColors(rPlace->arg1,aColor);

      docWindow->EditBox()->UpdateAll();
      docWindow->EditBox()->redraw();
    }
  }
}

void MainWindow::gen_random_cb(Fl_Widget *w, void *)
{
  DocControl* docControl = ((MainWindow*)(w->parent()))->docControl;

  std::vector<ParamsTab> paramsTabs;
  paramsTabs.push_back(ParamsTab("Main parameters",std::vector<BaseParam*>()));
  IntegerParam vc(5,"Vertex count");
  paramsTabs[0].second.push_back(&vc);
  IntegerParam ec(4,"Edge count");
  paramsTabs[0].second.push_back(&ec);
  CheckboxParam connectedOnly(true,"Connected only");
  paramsTabs[0].second.push_back(&connectedOnly);
  ParamsDialog params("Set parameters for the new graph", paramsTabs);
  params.show();
  while (params.shown())
    Fl::wait();
  if (!params())
    return;

  LogExecutor logger;
  typedef AlgThreeParams<grctk::GenRandom,
                         const size_t, const size_t, const bool,
                         grctk::AdjMatrix> R;
  grctk::AdjMatrix g;
  R* r = new R(vc.value(),ec.value(),connectedOnly.value(),
               g,logger);
  SimpleWizard wiz(r,"Generate a random graph");

  if (logger() && wiz())
  {
    std::ostringstream ossTitle;
    ossTitle << "Rand_" << vc.value() << "_e" << ec.value();
    DocWindow* docWindow = docControl->createNewFromGraph(g,ossTitle.str());
    typedef AlgTwoParamsNoRet<
      grctk::RandomizePositions,
      const grctk::AdjMatrix,
      grctk::Attribute<yaatk::Vector3D>& > RPlace;
    RPlace* rPlace = new RPlace(
      docWindow->EditBox()->graphAsAdjMatrix(),
      a3D, logger);
    SimpleWizard wizPlace(rPlace,"Randomizing vertices positions");
    if (logger() && wizPlace())
    {
      grce::ResetColors algResetColors;
      algResetColors(rPlace->arg1,aColor);

      docWindow->EditBox()->UpdateAll();
      docWindow->EditBox()->redraw();
    }
  }
}

void MainWindow::menu_placeholder_test_cb(Fl_Widget* w, void*)
{
  Fl_Menu_* mw = (Fl_Menu_*)w;
  const Fl_Menu_Item* m = mw->mvalue();
  if (m->submenu())
    return;
  if (!m)
    fl_message("NULL\n Not Implemented yet :-( \n");
  else if (m->shortcut())
    fl_message("%s - %s\n Not Implemented yet :-( \n",
               m->label(), fl_shortcut_label(m->shortcut()));
  else
    fl_message("%s\n Not Implemented yet :-( \n", m->label());
}

void MainWindow::about_cb(Fl_Widget *, void *)
{
  const std::string APP_NAME = "GRCE";
  const std::string APP_VERSION = "0.5.0.1 (2018.10.07)";
  const std::string APP_COPYRIGHT = "2001-2018 Alexander Yermolenko";
  const std::string APP_MAILTO = "mailto:yaa.mbox@@gmail.com";
  const std::string APP_NOTES = "";

  fl_message("%s",
    (APP_NAME + " v" + APP_VERSION +
    "\n\nCopyright (C) " + APP_COPYRIGHT +
    "\n\t\t" + APP_MAILTO +
    "\n\n" + APP_NOTES).c_str());
}

Fl_Menu_Item MainWindow::menuitems[] = {
  { "&File", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0 },
  { "&New Graph", FL_CTRL + 'N', (Fl_Callback *)new_graph_cb, 0, FL_MENU_DIVIDER, 0, 0, 0, 0 },
  { "L&oad Environment...", FL_CTRL + 'O', (Fl_Callback *)load_env_cb, 0, 0, 0, 0, 0, 0 },
  { "&Append/Merge Environment...", 0, (Fl_Callback *)append_env_cb, 0, FL_MENU_DIVIDER, 0, 0, 0, 0 },
  { "&Save Environment...", FL_CTRL + 'S', (Fl_Callback *)save_env_cb, 0, 0, 0, 0, 0, 0 },
  { "Save Environment As...", 0, (Fl_Callback *)save_env_as_cb, 0, FL_MENU_DIVIDER, 0, 0, 0, 0 },
  { "&Import from linktable...", FL_CTRL + 'I', (Fl_Callback *)import_cb, 0, 0, 0, 0, 0, 0 },
  { "Import from &R-Bincode...", 0, (Fl_Callback *)import_bic_cb, 0, FL_MENU_DIVIDER, 0, 0, 0, 0 },
  { "E&xit", FL_CTRL + 'x', (Fl_Callback *)quit_cb, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { "&Algorithms", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0 },
  { "Isomorphism", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0 },
  { "CMR...", 0, (Fl_Callback *)iso_CMR_cb, 0, 0, 0, 0, 0, 0 },
  // { "Opti", 0, (Fl_Callback *)iso_opti_cb, 0, 0, 0, 0, 0, 0 },
  {0},
  { "Generate", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0 },
  { "Generate complete graph...", FL_CTRL + 'C', (Fl_Callback *)gen_complete_cb, 0, 0, 0, 0, 0, 0 },
  { "Generate random graph...", FL_CTRL + 'R', (Fl_Callback *)gen_random_cb, 0, 0, 0, 0, 0, 0 },
  {0},
  { "Products", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0 },
  { "Cartesian (A2)...", 0, (Fl_Callback *)product_cb<grctk::CartesianProduct>, 0, 0, 0, 0, 0, 0 },
  { "Tensor product...", 0, (Fl_Callback *)product_cb<grctk::TensorProduct>, 0, 0, 0, 0, 0, 0 },
  { "Lexicographical (A3)...", 0, (Fl_Callback *)product_cb<grctk::LexicographicalProduct>, 0, 0, 0, 0, 0, 0 },
  { "Strong product...", 0, (Fl_Callback *)product_cb<grctk::StrongProduct>, 0, 0, 0, 0, 0, 0 },
  {0},
  { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  {"&Help", 0, 0, 0, FL_SUBMENU, 0, 0, 0, 0},
  {"About GRCE...", 0, about_cb, 0, 0, 0, 0, 0, 0},
  // {"Table of contents...", FL_F+1, help_cb, 0, 0, 0, 0, 0, 0},
  // {"Help on main window...", FL_CTRL+FL_F+1, manual_cb, 0, 0, 0, 0, 0, 0},
  { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

}
