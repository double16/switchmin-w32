//////// AB CLASSGEN Wed Mar 12 20:02:59 1997 ////////
// SwSystem Implementation
//////////////////////////////////////////////////////

#include "SwitchMinDLL_Global.h"
#include "SwSystem.HPP"
#include <iterator.h>

namespace swmin {

unsigned long log2(unsigned long);

unsigned long log2(unsigned long n)
{
  unsigned long x=0;
  n=n>>1;
  while (n) {
    n=n>>1;
    x++;
  }
  return x;
}


void SwSystem::SymbolicTerm(SymTerm& sym, const Term& t, TSwfForm form) const
{
  // Clear symbolic term
  sym.erase(sym.begin(), sym.end());

  // Find variables not occurring in term
  TDifVector dif;
  t.make_dif(dif);

  // Retrieve a variable state
  CELL_TYPE value=*t.begin();

  // Add each variable, checking 'dif' to see if it's been excluded
  TDifVector::iterator i,e=dif.end();
  unsigned int varnum;
  string var;
  for(varnum=0; varnum<inputs; varnum++, value=value>>1) {
    // check if we should exclude this one
    for(i=dif.begin(); i!=e; i++) if (log2(*i)==varnum) break;
    if (i==e) {  // add this one
      var=InputNames[varnum].name;
      // Check for complement
      if (
          ( ((value&1)==0) && (form==dnf) ) ||   // dnf: 0 bit
          ( ((value&1)==1) && (form==cnf) ) )    // cnf: 1 bit
        var.prepend("~");
      // Add to term
      sym.insert(var);
    } // if
  } // for
}

void SwSystem::SymbolicExpr(SymExpr& sym, TermSet& set, TSwfForm form) const
{
  // clear expression
  sym.erase(sym.begin(), sym.end());

  SymTerm term;
  TermSet::iterator i,e;
  for(i=set.begin(), e=set.end(); i!=e; i++) {
    SymbolicTerm(term, *i, form);
    sym.insert(term);
  }
}


SwSystem::SwSystem(unsigned long _i)
{
  inputs=_i;
  _commonCtorJob();
  InputNames.Gen_Sequence("x",0,inputs-1);
  //...
}

// copy constructor
SwSystem::SwSystem(const SwSystem& y)
{
  _commonCtorJob();
  inputs=y.inputs;
  name=y.name;
  desc=y.desc;
  funcs=y.funcs;
  CoverageTerms[dnf]=y.CoverageTerms[dnf];
  CoverageTerms[cnf]=y.CoverageTerms[cnf];
  CoverageTermsValid=y.CoverageTermsValid;
  MinFuncsValid=y.MinFuncsValid;
  MinFuncs=y.MinFuncs;
  HazardFree=y.HazardFree;
  InputNames=y.InputNames;
}

// assignment overload
SwSystem& SwSystem::operator=(const SwSystem& y)
{
  invariant();
  if(this == &y) return *this;

  inputs=y.inputs;
  name=y.name;
  desc=y.desc;
  funcs=y.funcs;
  CoverageTerms[dnf]=y.CoverageTerms[dnf];
  CoverageTerms[cnf]=y.CoverageTerms[cnf];
  CoverageTermsValid=y.CoverageTermsValid;
  MinFuncsValid=y.MinFuncsValid;
  MinFuncs=y.MinFuncs;
  HazardFree=y.HazardFree;
  InputNames=y.InputNames;

  return *this;
}

// called from all constructors
void SwSystem::_commonCtorJob()
{
  //...
  CoverageTermsValid=false;
  HazardFree=false;
  MinFuncsValid=false;  MinFuncs.erase(MinFuncs.begin(), MinFuncs.end());
}

// destructor
SwSystem::~SwSystem()
{
  //...
}



void SwSystem::invariant()
{
  //...
}

void SwSystem::SetInputs(unsigned long _i)
{
  if (inputs==_i) return;

  inputs=_i;
  InputNames.Gen_Sequence("x",0,inputs-1);
  FunctionSet::iterator i=funcs.begin(), e=funcs.end();
  while (i!=e) {
    (*i).SetInputs(inputs);
    i++;
  }
  FunctionsChanged();
}

void SwSystem::SetHazardFree(bool _haz)
{
  if (_haz==HazardFree) return;
  HazardFree=_haz;
  MinFuncsValid=false;
  if (MinFuncs.size()) MinFuncs.erase(MinFuncs.begin(), MinFuncs.end());
}

bool SwSystem::GetFuncFromID(string& id, Function& f) const
{
  FunctionSet::iterator i=funcs.begin(), e=funcs.end();
  while (i!=e) {
    if ( (*i).id == id ) break;
    i++;
  }
  if (i==e) return false;

  f=(*i);
  return true;
}

FunctionSet* SwSystem::pFuncs()
{
  FunctionsChanged();
  SetInputs(inputs);
  return &funcs;
}

bool SwSystem::ins(const Function& y)
{
  Function f(y);

  f.SetInputs(inputs);  // make inputs the same
  bool result=funcs.insert(f).second;
  if (result) CoverageTermsValid=false;
  return result;
}

//
// Prime reduction
//    uses table reduction method
//
bool SwSystem::MinFnValid(TSwfCost cost, TSwfForm form)
{
  if (!MinFuncsValid) {
    MinFuncs.erase(MinFuncs.begin(), MinFuncs.end());
    return false;
  }

  MinFunctionSet::iterator i=MinFuncs.begin(), e=MinFuncs.end();
  for( ; i!=e; i++) {
    if ( ((*i).CostType==cost) && ((*i).GetMode()==form) ) return true;
  }

  return false;
}

bool SwSystem::MakeMinFn(TSwfCost cost, TSwfForm form, SwSystemMinDisp* disp)
{
  if (!MinFnValid(cost, form)) {
    MinFunctionSet fs;
    BuildMinFn(fs, cost, form, disp);
    std::copy(fs.begin(), fs.end(), std::inserter(MinFuncs, MinFuncs.begin()) );
    MinFuncsValid=true;
    return true;
  }
  return false;
}

void SwSystem::BuildMinFn(MinFunctionSet& MinFn, TSwfCost cost, TSwfForm mode, SwSystemMinDisp* disp)
{
  bool changed;
  RdcTable table;

  string status;
  status="Minimizing for ";
  status += SwCostString[cost];
  status += " in ";
  status += (mode==dnf) ? "DNF":"CNF";
  disp->SetOperation(status);

  BuildRdcTable(table, cost, mode);

  // Clear minimized functions
  MinFn.erase(MinFn.begin(), MinFn.end());

  //
  // Create base min function to hold essential primes
  MinFunction minf(inputs);
  minf.id="";
  minf.id += SwCostString[cost][0];
  minf.id += (mode==dnf) ? 'd':'c';
  minf.desc="Min for ";
  minf.desc += SwCostString[cost];
  minf.desc += " in ";
  minf.desc += (mode==dnf) ? "DNF":"CNF";
  minf.HazardFree=HazardFree;
  minf.CostType=cost;
  minf.SetMode(mode);

  Term prime, col;

  disp->SetAllStart(table.Primes.size()+table.Cells.size());

  do {
    changed=false;

    disp->SetPrimes(table.Primes.size());
    disp->SetCells(table.Cells.size());
    disp->UpdateAll(table.Primes.size()+table.Cells.size());
    disp->SetStatus("Checking for essentials...");

    while (table.GetEssential(prime, col)) { // Check for all essentials
      disp->Essential(prime, col);
      // Add to min function
      minf.GetTermPtr()->insert(prime);
      // Reduct cost of already built term
      table.PriceSheet[prime]=prime.cost(inputs, cost, true);
      // Delete from table
      RemovePrime(table, prime, col, cost);
      disp->SetPrimes(table.Primes.size());
      disp->SetCells(table.Cells.size());
      disp->UpdateAll(table.Primes.size()+table.Cells.size());
      changed=true;
    }

    disp->SetStatus("Checking for column removal...");
    if (ColumnRemoval(table, disp)) {
      changed=true;
      disp->SetPrimes(table.Primes.size());
      disp->SetCells(table.Cells.size());
      disp->UpdateAll(table.Primes.size()+table.Cells.size());
    }
    disp->SetStatus("Checking for row removal...");
    if (RowRemoval(table, disp)) {
      changed=true;
      disp->SetPrimes(table.Primes.size());
      disp->SetCells(table.Cells.size());
      disp->UpdateAll(table.Primes.size()+table.Cells.size());
    }

  } while (changed);

  // If any primes left, add to min functions
  disp->SetStatus("Adding remaining terms...");
  MinFn.insert(minf);  // Insert base minimum function
  if (table.Primes.size()!=0) {
    TermSet eqPrimes;
    TermSet::iterator i,e;
    while (table.GetEquivalentPrimes(eqPrimes)) {
      AddConditionalTerms(MinFn, eqPrimes);
      disp->Conditional(eqPrimes);
      table.DeleteCommonCol(eqPrimes);
      disp->SetPrimes(table.Primes.size());
      disp->SetCells(table.Cells.size());
      disp->UpdateAll(table.Primes.size()+table.Cells.size());
    } // while
  } // if

  // Remove subsuming terms
  MinFunctionSet::iterator im, em;
  for(im=MinFn.begin(), em=MinFn.end(); im!=em; im++)
   (*im).cleanup();

}

void SwSystem::AddConditionalTerms(MinFunctionSet& fns, TermSet& terms)
{
  TermSet::iterator i,e;
  MinFunctionSet::iterator is, es;
  MinFunctionSet newSet;
  MinFunction f;
  unsigned int num, pos;
  char buffer[64];

  num=1;
  for(i=terms.begin(), e=terms.end(); i!=e; i++) {  // loop through each term
    for(is=fns.begin(), es=fns.end(); is!=es; is++) {  // loop through each original function
      f=*is;  // get function
      if ( (fns.size()>1) || (terms.size()>1) ) {// Append number to function ID
        if ((pos=f.id.find('#'))!=NPOS) f.id.remove(pos);  // remove previous number
        f.id += '#';
        ltoa(num, buffer, 10);
        f.id += buffer;
        num++;
      }
      f.GetTermPtr()->ins(*i);  // add term
      newSet.insert(f);  // add function to new list
    } // for
  } // for
  fns=newSet;
}

void SwSystem::RemovePrime(RdcTable& tbl, Term prime, Term col, TSwfCost cost)
{
  // Delete columns from table
  if (cost==Cost_Inputs) prime.funcs.and(col.funcs);  // Only delete columns from one function
  tbl.DeleteRowAndCol(prime);
}

bool SwSystem::ColumnRemoval(RdcTable& tbl, SwSystemMinDisp* disp)
{
  bool changed=false, update;
  TermSet::iterator c1, c2, e;
  unsigned long outputs=funcs.size();
  TBitField fn(outputs);
  Term t1, t2;

  for(unsigned long f=0; f<outputs; f++) {  // Do each function separatly
    fn=0;
    fn.set(f,1);  // setup function mask
    for(c1=tbl.Cells.begin(), e=tbl.Cells.end(); c1!=e; c1++) { // each column
      if ( (*c1).funcs[f]) {  // If column exists in this function
        c2=c1; c2++;  // column 2 becomes column after column 1
        for( ; c2!=e; c2++) {  // iterate through all columns after c1
          if ( (*c2).funcs[f] ) {  // if column 2 exists in this function
            update=false;
            if (tbl.ColumnDominating(*c1, *c2)) {
              disp->ColumnRemoval(*c1, *c2);
              tbl.DeleteCol(*c1);
              update=true;
            } // if
            else if (tbl.ColumnDominating(*c2, *c1)) {
              disp->ColumnRemoval(*c2, *c1);
              tbl.DeleteCol(*c2);
              update=true;
            } // elseif
            if (update) {
              changed=true;
              c1=tbl.Cells.begin();
              c2=c1; c2++;
              e=tbl.Cells.end();
            } // if
          } // if
        } // for
      } // if
    } // for
  } // for

  return changed;
}

bool SwSystem::RowRemoval(RdcTable& tbl, SwSystemMinDisp* disp)
{
  bool changed=false, update;
  TermSet::iterator r1, r2, e;

  for(r1=tbl.Primes.begin(), e=tbl.Primes.end(); r1!=e; r1++) {
    r2=r1;
    for(r2++; r2!=e; r2++) {
      update=false;
      if (tbl.RowDominating(*r1, *r2)) {
        disp->RowRemoval(*r2, *r1);
        tbl.DeleteRow(*r2);
        update=true;
      } // if
      else if (tbl.RowDominating(*r2, *r1)) {
        disp->RowRemoval(*r1, *r2);
        tbl.DeleteRow(*r1);
        update=true;
      } // elseif
      if (update) {
        changed=true;
        r1=tbl.Primes.begin();
        r2=r1; r2++;
        e=tbl.Primes.end();
      } // if
    } // for
  } // for

  return changed;
}

void SwSystem::BuildRdcTable(RdcTable& tbl, TSwfCost cost, TSwfForm mode)
{
  // Make coverage terms
  SwSystemCoverageDisp covdisp;
  MakeCoverageTerms(&covdisp);

  TermSet *prm=&tbl.Primes, *cel=&tbl.Cells;
  TermSet::iterator i,e;

  TermCostMap *prices=&tbl.PriceSheet;

  // Build prime set and cost map
  prm->erase(prm->begin(), prm->end());
  prices->erase(prices->begin(), prices->end());
  for(i=CoverageTerms[mode].begin(), e=CoverageTerms[mode].end(); i!=e; i++) {
    if ((*i).GetPrime()) {
      prm->ins(*i);  // Add to primes
      prices->insert(TermCostMap::value_type(*i, (*i).cost(inputs, cost, false)));
    }
  }

  // Build cell set
  cel->erase(cel->begin(), cel->end());
  BuildAllTerms(mode, *cel, true, false);
  if (HazardFree) {  // Add hazards
    // Build DC list
    TermSet DC;
    TermSet::iterator iDC, eDC;
    BuildAllTerms(mode, DC, false, true);
    for(i=CoverageTerms[mode].begin(), e=CoverageTerms[mode].end(); i!=e; i++) {
      if ((*i).size()>2) break;  // No need to check anymore
      if ((*i).size()<2) continue;  // Skip one cell terms
      // We have a two-cell term
      // Check for any don't cares
      for(iDC=DC.begin(), eDC=DC.end(); iDC!=eDC; iDC++) {
        if ((*i).is_partialcover(*iDC)) break;  // Covers a don't care, don't need it
      }
      if (iDC==eDC) cel->ins(*i); // Add Hazard
    } // for
  } // if
}

//
// Primes
//   Built using Tagged Quine-McCluskley method
//
bool SwSystem::MakeCoverageTerms(SwSystemCoverageDisp* disp)
{
  if (!CoverageTermsValid) {
    CalcCoverageTerms(dnf, disp);
    CalcCoverageTerms(cnf, disp);
    CoverageTermsValid=true;
    return true;
  }
  return false;
}

// Apply Tagged Quince-McCluskey procedure to calculate
// all terms which cover cells in the system
void SwSystem::CalcCoverageTerms(TSwfForm mode, SwSystemCoverageDisp* disp)
{
  TermSet terms;
  unsigned long n;
  char pch[255];
  ostrstream s(pch, 255);

  disp->SetMode( (mode==dnf) ? "DNF":"CNF");

  // Build initial set of primes
  BuildAllTerms(mode, terms, true, true);

  // Loop through each term while more terms are added
  TermSet::iterator CheckMe=terms.begin(), i;
  Term newTerm((unsigned long) funcs.size()), tCheckMe, tI;

  disp->SetStatus("Finding primes...");
  disp->SetCTerms(terms.size());
  disp->SetNTerms(0);

  n=0;
  while (CheckMe!=terms.end()) {
    i=CheckMe; i++;

    // Update progress display
    disp->SetNTerms(++n);
    s.seekp(0);

    s << (*CheckMe) << ends;
    disp->SetTerm(pch);

    // Loop until sizes are different
    while ( (i!=terms.end()) && ( (*i).size() == (*CheckMe).size()) ) {
      if ((*CheckMe).can_combine(*i)) { // Check if terms can combine
        newTerm=(*CheckMe).combine(*i);  // Create new term
        // Check if combined terms are still primed
        if ( (*CheckMe).funcs==newTerm.funcs) (*CheckMe).SetPrime(false);
        if ( (*i).funcs==newTerm.funcs) (*i).SetPrime(false);
        tCheckMe=*CheckMe; tI=*i;
        terms.ins(newTerm); // Add new term to prime list
        CheckMe=terms.find(tCheckMe); i=terms.find(tI);
        disp->SetCTerms(terms.size());
      }
      i++;
    }
    CheckMe++;
  }

  // Set CoverageTerms
  CoverageTerms[mode]=terms;
}

void SwSystem::BuildAllTerms(TSwfForm mode, TermSet& t, bool Terms, bool DC) const
{
  // Loop through each function, adding terms and don't cares
  unsigned long fnum=0;
  FunctionSet::iterator i=funcs.begin(), e=funcs.end();
  while (i!=e) {
    Function f(*i);
    f.SetMode(mode);

    // Add terms from "terms" member of function to list
    if (Terms) BuildAllTerms_OneSet(t,*f.GetTermPtr(),fnum);
    // Add terms from "dc" member of function to list
    if (DC) BuildAllTerms_OneSet(t,*f.GetDCPtr(),fnum);

    i++;
    fnum++;
  }
}

void SwSystem::BuildAllTerms_OneSet(TermSet& t, const TermSet& a, unsigned long fnum) const
{
  TermSet::iterator i=a.begin(), e=a.end();
  while (i!=e) {
    Term x((unsigned long) funcs.size());
    x.funcs.set(fnum,1);
    std::copy( (*i).begin(), (*i).end(), std::inserter(x, x.begin()) );
    t.ins(x);

    i++;
  }
}

void SwSystem::FunctionsChanged()
{
  CoverageTermsValid=MinFuncsValid=false;
  CoverageTerms[dnf].erase(CoverageTerms[dnf].begin(), CoverageTerms[dnf].end());
  CoverageTerms[cnf].erase(CoverageTerms[cnf].begin(), CoverageTerms[cnf].end());
  MinFuncs.erase(MinFuncs.begin(), MinFuncs.end());
}


IMPLEMENT_STREAMABLE(SwSystem);

void *SwSystem::Streamer::Read(ipstream& in, uint32 ver) const
{
  char *pch;

  // name
  pch=in.readString();
  GetObject()->name=string(pch);
  delete[] pch;

  // desc
  pch=in.readString();
  GetObject()->desc=string(pch);
  delete[] pch;

  GetObject()->inputs=in.readWord32();
  in >> GetObject()->HazardFree;

  if (ver>=4) in >> GetObject()->InputNames;
  else GetObject()->InputNames.Gen_Sequence("x",0,GetObject()->inputs - 1);

  // read functions
  in >> GetObject()->funcs;

  // read coverage cells
  in >> GetObject()->CoverageTermsValid;
  in >> GetObject()->CoverageTerms[dnf] >> GetObject()->CoverageTerms[cnf];

  // read minimized functions
  in >> GetObject()->MinFuncsValid;
  if (ver<3) {
    TSwfCost scEmpty;
    FunctionSet fsEmpty;
    in >> scEmpty;
    in >> fsEmpty;
  }
  else {
    in >> GetObject()->MinFuncs;
  }

  return GetObject();
}

void SwSystem::Streamer::Write(opstream& out) const
{
  out.writeString(GetObject()->name.c_str());
  out.writeString(GetObject()->desc.c_str());

  out.writeWord32(GetObject()->inputs);
  out << GetObject()->HazardFree;

  out << GetObject()->InputNames;

  // write functions
  out << GetObject()->funcs;

  // write coverage cells
  out << GetObject()->CoverageTermsValid;
  out << GetObject()->CoverageTerms[dnf] << GetObject()->CoverageTerms[cnf];

  // minimized functions
  out << GetObject()->MinFuncsValid;
  out << GetObject()->MinFuncs;
}

char* SwCostString[] = { "Literals", "Terms/Gates", "Inputs" };

void SwSystemCoverageDisp::SetMode(const string&) { }
void SwSystemCoverageDisp::SetTerm(const string&) { }
void SwSystemCoverageDisp::SetStatus(const string&) { }
void SwSystemCoverageDisp::SetCTerms(unsigned long _c) { cTerms=_c; }
void SwSystemCoverageDisp::SetNTerms(unsigned long _n) { nTerms=_n; }

SwSystemMinDisp::SwSystemMinDisp()
 : cPrime(0), cCells(0), cEss(0) { }
SwSystemMinDisp::~SwSystemMinDisp() { }
void SwSystemMinDisp::SetAllStart(unsigned long) { }
void SwSystemMinDisp::UpdateAll(unsigned long) { }
void SwSystemMinDisp::SetPrimes(unsigned long) { }
void SwSystemMinDisp::SetCells(unsigned long) { }
void SwSystemMinDisp::SetOperation(const string&) { }
void SwSystemMinDisp::SetStatus(const string&) { }
void SwSystemMinDisp::Essential(const Term&, const Term&) { }
void SwSystemMinDisp::ColumnRemoval(const Term&, const Term&) { }
void SwSystemMinDisp::RowRemoval(const Term&, const Term&) { }
void SwSystemMinDisp::Conditional(const TermSet&) { }

}

