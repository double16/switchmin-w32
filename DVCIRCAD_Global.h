//
// Circuit CAD Library common header
//

#ifndef DVCIRCAD_Global_h
#define DVCIRCAD_Global_h

#include <owl/pch.h>
#include <owl/window.h>
#include <classlib/objstrm.h>
#include <set.h>
#define RWSTD_NO_BOOL
#include <vector.h>
#undef RWSTD_NO_BOOL

#if defined(DLLUSE_DVCIRCAD)
 #define DLLBLD_DVCIRCAD __import
#else
 #define DLLBLD_DVCIRCAD __export
#endif

#include "..\stl_dll.h"

#endif

// EOF
