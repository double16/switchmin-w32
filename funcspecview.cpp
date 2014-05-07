//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         funcspecview.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TFuncSpecView (TWindowView).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include <math.h>

#include "funcspecview.h"
#include "primesview.h"
#include "FunctionDialog.h"
#include "inputsdialog.h"

#include "switchminapp.h"

#define DESC_SIZE 8192
const int IDC_DESC = 101;

//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TFuncSpecView, TWindowView)
//{{TFuncSpecViewRSP_TBL_BEGIN}}
  EV_WM_SIZE,
  EV_WM_GETMINMAXINFO,
  EV_BN_CLICKED(IDCANCEL, CanClose_BNClick),
  EV_BN_CLICKED(IDHELP, Help_BNClick),
  EV_BN_CLICKED(IDC_PROPWIN_BTNAPPLY, Apply_BNClick),
  EV_COMMAND(CM_ADDFUNC, CmAddFunc),
  EV_COMMAND(CM_DELFUNC, CmDelFunc),
  EV_COMMAND_ENABLE(CM_DELFUNC, CeEditSelected),
  EV_COMMAND(CM_EDITCLEAR, CmEditclear),
  EV_COMMAND_ENABLE(CM_EDITCLEAR, CeEditclear),
  EV_COMMAND(CM_EDITCOPY, CmEditcopy),
  EV_COMMAND_ENABLE(CM_EDITCOPY, CeEditSelected),
  EV_COMMAND(CM_EDITCUT, CmEditcut),
  EV_COMMAND_ENABLE(CM_EDITCUT, CeEditSelected),
  EV_COMMAND(CM_EDITDELETE, CmEditdelete),
  EV_COMMAND_ENABLE(CM_EDITDELETE, CeEditSelected),
  EV_COMMAND(CM_EDITFUNC, CmEditfunc),
  EV_COMMAND_ENABLE(CM_EDITFUNC, CeEditSelected),
  EV_COMMAND(CM_EDITPASTE, CmEditpaste),
  EV_COMMAND_ENABLE(CM_EDITPASTE, CeEditpaste),
  EV_COMMAND(CM_EDITUNDO, CmEditundo),
  EV_COMMAND_ENABLE(CM_EDITUNDO, CeEditundo),
  EV_COMMAND_ENABLE(CM_ADDFUNC, CeAddfunc),
//{{TFuncSpecViewRSP_TBL_END}}
  EV_EN_CHANGE(IDC_DESC, Desc_ENChange),
  EV_VN_ISDIRTY,
  EV_VN_COMMIT,
  EV_VN_REVERT,
  EV_VN_FUNCSPECCHANGED,
  EV_WM_HELP,
END_RESPONSE_TABLE;

//{{TFuncSpecView Implementation}}

const char* TFuncSpecView::StaticName() { return "Specification"; }

TFuncSpecView::TFuncSpecView(TSwitchDoc& doc, TWindow* parent)
:
    TWindowView(doc, parent)
{
  // INSERT>> Your constructor code here.
  Attr.ExStyle |= WS_EX_CONTEXTHELP;

  swdoc=&doc;
  SetBkgndColor(GetSysColor(COLOR_3DFACE));

  wTabs=new TPropertyWindow(this, 0, TShouldDelete::Delete);
  wTabs->SetButtons(TPropertyWindow::BtnApply | TPropertyWindow::BtnHelp | TPropertyWindow::BtnCanClose);

  SetViewMenu(new TMenuDescr(IDM_SPECVIEW));

  DirtyFlag=false;
  pInputData=NULL;
}


TFuncSpecView::~TFuncSpecView()
{
  // INSERT>> Your destructor code here.
  if (pInputData) delete pInputData;
  delete wTabs;
}

void TFuncSpecView::CanClose_BNClick()
{
  if (DirtyFlag) {
    switch (MessageBox("There have been changes made in this view.\n"
                   "Apply? (No will lose all changes).", "Confirm Changes",
                   MB_YESNOCANCEL | MB_ICONWARNING)) {
      case IDYES:
        VnCommit(true);
        break;
      case IDNO:
        VnRevert(false);
        break;
      case IDCANCEL:
        return;
    }
    return;  // Don't close view since "Cancel" button was clicked
  }
  // Close the view
  PostMessage(WM_CLOSE);
}


