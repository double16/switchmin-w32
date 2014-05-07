//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         primeviewdialog.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TPrimeViewDialog (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(primeviewdialog_h)              // Sentry, use file only if it's not already included.
#define primeviewdialog_h

#include <owl/dialog.h>

#include "switchminapp.rh"            // Definition of all resources.


//{{TDialog = TPrimeViewDialog}}
class TPrimeViewDialog : public TDialog {
  public:
    TPrimeViewDialog(TWindow* parent, TResId resId = IDD_PRIMEVIEWDLG, TModule* module = 0);
    virtual ~TPrimeViewDialog();
};    //{{TPrimeViewDialog}}


#endif  // primeviewdialog_h sentry.

