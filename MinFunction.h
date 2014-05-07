//////// AB CLASSGEN Thu Mar 06 10:51:03 1997 ////////
// Function Definition
//////////////////////////////////////////////////////
#if !defined(MinFunction_HPP)
#define MinFunction_HPP
#if !defined(RC_INVOKED)	// no Windows RC compiler

#include "SwitchMinDLL_Global.h"
#include "Function.HPP"

namespace swmin {

class DLLBLD_SWSYSTEM MinFunction : public Function
{
public:
   bool HazardFree;
   TSwfCost CostType;

	MinFunction();
   MinFunction(unsigned long);  // inputs
	MinFunction(const MinFunction&);
	MinFunction& operator=(const MinFunction&);
	MinFunction& operator=(const Function&);
	virtual ~MinFunction();

	bool operator==(const MinFunction&) const;
	bool operator<(const MinFunction&) const;

   void cleanup();
   unsigned long cost();

protected:
	virtual void invariant();

private:
	void _commonCtorJob();// called from all ctors

DECLARE_STREAMABLE(DLLBLD_SWSYSTEM, MinFunction, 2);
};

}

#endif
#endif
