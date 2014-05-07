//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         primesview.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TPrimesView (TWindowView).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include "primesview.h"
#include "switchminapp.h"
#include "stringvector.h"

//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TPrimesView, TWindowView)
//{{TPrimesViewRSP_TBL_BEGIN}}
  EV_WM_SIZE,
  EV_WM_GETMINMAXINFO,
  EV_COMMAND(CM_PRIMESUPDATE, CmPrimesUpdate),
  EV_COMMAND_ENABLE(CM_PRIMESUPDATE, CePrimesUpdate),
//{{TPrimesViewRSP_TBL_END}}
  EV_BN_CLICKED(IDHELP, Help_BNClicked),
  EV_BN_CLICKED(IDCANCEL, Close_BNClicked),
  EV_VN_FUNCSPECCHANGED,
  EV_VN_COVERAGETERMSCHANGED,
  EV_VN_REVERT,
  EV_WM_HELP,
END_RESPONSE_TABLE;

using namespace swmin;

//{{TPrimesView Implementation}}

const char* TPrimesView::StaticName() { return "Coverage Terms"; }

TPrimesView::TPrimesView(TSwitchDoc& doc, TWindow* parent)
:
    TWindowView(doc, parent)
{
  // INSERT>> Your constructor code here.
  Attr.ExStyle |= WS_EX_CONTEXTHELP;
  
  swdoc=&doc;
  SetBkgndColor(GetSysColor(COLOR_3DFACE));

  wTabs=new TPropertyWindow(this, 0, TShouldDelete::Delete);
  wTabs->SetButtons(TPropertyWindow::BtnHelp | TPropertyWindow::BtnCanClose);

  SetViewMenu(new TMenuDescr(IDM_PRIMEVIEW));
  PrimesInvalid=true;
}


TPrimesView::~TPrimesView()
{
  // INSERT>> Your destructor code here.
  delete wTabs;
}

void TPrimesView::EvHelp(HELPINFO* pHelpInfo)
{

}

void TPrimesView::Help_BNClicked()
{
  ((TSwitchMinApp*) GetApplication())->CallHelp(HELP_CONTEXT, IDH_vw_TERMS_MAIN);
}

void TPrimesView::Close_BNClicked()
{
  PostMessage(WM_CLOSE);
}

void TPrimesView::SetupWindow()
{
  TWindowView::SetupWindow();

  // INSERT>> Your code here.
  register TImageList* img = ((TSwitchMinApp*) GetApplication())->imglQMTerms;
  wTabs->SetImageList(*img);
  wTabs->SetCanClose_Close();

  wTermsDNF=new TTermViewDialog(this);
  wTermsCNF=new TTermViewDialog(this);

  wTabs->AddPage(wTermsDNF, "DNF", 0);
  wTabs->AddPage(wTermsCNF, "CNF", 1);

  CmPrimesUpdate();
}


void TPrimesView::EvSize(uint sizeType, TSize& size)
{
    TWindowView::EvSize(sizeType, size);

  // INSERT>> Your code here.
  uint showflag = PrimesInvalid ? SWP_HIDEWINDOW:SWP_SHOWWINDOW;
  wTabs->SetWindowPos(0, TRect(TPoint(0,0), size), showflag | SWP_NOZORDER);
}


void TPrimesView::Paint(TDC& dc, bool erase, TRect& rect)
{
  TSwitchMinApp* theApp = TYPESAFE_DOWNCAST(GetApplication(), TSwitchMinApp);
  if (theApp) {
    // Only paint if we're printing and we have something to paint, otherwise do nothing.
    //
    if (theApp->Printing && theApp->Printer && !rect.IsEmpty()) {
      // Use pageSize to get the size of the window to render into.  For a Window it's the client area,
      // for a printer it's the printer DC dimensions and for print preview it's the layout window.
      //
      TSize   pageSize(rect.right - rect.left, rect.bottom - rect.top);

      TPrintDialog::TData& printerData = theApp->Printer->GetSetup();

      // Compute the number of pages to print.
      //
      printerData.MinPage = 1;
      printerData.MaxPage = 1;

      // INSERT>> Special printing code goes here.
      PaintHeader(dc, rect);
    }
    else {
      // INSERT>> Normal painting code goes here.
      if (!PrimesInvalid) wTabs->InvalidateRect(rect, erase);
    }
  }


}

TPoint TPrimesView::PaintHeader(TDC& dc, TRect& rect)
{
  TPoint ptNext=swdoc->PaintHeader(dc, rect, "Primes", 1, 1);

  return ptNext;
}

void TPrimesView::EvGetMinMaxInfo(MINMAXINFO far& minmaxinfo)
{
  TSwitchMinApp* theApp = TYPESAFE_DOWNCAST(GetApplication(), TSwitchMinApp);
  if (theApp) {
    if (theApp->Printing) {
      minmaxinfo.ptMaxSize = TPoint(32000, 32000);
      minmaxinfo.ptMaxTrackSize = TPoint(32000, 32000);
      return;
    }
  }
  TWindow::EvGetMinMaxInfo(minmaxinfo);

}


void TPrimesView::CmPrimesUpdate()
{
  // create array of function IDs
  StringVector fname(swdoc->Outputs());
  swdoc->MakeFunctionNames(fname);

  // Set terms
  wTermsDNF->SetTerms(swdoc->GetCoverageTerms(dnf, this), fname);
  wTermsCNF->SetTerms(swdoc->GetCoverageTerms(cnf, this), fname);

  wTabs->Show(SW_SHOW);

  PrimesInvalid=false;
}


void TPrimesView::CePrimesUpdate(TCommandEnabler &tce)
{
  // INSERT>> Your code here.
  tce.Enable(PrimesInvalid);
}

bool TPrimesView::VnCoverageTermsChanged(long)
{
  CmPrimesUpdate();
  return true;
}

bool TPrimesView::VnFuncSpecChanged(long)
{
  PrimesInvalid=true;
  wTabs->Show(SW_HIDE);

  return true;
}


const char far* TPrimesView::GetViewName()
{
  /*const char far* result;
  result = TWindowView::GetViewName();
  return result;*/

  return StaticName();
}

bool TPrimesView::VnRevert(bool /*clear*/)
{
  PrimesInvalid=false;
  wTabs->Show(SW_SHOW);

  return true;
}



bool TPrimesView::SetDocTitle(const char far* docname, int index)
{
  bool result;

  string title=docname;
  title += " : ";
  title += GetViewName();

  result = TWindowView::SetDocTitle(title.c_str(), index);

  return result;
}

