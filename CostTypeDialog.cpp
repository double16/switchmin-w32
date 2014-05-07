//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         costtypedialog.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TCostTypeDialog (TDialog).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include "SwitchMinApp.h"
#include "costtypedialog.h"


//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TCostTypeDialog, TDialog)
//{{TCostTypeDialogRSP_TBL_BEGIN}}
  EV_BN_CLICKED(IDHELP, HelpBNClicked),
//{{TCostTypeDialogRSP_TBL_END}}
  EV_WM_HELP,
END_RESPONSE_TABLE;


//{{TCostTypeDialog Implementation}}

TCostTypeDialogXfer TCostTypeDialogData;

TCostTypeDialog::TCostTypeDialog(TWindow* parent, TResId resId, TModule* module)
:
    TDialog(parent, resId, module)
{
//{{TCostTypeDialogXFER_USE}}
  Inputs = new TCheckBox(this, IDC_COSTINPUTS, 0);
  Literals = new TCheckBox(this, IDC_COSTLITERALS, 0);
  Terms = new TCheckBox(this, IDC_COSTTERMS, 0);
  LiteralsCNF = new TCheckBox(this, IDC_COSTLITERALSCNF, 0);
  LiteralsDNF = new TCheckBox(this, IDC_COSTLITERALSDNF, 0);
  TermsCNF = new TCheckBox(this, IDC_COSTTERMSCNF, 0);
  TermsDNF = new TCheckBox(this, IDC_COSTTERMSDNF, 0);
  InputsCNF = new TCheckBox(this, IDC_COSTTYPEINPUTCNF, 0);
  InputsDNF = new TCheckBox(this, IDC_COSTTYPEINPUTDNF, 0);

  SetTransferBuffer(&TCostTypeDialogData);
//{{TCostTypeDialogXFER_USE_END}}

  // INSERT>> Your constructor code here.

}


TCostTypeDialog::~TCostTypeDialog()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}

static DWORD aIDHs[] = {
  IDC_COSTINPUTS, IDH_dlg_COSTYPE_cb_INPUTS,
  IDC_COSTLITERALS, IDH_dlg_COSTYPE_cb_LITERALS,
  IDC_COSTTERMS, IDH_dlg_COSTYPE_cb_GATES,
  IDC_COSTTYPEINPUTDNF, IDH_dlg_COSTYPE_cb_DNF,
  IDC_COSTTERMSDNF, IDH_dlg_COSTYPE_cb_DNF,
  IDC_COSTLITERALSDNF, IDH_dlg_COSTYPE_cb_DNF,
  IDC_COSTTERMSCNF, IDH_dlg_COSTYPE_cb_CNF,
  IDC_COSTLITERALSCNF, IDH_dlg_COSTYPE_cb_CNF,
  IDC_COSTTYPEINPUTCNF, IDH_dlg_COSTYPE_cb_CNF,
  0,0
};

void TCostTypeDialog::EvHelp(HELPINFO* pHelpInfo)
{
  ::WinHelp( (HWND) pHelpInfo->hItemHandle, HelpFileName, HELP_WM_HELP, (DWORD) aIDHs);
}

void TCostTypeDialog::SetupWindow()
{
  TDialog::SetupWindow();

  // INSERT>> Your code here.
  EnableTransfer();
  TransferData(tdSetData);
  //SetInputsState();
  //SetLiteralsState();
  //SetTermsState();
}


void TCostTypeDialog::InputsBNClicked()
{
  // INSERT>> Your code here.
  Inputs->Toggle();
  SetInputsState();
}


void TCostTypeDialog::LiteralsBNClicked()
{
  // INSERT>> Your code here.
  Literals->Toggle();
  SetLiteralsState();
}


void TCostTypeDialog::TermsBNClicked()
{
  // INSERT>> Your code here.
  Terms->Toggle();
  SetTermsState();
}

void TCostTypeDialog::SetInputsState()
{
  if (Inputs->GetCheck()==BF_CHECKED) {
    InputsDNF->SetStyle(InputsDNF->GetStyle() & ~WS_DISABLED, true);
    InputsCNF->SetStyle(InputsDNF->GetStyle() & ~WS_DISABLED, true);
  }
  else {
    InputsDNF->SetStyle(InputsDNF->GetStyle() | WS_DISABLED, true);
    InputsCNF->SetStyle(InputsDNF->GetStyle() | WS_DISABLED, true);
  }
}

void TCostTypeDialog::SetLiteralsState()
{
  if (Literals->GetCheck()==BF_CHECKED) {
    LiteralsDNF->SetStyle(LiteralsDNF->GetStyle() & ~WS_DISABLED, true);
    LiteralsCNF->SetStyle(LiteralsDNF->GetStyle() & ~WS_DISABLED, true);
  }
  else {
    LiteralsDNF->SetStyle(LiteralsDNF->GetStyle() | WS_DISABLED, true);
    LiteralsCNF->SetStyle(LiteralsDNF->GetStyle() | WS_DISABLED, true);
  }
}

void TCostTypeDialog::SetTermsState()
{
  if (Terms->GetCheck()==BF_CHECKED) {
    TermsDNF->SetStyle(TermsDNF->GetStyle() & ~WS_DISABLED, true);
    TermsCNF->SetStyle(TermsDNF->GetStyle() & ~WS_DISABLED, true);
  }
  else {
    TermsDNF->SetStyle(TermsDNF->GetStyle() | WS_DISABLED, true);
    TermsCNF->SetStyle(TermsDNF->GetStyle() | WS_DISABLED, true);
  }
}


void TCostTypeDialog::HelpBNClicked()
{
  // INSERT>> Your code here.
  ((TSwitchMinApp*) GetApplication())->CallHelp(HELP_CONTEXT, IDH_dlg_COSTYPE);

}

