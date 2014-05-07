#include "SwitchMinDLL_Global.h"
#define DLLBLD_SWSYSTEM_SYMBOLIC
#include "symbolic.h"

namespace swmin {

InputDesc::InputDesc()
{
}

InputDesc::InputDesc(const string& n, const string& d)
 : name(n), desc(d)
{
}
InputDesc::~InputDesc()
{
}

bool InputDesc::operator==(const InputDesc& y) const
{
  return name==y.name;
}

bool InputDesc::operator<(const InputDesc& y) const
{
  return name<y.name;
}

IMPLEMENT_STREAMABLE(InputDesc);

void *InputDesc::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{
  char *pch;

  pch=in.readString();
  GetObject()->name=string(pch);
  delete[] pch;

  pch=in.readString();
  GetObject()->desc=string(pch);
  delete[] pch;

  return GetObject();
}

void InputDesc::Streamer::Write(opstream& out) const
{
  out.writeString(GetObject()->name.c_str());
  out.writeString(GetObject()->desc.c_str());
}

void InputVector::Gen_Sequence(const string& prefix, unsigned int beg, unsigned int last)
{
  InputDesc input;

  unsigned int num=0;
  char buffer[64];
  for(unsigned int i=beg; i<=last; i++, num++) {
    ltoa(num, buffer, 10);
    if ( i < size() ) at(i).name = prefix+buffer;
    else {
      input.name=prefix+buffer;
      push_back(input);
    }
  }
}

IMPLEMENT_STREAMABLE(InputVector);

InputVector::InputVector()
 : std::vector<InputDesc>()
{

}


void *InputVector::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{
  GetObject()->erase(GetObject()->begin(), GetObject()->end());

  size_type count, i=0;
  InputDesc x;
  count=in.readWord32();
  GetObject()->resize(count);
  while (count--) {
    in >> x;
    GetObject()->at(i++)=x;
  }

  return GetObject();
}

void InputVector::Streamer::Write(opstream& out) const
{
  out.writeWord32(GetObject()->size());
  iterator i = GetObject()->begin(), e = GetObject()->end();
  while (i!=e) {
    out << *i;
    i++;
  }
}


bool stringLess::operator()(const string& x, const string& y) const
{
  return x<y;
}

namespace swmin {
typedef std::set<string, stringLess > SymTerm_type;
}

SymTerm::SymTerm() : SymTerm_type() {}

SymTerm::SymTerm(const SymTerm& y) : SymTerm_type(y) {}

bool SymTerm::operator==(const SymTerm& y) const
{
  if (size()!=y.size()) return false;
  SymTerm::iterator i1, i2, e;
  for(i1=begin(), i2=y.begin(), e=end(); i1!=e; i1++, i2++) {
    if ( *i1 != *i2 ) return false;
  }
  return true;
}

bool SymTerm::operator<(const SymTerm& y) const
{
  if (size()!=y.size()) return size() < y.size();
  SymTerm::iterator i1, i2, e;
  for(i1=begin(), i2=y.begin(), e=end(); i1!=e; i1++, i2++) {
    if ( *i1 != *i2 ) return stringLess()(*i1, *i2);
  }
  return false;
}

namespace swmin {
typedef std::set<SymTerm, std::less<SymTerm> > SymExpr_type;
}

SymExpr::SymExpr() : SymExpr_type() {}

SymExpr::SymExpr(const SymExpr& y) : SymExpr_type(y) {}

}

