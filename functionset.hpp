//////// AB CLASSGEN Sun Apr 06 15:37:04 1997 ////////
// FunctionSet Definition
//////////////////////////////////////////////////////
#if !defined(FunctionSet_HPP)
#define FunctionSet_HPP
#if !defined(RC_INVOKED)	// no Windows RC compiler

#include "Function.hpp"

namespace swmin {

#if defined(DLLUSE_SWSYSTEM) || defined(DLLBLD_SWSYSTEM_FUNCTIONSET)
#pragma option -vi-
DLLBLD_STLSET(DLLBLD_SWSYSTEM, Function, std::less<Function>);
#pragma option -vi
#endif

class DLLBLD_SWSYSTEM FunctionSet : public std::set<Function, std::less<Function> >, virtual TStreamableBase
{
public:
	FunctionSet();
	FunctionSet(const FunctionSet&);
	FunctionSet& operator=(const FunctionSet&);
	~FunctionSet();


protected:
	void invariant();

private:
	void _commonCtorJob();// called from all ctors
DECLARE_STREAMABLE(DLLBLD_SWSYSTEM, FunctionSet, 1);
};

}

#endif
#endif
