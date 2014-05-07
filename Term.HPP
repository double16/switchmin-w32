//////// AB CLASSGEN Thu Mar 06 10:49:21 1997 ////////
// Term Definition
//////////////////////////////////////////////////////
#if !defined(Term_HPP)
#define Term_HPP
#if !defined(RC_INVOKED)	// no Windows RC compiler

#include "SwitchMinDLL_Global.h"
#include "TBitField.hpp"
#include <vector.h>

namespace swmin {

#if defined(DLLUSE_SWSYSTEM) || defined(DLLBLD_SWSYSTEM_TERM)
#pragma option -vi-
DLLBLD_STLVECTOR(DLLBLD_SWSYSTEM, CELL_TYPE);
#pragma option -vi
#endif

typedef std::vector<CELL_TYPE> TDifVector;

#if defined(DLLUSE_SWSYSTEM) || defined(DLLBLD_SWSYSTEM_TERM)
#pragma option -vi-
DLLBLD_STLSET(DLLBLD_SWSYSTEM, CELL_TYPE, std::less<CELL_TYPE>);
#pragma option -vi
#endif

class DLLBLD_SWSYSTEM Term : public std::set<CELL_TYPE, std::less<CELL_TYPE> >, virtual TStreamableBase
{
public:
   bool Prime;
   TBitField funcs;

	Term();
   // functions
	Term(unsigned long);
	Term(const Term&);
	Term& operator=(const Term&);
	virtual ~Term();

	bool operator==(const Term&) const;
	bool operator<(const Term&) const;

   void SetPrime(bool _p) { Prime=_p; }
   bool GetPrime() const { return Prime; }

   bool is_subset(const Term&) const;			// true if parm is a subset of this
   void make_dif(TDifVector&) const; // creates differences vector
   bool is_difsame(const Term&) const;			// true if differences are the same
   bool can_combine(const Term&) const;		// true if terms can combine
   bool is_partialcover(const Term&) const;  // true if this covers parm in at least one function

   unsigned int index() const;			// sum of 1's
   unsigned int cost(unsigned long, enum TSwfCost, bool) const;	// return cost given inputs, criteria and (true=re-use)

   Term combine(const Term&) const;				// returns combination of parm and this

   TBitField* GetFns() { return &funcs; }

   friend ostream& DLLBLD_SWSYSTEM operator<<(ostream&, const Term&);
   friend istream& DLLBLD_SWSYSTEM operator>>(istream&, Term&);

  protected:
	virtual void invariant();

   unsigned int index(CELL_TYPE) const;

private:
	void _commonCtorJob();// called from all ctors

DECLARE_STREAMABLE(DLLBLD_SWSYSTEM, Term, 1);
};

}

#endif
#endif
