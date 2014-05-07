//////// AB CLASSGEN Thu Mar 06 10:49:21 1997 ////////
// Term Implementation
//////////////////////////////////////////////////////

#include "SwitchMinDLL_Global.h"

#define DLLBLD_SWSYSTEM_TERM
#include "Term.HPP"
#include <math.h>

namespace swmin {

typedef std::set<CELL_TYPE, std::less<CELL_TYPE> > set_type;

// default constructor
Term::Term()
{
  _commonCtorJob();
}

// constructor with # of functions
Term::Term(unsigned long _f)
 : funcs(_f)
{
  _commonCtorJob();
}

// copy constructor
Term::Term(const Term& y)
 : set_type((set_type&) y)
{
  Prime=y.Prime;
  funcs=y.funcs;
}

// assignment overload
Term& Term::operator=(const Term& y)
{
  invariant();
  if(this == &y) return *this;

  set_type::operator=(y);
  Prime=y.Prime;
  funcs=y.funcs;

  return *this;
}

// called from all constructors
void Term::_commonCtorJob()
{
  Prime=true;
}

// destructor
Term::~Term()
{
  //...
}



void Term::invariant()
{
  //...
}

bool Term::operator==(const Term& y) const
{
  if (size()!=y.size()) return false;
  return std::equal(begin(), end(), y.begin());
}

bool Term::operator<(const Term& y) const
{
  if (size() != y.size()) return size() < y.size();
  return std::lexicographical_compare(begin(), end(), y.begin(), y.end());
}

bool Term::is_subset(const Term& y) const			// true if parm is a subset of this
{
  // Check if cells covered in y are a subset of this
  if (!std::includes(begin(), end(), y.begin(), y.end())) return false;
  // Cells are covered, check if functions are covered
  return funcs.is_subset(y.funcs);
}

bool Term::is_partialcover(const Term& y) const  // true if this covers y in at least one function
{
  // Check if cells covered in y are a subset of this
  if (!std::includes(begin(), end(), y.begin(), y.end())) return false;
  // Cells are covered, check if at least one of the same functions is covered
  return (TBitField(funcs).and(y.funcs).truth());
}

bool Term::is_difsame(const Term& y) const			// true if differences are the same
{
  if ((size()==0) || (size()!=y.size())) return false;  // MUST have same number of cells
  if (index(size())!=1) return false;   // number of cells MUST be a power of 2

  TDifVector a,b;
  make_dif(a);
  y.make_dif(b);

  return a==b;

  /*
  CELL_TYPE a0=*begin(), b0=*y.begin();  // first elements
  iterator a=begin(), b=y.begin();
  a++, b++;
  unsigned int next=1;     // index increment
  while (a!=end()) {
    if ( (*a-a0) != (*b-b0) ) return false;  // differences are not the same
    for(unsigned int i=0; i<next; i++) { a++; b++; }
    next=next<<1;  // check at each power of two index
  }

  return true;
  */
}

void Term::make_dif(TDifVector& v) const // creates differences vector
{
  v.erase(v.begin(), v.end());  // clear vector
  if (index(size())!=1) return; // number of cells MUST be a power of 2

  CELL_TYPE a0=*begin();
  iterator a=begin(), e=end();
  a++;
  unsigned int next=1;     // index increment
  while (a!=end()) {
    v.push_back( (*a) - a0 );
    for(unsigned int i=0; i<next; i++) a++;
    next=next<<1;  // check at each power of two index
  }
}

bool Term::can_combine(const Term& y) const		// true if terms can combine
{
  // 1.  must cover same number of cells
  if ((size()==0) || (size()!=y.size())) return false;
  // 2. difference must be the same (changing variables)
  if (!is_difsame(y)) return false;
  // 3. First cells must differ by a power of two
  if ( index(*y.begin() - *begin()) != 1) return false;
  // 4. Indices must differ by a power of two
  if ( index(y.index() - index() ) != 1) return false;
  // 5. Must have non-empty intersection of functions
  if (!TBitField(funcs).and(y.funcs).truth()) return false;

  return true;
}

unsigned int Term::index(CELL_TYPE x) const
{
  int i=0;			// init bit counter
  while (x) {  	// loop while x has bits
    if (x&1) i++; // if 1's place has a bit, count it
    x=x>>1;			// shift bits right and discard 1's place
  }
  return i;
}

unsigned int Term::index() const			// sum of 1's
{
  iterator a=begin();
  int i=0;
  while (a!=end()) i += index(*a++);
  return i;
}

unsigned int Term::cost(unsigned long inputs, enum TSwfCost c, bool reuse) const	// return cost given criteria and (true=re-use)
{
  if (reuse) {  // re-use cost
    switch(c) {
      case Cost_Literals: return 0;
      case Cost_Term: return 0;
      case Cost_Inputs: return 1;
    }
  }


  unsigned long vars=inputs-(log10(size())/log10(2));  // get # of vars in this term

  if (c==Cost_Term) return (vars>1) ? 1:0;

  if ((c==Cost_Inputs) && (vars<=1) ) return 0;  // no cost if no gate

  return (unsigned int) vars;
}


Term Term::combine(const Term& y) const				// returns combination of parm and this
{
  Term t(*this);  // start with this term
  t.SetPrime(true);  // force to prime
  std::copy(y.begin(), y.end(), std::inserter(t, t.end()));  // copy new cells from y
  t.funcs.and(y.funcs);  // set functions to intersection of this and y
  return t;
}

IMPLEMENT_STREAMABLE(Term);

void *Term::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{
  in >> GetObject()->Prime >> GetObject()->funcs;

  GetObject()->erase(GetObject()->begin(), GetObject()->end());

  size_type count;
  CELL_TYPE x;
  count=in.readWord32();
  while (count--) {
    in >> x;
    GetObject()->insert(x);
  }

  return GetObject();
}

void Term::Streamer::Write(opstream& out) const
{
  out << GetObject()->Prime << GetObject()->funcs;

  out.writeWord32(GetObject()->size());
  iterator i = GetObject()->begin(), e = GetObject()->end();
  while (i!=e) {
    out << *i;
    i++;
  }
}

ostream& operator<<(ostream& os, const Term& t)
{
  Term::iterator i=t.begin(), e=t.end();
  if (t.size()>1) os << '(';
  while (i!=e) {
    os << *i++;
    if (i!=e) os << ',';
  }
  if (t.size()>1) os << ')';

  return os;
}

istream& operator>>(istream& is, Term& t)
{
  t.erase(t.begin(), t.end());

  long flags=is.flags();
  is.setf(ios::skipws);

  char c;
  CELL_TYPE cell;

  is >> c;
  if (c=='(') { // read group of CELL_TYPE
    while (!is.eof() && (c!=')') ) {
      is >> cell;
      if (is.fail()) break;
      t.insert(cell);
      is >> c;
    }
  }
  else {  // read only one cell
    is.putback(c);
    is >> cell;
    if (!is.fail()) t.insert(cell);
  }

  is.flags(flags);
  return is;
}

}

