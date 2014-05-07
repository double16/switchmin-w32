//////// AB CLASSGEN Sun Apr 06 15:37:04 1997 ////////
// FunctionSet Implementation
//////////////////////////////////////////////////////
#include "SwitchMinDLL_Global.h"
#define DLLBLD_SWSYSTEM_FUNCTIONSET
#include "FunctionSet.HPP"
#include <assert.h>

namespace swmin {

typedef std::set<Function, std::less<Function> > set_type;

// default constructor
FunctionSet::FunctionSet()
{
  _commonCtorJob();
  //...
}

// copy constructor
FunctionSet::FunctionSet(const FunctionSet& y)
 : set_type((set_type&) y)
{
  _commonCtorJob();
  //...
}

// assignment overload
FunctionSet& FunctionSet::operator=(const FunctionSet& y)
{
  invariant();
  if(this == &y) return *this;
  set_type::operator=(y);
  return *this;
}

// called from all constructors
void FunctionSet::_commonCtorJob()
{
  //...

}

// destructor
FunctionSet::~FunctionSet()
{
  //...
}



void FunctionSet::invariant()
{
  //...
}


IMPLEMENT_STREAMABLE(FunctionSet);

void FunctionSet::Streamer::Write(opstream& out) const
{
  out.writeWord32(GetObject()->size());
  iterator i = GetObject()->begin(), e = GetObject()->end();
  while (i!=e) {
    out << *i;
    i++;
  }
}

void *FunctionSet::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{
  GetObject()->erase(GetObject()->begin(), GetObject()->end());

  size_type count;
  Function x;
  count=in.readWord32();
  while (count--) {
    in >> x;
    GetObject()->insert(x);
  }

  return GetObject();
}

}