void TFuncSpecView::Apply_BNClick()
{
  VnCommit(false);
}

void TFuncSpecView::EvHelp(HELPINFO* pHelpInfo)
{
 ((TSwitchMinApp*) GetApplication())->CallHelp(HELP_CONTEXT, IDH_vw_FS_MAIN);

}

void TFuncSpecView::Help_BNClick()
{
 ((TSwitchMinApp*) GetApplication())->CallHelp(HELP_CONTEXT, IDH_vw_FS_MAIN);
}



void TFuncSpecView::SetupWindow()
{
  TWindowView::SetupWindow();

  // INSERT>> Your code here.
  register TImageList* img = ((TSwitchMinApp*) GetApplication())->imglFnSpec;
  wTabs->SetImageList(*img);

  // Description
  wDesc=new TEdit(this, IDC_DESC, swdoc->GetSystem()->desc.c_str(), 0, 0, 0, 0, DESC_SIZE, true);

  // Options
  wGeneral=new TFuncSpecGenDialog(this);

  // Outputs
  wFuncs=new TFuncSpecFuncsDialog(this);

  // Inputs
  wInputs=new TInputsDialog(this);
  pInputData=new TInputsDialogXfer;
  wInputs->SetTransferBuffer(pInputData);

  wTabs->AddPage(wDesc, "Description",0);
  wTabs->AddPage(wInputs, "Inputs",1);
  wTabs->AddPage(wFuncs, "Outputs",2);
  wTabs->AddPage(wGeneral, "Options",3);

  // Load data into controls
  VnRevert(true);
}

void TFuncSpecView::SetDirty(bool flag)
{
  DirtyFlag=flag;
  if (wTabs->GetHandle()) {
    wInputs->SetModify(false);
    wTabs->SetApply(flag);
    if (flag) wTabs->SetCanClose_Cancel();
    else wTabs->SetCanClose_Close();
  }
}

const swmin::SwSystem* TFuncSpecView::GetSystem()
{
  return swdoc->GetSystem();
}


bool TFuncSpecView::ChangeInputs(unsigned long inputs)
{
//  swdoc->SetInputs(inputs, this);
  SetDirty(true);
  return true;
}

bool TFuncSpecView::ChangeDesc(const string& _d)
{
//  swdoc->SetDesc(_d, this);
  SetDirty(true);
  return true;
}

bool TFuncSpecView::ChangeHazardFree(bool _h)
{
  //swdoc->SetHazardFree(_h, this);
  SetDirty(true);
  return true;
}

bool TFuncSpecView::AddFunction(swmin::Function& f)			// true if function added successfully
{
  return swdoc->AddFunction(f, this);
}

bool TFuncSpecView::DelFunction(swmin::Function& f)			// true if function deleted successfully
{
  return swdoc->DelFunction(f, this);
}

bool TFuncSpecView::ReplaceFunction(swmin::Function& f)	// true if function is added successfully
{
  return swdoc->ReplaceFunction(f, this);
}


void TFuncSpecView::EvSize(uint sizeType, TSize& size)
{
    TWindowView::EvSize(sizeType, size);

  // INSERT>> Your code here.
  // Resize tab control
  wTabs->SetWindowPos(0, TRect(TPoint(0,0), size), SWP_SHOWWINDOW | SWP_NOZORDER);

}

void TFuncSpecView::Desc_ENChange()
{
  /*if (wDesc->IsModified())*/ SetDirty(true);
}

//
// Return true if the view is dirty (modified) since the last save.
//
bool TFuncSpecView::VnIsDirty() {
  if (DirtyFlag) return true;

  // Check if any page is dirty
  if (wInputs->IsModified()) SetDirty(true);
  else if (wDesc->IsModified()) SetDirty(true);

  return DirtyFlag;
}

//
// Commit view changes to document
//
bool
TFuncSpecView::VnCommit(bool force)
{
  if (!force && !DirtyFlag)
    return true;

  // Description
  if (wDesc->IsModified()) {
    TAPointer<char> buffer = new char[DESC_SIZE];
    wDesc->GetText(buffer, DESC_SIZE);
    swdoc->SetDesc((char*) buffer, this);
    wDesc->ClearModify();
  }

  // Inputs
  if (wInputs->IsModified()) {
    swmin::InputVector iv;
    string name, desc;
    wInputs->TransferData(tdGetData);
    for(int i=0; i<pInputData->InputGrid.size(); i++) {
      pInputData->InputGrid.Get(i, name, desc);
      iv.push_back(swmin::InputDesc(name, desc));
    }
    swdoc->SetInputs(iv, this);
  }

  // Outputs
    // Done at time of change

  // Options
  wGeneral->TransferData(tdGetData);
  swdoc->SetHazardFree(TFuncSpecGenDialogData.HazardFree ? true:false, this);

  SetDirty(false);

  return true;
}

