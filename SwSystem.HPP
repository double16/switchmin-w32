//////// AB CLASSGEN Wed Mar 12 20:02:59 1997 ////////
// SwSystem Definition
//////////////////////////////////////////////////////
#if !defined(SwSystem_HPP)
#define SwSystem_HPP
#if !defined(RC_INVOKED)	// no Windows RC compiler

#include "SwitchMinDLL_Global.h"

#include "Function.HPP"
#include "FunctionSet.HPP"
#include "MinFunctionSet.h"
#include "FuncText.h"
#include "RdcTable.h"
#include "symbolic.h"

namespace swmin {

extern char* DLLBLD_SWSYSTEM SwCostString[];

class DLLBLD_SWSYSTEM SwSystemCoverageDisp
{
 protected:
  unsigned long cTerms, nTerms;
 public:
  virtual void SetMode(const string&);
  virtual void SetTerm(const string&);
  virtual void SetStatus(const string&);
  virtual void SetCTerms(unsigned long);
  virtual void SetNTerms(unsigned long);
};

class DLLBLD_SWSYSTEM SwSystemMinDisp
{
  protected:
    unsigned long cPrime, cCells, cAllStart, cEss;
  public:
    SwSystemMinDisp();
    virtual ~SwSystemMinDisp();
    virtual void SetAllStart(unsigned long);
    virtual void UpdateAll(unsigned long);
    virtual void SetPrimes(unsigned long);
    virtual void SetCells(unsigned long);
    virtual void SetOperation(const string&);
    virtual void SetStatus(const string&);
    virtual void Essential(const Term&, const Term&); // ess, column
    virtual void ColumnRemoval(const Term&, const Term&); // removed, dominated
    virtual void RowRemoval(const Term&, const Term&);  // removed, dominating
    virtual void Conditional(const TermSet&);
};

class DLLBLD_SWSYSTEM SwSystem : public TStreamableBase
{
protected:
  unsigned long inputs;
  bool HazardFree;
  bool MinFuncsValid;

public:
  string name, desc;
  FunctionSet funcs;
  InputVector InputNames;

  bool CoverageTermsValid;
  TermSet CoverageTerms[cnf+1];

  MinFunctionSet MinFuncs;

	SwSystem(unsigned long = 4);
	SwSystem(const SwSystem&);
	SwSystem& operator=(const SwSystem&);
	virtual ~SwSystem();

   void SetInputs(unsigned long);
   unsigned long GetInputs() const { return inputs; }

   void SetHazardFree(bool);
   bool GetHazardFree() const { return HazardFree; }

   //
   // Function set manipulation
   const FunctionSet* GetFuncsConst() const { return &funcs; }
   FunctionSet* pFuncs();
   bool ins(const Function&);
   void FunctionsChanged();
   bool GetFuncFromID(string&, Function&) const;

   //
   // Coverage term access
   bool MakeCoverageTerms(SwSystemCoverageDisp*);  // true if recalced
   void CalcCoverageTerms(TSwfForm, SwSystemCoverageDisp*);

   //
   // Minimized function access
   bool MinFnValid(TSwfCost, TSwfForm);
   bool MakeMinFn(TSwfCost, TSwfForm, SwSystemMinDisp*);  // true if recalced
   void BuildMinFn(MinFunctionSet&, TSwfCost, TSwfForm, SwSystemMinDisp*);

   //
   // Symbolic methods
   void SymbolicTerm(SymTerm&, const Term&, TSwfForm) const;
   void SymbolicExpr(SymExpr&, TermSet&, TSwfForm) const;

protected:
	virtual void invariant();

   void BuildRdcTable(RdcTable&, TSwfCost, TSwfForm);
   void RemovePrime(RdcTable&, Term, Term, TSwfCost);  // table, prime, column, dnf|cnf
   bool ColumnRemoval(RdcTable&, SwSystemMinDisp* disp);  // returns true if table has changed
   bool RowRemoval(RdcTable&, SwSystemMinDisp* disp);  // returns true if table has changed
   void AddConditionalTerms(MinFunctionSet&, TermSet&);
   void BuildAllTerms(TSwfForm, TermSet&, bool = true, bool = true) const;
   void BuildAllTerms_OneSet(TermSet&, const TermSet&, unsigned long) const;

private:
	void _commonCtorJob();// called from all ctors

DECLARE_STREAMABLE(DLLBLD_SWSYSTEM, SwSystem, 4);
};

}

#endif
#endif
