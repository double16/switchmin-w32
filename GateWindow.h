//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         gatewindow.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TGateWindow (TWindow).
//
//----------------------------------------------------------------------------
#if !defined(gatewindow_h)              // Sentry, use file only if it's not already included.
#define gatewindow_h

#include <owl/window.h>

#include "switchminapp.rh"            // Definition of all resources.
#include "switchdoc.h"
#include "stringvector.h"

#include "dvcircad.h"				// Circuit CAD interface

//{{TWindow = TGateWindow}}
class TGateWindow : public TCadWindow {
  public:
    enum DesignType { BoolExpr=0, ANDOR, NAND, NOR };

    TGateWindow(TWindow* parent, const char far* title = 0, TModule* module = 0);
    virtual ~TGateWindow();

    virtual void SetFunction(const swmin::MinFunction&, DesignType, const TSwitchDoc*);

  protected:
    typedef std::vector<unsigned long> ULongVector;
    typedef struct {
      TCadLogicGateBase* gate;
      bool drawn;
      int x;
      int ymin, ymax;
    } TermDrawInfo;
    typedef std::map<swmin::Term, TermDrawInfo, std::less<swmin::Term> > TermGateMap;
    typedef struct {
      const swmin::MinFunction* f;
      DesignType design;
      const TSwitchDoc* doc;
      ULongVector FnTermCount;
      TermGateMap TermGates;
    } BuildDesignInfo;

    void BuildTermGates(BuildDesignInfo&);
    void BuildFnGates(BuildDesignInfo&);
    TCadLogicGateBase* MakeTermGate(swmin::SymTerm&, DesignType, swmin::TSwfForm, int);

    void CreateBoolExpr(const swmin::MinFunction&, const TSwitchDoc*);

};    //{{TGateWindow}}


#endif  // gatewindow_h sentry.

