////// AB CLASSGEN Wed Apr 02 22:04:50 1997 ////////
// RdcTable Definition
//////////////////////////////////////////////////////
#if !defined(RdcTable_HPP)
#define RdcTable_HPP
#if !defined(RC_INVOKED)	// no Windows RC compiler

#include "TermSet.HPP"
#include <map.h>

namespace swmin {

#if defined(DLLUSE_SWSYSTEM) || defined(DLLBLD_SWSYSTEM_RDCTABLE)
#pragma option -vi-
DLLBLD_STLMAP(DLLBLD_SWSYSTEM, Term, unsigned long, std::less<Term>);
#pragma option -vi
#endif
typedef std::map<Term, unsigned long, std::less<Term> > TermCostMap;

class DLLBLD_SWSYSTEM RdcTable
{
public:
   // Following data MUST be built by external routine!
   TermSet Primes,  // primes in system
           Cells;   // cells required to cover
   TermCostMap PriceSheet;  // cost of each prime

	RdcTable();
   RdcTable(TermSet&, TermSet&);  // Primes, Cells
	~RdcTable();

   void DeleteRow(Term);  // Delete row _equal_ to term with given functions
   void DeleteCol(Term);  // Delete column with given functions
   void DeleteRowAndCol(Term);  // Delete row and columns belonging to row
   void DeleteCommonCol(TermSet&);  // Deletes columns common to row in TermSet

   unsigned long PrimesCovering(const Term&) const;  // # of primes covering given term
   unsigned long ColumnsInRow(const Term&) const;  // # of columns left in row
   bool ColumnDominating(const Term&, const Term&) const;  // returns true if c1 dominates c2
   bool RowDominating(const Term&, const Term&) const;     // returns true if r1 dominates r2

   bool GetEquivalentPrimes(TermSet&);

   bool noCol() const;
   bool noRow() const;

   // returns true if there is at least one essential prime and puts one
   // of them into first parameter and column in second
   bool GetEssential(Term&, Term&) const;

protected:
	void invariant();

   bool RemoveEmptyRows(Term);  // parm: column causing possible empty
   bool RemoveEmptyColumns(Term);  // parm: row causing possible empty

   void BuildColumn(const Term&, TermSet&) const;
   void BuildRow(const Term&, TermSet&) const;

private:
	void _commonCtorJob();// called from all ctors
};

}

#endif
#endif
