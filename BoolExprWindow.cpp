//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         boolexprwindow.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TBoolExprWindow (TWindow).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include "boolexprwindow.h"


//{{TBoolExprWindow Implementation}}


TBoolExprWindow::TBoolExprWindow(TWindow* parent, const char far* title, TModule* module)
:
    TGateWindow(parent, title, module)
{
  // INSERT>> Your constructor code here.

}


TBoolExprWindow::~TBoolExprWindow()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}


void TBoolExprWindow::SetFunction(const MinFunction& f, TGateWindow::DesignType, const TSwitchDoc* doc)
{
  StringVector FnName(doc->GetSystem()->GetFuncsConst()->size());
  if (!doc->MakeFunctionNames(FnName)) return;

  static char pch[2048];
  ostrstream os(pch, 2048);
  TermSet::iterator i,e;
  SymTerm::iterator is, es;
  TSwfForm form=f.GetMode();
  SymTerm sym;

  TCadRenderInfo render;
  TCadGroup* eqn;
  TCadBooleanLabel* blbl;
  TCadLabel* lbl;
  int x;

  Clear();  // Erase current design

  //
  // Create each function equation

   // Add normal text to equation
   #define ADDTEXT(str) \
          lbl=new TCadLabel(str); \
          lbl->pos=TPoint(x,0); \
          eqn->Add(lbl); \
          x += lbl->GetExtent(render).Size().cx
   // Add boolean variable to equation
   #define ADDBOOL(str) \
          blbl=new TCadBooleanLabel(str); \
          blbl->pos=TPoint(x,0); \
          eqn->Add(blbl); \
          x += blbl->GetExtent(render).Size().cx

  for(unsigned long fn=0; fn<FnName.size(); fn++) {
    x=0;
    eqn=new TCadGroup();

    // Create name and '='
    ADDBOOL(FnName[fn].c_str());
    ADDTEXT(" = ");

    // Add terms
    i=f.GetTermPtr()->begin();
    e=f.GetTermPtr()->end();
    for( ; i!=e; i++) {
      if ( (*i).funcs[fn] ) {  // Part of this function
        // Output term
        doc->GetSystem()->SymbolicTerm(sym, *i, form);  // get variables
        if (sym.size()>1) { ADDTEXT("("); }  // Enclose term in ()'s
        for(is=sym.begin(), es=sym.end(); is!=es; is++) {
          ADDBOOL((*is).c_str());  // create variable
          ADDTEXT((form==dnf) ? "*":"+");  // create operator
        }
        x -= lbl->GetExtent(render).Size().cx;
        eqn->Del(lbl);  // delete last operator

        if (sym.size()>1) {  ADDTEXT(")"); } // Enclose term in ()'s

        ADDTEXT((form==dnf) ? " + ":" * ");
      }
    }
    eqn->Del(lbl);  // Delete last operator

    // Set y position
    eqn->pos.y = eqn->GetExtent(render).Size().cy * fn;
    Add(eqn);
  }

  #undef ADDTEXT
  #undef ADDBOOL

}


