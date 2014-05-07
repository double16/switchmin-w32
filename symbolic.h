//
// Classes for symbolic representations of switching functions
//
#ifndef Symbolic_h
#define Symbolic_h

#include <cstring.h>
#include <vector.h>

namespace swmin {

// Input descriptor
class DLLBLD_SWSYSTEM InputDesc : virtual TStreamableBase
{
  public:
    string name, desc;

    InputDesc();
    InputDesc(const string&, const string& = string(""));
    ~InputDesc();

    bool operator==(const InputDesc&) const;
    bool operator<(const InputDesc&) const;
DECLARE_STREAMABLE(DLLBLD_SWSYSTEM, InputDesc, 1);
};


#if defined(DLLUSE_SWSYSTEM) || defined(DLLBLD_SWSYSTEM_SYMBOLIC)
#pragma option -vi-
DLLBLD_STLVECTOR(DLLBLD_SWSYSTEM, swmin::InputDesc);
#pragma option -vi
#endif

class DLLBLD_SWSYSTEM InputVector : public std::vector<InputDesc>, virtual TStreamableBase
{
  public:
    InputVector();

    // Generates a naming sequence, prefixed by the string parm, from
    // indices specified.  Numbers are appended to each input name starting at
    // zero.
    void Gen_Sequence(const string&, unsigned int, unsigned int);
DECLARE_STREAMABLE(DLLBLD_SWSYSTEM, InputVector, 1);
};


template class DLLBLD_SWSYSTEM std::binary_function<string, string, bool>;

struct DLLBLD_SWSYSTEM stringLess : public std::binary_function<string, string, bool>
{
    bool operator() (const string& x, const string& y) const;
};


#if defined(DLLUSE_SWSYSTEM) || defined(DLLBLD_SWSYSTEM_SYMBOLIC)
#pragma option -vi-
DLLBLD_STLSET(DLLBLD_SWSYSTEM, string, swmin::stringLess);
#pragma option -vi
#endif

class DLLBLD_SWSYSTEM SymTerm : public std::set<string, stringLess >
{
  public:
    SymTerm();
    SymTerm(const SymTerm&);

    bool operator==(const SymTerm&) const;
    bool operator<(const SymTerm&) const;
};


#if defined(DLLUSE_SWSYSTEM) || defined(DLLBLD_SWSYSTEM_SYMBOLIC)
#pragma option -vi-
DLLBLD_STLSET(DLLBLD_SWSYSTEM, swmin::SymTerm, std::less<swmin::SymTerm>);
#pragma option -vi
#endif

class DLLBLD_SWSYSTEM SymExpr : public std::set<SymTerm, std::less<SymTerm> >
{
  public:
    SymExpr();
    SymExpr(const SymExpr&);
};

}

#endif

// EOF

