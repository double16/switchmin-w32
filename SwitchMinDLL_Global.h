//
// SwitchMin DLL global headers
//
#ifndef SWITCHMINDLL_GLOBAL_H
#define SWITCHMINDLL_GLOBAL_H

#include <assert.h>
#include <cstring.h>
#include <ctype.h>
#include <except.h>
#include <mem.h>
#include <iostream>

#include <set>
#include <map>
#define RWSTD_NO_BOOL
#include <vector>
#include <algorithm>

#include <classlib\objstrm>

#if defined(DLLUSE_SWSYSTEM)
 #define DLLBLD_SWSYSTEM __import
#else
 #define DLLBLD_SWSYSTEM __export
#endif

namespace swmin {

  typedef unsigned long int CELL_TYPE;
  enum TSwfCost { Cost_Literals=0, Cost_Term, Cost_Inputs };
  enum TSwfForm { dnf=0, cnf=1};

}

#include "stl_dll.h"

#endif

// EOF

