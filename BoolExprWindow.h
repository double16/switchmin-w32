//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         boolexprwindow.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TBoolExprWindow (TWindow).
//
//----------------------------------------------------------------------------
#if !defined(boolexprwindow_h)              // Sentry, use file only if it's not already included.
#define boolexprwindow_h

#include <owl/window.h>

#include "switchminapp.rh"            // Definition of all resources.
#include "gatewindow.h"

//{{TWindow = TBoolExprWindow}}
class TBoolExprWindow : public TGateWindow {

  public:
    TBoolExprWindow(TWindow* parent, const char far* title = 0, TModule* module = 0);
    virtual ~TBoolExprWindow();

    void SetFunction(const MinFunction&, DesignType, const TSwitchDoc*);

};    //{{TBoolExprWindow}}


#endif  // boolexprwindow_h sentry.

