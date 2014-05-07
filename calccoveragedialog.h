//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         calccoveragedialog.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TCalcCoverageDialog (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(calccoveragedialog_h)              // Sentry, use file only if it's not already included.
#define calccoveragedialog_h

#include <owl/dialog.h>

#include "switchminapp.rh"            // Definition of all resources.
#include <owl/gauge.h>
#include <owl/static.h>
#include "switchdoc.h"

//{{TDialog = TCalcCoverageDialog}}
struct TCalcCoverageDialogXfer {
//{{TCalcCoverageDialogXFER_DATA}}
    char    Mode[ 255 ];
    char    Term[ 255 ];
//{{TCalcCoverageDialogXFER_DATA_END}}
};

class TCalcCoverageDialog : public TDialog, swmin::SwSystemCoverageDisp
{
  public:
    TCalcCoverageDialog(TWindow* parent, TResId resId = IDD_CALCCOVERAGE, TModule* module = 0);
    virtual ~TCalcCoverageDialog();

    void SetMode(const string&);
    void SetTerm(const string&);
    void SetStatus(const string&);
    void SetCTerms(unsigned long);
    void SetNTerms(unsigned long);

//{{TCalcCoverageDialogXFER_DEF}}
  protected:
    TStatic* Mode;
    TStatic* Term;
    TGauge* Progress;

//{{TCalcCoverageDialogXFER_DEF_END}}
};    //{{TCalcCoverageDialog}}

#endif  // calccoveragedialog_h sentry.

