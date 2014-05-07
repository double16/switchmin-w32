//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         inputsdialog.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TInputsDialog (TDialog).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include "inputsdialog.h"


//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TInputsDialog, TDialog)
//{{TInputsDialogRSP_TBL_BEGIN}}
  EV_WM_SIZE,
//{{TInputsDialogRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TInputsDialog Implementation}}


static TInputsDialogXfer TInputsDialogData;

TInputsDialog::TInputsDialog(TWindow* parent, TResId resId, TModule* module)
:
    TGDialog(parent, resId, module)
{
//{{TInputsDialogXFER_USE}}
  InputGrid = new TInputsGrid(this, IDC_INPUTGRID, 32);

  SetTransferBuffer(&TInputsDialogData);
//{{TInputsDialogXFER_USE_END}}

  // INSERT>> Your constructor code here.
  geName = new TGEdit(this, IDC_INPUTGRIDNAME, "", 0, 0, 80, 16, 16);
  geDesc = new TGEdit(this, IDC_INPUTGRIDDESC, "", 0, 0, 160, 16, 128);

  InputGrid->Add(geName, "Name");
  InputGrid->Add(geDesc, "Description");
}


TInputsDialog::~TInputsDialog()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}


bool TInputsDialog::IsModified() const
{
  return InputGrid->IsModified();
}

void TInputsDialog::SetModify(bool flag)
{
  InputGrid->SetModify(flag);
}

void TInputsDialog::EvSize(uint sizeType, TSize& size)
{
    TDialog::EvSize(sizeType, size);

  // INSERT>> Your code here.
  InputGrid->SetWindowPos(0, TRect(TPoint(0,0), size), SWP_SHOWWINDOW | SWP_NOZORDER);

}

