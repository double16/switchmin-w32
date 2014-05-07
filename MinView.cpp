//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         minview.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TMinView (TWindowView).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include "minview.h"

#include "switchminapp.h"

//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TMinView, TWindowView)
//{{TMinViewRSP_TBL_BEGIN}}
  EV_WM_SIZE,
//{{TMinViewRSP_TBL_END}}
  EV_VN_REVERT,
  EV_VN_FUNCSPECCHANGED,
  EV_VN_COVERAGETERMSCHANGED,
  EV_VN_MINCHANGED,
  EV_VN_AREYOUMINVIEW,
  EV_WM_HELP,
END_RESPONSE_TABLE;


//{{TMinView Implementation}}

const char* TMinView::StaticName() { return "Minimized Functions"; }

TMinView::TMinView(TSwitchDoc& doc, TWindow* parent)
:
    TWindowView(doc, parent)
{
  // INSERT>> Your constructor code here.
  Attr.ExStyle |= WS_EX_CONTEXTHELP;

  swdoc=&doc;

  // Create pane splitter window
  pane=new TPaneSplitter(this, NULL, 5);
  pane->Attr.Style |= WS_CLIPCHILDREN;
  pane->SetSplitterCushion(20);

  design=NULL;
  select=NULL;
}


TMinView::~TMinView()
{
  // INSERT>> Your destructor code here.
  pane->RemoveAllPanes(TShouldDelete::NoDelete);
  delete select;
  delete design;
  delete pane;
}

void TMinView::EvHelp(HELPINFO* pHelpInfo)
{
  ((TSwitchMinApp*) GetApplication())->CallHelp(HELP_CONTEXT, IDH_vw_MINFN_MAIN);
}

void TMinView::ZoomIn()
{
  design->ScaleScale(2.0);  // Increase by 200% each time
}

void TMinView::ZoomOut()
{
  design->ScaleScale(0.5);  // Decrease by 50% each time
}

void TMinView::SetupWindow()
{
  TWindowView::SetupWindow();

  // INSERT>> Your code here.
  pane->Create();

  select=new TMinFuncDialog(this, NULL);
  //select->Attr.Style |= WS_CLIPSIBLINGS;
  //select->Attr.ExStyle |= WS_EX_CLIENTEDGE;
  //select->Create();
  pane->SplitPane(select, 0, psNone);

  design=new TGateWindow(0);
  design->Attr.Style |= WS_CLIPSIBLINGS;
  design->Attr.ExStyle |= WS_EX_CLIENTEDGE;
  pane->SplitPane(select, design, psVertical);

  FnCache=swdoc->GetMinFn(this);
  select->SetFnList(FnCache);
}

void TMinView::EvSize(uint sizeType, TSize& size)
{
    TWindowView::EvSize(sizeType, size);

  // INSERT>> Your code here.
  pane->SetWindowPos(0, TRect(TPoint(0,0), size), SWP_SHOWWINDOW | SWP_NOZORDER);
}

bool TMinView::VnFuncSpecChanged(long)
{
  swmin::MinFunctionSet empty;
  FnCache=empty;
  select->SetFnList(FnCache);

  NewDesign();

  return true;
}

bool TMinView::VnCoverageTermsChanged(long _p)
{
  return VnFuncSpecChanged(_p);
}

bool TMinView::VnRevert(bool /*clear*/)
{
  FnCache=swdoc->GetMinFn(this);
  select->SetFnList(FnCache);

  NewDesign();

  return true;
}

bool TMinView::VnMinChanged(long)
{
  FnCache=swdoc->GetMinFn(this);
  select->SetFnList(FnCache);
  NewDesign();
  return true;
}

bool TMinView::VnAreYouMinView(long)
{
  return true;
}

void TMinView::DesignChange(TGateWindow::DesignType /*design*/)
{
  string id;
  if (select->GetSelID(id)) FunctionChange(id);
}

void TMinView::FunctionChange(string& id)
{
  swmin::MinFunction f;
  swmin::MinFunctionSet::iterator i;

  // Get function from id
  f.id=id;
  if ( (i=FnCache.find(f)) == FnCache.end() ) return;  // could not find function
  f=*i;

  /*
  swmin::MinFunctionSet::iterator i,e;
  for(i=FnCache.begin(), e=FnCache.end(); i!=e; i++) {
    if ( (*i).id == id) {
      f=(*i);
      break;
    }
  }

  if (i==e) return;  // could not find function
  */

  //
  // We now have function

  DesignType destype=(DesignType) select->GetSelDesign();
  if (NewDesign(destype))
    design->SetFunction(f, destype, swdoc);
}

