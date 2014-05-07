//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         minfuncdialog.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TMinFuncDialog (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(minfuncdialog_h)              // Sentry, use file only if it's not already included.
#define minfuncdialog_h

#include <owl/dialog.h>


#include <owl/commctrl.h>
#include "switchminapp.rh"            // Definition of all resources.
#include <owl/listwind.h>
#include <owl/radiobut.h>
#include "listwindsorter.h"
#include "switchdoc.h"

class TMinFuncSorter : public TListWindSorter
{
  int DoCompare(TItemData* item1, TItemData* item2, uint32 column) const;
};

//{{TDialog = TMinFuncDialog}}
struct TMinFuncDialogXfer {
//{{TMinFuncDialogXFER_DATA}}
    uint16    AndOr;
    uint16    BooleanExpr;
    uint16    NAND;
    uint16    NOR;
//{{TMinFuncDialogXFER_DATA_END}}
};

class TMinFuncDialog : public TDialog {
  TMinFuncSorter FnListSort;
  class TMinView* minview;

  public:
    TMinFuncDialog(TMinView* view, TWindow* parent, TResId resId = IDD_MINFUNC, TModule* module = 0);
    virtual ~TMinFuncDialog();

    void SetFnList(const swmin::MinFunctionSet&);
    bool GetSelID(string&) const;  // puts selected function ID in string, returns true if OK
    int GetSelDesign() const;

  protected:
    void AddToList(const swmin::MinFunction&);

//{{TMinFuncDialogXFER_DEF}}
  protected:
    TRadioButton* AndOr;
    TRadioButton* BooleanExpr;
    TListWindow* FnList;
    TRadioButton* NAND;
    TRadioButton* NOR;

//{{TMinFuncDialogXFER_DEF_END}}

//{{TMinFuncDialogVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
//{{TMinFuncDialogVIRTUAL_END}}

//{{TMinFuncDialogRSP_TBL_BEGIN}}
  protected:
    void Design_AndOR();
    void Design_BoolExpr();
    void Design_NAND();
    void Design_NOR();
    void ZoomIn();
    void ZoomOut();
    void LVNColumnclick(TLwNotify& lwn);
    void SelFnChanged(TLwNotify& lwn);
    void EvSize(uint sizeType, TSize& size);
//{{TMinFuncDialogRSP_TBL_END}}
    void EvHelp(HELPINFO*);

DECLARE_RESPONSE_TABLE(TMinFuncDialog);
};    //{{TMinFuncDialog}}


#endif  // minfuncdialog_h sentry.