//
// Clear changes in view, reload data from document
//
bool
TFuncSpecView::VnRevert(bool /*clear*/)
{
  int i;

  // Description
  wDesc->SetText(swdoc->GetSystem()->desc.c_str());

  // Inputs
  swmin::InputVector inputs = swdoc->GetSystem()->InputNames;
  for(i=0; i<inputs.size(); i++) {
    pInputData->InputGrid.Set(i, inputs[i].name, inputs[i].desc);
  }
  wInputs->TransferData(tdSetData);

  // Outputs
  wFuncs->SetFunctions(*(swdoc->GetSystem()->GetFuncsConst()));

  // Options
  TFuncSpecGenDialogData.HazardFree= swdoc->GetSystem()->GetHazardFree() ? 1:0;
  wGeneral->TransferData(tdSetData);

  SetDirty(false);

  return true;
}


void TFuncSpecView::Paint(TDC& dc, bool erase, TRect& rect)
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

      // Get area we can draw in
      TRect drawingArea(swdoc->PaintHeader(dc, rect, "", 0, 0, false), rect.BottomRight());

      TEXTMETRIC tm;
      dc.SelectObject(TFont("Arial", -12));
      if (!dc.GetTextMetrics(tm)) {
        dc.SelectObject(TFont("Arial", -12));
        dc.GetTextMetrics(tm);
      }
      int fHeight=tm.tmHeight+tm.tmExternalLeading;

      unsigned long inputs=swdoc->GetSystem()->GetInputs();

      XPosVector xpos;
      uint divider;
      char* data;

      Print_CreateWidths(xpos, divider, dc);
      if (xpos.back().x < rect.Size().cx) {  // put space in structure to stretch across page
        uint spacing=(rect.Size().cx - xpos.back().x)/(xpos.size()+1);
        uint space_inc=spacing;
        for(XPosVector::size_type i=0; i<xpos.size(); i++) {
          if (i==inputs) {  // after inputs section, increase divider
            divider += space_inc;
            space_inc += spacing;
          }
          xpos[i].x += space_inc;
          space_inc += spacing;
        }
      }

      data=new char[xpos.size()];

      unsigned long PagesDown=1+pow(2,inputs)/( (drawingArea.Size().cy/fHeight) - 2);  // 2 lines for header and horz divider
      unsigned long PagesAcross=1+xpos.back().x/drawingArea.Size().cx;
      unsigned long Pages=1+PagesDown*PagesAcross;

      // Compute the number of pages to print.
      //
      printerData.MinPage = 1;
      printerData.MaxPage = Pages;


      int fromPage = printerData.FromPage == -1 ? 1 : printerData.FromPage;
      int toPage = printerData.ToPage == -1 ? 1 : printerData.ToPage;
      int currentPage = fromPage;

      TPoint p;

      dc.SelectObject(TPen(TColor::Black));

      while (currentPage <= toPage) {
        swdoc->PaintHeader(dc, rect, "Specification", currentPage, Pages, true);
        if (currentPage==1) Print_Summary(dc, drawingArea);
        else {
          // Calculate origin of line by page number
            // x
            int col=((currentPage-2) ) % PagesAcross;
            p.x=drawingArea.left - col * drawingArea.Size().cx;
            // y
            int row=floor((currentPage-2)/PagesAcross);
            p.y=drawingArea.top/* - row * drawingArea.Size().cy*/;

          // Calculate starting and ending input states
          CELL_TYPE state, stateEnd;
          state=(row*(drawingArea.Size().cy-fHeight))/fHeight;
          stateEnd=state+(drawingArea.Size().cy-fHeight)/fHeight;
          if (stateEnd >= (pow(2,inputs)-1) ) stateEnd=pow(2,inputs)-1;

          //
          // Draw header and lines

          // Vertical
          if ( ((p.x+divider) > drawingArea.left) && ((p.x+divider)<drawingArea.right) ) {
            dc.MoveTo(p.x+divider, p.y);
            dc.LineTo(p.x+divider, p.y+((stateEnd-state)+3)*fHeight);
          }

          // Labels
          for(XPosVector::size_type i=0; i<xpos.size(); i++)
            dc.TextOut(TPoint(xpos[i].x, p.y), xpos[i].s.c_str());
          p.y += fHeight;

          // Horizontal
          dc.MoveTo(drawingArea.left, p.y + fHeight/2);
          dc.LineTo(p.x+xpos.back().x, p.y + fHeight/2);
          p.y += fHeight;

          //
          // Draw each line
          while(state <= stateEnd) {
            Print_GenerateLine(data, state);
            Print_Line(dc, p, xpos, data);
            p.y += fHeight;
            state++;
          }
        }

        currentPage++;
      }

      delete[] data;
    }
    else {
      // INSERT>> Normal painting code goes here.
      wTabs->InvalidateRect(rect, erase);
    }
  }
}

