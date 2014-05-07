//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         gatewindow.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TGateWindow (TWindow).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include "gatewindow.h"

using namespace swmin;

//{{TGateWindow Implementation}}


TGateWindow::TGateWindow(TWindow* parent, const char far* title, TModule* module)
:
    TCadWindow(parent, title, module)
{
  // Override the default window style for TWindow.
  //
      Attr.Style |= WS_CHILD | WS_VISIBLE;

  // INSERT>> Your constructor code here.

}


TGateWindow::~TGateWindow()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}

void TGateWindow::SetFunction(const MinFunction& f, TGateWindow::DesignType design, const TSwitchDoc* doc)
{
  Clear();

  if (design==BoolExpr) {
    CreateBoolExpr(f, doc);
    return;
  }

  unsigned long outputs = doc->GetSystem()->GetFuncsConst()->size();

  BuildDesignInfo info;
  info.f = &f;
  info.design=design;
  info.doc=doc;
  info.FnTermCount.insert(info.FnTermCount.end(), outputs, 0);

  LockUpdateBegin();

  BuildTermGates(info);
  BuildFnGates(info);

  LockUpdateEnd();
}

void TGateWindow::BuildFnGates(TGateWindow::BuildDesignInfo& z)
{
  unsigned long outputs = z.doc->GetSystem()->GetFuncsConst()->size();
  TSwfForm form=z.f->GetMode();

  TPoint pFnGate,  // point for next function gate
         p1, p2;   // misc points
  int xWire;  // x-position for vertical part of wire
  const int WireSpacing = 5;  // spacing between vertical wires of each function

  TCadLogicGateBase* gate;
  TCadBooleanLabel* blbl;
  TCadWire* wire;
  TCadRenderInfo render;
  TSize bSize, gSize;

  TermGateMap::iterator i,e;

  // Get names of output functions
  StringVector FnName(outputs);
  if (!z.doc->MakeFunctionNames(FnName)) return;

  // Find number of wires to build
  int wires=z.TermGates.size();
  //for(i=z.TermGates.begin(), e=z.TermGates.end(); i!=e; i++)
  // wires += (*i).first.funcs.index();

  // Calculate starting wire x position and function gate position
  xWire=(*(z.TermGates.begin())).second.gate->OutputPin(render).x+WireSpacing;
  pFnGate=TPoint( xWire + (wires+2)*WireSpacing, 0);

  // For each function
  for(unsigned long fn=0; fn<outputs; fn++) {
    unsigned long terms=z.FnTermCount[fn];
    if (terms==0) continue;

    //
    // Create gate
    switch (z.design) {
      case ANDOR:
        if (form==dnf) gate=new TCadLogicGateOR("", terms);
        else gate=new TCadLogicGateAND("", terms);
        break;
      case NAND:
        if (terms>1) gate=new TCadLogicGateNAND("", terms);
        else gate=new TCadLogicGateINV("");
        break;
      case NOR:
        if (terms>1) gate=new TCadLogicGateNOR("", terms);
        else gate=new TCadLogicGateINV("");
        break;
    }
    gate->pos=pFnGate;
    gSize=gate->GetExtent(render).Size();
    Add(gate);

    //
    // Create label & inverter for non-native designs
    blbl=new TCadBooleanLabel(FnName[fn]);
    bSize=blbl->GetExtent(render).Size();
    // If not native gate, create inverter and set output pin point
    if ( ((form==dnf) && (z.design==NOR)) || ((form==cnf) && (z.design==NAND)) ) {
      TCadLogicGateINV* inv=new TCadLogicGateINV("");  // create inverter
      p1=gate->OutputPin(render);  // get output pin of gate
      inv->pos=TPoint(p1.x, p1.y - inv->InputPin(render, 0).y);  // attach inv to output
      p2=inv->OutputPin(render);
      Add(inv);
    }
    else p2=gate->OutputPin(render);  // set output pin point to gate output
    // Attach label
    blbl->pos=TPoint(p2.x, p2.y - (bSize.cy/2));  // center at output pin
    Add(blbl);

    //
    // Create wires
    unsigned long outInput=0;
    for(i=z.TermGates.begin(), e=z.TermGates.end(); i!=e; i++) {
      // create wire from output pin to input pin of function
      if ( ! (*i).first.funcs[fn] ) continue;  // term is not in this function
      TermDrawInfo di= (*i).second;

      p1=di.gate->OutputPin(render);  // point at term output
      p2=gate->InputPin(render, outInput);  // input at function gate

      wire=new TCadWire();
      wire->Add(p2, false);  // point at input of function

      if ( di.drawn ) { // already drawn, add to existing wire
        wire->Add(TPoint(di.x, p2.y), true);  // add horz wire
        if (p2.y > di.ymax) {  // add wire above existing vertical
          wire->Add(TPoint(di.x, di.ymax), false);
          (*i).second.ymax=p2.y;
        }
        else if (p2.y < di.ymin) { // add wire below existing vertical
          wire->Add(TPoint(di.x, di.ymin), false);
          (*i).second.ymin=p2.y;
        }
      }

      else {  // add first wire to term
        bool dot=((*i).first.funcs.index()>1);
        //if (p1.y != p2.y) {  // need to move across and vertical
          wire->Add(TPoint(xWire, p2.y), dot);  // move across from function
          wire->Add(TPoint(xWire, p1.y), dot);  // move across from term
          if (p1.y > p2.y) { (*i).second.ymin=p2.y; (*i).second.ymax=p1.y; }
          else { (*i).second.ymin=p1.y; (*i).second.ymax=p2.y; }
        //}
        //else (*i).second.ymax=(*i).second.ymin=p1.y;
        wire->Add(p1, false/*( dot && (p1.y==p2.y) )*/ );  // point at output of term
        (*i).second.x=xWire;
        (*i).second.drawn=true;
        xWire += WireSpacing;
      }

      Add(wire);
      outInput++;
    }

    pFnGate.y += gate->GetExtent(render).Size().cy * 2;
  }
}

