//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         funcspecfuncsdialog.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TFuncSpecFuncsDialog (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(funcspecfuncsdialog_h)              // Sentry, use file only if it's not already included.
#define funcspecfuncsdialog_h

#include <owl/dialog.h>
#include <owl/commctrl.h>
#include <owl/listwind.h>

#include "listwindsorter.h"
#include "switchminapp.rh"            // Definition of all resources.
#include "switchdoc.h"

#include "swsystemdll.h"
#include "c:\code\dvcommon\dvcommon.h"
#include <vector.h>

//{{TDialog = TFuncSpecFuncsDialog}}
struct TFuncSpecFuncsDialogXfer {
//{{TFuncSpecFuncsDialogXFER_DATA}}
//{{TFuncSpecFuncsDialogXFER_DATA_END}}
};

class TFuncSpecFuncsDialog : public TPropWinDialog {
  TListWindSorter Sorter;
  class TFuncSpecView* view;

  public:
    TFuncSpecFuncsDialog(TWindow* parent, TResId resId = IDD_FUNCSPECFUNCS, TModule* module = 0);
    virtual ~TFuncSpecFuncsDialog();

    TListWindow* List() { return Functions; }

    void SetFunctions(const swmin::FunctionSet&);

    void AddFunction(const swmin::Function&);
    void AddFunction(const swmin::FuncText&);
    void DelFunction(const swmin::Function&);
    void DelFunction(const swmin::FuncText&);
    void ReplaceFunction(const swmin::Function&);
    void ReplaceFunction(const swmin::FuncText&);

    bool GetIDFromListWind(int, string&);

  protected:
    void AddFunctionToList(const swmin::Function&);
    void AddFunctionToList(const swmin::FuncText&);

//{{TFuncSpecFuncsDialogXFER_DEF}}
  protected:
    TListWindow* Functions;

//{{TFuncSpecFuncsDialogXFER_DEF_END}}

//{{TFuncSpecFuncsDialogVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
//{{TFuncSpecFuncsDialogVIRTUAL_END}}

//{{TFuncSpecFuncsDialogRSP_TBL_BEGIN}}
  protected:
    void EvSize(uint sizeType, TSize& size);
    void LVNColumnClick(TLwNotify& lwn);
//{{TFuncSpecFuncsDialogRSP_TBL_END}}
    void LBNDblClk();
    void EvHelp(HELPINFO*);
DECLARE_RESPONSE_TABLE(TFuncSpecFuncsDialog);
};    //{{TFuncSpecFuncsDialog}}


#endif  // funcspecfuncsdialog_h sentry.

