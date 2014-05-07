//////// AB CLASSGEN Thu Mar 06 10:51:03 1997 ////////
// Function Definition
//////////////////////////////////////////////////////
#if !defined(Function_HPP)
#define Function_HPP
#if !defined(RC_INVOKED)	// no Windows RC compiler

#include "SwitchMinDLL_Global.h"
#include "TermSet.HPP"

namespace swmin {

class DLLBLD_SWSYSTEM Function : public TStreamableBase
{
  friend class FuncText;

public:
   string id, desc;

	Function();
   Function(unsigned long);  // inputs
	Function(const Function&);
	Function& operator=(const Function&);
	virtual ~Function();

	bool operator==(const Function&) const;
	bool operator<(const Function&) const;

   unsigned long GetInputs() const { return inputs; }
   void SetInputs(unsigned long);

   TermSet* GetTermPtr() { return &terms; }
   TermSet* GetDCPtr() { return &dc; }

   TSwfForm GetMode() const { return mode; }
   // sets form to dnf or cnf, converts if necessary
   void SetMode(TSwfForm);

protected:
   unsigned long inputs;
   TSwfForm mode;
   TermSet terms;
   TermSet dc;

   void RemoveDCTerms();  // removes dc cells from terms

	virtual void invariant();

private:
	void _commonCtorJob();// called from all ctors

DECLARE_STREAMABLE(DLLBLD_SWSYSTEM, Function, 3);
};

}

#endif
#endif
