/*
  The PropEdit class.

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

#include "PropEdit.hpp"

namespace grce
{

int
PropEdit::handle(int e)
{
  switch(e)
  {
  default:
    return Fl_Window::handle(e);
  }
}

PropEdit::PropEdit(
  std::set<grctk::Object> vertexSet,
  std::set<grctk::Object> edgeSet):
  Fl_Window(400,370,"Edit properties"),
  vSet(vertexSet),
  eSet(edgeSet),
  vpropedit(NULL),
  epropedit(NULL),
  modalresult(MR_OK)
{
  set_modal();

  btn_ok = new Fl_Return_Button(50,325,100,30,"Ok");
  btn_ok->callback((Fl_Callback*)btn_ok_cb);

  btn_cancel = new Fl_Button(250,325,100,30,"Cancel");
  btn_cancel->callback((Fl_Callback*)btn_cancel_cb);

  checkbtn_altered_only = new Fl_Check_Button(90,290,210,30,"Apply altered values only");
  checkbtn_altered_only->value(1);

  tabs = new Fl_Tabs(5,5,390,285);
  if (!vSet.empty()) vpropedit = new VPropEdit(*vSet.begin());
  if (!eSet.empty()) epropedit = new EPropEdit(*eSet.begin());

  tabs->end();

  end();
  show();
}

PropEdit::~PropEdit()
{
}

void
PropEdit::btn_ok_cb(Fl_Widget *w, void *)
{
  PropEdit* PropEdit_Ptr;
  PropEdit_Ptr = (PropEdit*)(w->parent());

  for(std::set<grctk::Object>::iterator i = PropEdit_Ptr->vSet.begin();
      i != PropEdit_Ptr->vSet.end();
      ++i)
  {
    if (PropEdit_Ptr->vpropedit->colorAltered || !PropEdit_Ptr->checkbtn_altered_only->value())
      aColor[*i] = aColor[PropEdit_Ptr->vpropedit->vertexSample()];
    if (PropEdit_Ptr->vpropedit->labelAltered || !PropEdit_Ptr->checkbtn_altered_only->value())
      aLabel[*i] = aLabel[PropEdit_Ptr->vpropedit->vertexSample()];
    if (PropEdit_Ptr->vpropedit->labelVisibleAltered || !PropEdit_Ptr->checkbtn_altered_only->value())
      aLabelVisible[*i] = aLabelVisible[PropEdit_Ptr->vpropedit->vertexSample()];
    if (PropEdit_Ptr->vpropedit->weightAltered || !PropEdit_Ptr->checkbtn_altered_only->value())
      aDouble[*i] = aDouble[PropEdit_Ptr->vpropedit->vertexSample()];
    if (PropEdit_Ptr->vpropedit->intWeightAltered || !PropEdit_Ptr->checkbtn_altered_only->value())
      aInt[*i] = aInt[PropEdit_Ptr->vpropedit->vertexSample()];
  }

  for(std::set<grctk::Object>::iterator i = PropEdit_Ptr->eSet.begin();
      i != PropEdit_Ptr->eSet.end();
      ++i)
  {
    if (PropEdit_Ptr->epropedit->colorAltered || !PropEdit_Ptr->checkbtn_altered_only->value())
      aColor[*i] = aColor[PropEdit_Ptr->epropedit->edgeSample()];
    if (PropEdit_Ptr->epropedit->labelAltered || !PropEdit_Ptr->checkbtn_altered_only->value())
      aLabel[*i] = aLabel[PropEdit_Ptr->epropedit->edgeSample()];
    if (PropEdit_Ptr->epropedit->labelVisibleAltered || !PropEdit_Ptr->checkbtn_altered_only->value())
      aLabelVisible[*i] = aLabelVisible[PropEdit_Ptr->epropedit->edgeSample()];
    if (PropEdit_Ptr->epropedit->weightAltered || !PropEdit_Ptr->checkbtn_altered_only->value())
      aDouble[*i] = aDouble[PropEdit_Ptr->epropedit->edgeSample()];
    if (PropEdit_Ptr->epropedit->intWeightAltered || !PropEdit_Ptr->checkbtn_altered_only->value())
      aInt[*i] = aInt[PropEdit_Ptr->epropedit->edgeSample()];
  }

  PropEdit_Ptr->modalresult = MR_OK;
  PropEdit_Ptr->hide();
}

void
PropEdit::btn_cancel_cb(Fl_Widget *w, void *)
{
  PropEdit* PropEdit_Ptr;
  PropEdit_Ptr = (PropEdit*)(w->parent());

  PropEdit_Ptr->modalresult = MR_CANCEL;
  PropEdit_Ptr->hide();
}

PropEdit::ModalResult
PropEdit::modalResult() const
{
  return modalresult;
}

}
