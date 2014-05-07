//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         funcspecview.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TFuncSpecView (TWindowView).
//
//----------------------------------------------------------------------------
#if !defined(funcspecview_h)              // Sentry, use file only if it's not already included.
#define funcspecview_h

#include <owl/docview.h>
#include <owl/tabctrl.h>
#include <owl/edit.h>
#include "c:\code\dvcommon\dvcommon.h"
//#include "griddll.h"

#include "switchminapp.rh"            // Definition of all resources.

#include "switchdoc.h"
#include "funcspecfuncsdialog.h"
#include "funcspecgendialog.h"

//{{TWindowView = TFuncSpecView}}
class TFuncSpecView : public TWindowView {
    TSwitchDoc* swdoc;
    TPropertyWindow* wTabs;
    TFuncSpecFuncsDialog* wFuncs;
    TFuncSpecGenDialog* wGeneral;
    class TInputsDialog *wInputs;
    struct TInputsDialogXfer *pInputData;
    TEdit *wDesc;
    bool DirtyFlag;

  public:
    TFuncSpecView(TSwitchDoc& doc, TWindow* parent = 0);
    virtual ~TFuncSpecView();
    static const char far* StaticName();

    const swmin::SwSystem* GetSystem();

    bool ChangeInputs(unsigned long);
    bool ChangeDesc(const string&);
    bool ChangeHazardFree(bool);

    bool AddFunction(swmin::Function&);			// true if function added successfully
    bool DelFunction(swmin::Function&);			// true if function deleted successfully
    bool ReplaceFunction(swmin::Function&);	// true if function is added successfully

    void SetDirty(bool = true);

  protected:
    // Printing helper functions & variables
    typedef struct {
      uint x;
      string s;
    } XPosInfo;
    typedef std::vector<XPosInfo> XPosVector;

    void Print_CreateWidths(XPosVector&, uint&, TDC&) const;
    void Print_Summary(TDC&, TRect) const;
    void Print_GenerateLine(char*, CELL_TYPE) const;
    void Print_Line(TDC&, TPoint, const XPosVector&, char*) const;

//{{TFuncSpecViewVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
    virtual void Paint(TDC& dc, bool erase, TRect& rect);
    virtual const char far* GetViewName();
    virtual bool SetDocTitle(const char far* docname, int index);
    virtual bool CanClose();
//{{TFuncSpecViewVIRTUAL_END}}

//{{TFuncSpecViewRSP_TBL_BEGIN}}
  protected:
    void EvSize(uint sizeType, TSize& size);
    void EvGetMinMaxInfo(MINMAXINFO far& minmaxinfo);
    void CmAddFunc();
    void CmDelFunc();
    void CeEditSelected(TCommandEnabler &tce);
    void CmEditclear();
    void CeEditclear(TCommandEnabler &tce);
    void CmEditcopy();
    void CmEditcut();
    void CmEditdelete();
    void CmEditfunc();
    void CmEditpaste();
    void CeEditpaste(TCommandEnabler &tce);
    void CmEditundo();
    void CeEditundo(TCommandEnabler &tce);
    void CeAddfunc(TCommandEnabler &tce);
//{{TFuncSpecViewRSP_TBL_END}}
    void Desc_ENChange();
    void CanClose_BNClick();
    void Apply_BNClick();
    void Help_BNClick();
    bool VnCommit(bool force);
    bool VnRevert(bool clear);
    bool VnIsDirty();
    bool VnFuncSpecChanged(long);
    void EvHelp(HELPINFO*);

DECLARE_RESPONSE_TABLE(TFuncSpecView);
};    //{{TFuncSpecView}}


#endif  // funcspecview_h sentry.

