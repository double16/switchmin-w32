//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         minview.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TMinView (TWindowView).
//
//----------------------------------------------------------------------------
#if !defined(minview_h)              // Sentry, use file only if it's not already included.
#define minview_h

#include <owl/docview.h>
#include <owl/panespli.h>

#include "switchminapp.rh"            // Definition of all resources.

#include "switchdoc.h"
#include "minfuncdialog.h"					// Min Functions Selector Dialog
#include "gatewindow.h"

//{{TWindowView = TMinView}}
class TMinView : public TWindowView {
    TSwitchDoc* swdoc;
    TPaneSplitter* pane;
    TMinFuncDialog* select;
    TGateWindow* design;
    swmin::MinFunctionSet FnCache;

  public:
    TMinView(TSwitchDoc& doc, TWindow* parent = 0);
    virtual ~TMinView();

    void DesignChange(TGateWindow::DesignType);
    void FunctionChange(string&);  // call with function ID
    void ZoomIn();
    void ZoomOut();

    static const char* StaticName();

  protected:
    TGateWindow* NewDesign(TGateWindow::DesignType = TGateWindow::BoolExpr);

//{{TMinViewVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
    virtual void Paint(TDC& dc, bool erase, TRect& rect);
    virtual const char far* GetViewName();
    virtual bool SetDocTitle(const char far* docname, int index);
//{{TMinViewVIRTUAL_END}}

//{{TMinViewRSP_TBL_BEGIN}}
  protected:
    void EvSize(uint sizeType, TSize& size);
//{{TMinViewRSP_TBL_END}}
    bool VnFuncSpecChanged(long);
    bool VnCoverageTermsChanged(long);
    bool VnMinChanged(long);
    bool VnRevert(bool clear);
    bool VnAreYouMinView(long);
    void EvHelp(HELPINFO*);
DECLARE_RESPONSE_TABLE(TMinView);
};    //{{TMinView}}


#endif  // minview_h sentry.