void TFuncSpecView::Print_CreateWidths(TFuncSpecView::XPosVector& xpos, uint& divider, TDC& dc) const
{
  xpos.erase(xpos.begin(), xpos.end());

  uint x=0;
  string text;
  char pch[64];
  ostrstream os(pch, 64);
  XPosInfo xinfo;

  // Calculate for inputs
  unsigned long inputs=swdoc->GetSystem()->GetInputs();
  while (inputs--) {
    xinfo.x=x;
    // create input symbol
    os.seekp(0);
    os << 'x' << inputs << " " << ends;
    xinfo.s=pch;

    xpos.push_back(xinfo);

    // move x position
    x += dc.GetTextExtent(pch, strlen(pch)).cx;
  }

  // Calculate for divider
  divider=x;
  x += (divider - xpos.back().x);

  // Calculate for outputs
  swmin::FunctionSet::iterator i=swdoc->GetSystem()->GetFuncsConst()->begin(),
                        e=swdoc->GetSystem()->GetFuncsConst()->end();
  while (i!=e) {
    xinfo.x=x;
    // create output symbol
    os.seekp(0);
    os << (*i).id << " " << ends;
    xinfo.s=pch;

    xpos.push_back(xinfo);

    // move x position
    x += dc.GetTextExtent(pch, strlen(pch)).cx;

    i++;
  }

  xinfo.x=x;
  xinfo.s="";
  xpos.push_back(xinfo);  // save ending x-position
}

void TFuncSpecView::Print_Summary(TDC& dc, TRect rect) const
{
  dc.TextOut(rect.TopLeft(), "[Insert System Summary Here]");
}

void TFuncSpecView::Print_GenerateLine(char* data, CELL_TYPE state) const
{
  // Generate binary number from 'state'
  unsigned long inputs=swdoc->GetSystem()->GetInputs(), inx;
  inx=inputs;
  CELL_TYPE st=state;
  while (inx--) {
    data[inx]= (st&1) ? '1':'0';
    st = st>>1;
  }

  // Generate function values by searching terms
  swmin::FunctionSet::iterator i=swdoc->GetSystem()->GetFuncsConst()->begin(),
                        e=swdoc->GetSystem()->GetFuncsConst()->end();
  const swmin::TermSet* termset;
  swmin::Term term;
  term.insert(state);

  inx=inputs;
  while (i!=e) {
    // Check if it's a don't care
    termset=(*i).GetDCPtr();
    if ( termset->find(term) != termset->end()) data[inx]='+';
    // Check if it's a one or zero
    else {
      termset=(*i).GetTermPtr();
      if ((*i).GetMode()==swmin::dnf) {  // DNF
        if (termset->find(term) != termset->end()) data[inx]='1';
        else data[inx]='0';
      }
      else { // CNF
        if (termset->find(term) != termset->end()) data[inx]='0';
        else data[inx]='1';
      }
    }

    inx++;
    i++;
  }

}

void TFuncSpecView::Print_Line(TDC& dc, TPoint origin, const TFuncSpecView::XPosVector& xpos, char* data) const
{
  XPosVector::size_type i, count=xpos.size()-1;  // subtract one for end marker
  char text[]="+";

  for(i=0; i<count; i++) {
    text[0]=data[i];
    dc.TextOut(origin.OffsetBy(xpos[i].x, 0), text, 1);
  }
}



