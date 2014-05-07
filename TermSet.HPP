//////// AB CLASSGEN Thu Mar 06 10:50:40 1997 ////////
// TermSet Definition
//////////////////////////////////////////////////////
#if !defined(TermSet_HPP)
#define TermSet_HPP
#if !defined(RC_INVOKED)	// no Windows RC compiler

#include "Term.HPP"

namespace swmin {

#if defined(DLLUSE_SWSYSTEM) || defined(DLLBLD_SWSYSTEM_TERMSET)
#pragma option -vi-
DLLBLD_STLSET(DLLBLD_SWSYSTEM, Term, std::less<Term>);
#pragma option -vi
#endif

class DLLBLD_SWSYSTEM TermSet : public std::set<Term, std::less<Term> >, virtual TStreamableBase
{
public:
	TermSet();
	TermSet(const TermSet&);
	TermSet& operator=(const TermSet&);
	virtual ~TermSet();

	bool operator==(const TermSet&) const;
	bool operator<(const TermSet&) const;

   bool is_subset(const TermSet&) const;

   unsigned int cost(unsigned long, TSwfCost) const;

   void ins(const Term&);
   void remove_subsuming();  // removes subsuming terms

   friend ostream& DLLBLD_SWSYSTEM operator<<(ostream&, const TermSet&);
   friend istream& DLLBLD_SWSYSTEM operator>>(istream&, TermSet&);

 protected:
	virtual void invariant();

private:
	void _commonCtorJob();// called from all ctors

DECLARE_STREAMABLE(DLLBLD_SWSYSTEM, TermSet, 1);
};

}

#endif
#endif
