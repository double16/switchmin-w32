//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         inputsdialog.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TInputsDialog (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(inputsdialog_h)              // Sentry, use file only if it's not already included.
#define inputsdialog_h

#include <owl/dialog.h>
#include <owl/button.h>

#include "switchminapp.rh"            // Definition of all resources.

#include "inputsgrid.h"

//{{TDialog = TInputsDialog}}
struct TInputsDialogXfer {
//{{TInputsDialogXFER_DATA}}
  TInputsGridData InputGrid;
//{{TInputsDialogXFER_DATA_END}}
};

class TInputsDialog : public TGDialog {
  public:
    TInputsDialog(TWindow* parent, TResId resId = IDD_INPUTS, TModule* module = 0);
    virtual ~TInputsDialog();

    bool IsModified() const;
    void SetModify(bool flag = false);

  protected:
    TGEdit *geName, *geDesc;

//{{TInputsDialogXFER_DEF}}
  protected:
    TInputsGrid* InputGrid;

//{{TInputsDialogXFER_DEF_END}}

//{{TInputsDialogRSP_TBL_BEGIN}}
  protected:
    void EvSize(uint sizeType, TSize& size);
//{{TInputsDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TInputsDialog);
};    //{{TInputsDialog}}


#endif  // inputsdialog_h sentry.

