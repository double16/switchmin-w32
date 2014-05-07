//////// AB CLASSGEN Sun Apr 06 15:37:04 1997 ////////
// MinFunctionSet Definition
//////////////////////////////////////////////////////
#if !defined(MinFunctionSet_HPP)
#define MinFunctionSet_HPP
#if !defined(RC_INVOKED)	// no Windows RC compiler

#include "MinFunction.h"

namespace swmin {

#if defined(DLLUSE_SWSYSTEM) || defined(DLLBLD_SWSYSTEM_MINFUNCTIONSET)
#pragma option -vi-
DLLBLD_STLSET(DLLBLD_SWSYSTEM, MinFunction, std::less<MinFunction>);
#pragma option -vi
#endif

class DLLBLD_SWSYSTEM MinFunctionSet : public std::set<MinFunction, std::less<MinFunction> >, virtual TStreamableBase
{
public:
	MinFunctionSet();
	MinFunctionSet(const MinFunctionSet&);
	MinFunctionSet& operator=(const MinFunctionSet&);
	~MinFunctionSet();


protected:
	void invariant();

private:
	void _commonCtorJob();// called from all ctors
DECLARE_STREAMABLE(DLLBLD_SWSYSTEM, MinFunctionSet, 1);
};

}

#endif
#endif
