//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         calccoveragedialog.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TCalcCoverageDialog (TDialog).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include "calccoveragedialog.h"


//{{TCalcCoverageDialog Implementation}}


static TCalcCoverageDialogXfer TCalcCoverageDialogData;

TCalcCoverageDialog::TCalcCoverageDialog(TWindow* parent, TResId resId, TModule* module)
:
    TDialog(parent, resId, module)
{
//{{TCalcCoverageDialogXFER_USE}}
  Mode = new TStatic(this, IDC_CALCCOVMODE, 255);
  Term = new TStatic(this, IDC_CALCCOVTERM, 255);
  Progress = new TGauge(this, IDC_CALCCOVPROGRESS);

  SetTransferBuffer(&TCalcCoverageDialogData);
//{{TCalcCoverageDialogXFER_USE_END}}

  // INSERT>> Your constructor code here.

}


TCalcCoverageDialog::~TCalcCoverageDialog()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}

void TCalcCoverageDialog::SetMode(const string& _m)
{
  Mode->SetText(_m.c_str());
  GetApplication()->PumpWaitingMessages();
}

void TCalcCoverageDialog::SetTerm(const string& _t)
{
  Term->SetText(_t.c_str());
  GetApplication()->PumpWaitingMessages();
}

void TCalcCoverageDialog::SetStatus(const string&)
{ }

void TCalcCoverageDialog::SetCTerms(unsigned long _c)
{
 cTerms=_c;
 Progress->SetRange(0, _c);
 GetApplication()->PumpWaitingMessages();
}

void TCalcCoverageDialog::SetNTerms(unsigned long _n)
{
  nTerms=_n;
  Progress->SetValue(_n);
  GetApplication()->PumpWaitingMessages();
}