void TFuncSpecView::EvGetMinMaxInfo(MINMAXINFO far& minmaxinfo)
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


void TFuncSpecView::CmAddFunc()
{
  swmin::Function f;

  if (TFunctionDialog(this).EditFunction(f)==IDOK) {  // OK to add new function
    // Add to system
    if (swdoc->AddFunction(f, this)) wFuncs->AddFunction(f);
    else MessageBox("Could not add function to system", "Error",
                     MB_ICONERROR | MB_OK | MB_HELP);
  }

}


void TFuncSpecView::CmDelFunc()
{
  // INSERT>> Your code here.
  CmEditdelete();
}


void TFuncSpecView::CmEditclear()
{
  // INSERT>> Your code here.

}


void TFuncSpecView::CeEditclear(TCommandEnabler &tce)
{
  // INSERT>> Your code here.
  tce.Enable( (wFuncs->List()->GetItemCount() > 0) &&
              (wTabs->GetSel()==wFuncs) );
}


void TFuncSpecView::CmEditcopy()
{
  // INSERT>> Your code here.

}


void TFuncSpecView::CmEditcut()
{
  // INSERT>> Your code here.

}


void TFuncSpecView::CmEditdelete()
{
  int sel=wFuncs->List()->GetNextItem(-1, TListWindow::Selected);
  if (sel<0) return;

  string ID;
  if (!wFuncs->GetIDFromListWind(sel, ID)) return;
  swmin::Function f;
  if (!swdoc->GetFuncFromID(ID, f)) return;

  if (swdoc->DelFunction(f, this)) wFuncs->DelFunction(f);
  else MessageBox("Could not delete function", "Error", MB_OK | MB_ICONERROR);
}


void TFuncSpecView::CeEditSelected(TCommandEnabler &tce)
{
  // INSERT>> Your code here.
  tce.Enable( (wFuncs->List()->GetNextItem(-1, TListWindow::Selected) >=0) &&
              (wTabs->GetSel()==wFuncs) );

}


void TFuncSpecView::CmEditfunc()
{
  // Get selected index
  int sel=wFuncs->List()->GetNextItem(-1, TListWindow::Selected);
  if (sel<0) return;

  // Get function from system
  string ID;
  if (!wFuncs->GetIDFromListWind(sel, ID)) return;
  swmin::Function f;
  if (!swdoc->GetFuncFromID(ID, f)) return;

  // Edit function using dialog
  if (TFunctionDialog(this).EditFunction(f)==IDOK) {
    // Replace in system
    if (swdoc->ReplaceFunction(f, this)) wFuncs->ReplaceFunction(f);
    else MessageBox("Could not change function", "Error",
                     MB_ICONERROR | MB_OK | MB_HELP);
  }

}


void TFuncSpecView::CmEditpaste()
{
  // INSERT>> Your code here.

}


void TFuncSpecView::CeEditpaste(TCommandEnabler &tce)
{
  // INSERT>> Your code here.
  tce.Enable(false && (wTabs->GetSel()==wFuncs) );
}


void TFuncSpecView::CmEditundo()
{
  // INSERT>> Your code here.

}


void TFuncSpecView::CeEditundo(TCommandEnabler &tce)
{
  // INSERT>> Your code here.
  tce.Enable(false && (wTabs->GetSel()==wFuncs) );
}


void TFuncSpecView::CeAddfunc(TCommandEnabler &tce)
{
  // INSERT>> Your code here.
  tce.Enable((wTabs->GetSel()==wFuncs));
}


const char far* TFuncSpecView::GetViewName()
{
  /*const char far* result;
  result = TWindowView::GetViewName();
  return result;*/
  return StaticName();
}

bool TFuncSpecView::VnFuncSpecChanged(long)
{
  VnRevert(true);
  return false;
}


bool TFuncSpecView::SetDocTitle(const char far* docname, int index)
{
  bool result;

  string title=docname;
  title += " : ";
  title += GetViewName();

  result = TWindowView::SetDocTitle(title.c_str(), index);

  return result;
}


bool TFuncSpecView::CanClose()
{
  bool result;

  result = TWindowView::CanClose();

  // INSERT>> Your code here.

  return result;
}

