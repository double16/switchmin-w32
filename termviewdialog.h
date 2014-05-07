//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         termviewdialog.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TTermViewDialog (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(termviewdialog_h)              // Sentry, use file only if it's not already included.
#define termviewdialog_h

#include <owl/dialog.h>


#include <owl/checkbox.h>
#include <owl/commctrl.h>
#include <owl/listwind.h>
#include "listwindsorter.h"

#include "switchminapp.rh"            // Definition of all resources.
#include "switchdoc.h"

//{{TDialog = TTermViewDialog}}
struct TTermViewDialogXfer {
//{{TTermViewDialogXFER_DATA}}
    uint    ShowPrimesOnly;
//{{TTermViewDialogXFER_DATA_END}}
};

struct TTermViewDisplay {
  string cells, funcs, prime;
};

class TTermViewDialog : public TPropWinDialog {
  TListWindSorter Sorter;
  
  public:
    TTermViewDialog(TWindow* parent, TResId resId = IDD_TERMVIEW, TModule* module = 0);
    virtual ~TTermViewDialog();

    void SetTerms(const swmin::TermSet&, const StringVector&);
    void GetTermString(const swmin::Term&, const StringVector&, TTermViewDisplay&);

  protected:
    swmin::TermSet TermCache;
    StringVector FuncsCache;

    void LoadData(bool PrimeOnly);
    bool AddTerm(const swmin::Term&, const StringVector&);

//{{TTermViewDialogXFER_DEF}}
  protected:
    TListWindow* Terms;
    TCheckBox* ShowPrimesOnly;

//{{TTermViewDialogXFER_DEF_END}}

//{{TTermViewDialogVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
//{{TTermViewDialogVIRTUAL_END}}

//{{TTermViewDialogRSP_TBL_BEGIN}}
  protected:
    void BNClicked();
    void EvSize(uint sizeType, TSize& size);
    void LVNColumnclick(TLwNotify& lwn);
    void LVNDeleteitem(TLwNotify& lwn);
    void LVNDeleteallitems(TLwNotify& lwn);
//{{TTermViewDialogRSP_TBL_END}}
    void EvHelp(HELPINFO*);
DECLARE_RESPONSE_TABLE(TTermViewDialog);
};    //{{TTermViewDialog}}

#endif  // termviewdialog_h sentry.

