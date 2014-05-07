//----------------------------------------------------------------------------
//  Project SwitchMin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    SwitchMin Application
//  FILE:         switchminmdichild.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TSwitchMinMDIChild (TMDIChild).
//
//----------------------------------------------------------------------------

#include "SwitchMinEXE_Global.h"

#include "switchminapp.h"
#include "switchminmdichild.h"


//{{TSwitchMinMDIChild Implementation}}


//--------------------------------------------------------
// TSwitchMinMDIChild
// ~~~~~~~~~~
// Construction/Destruction handling.
//
TSwitchMinMDIChild::TSwitchMinMDIChild(TMDIClient& parent, const char far* title, TWindow* clientWnd, bool shrinkToClient, TModule* module)
:
  TMDIChild(parent, title, clientWnd, shrinkToClient, module)
{
  // INSERT>> Your constructor code here.

}


TSwitchMinMDIChild::~TSwitchMinMDIChild()
{
  Destroy();

  // INSERT>> Your destructor code here.

}
