//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         minfnbuilddialog.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TMinFnBuildDialog (TDialog).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include "minfnbuilddialog.h"


//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TMinFnBuildDialog, TDialog)
//{{TMinFnBuildDialogRSP_TBL_BEGIN}}
  EV_BN_CLICKED(IDC_MINFNDETAILS, DetailsBNClicked),
//{{TMinFnBuildDialogRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TMinFnBuildDialog Implementation}}


static TMinFnBuildDialogXfer TMinFnBuildDialogData;

TMinFnBuildDialog::TMinFnBuildDialog(TWindow* parent, TResId resId, TModule* module)
:
    TDialog(parent, resId, module), os(pchOut, 2048)
{
//{{TMinFnBuildDialogXFER_USE}}
  ColumnCount = new TStatic(this, IDC_MINFUNCBLDCOLS, 255);
  RowCount = new TStatic(this, IDC_MINFUNCBLDPRIMES, 255);
  Messages = new TListBox(this, IDC_MINFNBLDMSGS);
  Status = new TStatic(this, IDC_MINFUNCBLDSTATUS, 255);
  Progress = new TGauge(this, IDC_MINFNPROGRESS);
  Details = new TButton(this, IDC_MINFNDETAILS);

  //SetTransferBuffer(&TMinFnBuildDialogData);*/
//{{TMinFnBuildDialogXFER_USE_END}}

  // INSERT>> Your constructor code here.

}


TMinFnBuildDialog::~TMinFnBuildDialog()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}

void TMinFnBuildDialog::SetAllStart(unsigned long n)
{
  if (n>0) cAllStart=n;
  else cAllStart=1;
  UpdateAll(n);
}

void TMinFnBuildDialog::UpdateAll(unsigned long n)
{
  if (n>cAllStart) SetAllStart(n);
  else Progress->SetValue( ((cAllStart-n)*100)/cAllStart );
  GetApplication()->PumpWaitingMessages();
}

void TMinFnBuildDialog::SetOperation(const string& s)
{
  Status->SetText(s.c_str());
  GetApplication()->PumpWaitingMessages();
}

void TMinFnBuildDialog::Msg(const char* s)
{
  Messages->SetSelIndex(Messages->AddString(s));
  Messages->Invalidate(true);
  Messages->SendMessage(WM_PAINT);
  GetApplication()->PumpWaitingMessages();
}

void TMinFnBuildDialog::SetPrimes(unsigned long n)
{
  cPrime=n;
  os.seekp(0);
  os << n << ends;

  RowCount->SetText(pchOut);
  GetApplication()->PumpWaitingMessages();
}

void TMinFnBuildDialog::SetCells(unsigned long n)
{
  cCells=n;
  os.seekp(0);
  os << n << ends;

  ColumnCount->SetText(pchOut);
  GetApplication()->PumpWaitingMessages();
}

void TMinFnBuildDialog::SetStatus(const string& s)
{
  //Status->SetText(s.c_str());
  Msg(s.c_str());
}

void TMinFnBuildDialog::Essential(const swmin::Term& t, const swmin::Term& col)
{
  cEss++;
  os.seekp(0);
  os << t << t.funcs << " essential in " << col << col.funcs << ends;

  Msg(pchOut);
}

void TMinFnBuildDialog::ColumnRemoval(const swmin::Term& c1, const swmin::Term& c2)
{
  os.seekp(0);
  os << "Column " << c1 << c1.funcs << " dominating " << c2 << c2.funcs << ends;

  Msg(pchOut);
}

void TMinFnBuildDialog::RowRemoval(const swmin::Term& r1, const swmin::Term& r2)
{
  os.seekp(0);
  os << "Row " << r1 << r1.funcs << " dominated by " << r2 << r2.funcs << ends;

  Msg(pchOut);
}

void TMinFnBuildDialog::Conditional(const swmin::TermSet& set)
{
  os.seekp(0);
  os << "Adding ";

  swmin::TermSet::iterator i=set.begin(),e=set.end();
  for( ; i!=e; i++) os << (*i) << (*i).funcs << "+";
  os.seekp(os.tellp()-1);
  os << ends;

  Msg(pchOut);
}


void TMinFnBuildDialog::DetailsBNClicked()
{
  // INSERT>> Your code here.
  DetailsOn=!DetailsOn;
  TRect scr=GetWindowRect(), scrRow=RowCount->GetWindowRect();

  if (DetailsOn) {
    scr.bottom=scr.top+szScreen.cy;
    Details->SetCaption("No Details");
  }
  else {
    scr.bottom=scrRow.top;
    Details->SetCaption("Details");
  }

  MoveWindow(scr, true);
}


void TMinFnBuildDialog::SetupWindow()
{
  TDialog::SetupWindow();

  // INSERT>> Your code here.
  Progress->SetCaption("%d%%");
  Progress->SetRange(0,100);

  szScreen=GetWindowRect().Size();

  DetailsOn=true;
  DetailsBNClicked();
}