TGateWindow* TMinView::NewDesign(TGateWindow::DesignType _type)
{
  TGateWindow* newDesign;

  // Create new design window
  switch (_type) {
    case TGateWindow::ANDOR :
    case TGateWindow::NAND :
    case TGateWindow::NOR :
    case TGateWindow::BoolExpr :
      newDesign=new TGateWindow(0);
      break;
    default:
      newDesign=NULL;
      break;
  }

  if (!newDesign) return NULL;

  // Set window attributes
  newDesign->Attr.Style |= WS_CLIPSIBLINGS;
  newDesign->Attr.ExStyle |= WS_EX_CLIENTEDGE;

  // Replace current design window
  float Scale=design->GetScale();
  pane->ReplacePane(design, newDesign, TShouldDelete::NoDelete);
  delete design;
  design=newDesign;
  design->SetScale(Scale);

  return design;
}

void TMinView::Paint(TDC& dc, bool erase, TRect& rect)
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

      // Get area we can draw in
      TRect drawingArea(swdoc->PaintHeader(dc, rect, "", 0, 0, false), rect.BottomRight());
      // Allow for one line of text below header
      TFont* font;
      HFONT hFont = (HFONT) GetWindowFont();
      if (!hFont) font=new TFont("Arial", -12);
      else font=new TFont(hFont);
      dc.SelectObject(*font);
      TEXTMETRIC tm;
      int fHeight=dc.GetTextMetrics(tm) ? tm.tmHeight+tm.tmExternalLeading : 10;
      drawingArea.top += fHeight;

      // Have CAD window build the pages of the drawing
      design->ScaleScale(2);  // Increase scale by 2 for printing
      design->BuildPages(drawingArea);

      if (design->GetPages()) {
        char pch[128];
        ostrstream os(pch, 128);

        TPrintDialog::TData& printerData = theApp->Printer->GetSetup();

        int pages=design->GetPages();

        // Compute the number of pages to print.
        //
        printerData.MinPage = 1;
        printerData.MaxPage = pages;

        // INSERT>> Special printing code goes here.

        // Get desc of function for title
        string id, title("Minimum Circuit Design");
        if (select->GetSelID(id)) {  // got ID of selected function
          swmin::MinFunction f;
          swmin::MinFunctionSet::iterator i;
          f.id=id;
          if ( (i=FnCache.find(f)) != FnCache.end() ) {
            os.seekp(0);
            os << (*i).desc << ", cost = " << (*i).cost() << ends;
            title=pch;
          }
        }


        // Calc page range to print
        int fromPage = printerData.FromPage == -1 ? 1 : printerData.FromPage;
        int toPage = printerData.ToPage == -1 ? 1 : printerData.ToPage;
        int currentPage = fromPage;

        TCadPage cadpage;
        TPoint p;

        while (currentPage <= toPage) {
          if (design->GetPage(currentPage, cadpage)) {
            // output header
            p=swdoc->PaintHeader(dc, rect, title, currentPage, pages, true);
            // output row, column
            os.seekp(0);
            os << "Row " << cadpage.row << ", Col " << cadpage.col << "  " << ends;
            dc.SelectObject(*font);
            dc.TextOut(TPoint(drawingArea.right - dc.GetTextExtent(pch, strlen(pch)).cx, p.y), pch);
            // output drawing
            design->Print(dc, drawingArea, currentPage);
          }

          currentPage++;
        } // while
      } // if
      dc.RestoreFont();
      delete font;
      design->ScaleScale(0.5);  // Restore modified printing scale
    }
    else {
      // INSERT>> Normal painting code goes here.
      pane->Invalidate(erase);
      select->Invalidate(erase);
      design->Invalidate(erase);
    }
  }

}


const char far* TMinView::GetViewName()
{
  return StaticName();
}


bool TMinView::SetDocTitle(const char far* docname, int index)
{
  bool result;

  string title=docname;
  title += " : ";
  title += GetViewName();

  result = TWindowView::SetDocTitle(title.c_str(), index);

  return result;
}

