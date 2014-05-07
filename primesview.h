//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         primesview.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TPrimesView (TWindowView).
//
//----------------------------------------------------------------------------
#if !defined(primesview_h)              // Sentry, use file only if it's not already included.
#define primesview_h

#include <owl/docview.h>
#include "c:\code\dvcommon\dvcommon.h"

#include "switchminapp.rh"            // Definition of all resources.

#include "switchdoc.h"
#include "termviewdialog.h"

//{{TWindowView = TPrimesView}}
class TPrimesView : public TWindowView {
    TSwitchDoc* swdoc;
    TPropertyWindow* wTabs;
    TTermViewDialog* wTermsDNF, *wTermsCNF;
    bool PrimesInvalid;

  public:
    TPrimesView(TSwitchDoc& doc, TWindow* parent = 0);
    virtual ~TPrimesView();
    static const char far* StaticName();

  protected:
    TPoint PaintHeader(TDC&, TRect&);

//{{TPrimesViewVIRTUAL_BEGIN}}
  public:
    virtual void Paint(TDC& dc, bool erase, TRect& rect);
    virtual void SetupWindow();
    virtual const char far* GetViewName();
    virtual bool SetDocTitle(const char far* docname, int index);
//{{TPrimesViewVIRTUAL_END}}

//{{TPrimesViewRSP_TBL_BEGIN}}
  protected:
    void EvSize(uint sizeType, TSize& size);
    void EvGetMinMaxInfo(MINMAXINFO far& minmaxinfo);
    void CmPrimesUpdate();
    void CePrimesUpdate(TCommandEnabler &tce);
//{{TPrimesViewRSP_TBL_END}}
    void Help_BNClicked();
    void Close_BNClicked();
    bool VnFuncSpecChanged(long);
    bool VnCoverageTermsChanged(long);
    bool VnRevert(bool clear);
    void EvHelp(HELPINFO*);
    
DECLARE_RESPONSE_TABLE(TPrimesView);
};    //{{TPrimesView}}


#endif  // primesview_h sentry.

