//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         funcspecgendialog.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TFuncSpecGenDialog (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(funcspecgendialog_h)              // Sentry, use file only if it's not already included.
#define funcspecgendialog_h

#include <owl/dialog.h>
#include <owl/checkbox.h>
#include <owl/commctrl.h>
#include <owl/edit.h>

#include "switchminapp.rh"            // Definition of all resources.
#include "c:\code\dvcommon\dvcommon.h"

//{{TDialog = TFuncSpecGenDialog}}
struct TFuncSpecGenDialogXfer {
//{{TFuncSpecGenDialogXFER_DATA}}
    uint    HazardFree;
//{{TFuncSpecGenDialogXFER_DATA_END}}
};

extern TFuncSpecGenDialogXfer TFuncSpecGenDialogData;

class TFuncSpecGenDialog : public TPropWinDialog {
    class TFuncSpecView* view;

  public:
    TFuncSpecGenDialog(TWindow* parent, TResId resId = IDD_FUNCSPECGENERAL, TModule* module = 0);
    virtual ~TFuncSpecGenDialog();

//{{TFuncSpecGenDialogXFER_DEF}}
  protected:
    TCheckBox* HazardFree;

//{{TFuncSpecGenDialogXFER_DEF_END}}

//{{TFuncSpecGenDialogRSP_TBL_BEGIN}}
  protected:
    void HazardFree_BNKillfocus();
    void EvHelp(HELPINFO*);
//{{TFuncSpecGenDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFuncSpecGenDialog);
};    //{{TFuncSpecGenDialog}}


#endif  // funcspecgendialog_h sentry.

