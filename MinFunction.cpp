//////// AB CLASSGEN Thu Mar 06 10:51:03 1997 ////////
// Function Implementation
//////////////////////////////////////////////////////
#include "SwitchMinDLL_Global.h"
#include "MinFunction.h"

namespace swmin {

void MinFunction::cleanup()
{
  // Remove terms that aren't used in functions
  TermSet::iterator i,e;
  for(i=terms.begin(), e=terms.end(); i!=e; i++) {
    if ( (*i).funcs.index()==0 ) {  // no functions use this one
      terms.erase(i);  // erase it;
      i=terms.begin(); e=terms.end();  // reset iterators
    }
  }


  terms.remove_subsuming();
}

// default constructor
MinFunction::MinFunction()
{
  _commonCtorJob();
}

// inputs
MinFunction::MinFunction(unsigned long _i)
 : Function(_i)
{
  _commonCtorJob();
}

// copy constructor
MinFunction::MinFunction(const MinFunction& y)
 : Function(y)
{
  HazardFree=y.HazardFree;
  CostType=y.CostType;
}

// assignment overload
MinFunction& MinFunction::operator=(const MinFunction& y)
{
  invariant();
  if(this == &y) return *this;

  Function::operator=(y);
  HazardFree=y.HazardFree;
  CostType=y.CostType;

  return *this;
}

MinFunction& MinFunction::operator=(const Function& y)
{
  if(this == &y) return *this;
  Function::operator=(y);
  _commonCtorJob();
  return *this;
}

// called from all constructors
void MinFunction::_commonCtorJob()
{
  HazardFree=false;
  CostType=Cost_Inputs;
}

// destructor
MinFunction::~MinFunction()
{
  //...
}



void MinFunction::invariant()
{
  //...
}

bool MinFunction::operator==(const MinFunction& y) const
{
  return Function::operator==(y);
}

bool MinFunction::operator<(const MinFunction& y) const
{
  return Function::operator<(y);
}

unsigned long MinFunction::cost()
{
  return terms.cost(inputs, CostType);
}

IMPLEMENT_STREAMABLE1(MinFunction, Function);

void MinFunction::Streamer::Write(opstream& out) const
{

  ::WriteBaseObject((Function*) GetObject(), out );

  out << GetObject()->HazardFree;
  out.writeWord16(GetObject()->CostType);
}

void *MinFunction::Streamer::Read(ipstream& in, uint32 ver) const
{
  ::ReadBaseObject( (Function*) GetObject(), in);

  if (ver<2) {
    in >> GetObject()->HazardFree >> GetObject()->CostType;
  }
  else {
    in >> GetObject()->HazardFree;
    GetObject()->CostType = in.readWord16();
  }

  return GetObject();
}

}

