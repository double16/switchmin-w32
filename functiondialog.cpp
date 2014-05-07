//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         functiondialog.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TFunctionDialog (TDialog).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include "functiondialog.h"
#include "switchminapp.h"


using namespace swmin;

//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TFunctionDialog, TDialog)
//{{TFunctionDialogRSP_TBL_BEGIN}}
//{{TFunctionDialogRSP_TBL_END}}
  EV_WM_HELP,
END_RESPONSE_TABLE;


//{{TFunctionDialog Implementation}}


TFunctionDialogXfer TFunctionDialogData;

TFunctionDialog::TFunctionDialog(TWindow* parent, TResId resId, TModule* module)
:
    TDialog(parent, resId, module)
{
//{{TFunctionDialogXFER_USE}}
  CNF = new TRadioButton(this, IDC_FUNCTION_CNF, 0);
  DontCares = new TEdit(this, IDC_FUNCTION_DC, 255);
  Desc = new TEdit(this, IDC_FUNCTION_DESC, 255);
  DNF = new TRadioButton(this, IDC_FUNCTION_DNF, 0);
  ID = new TEdit(this, IDC_FUNCTION_ID, 255);
  Terms = new TEdit(this, IDC_FUNCTION_TERMS, 255);

  SetTransferBuffer(&TFunctionDialogData);
//{{TFunctionDialogXFER_USE_END}}

  // INSERT>> Your constructor code here.

}


TFunctionDialog::~TFunctionDialog()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}

static DWORD aIDHs[] = {
  IDC_FUNCTION_CNF,IDH_dlg_FUNCTION_rb_CNF,
  IDC_FUNCTION_DNF,IDH_dlg_FUNCTION_rb_DNF,
  IDC_FUNCTION_DESC,IDH_dlg_FUNCTION_f_DESCRIPTION,
  IDC_FUNCTION_TERMS,IDH_dlg_FUNCTION_f_TERMS,
  IDC_FUNCTION_DC,IDH_dlg_FUNCTION_f_DONTCARE,
  IDC_FUNCTION_ID,IDH_dlg_FUNCTION_f_ID,
  IDC_FUNCTION_GRP_SPEC,IDH_dlg_FUNCTION_grp_SPEC,
  IDC_FUNCTION_GRP_MODE,IDH_dlg_FUNCTION_grp_MODE,
  0,0
};

void TFunctionDialog::EvHelp(HELPINFO* pHelpInfo)
{
  ::WinHelp( (HWND) pHelpInfo->hItemHandle, HelpFileName, HELP_WM_HELP, (DWORD) aIDHs);
}

bool TFunctionDialog::FnFromXfer(Function& f, TFunctionDialogXfer& data)
{
  FuncText ft;
  ft.id=data.ID;
  ft.desc=data.Desc;
  ft.terms=data.Terms;
  ft.dc=data.DontCares;
  if (data.DNF) ft.mode="dnf";
  else if (data.CNF) ft.mode="cnf";
  ft.Get(f);

  return true;
}

bool TFunctionDialog::FnToXfer(TFunctionDialogXfer& data, Function& f)
{
  memset(&data, 0, sizeof(data));

  FuncText ft;
  ft.Set(f);
  ft.id.copy(data.ID, sizeof(data.ID));
  ft.desc.copy(data.Desc, sizeof(data.Desc));
  ft.terms.copy(data.Terms, sizeof(data.Terms));
  ft.dc.copy(data.DontCares, sizeof(data.DontCares));
  if (ft.mode=="cnf") {
    data.CNF=1;
    data.DNF=0;
  }
  else {
    data.DNF=1;
    data.CNF=0;
  }

  return true;
}


int TFunctionDialog::EditFunction(Function& f)
{
  bool retry;
  int result;

  if (!FnToXfer( *((TFunctionDialogXfer*) TransferBuffer), f)) return -1;

  do {
    retry=false;
    result=Execute();

    if (result==IDOK) {
      if (!FnFromXfer(f, *((TFunctionDialogXfer*) TransferBuffer) ) ) {
        if (MessageBox("Some field(s) have invalid data."
                   " Do you want to edit?", "Data Format Error",
         MB_ICONERROR | MB_YESNO | MB_HELP)==IDYES) retry=true;
      }
    }
  } while (retry);

  return result;
}




