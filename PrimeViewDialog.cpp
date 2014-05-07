//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         primeviewdialog.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TPrimeViewDialog (TDialog).
//
//----------------------------------------------------------------------------
#include <owl/pch.h>

#include "primeviewdialog.h"


//{{TPrimeViewDialog Implementation}}


TPrimeViewDialog::TPrimeViewDialog(TWindow* parent, TResId resId, TModule* module)
:
    TDialog(parent, resId, module)
{
  // INSERT>> Your constructor code here.

}


TPrimeViewDialog::~TPrimeViewDialog()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}

