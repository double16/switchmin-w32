//----------------------------------------------------------------------------
//  Project SwitchMin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    SwitchMin Application
//  FILE:         switchminmdichild.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TSwitchMinMDIChild (TMDIChild).
//
//----------------------------------------------------------------------------
#if !defined(switchminmdichild_h)              // Sentry, use file only if it's not already included.
#define switchminmdichild_h

#include "switchminapp.rh"            // Definition of all resources.


//{{TMDIChild = TSwitchMinMDIChild}}
class TSwitchMinMDIChild : public TMDIChild {
  public:
    TSwitchMinMDIChild(TMDIClient& parent, const char far* title, TWindow* clientWnd, bool shrinkToClient = false, TModule* module = 0);
    virtual ~TSwitchMinMDIChild();
};    //{{TSwitchMinMDIChild}}


#endif  // switchminmdichild_h sentry.
