//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         functiondialog.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TFunctionDialog (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(functiondialog_h)              // Sentry, use file only if it's not already included.
#define functiondialog_h

#include <owl/dialog.h>

#include "switchminapp.rh"            // Definition of all resources.
#include <owl/edit.h>
#include <owl/radiobut.h>

#include "switchdoc.h"

//{{TDialog = TFunctionDialog}}
struct TFunctionDialogXfer {
//{{TFunctionDialogXFER_DATA}}
    uint16    CNF;
    char    DontCares[ 255 ];
    char    Desc[ 255 ];
    uint16    DNF;
    char    ID[ 255 ];
    char    Terms[ 255 ];
//{{TFunctionDialogXFER_DATA_END}}
};

extern TFunctionDialogXfer TFunctionDialogData;

class TFunctionDialog : public TDialog {
  public:
    TFunctionDialog(TWindow* parent, TResId resId = IDD_FUNCTION, TModule* module = 0);
    virtual ~TFunctionDialog();

    static bool FnFromXfer(swmin::Function&, TFunctionDialogXfer&);
    static bool FnToXfer(TFunctionDialogXfer&, swmin::Function&);

    // Loads function into xfer buffer, runs dialog, saves xfer to function
    int EditFunction(swmin::Function&);

//{{TFunctionDialogXFER_DEF}}
  protected:
    TRadioButton* CNF;
    TEdit* DontCares;
    TEdit* Desc;
    TRadioButton* DNF;
    TEdit* ID;
    TEdit* Terms;

//{{TFunctionDialogXFER_DEF_END}}

//{{TFunctionDialogRSP_TBL_BEGIN}}
  protected:
//{{TFunctionDialogRSP_TBL_END}}
    void EvHelp(HELPINFO*);

DECLARE_RESPONSE_TABLE(TFunctionDialog);
};    //{{TFunctionDialog}}


#endif  // functiondialog_h sentry.

