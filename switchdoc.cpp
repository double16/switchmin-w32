//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         switchdoc.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TSwitchDoc (TDocument).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include <owl/dibitmap.h>

#include "switchdoc.h"
#include "calccoveragedialog.h"
#include "minfnbuilddialog.h"
#include "stringvector.h"

//{{TSwitchDoc Implementation}}


TSwitchDoc::TSwitchDoc(TDocument* parent)
:
    TDocument(parent)
{
  // INSERT>> Your constructor code here.
  Trademark=NULL;
}


TSwitchDoc::~TSwitchDoc()
{
  // INSERT>> Your destructor code here.
  delete Trademark;
}

TPoint TSwitchDoc::PaintHeader(TDC& dc, const TRect& rect, const string& title, uint page, uint pages, bool paint)
{
  TPoint ptNext(rect.TopLeft()), ptTextNext;

  //
  // Output SwitchMin trademark
  if (!Trademark) Trademark=new TPrintableDIB(GetDocManager().GetApplication(), TResId(IDB_PRINTHEADER));

  if (paint)
    dc.BitBlt(TRect(rect.TopLeft(), Trademark->BMP()->Size()), *Trademark->DC(), TPoint(0,0), SRCCOPY);

  ptNext.y += Trademark->DIB()->Height()+5;

  //
  // Output page count
  char pch[256];
  ostrstream os(pch, 256);

  if (paint) {
    os.seekp(0);
    os << "Page " << page << " of " << pages << "  " << ends;
    TPoint p(rect.right - dc.GetTextExtent(pch, strlen(pch)).cx, rect.top);
    dc.TextOut(p, pch);
  }

  //
  // Output title of printout
  TEXTMETRIC metric;
  TFont((HFONT) dc.GetCurrentObject(OBJ_FONT)).GetTextMetrics(metric, dc);
  int nextline=metric.tmHeight+metric.tmExternalLeading;
  ptTextNext=TPoint(Trademark->DIB()->Width()+5, rect.top);

  if (paint) dc.TextOut(ptTextNext, title.c_str());
  ptTextNext.y += nextline;
  if (paint) dc.TextOut(ptTextNext, SwSys.desc.c_str());
  ptTextNext.y += nextline;

  // Figure out next valid drawing point
  if (ptTextNext.y>ptNext.y) ptNext.y=ptTextNext.y;
  return ptNext;
}

bool TSwitchDoc::AddFunction(swmin::Function& f, TView* view)
{
  if (!SwSys.ins(f)) return false;
  DirtyFlag=true;
  NotifyViews(vnFuncSpecChanged, 0, view);
  return true;
}

bool TSwitchDoc::DelFunction(swmin::Function& f, TView* view)
{
  if (SwSys.pFuncs()->erase(f)==0) return false;
  DirtyFlag=true;
  NotifyViews(vnFuncSpecChanged, 0, view);
  return true;
}

bool TSwitchDoc::ReplaceFunction(swmin::Function& f, TView* view)
{
  if (SwSys.pFuncs()->erase(f)==0) return false;
  return AddFunction(f, view);
}

bool TSwitchDoc::SetDesc(const string& _d, TView*)
{
  SwSys.desc=_d;
  DirtyFlag=true;
  return true;
}

bool TSwitchDoc::SetInputs(unsigned long _i, TView* view)
{
  SwSys.SetInputs(_i);
  DirtyFlag=true;
  NotifyViews(vnFuncSpecChanged, 0, view);
  return true;
}

bool TSwitchDoc::SetInputs(const swmin::InputVector& iv, TView* view)
{
  SwSys.InputNames=iv;
  DirtyFlag=true;
  return SetInputs(iv.size(), view);
}


bool TSwitchDoc::SetHazardFree(bool _h, TView* view)
{
  SwSys.SetHazardFree(_h);
  DirtyFlag=true;
  NotifyViews(vnMinChanged, 0, view);
  return true;
}

bool TSwitchDoc::MakeCoverageTerms(swmin::SwSystemCoverageDisp* disp, TView* view)
{
  bool Updated=!SwSys.CoverageTermsValid;
  SwSys.MakeCoverageTerms(disp);
  if (Updated) {
    //DirtyFlag=true;
    NotifyViews(vnCoverageTermsChanged, 0, view);
  }
  return true;
}

const swmin::TermSet& TSwitchDoc::GetCoverageTerms(swmin::TSwfForm mode, TView* view)
{
  if (!SwSys.CoverageTermsValid) {
    TCalcCoverageDialog dlg(GetDocManager().GetApplication()->GetMainWindow());
    dlg.Create();
    MakeCoverageTerms( (SwSystemCoverageDisp*) &dlg, view);
  }
  return SwSys.CoverageTerms[mode];
}

bool TSwitchDoc::MakeMinFn(swmin::TSwfCost cost, swmin::TSwfForm form, swmin::SwSystemMinDisp* disp, TView* view)
{
  if (!SwSys.MinFnValid(cost, form)) {  // need to build
    GetCoverageTerms(swmin::dnf);  // Make primes, uses a display unlike SwSystem make
    if (!disp) {  // If no display given, create one
      TMinFnBuildDialog dlg(GetDocManager().GetApplication()->GetMainWindow());
      dlg.Create();
      disp=(SwSystemMinDisp*) &dlg;
      //TMinFnBuildDialog* dlg= new TMinFnBuildDialog(GetDocManager().GetApplication()->GetMainWindow());
      //dlg->Create();
      //disp=(SwSystemMinDisp*) dlg;
    }
    SwSys.MakeMinFn(cost, form, disp);
    //DirtyFlag=true;
    NotifyViews(vnMinChanged, 0, view);
  }
  return true;
}

const swmin::MinFunctionSet& TSwitchDoc::GetMinFn(TView*)
{
  return SwSys.MinFuncs;
}


bool TSwitchDoc::GetFuncFromID(string& id, swmin::Function& f) const
{
  return SwSys.GetFuncFromID(id, f);
}

bool TSwitchDoc::MakeFunctionNames(StringVector& fname) const
{
  fname.reserve(SwSys.GetFuncsConst()->size());
  swmin::FunctionSet::const_iterator i=SwSys.GetFuncsConst()->begin(), e=SwSys.GetFuncsConst()->end();
  StringVector::size_type s=0;
  while (i!=e) {
    fname[s++]=(*i).id;
    i++;
  }
  return true;
}


bool TSwitchDoc::Open(int /*mode*/, const char far* path)
{
  if (path) SetDocPath(path);
  if (GetDocPath()) {
    ifpstream is(GetDocPath());
    if (!is) return false;

    //OpenFlag=true;
    is >> SwSys;
  }
  SetDirty(false);
  return true;
}


bool TSwitchDoc::Commit(bool force)
{
  // INSERT>> Your code here.
  if (!TDocument::Commit(force)) return false;

  if (!IsDirty() && !force) return true;

  if (GetDocPath()) {
    ofpstream os(GetDocPath());
    if (!os) return false;
    os << SwSys;
  }

  SetDirty(false);
  return true;
}


bool TSwitchDoc::Revert(bool clear)
{
  if (!TDocument::Revert(clear)) return false;
  if (!clear) Open(0);
  return true;
}

bool TSwitchDoc::MinViewActive()
{
  return QueryViews(vnAreYouMinView, 0)!=NULL;
}

