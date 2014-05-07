//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         minfnbuilddialog.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TMinFnBuildDialog (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(minfnbuilddialog_h)              // Sentry, use file only if it's not already included.
#define minfnbuilddialog_h

#include <owl/dialog.h>


#include <owl/button.h>
#include <owl/commctrl.h>
#include "switchminapp.rh"            // Definition of all resources.
#include <owl/gauge.h>
#include <owl/listbox.h>
#include <owl/static.h>
#include "switchdoc.h"


//{{TDialog = TMinFnBuildDialog}}
struct TMinFnBuildDialogXfer {
//{{TMinFnBuildDialogXFER_DATA}}
    char    ColumnCount[ 255 ];
    char    RowCount[ 255 ];
    TListBoxData  Messages;
    char    Status[ 255 ];
//{{TMinFnBuildDialogXFER_DATA_END}}
};

class TMinFnBuildDialog : public TDialog, swmin::SwSystemMinDisp
{
    char pchOut[2048];
    ostrstream os;
    bool DetailsOn;
    TSize szScreen;

  public:
    TMinFnBuildDialog(TWindow* parent, TResId resId = IDD_MINFUNCBUILD, TModule* module = 0);
    virtual ~TMinFnBuildDialog();

    void SetAllStart(unsigned long);
    void UpdateAll(unsigned long);
    void SetOperation(const string&);
    void SetPrimes(unsigned long);
    void SetCells(unsigned long);
    void SetStatus(const string&);
    void Essential(const swmin::Term&, const swmin::Term&);
    void ColumnRemoval(const swmin::Term&, const swmin::Term&); // removed, dominated
    void RowRemoval(const swmin::Term&, const swmin::Term&);  // removed, dominating
    void Conditional(const swmin::TermSet&);

  protected:
    void Msg(const char*);

//{{TMinFnBuildDialogXFER_DEF}}
  protected:
    TStatic* ColumnCount;
    TStatic* RowCount;
    TListBox* Messages;
    TStatic* Status;
    TGauge* Progress;
    TButton* Details;

//{{TMinFnBuildDialogXFER_DEF_END}}

//{{TMinFnBuildDialogVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
//{{TMinFnBuildDialogVIRTUAL_END}}

//{{TMinFnBuildDialogRSP_TBL_BEGIN}}
  protected:
    void DetailsBNClicked();
//{{TMinFnBuildDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TMinFnBuildDialog);
};    //{{TMinFnBuildDialog}}


#endif  // minfnbuilddialog_h sentry.

