//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         switchdoc.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TSwitchDoc (TDocument).
//
//----------------------------------------------------------------------------
#if !defined(switchdoc_h)              // Sentry, use file only if it's not already included.
#define switchdoc_h

#include <owl/docview.h>

#include "switchminapp.rh"            // Definition of all resources.
#include "printabledib.h"
#include "stringvector.h"

#include "swsystemdll.h"

//
// TSwitchDoc custom notifications

const int vnFuncSpecChanged = vnCustomBase+1;
const int vnCoverageTermsChanged = vnCustomBase+2;
const int vnMinChanged = vnCustomBase+3;
const int vnAreYouMinView = vnCustomBase+4;

NOTIFY_SIG(vnFuncSpecChanged, long)
NOTIFY_SIG(vnCoverageTermsChanged, long)
NOTIFY_SIG(vnMinChanged, long)
NOTIFY_SIG(vnAreYouMinView, long)

#define EV_VN_FUNCSPECCHANGED \
	VN_DEFINE(vnFuncSpecChanged, VnFuncSpecChanged, long)
#define EV_VN_COVERAGETERMSCHANGED \
	VN_DEFINE(vnCoverageTermsChanged, VnCoverageTermsChanged, long)
#define EV_VN_MINCHANGED \
	VN_DEFINE(vnMinChanged, VnMinChanged, long)
#define EV_VN_AREYOUMINVIEW \
   VN_DEFINE(vnAreYouMinView, VnAreYouMinView, long)

// End of TSwitchDoc custom notifications

//{{TDocument = TSwitchDoc}}
class TSwitchDoc : public TDocument {
   swmin::SwSystem SwSys;

   TPrintableDIB* Trademark;

  public:
    TSwitchDoc(TDocument* parent = 0);
    virtual ~TSwitchDoc();

    const swmin::SwSystem* GetSystem() const { return &SwSys; }

    bool AddFunction(swmin::Function&, TView* = NULL);			// true if function added successfully
    bool DelFunction(swmin::Function&, TView* = NULL);			// true if function deleted successfully
    bool ReplaceFunction(swmin::Function&, TView* = NULL);	// true if function is added successfully

    bool SetDesc(const string&, TView* = NULL);
    bool SetInputs(unsigned long, TView* = NULL);
    bool SetInputs(const swmin::InputVector&, TView* = NULL);
    bool SetHazardFree(bool, TView* = NULL);

    bool MakeFunctionNames(StringVector&) const;

    bool MakeCoverageTerms(swmin::SwSystemCoverageDisp*, TView* = NULL);
    const swmin::TermSet& GetCoverageTerms(swmin::TSwfForm, TView* = NULL);

    bool MakeMinFn(swmin::TSwfCost, swmin::TSwfForm, swmin::SwSystemMinDisp* = NULL, TView* = NULL);
    const swmin::MinFunctionSet& GetMinFn(TView* = NULL);
    bool MinViewActive();

    bool GetFuncFromID(string&, swmin::Function&) const;
    unsigned long Outputs() const { return SwSys.funcs.size(); }

    //  Paints header for printed page given DC, rect, title of page,
    //  current page, number of pages.
    //  Returns starting point for output.  If bool is false, no
    // actual painting is done, only calculation of starting point.
    TPoint PaintHeader(TDC&, const TRect&, const string&, uint, uint, bool = true);

//{{TSwitchDocVIRTUAL_BEGIN}}
  public:
    virtual bool Open(int mode, const char far* path=0);
    virtual bool Commit(bool force=false);
    virtual bool Revert(bool clear=false);
//{{TSwitchDocVIRTUAL_END}}
};    //{{TSwitchDoc}}


#endif  // switchdoc_h sentry.