void TGateWindow::BuildTermGates(TGateWindow::BuildDesignInfo& z)
{
  unsigned long outputs = z.doc->GetSystem()->GetFuncsConst()->size();

  TermSet::iterator i,e;
  TSwfForm form=z.f->GetMode();
  SymTerm sym;

  // CAD variables
  TCadRenderInfo render;
  TCadLogicGateBase* gate;

  //
  // Build gates for all terms
  int y=0;

  i=z.f->GetTermPtr()->begin();
  e=z.f->GetTermPtr()->end();
  for( ; i!=e; i++) {
    if ( (*i).funcs.index() == 0) continue;  // no functions use this term ???
    // Count term in functions it's in
    for(unsigned long j=0; j<outputs; j++) if ((*i).funcs[j]) z.FnTermCount[j]++;
    // get variables
    z.doc->GetSystem()->SymbolicTerm(sym, *i, form);
    // Create gate
    gate=MakeTermGate(sym, z.design, form, y);
    // Add to term->gate map
    TermDrawInfo drawinfo;
    drawinfo.gate=gate;
    drawinfo.drawn=false;
    z.TermGates.insert(TermGateMap::value_type(*i, drawinfo));
    // Increment y for next gate
    y += gate->GetExtent(render).Size().cy * 1.5;
  }
}

TCadLogicGateBase* TGateWindow::MakeTermGate(SymTerm& sym, TGateWindow::DesignType design, TSwfForm form, int y)
{
  TCadLogicGateBase* gate;
  SymTerm::iterator is,es;
  bool native;
  string var;

  native= !( ((form==dnf) && (design==NOR)) || ((form==cnf) && (design==NAND)) );

  // Create gate
  switch (design) {
    case ANDOR:
      if (form==dnf) gate=new TCadLogicGateAND("", sym.size());
      else gate=new TCadLogicGateOR("", sym.size());
      break;
    case NAND:
      if (sym.size()>1) gate=new TCadLogicGateNAND("", sym.size());
      else gate=new TCadLogicGateINV("");
      break;
    case NOR:
      if (sym.size()>1) gate=new TCadLogicGateNOR("", sym.size());
      else gate=new TCadLogicGateINV("");
      break;
    default: return NULL;
  }
  gate->pos=TPoint(0,y);
  Add(gate);

  // Add variable labels to left of output pins
  int i;
  TPoint p;
  TSize sz;
  TCadRenderInfo render;
  TCadBooleanLabel* blbl;

  for(is=sym.begin(), es=sym.end(), i=0; is!=es; is++, i++) {
    // create variable label
    var=*is;
    if (!native) {
      if (var[0]=='~') var.remove(0,1);  // remove complement
      else var.prepend("~");  // add complement
    }

    // create CAD boolean label object
    p=gate->InputPin(render, i);  // Get point of input pin
    blbl=new TCadBooleanLabel(var);  // create new boolean label
    sz=blbl->GetExtent(render).Size();
    blbl->pos=TPoint(p.x - sz.cx, p.y-(sz.cy/2));  // right justify
    Add(blbl);
  }

  return gate;
}

void TGateWindow::CreateBoolExpr(const MinFunction& f, const TSwitchDoc* doc)
{
  StringVector FnName(doc->GetSystem()->GetFuncsConst()->size());
  if (!doc->MakeFunctionNames(FnName)) return;

  TermSet::iterator i,e;
  SymTerm::iterator is, es;
  TSwfForm form=f.GetMode();
  SymTerm sym;

  TCadRenderInfo render;
  TCadGroup* eqn;
  TCadBooleanLabel* blbl;
  TCadLabel* lbl;
  int x;

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

  LockUpdateBegin();

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

  LockUpdateEnd();

  #undef ADDTEXT
  #undef ADDBOOL

}


