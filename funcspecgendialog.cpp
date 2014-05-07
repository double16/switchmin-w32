//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         funcspecgendialog.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TFuncSpecGenDialog (TDialog).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include <owl/validate.h>

#include "funcspecgendialog.h"
#include "FuncSpecView.h"
#include "switchminapp.h"
//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TFuncSpecGenDialog, TPropWinDialog)
//{{TFuncSpecGenDialogRSP_TBL_BEGIN}}
  EV_BN_KILLFOCUS(IDC_FNGENHAZFREE, HazardFree_BNKillfocus),
  EV_BN_CLICKED(IDC_FNGENHAZFREE, HazardFree_BNKillfocus),
  EV_WM_HELP,
//{{TFuncSpecGenDialogRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TFuncSpecGenDialog Implementation}}


TFuncSpecGenDialogXfer TFuncSpecGenDialogData;

TFuncSpecGenDialog::TFuncSpecGenDialog(TWindow* parent, TResId resId, TModule* module)
:
    TPropWinDialog(parent, resId, module)
{
//{{TFuncSpecGenDialogXFER_USE}}
  HazardFree = new TCheckBox(this, IDC_FNGENHAZFREE, 0);

  SetTransferBuffer(&TFuncSpecGenDialogData);
//{{TFuncSpecGenDialogXFER_USE_END}}

  // INSERT>> Your constructor code here.
  view = TYPESAFE_DOWNCAST(parent, TFuncSpecView);
}


TFuncSpecGenDialog::~TFuncSpecGenDialog()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}



void TFuncSpecGenDialog::HazardFree_BNKillfocus()
{
  // INSERT>> Your code here.
  if ( ( (HazardFree->GetCheck()==BF_CHECKED) && (!TFuncSpecGenDialogData.HazardFree) ) ||
       ( (HazardFree->GetCheck()==BF_UNCHECKED) && (TFuncSpecGenDialogData.HazardFree) )
    ) {
    TransferData(tdGetData);
    if (view) view->ChangeHazardFree(TFuncSpecGenDialogData.HazardFree);
    else MessageBox("Error locating view in TFuncSpecGenDialog::HazardFree_BNKillfocus()", "Software Error", MB_OK | MB_ICONERROR);
  }

}

static DWORD aIDHs[] = {
  IDC_FNGENHAZFREE, IDH_dlg_FNSPEC_cb_HAZFREE,
  0,0
};

void TFuncSpecGenDialog::EvHelp(HELPINFO* pHelpInfo)
{
  ::WinHelp( (HWND) pHelpInfo->hItemHandle, HelpFileName, HELP_WM_HELP, (DWORD) aIDHs);
}

