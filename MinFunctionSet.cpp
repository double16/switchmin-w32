//////// AB CLASSGEN Sun Apr 06 15:37:04 1997 ////////
// FunctionSet Implementation
//////////////////////////////////////////////////////
#include "SwitchMinDLL_Global.h"
#define DLLBLD_SWSYSTEM_MINFUNCTIONSET
#include "MinFunctionSet.h"

namespace swmin {

typedef std::set<MinFunction, std::less<MinFunction> > set_type;

// default constructor
MinFunctionSet::MinFunctionSet()
{
  _commonCtorJob();
  //...
}

// copy constructor
MinFunctionSet::MinFunctionSet(const MinFunctionSet& y)
 : set_type((set_type&) y)
{
  _commonCtorJob();
  //...
}

// assignment overload
MinFunctionSet& MinFunctionSet::operator=(const MinFunctionSet& y)
{
  invariant();
  if(this == &y) return *this;
  set_type::operator=(y);
  return *this;
}

// called from all constructors
void MinFunctionSet::_commonCtorJob()
{
  //...

}

// destructor
MinFunctionSet::~MinFunctionSet()
{
  //...
}



void MinFunctionSet::invariant()
{
  //...
}


IMPLEMENT_STREAMABLE(MinFunctionSet);

void MinFunctionSet::Streamer::Write(opstream& out) const
{
  out.writeWord32(GetObject()->size());
  iterator i = GetObject()->begin(), e = GetObject()->end();
  while (i!=e) {
    out << *i;
    i++;
  }
}

void *MinFunctionSet::Streamer::Read(ipstream& in, uint32 ver) const
{
  GetObject()->erase(GetObject()->begin(), GetObject()->end());

  size_type count;
  MinFunction x;
  count=in.readWord32();
  while (count--) {
    in >> x;
    GetObject()->insert(x);
  }

  return GetObject();
}

}

