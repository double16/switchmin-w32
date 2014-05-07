//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         costtypedialog.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TCostTypeDialog (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(costtypedialog_h)              // Sentry, use file only if it's not already included.
#define costtypedialog_h

#include <owl/dialog.h>


#include <owl/commctrl.h>
#include "switchminapp.rh"            // Definition of all resources.
#include <owl/checkbox.h>


//{{TDialog = TCostTypeDialog}}
struct TCostTypeDialogXfer {
//{{TCostTypeDialogXFER_DATA}}
    uint16    Inputs;
    uint16    Literals;
    uint16    Terms;
    uint16    LiteralsCNF;
    uint16    LiteralsDNF;
    uint16    TermsCNF;
    uint16    TermsDNF;
    uint16    InputsCNF;
    uint16    InputsDNF;
//{{TCostTypeDialogXFER_DATA_END}}
};

extern TCostTypeDialogXfer TCostTypeDialogData;

class TCostTypeDialog : public TDialog {
  public:
    TCostTypeDialog(TWindow* parent, TResId resId = IDD_COSTTYPE, TModule* module = 0);
    virtual ~TCostTypeDialog();

  protected:
    void SetInputsState();
    void SetLiteralsState();
    void SetTermsState();

//{{TCostTypeDialogXFER_DEF}}
  protected:
    TCheckBox* Inputs;
    TCheckBox* Literals;
    TCheckBox* Terms;
    TCheckBox* LiteralsCNF;
    TCheckBox* LiteralsDNF;
    TCheckBox* TermsCNF;
    TCheckBox* TermsDNF;
    TCheckBox* InputsCNF;
    TCheckBox* InputsDNF;

//{{TCostTypeDialogXFER_DEF_END}}

//{{TCostTypeDialogVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
//{{TCostTypeDialogVIRTUAL_END}}

//{{TCostTypeDialogRSP_TBL_BEGIN}}
  protected:
    void InputsBNClicked();
    void LiteralsBNClicked();
    void TermsBNClicked();
    void HelpBNClicked();
//{{TCostTypeDialogRSP_TBL_END}}
    void EvHelp(HELPINFO*);
DECLARE_RESPONSE_TABLE(TCostTypeDialog);
};    //{{TCostTypeDialog}}


#endif  // costtypedialog_h sentry.

