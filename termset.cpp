//////// AB CLASSGEN Thu Mar 06 10:50:40 1997 ////////
// TermSet Implementation
//////////////////////////////////////////////////////
#include "SwitchMinDLL_Global.h"
#define DLLBLD_SWSYSTEM_TERMSET
#include "TermSet.HPP"
#include <assert.h>

namespace swmin {

typedef std::set<Term, std::less<Term> > set_type;

// default constructor
TermSet::TermSet()
{
  _commonCtorJob();
  //...
}

// called from all constructors
void TermSet::_commonCtorJob()
{
  //...

}

// destructor
TermSet::~TermSet()
{
  //...
}


TermSet::TermSet(const TermSet& y)
 : set_type((set_type&) y)
{
}

TermSet& TermSet::operator=(const TermSet& y)
{
  invariant();
  if (this==&y) return *this;

  set_type::operator=(y);

  return *this;
}


void TermSet::invariant()
{
  //...
}

bool TermSet::is_subset(const TermSet& y) const
{
  TermSet::iterator i, e, j;

  for(i=y.begin(), e=y.end(); i!=e; i++) {
    if ( (j=find(*i))==end() ) return false;  // term in y isn't here
    if ( !(*j).is_subset(*i) ) return false;  // y term isn't a subset of this term
  }

  return true;
}

bool TermSet::operator==(const TermSet& y) const
{
  if (size()!=y.size()) return false;
  return std::equal(begin(), end(), y.begin());
}

bool TermSet::operator<(const TermSet& y) const
{
  return std::lexicographical_compare(begin(), end(), y.begin(), y.end());
}

void TermSet::ins(const Term& y)
{
  iterator i;
  if (y.size()==0) return;
  if ( (i=find(y)) == end()) {
    set_type::insert(y);
    return;
  }

  (*i).funcs.or(y.funcs);  // add funcs in y to existing term
}


unsigned int TermSet::cost(unsigned long inputs, TSwfCost crit) const
{
  TermSet::iterator i,e;
  unsigned long c=0, idx;
  typedef std::vector<unsigned long> TULongVector;
  TULongVector FnTally;  // holds # of terms used in each function

  unsigned long cFn=(*begin()).funcs.getBits();
  FnTally.resize(cFn, 0);

  // calculate cost of terms
  for(i=begin(), e=end(); i!=e; i++) {
    idx=(*i).funcs.index();
    if (idx>0) c += (*i).cost(inputs, crit, false);  // first use of term
    /*if ( idx > 1 )
      c += ((*i).cost(inputs, crit, true)) * ( idx - 1 );*/
    for(unsigned long fn=0; fn<cFn; fn++)
      if ((*i).funcs[fn]) FnTally[fn]=FnTally[fn]+1;
  }

  // calculate cost of combining terms
  for(unsigned long fn=0; fn<cFn; fn++) {
    switch (crit) {
      case Cost_Inputs:
        if (FnTally[fn]>1) c += FnTally[fn];
        break;
      case Cost_Term:
        if (FnTally[fn]>1) c++;  // one gate needed for this function
        break;
      case Cost_Literals:
        break;
    }
  }

  return c;
}

void TermSet::remove_subsuming()  // removes subsuming terms
{
  iterator i1, i2, e;
  bool reset=false;
  e=end();
  for(i1=begin(); i1!=e; i1++) {
    // check if this term subsumes any other term
    if (reset) {
      i1=begin(); e=end();
      reset=false;
    }
    i2=i1;
    for(i2++; i2!=e; i2++) {
      if ((*i2).is_partialcover(*i1)) {  // found one
        TBitField mask(*((*i1).GetFns()));
        mask.and(*((*i2).GetFns()));  // mask out intersection of functions
        mask.not();
        if ((*i1).GetFns()->and(mask).truth()==0) {  // delete if necessary
          erase(i1);
          reset=true;
          i1=begin(); e=end();
          break;
        } // if
      } // if
    } // for
  } // for
}


IMPLEMENT_STREAMABLE(TermSet);

void TermSet::Streamer::Write(opstream& out) const
{
  out.writeWord32(GetObject()->size());
  iterator i = GetObject()->begin(), e = GetObject()->end();
  while (i!=e) {
    out << *i;
    i++;
  }
}

void *TermSet::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{
  GetObject()->erase(GetObject()->begin(), GetObject()->end());

  size_type count;
  Term x;
  count=in.readWord32();
  while (count--) {
    in >> x;
    GetObject()->insert(x);
  }

  return GetObject();
}

ostream& operator<<(ostream& os, const TermSet& t)
{
  TermSet::iterator i=t.begin(), e=t.end();
  while (i!=e) {
    os << *i++;
    if (i!=e) os << ',';
  }

  return os;
}

istream& operator>>(istream& is, TermSet& t)
{
  t.erase(t.begin(), t.end());

  long flags=is.flags();
  is.setf(ios::skipws);

  Term x;

  do {
    if (is.peek()==',') is.ignore();
    is >> x;
    t.insert(x);
  } while (is.peek()==',');

  is.flags(flags);
  return is;
}

}

