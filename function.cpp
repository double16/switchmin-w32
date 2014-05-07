//////// AB CLASSGEN Thu Mar 06 10:51:03 1997 ////////
// Function Implementation
//////////////////////////////////////////////////////
#include "SwitchMinDLL_Global.h"
#include "Function.HPP"

namespace swmin {

// default constructor
Function::Function()
{
  inputs=4;
  _commonCtorJob();
}

// inputs
Function::Function(unsigned long _i)
{
  inputs=_i;
  _commonCtorJob();
}

// copy constructor
Function::Function(const Function& y)
{
  inputs=y.inputs;
  mode=y.mode;
  terms=y.terms;
  dc=y.dc;
  id=y.id;
  desc=y.desc;
}

// assignment overload
Function& Function::operator=(const Function& y)
{
  invariant();
  if(this == &y) return *this;

  inputs=y.inputs;
  mode=y.mode;
  terms=y.terms;
  dc=y.dc;
  id=y.id;
  desc=y.desc;

  return *this;
}

// called from all constructors
void Function::_commonCtorJob()
{
  mode=dnf;
}

// destructor
Function::~Function()
{
  //...
}



void Function::invariant()
{
  //...
}

bool Function::operator==(const Function& y) const
{
  return (id==y.id);
/*
  if (inputs!=y.inputs) return false;

  // Check if covered cells are the same
  if (mode==y.mode) // modes are the same, check terms and dc's
    return (terms==y.terms) && (dc==y.dc);

  // modes are different, convert y to this mode and check
  Function f(y);
  f.SetMode(mode);
  return (terms==f.terms) && (dc==f.dc);
*/
}

bool Function::operator<(const Function& y) const
{
  return (id<y.id);
/*
  if (inputs!=y.inputs) return inputs<y.inputs;

  if (mode==y.mode) {
    if (!(terms==y.terms)) return terms<y.terms;
    else return dc<y.dc;
  }

  // modes are different, convert y to this mode and check
  Function f(y);
  f.SetMode(mode);
  if (!(terms==f.terms)) return terms<f.terms;
  else return dc<f.dc;
*/
}


void Function::SetInputs(unsigned long _i)
{
  inputs=_i;
}

void Function::SetMode(TSwfForm _m)
{
  if (_m==mode) return;  // no change

  TermSet newTerms;
  Term t;

  unsigned long lastCell = 1;
  for(unsigned long i=0; i < inputs; i++) lastCell=lastCell<<1;
  for(CELL_TYPE cell = 0; cell < lastCell; cell++) {
    t.erase(t.begin(), t.end());
    t.insert(cell);
    if (terms.find(t)==terms.end()) newTerms.insert(t);
  }
  terms=newTerms;  // change terms to new set
  RemoveDCTerms(); // remove any dc terms that appear in the coverage
  mode=_m;
}

void Function::RemoveDCTerms()
{
  TermSet::iterator idc=dc.begin(), i;
  while (idc!=dc.end()) {  // for each term in dc
    if ( (i=terms.find(*idc)) != terms.end() ) terms.erase(i);
    idc++;
  }
}

IMPLEMENT_STREAMABLE(Function);

void Function::Streamer::Write(opstream& out) const
{
  out.writeString(GetObject()->id.c_str());
  out.writeString(GetObject()->desc.c_str());
  out << GetObject()->inputs;
  out.writeWord16(GetObject()->mode);
  out << GetObject()->terms << GetObject()->dc;
}

void *Function::Streamer::Read(ipstream& in, uint32 ver) const
{
  char *pch;

  // id
  pch=in.readString();
  GetObject()->id=string(pch);
  delete[] pch;

  // desc
  pch=in.readString();
  GetObject()->desc=string(pch);
  delete[] pch;

  if (ver<3) {
    in >> GetObject()->inputs >> GetObject()->mode;
  }
  else {
    in >> GetObject()->inputs;
    GetObject()->mode = in.readWord16();
  }

  if (ver<2) {
    bool bool1;
    in >> bool1;
  }

  in >> GetObject()->terms >> GetObject()->dc;

  return GetObject();
}

}

