//
// Function to Text (and vice versa) class
//
// Allows a text representation of Function
//

#include "SwitchMinDLL_Global.h"
#include "FuncText.h"

#include <stdlib.h>
#include <ctype.h>
#include <strstrea.h>

namespace swmin {

bool FuncText::Set(const Function& f)
{
  static char pch[1024];
  strstream s(pch, 1024, ios::out);

  id=f.id;
  desc=f.desc;

  s.seekp(0);
  s << f.inputs << ends;
  inputs=string(pch);

  mode= (f.mode==dnf) ? "dnf":"cnf";

  if (!TermsToText(f.terms, terms)) return false;
  if (!TermsToText(f.dc, dc)) return false;

  return true;
}

bool FuncText::Get(Function& f)
{
  f.id=id;
  f.desc=desc;
  f.inputs=atol(inputs.c_str());

  if (mode=="dnf") f.mode=dnf;
  else if (mode=="cnf") f.mode=cnf;
  else return false;


  if (!TextToTerms(terms, f.terms)) return false;
  if (!TextToTerms(dc, f.dc)) return false;

  return true;
}

bool FuncText::TextToTerms(const string& s, TermSet& t)
{
  char* pch=new char[s.length()+1];
  strncpy(pch, s.c_str(), s.length()+1);
  istrstream str(pch);
  str >> t;
  delete[] pch;

  return true;
}

bool FuncText::TermsToText(const TermSet& t, string& s)
{
  static char pch[1024];
  ostrstream str(pch, 1024);

  str << t << ends;
  s=pch;

  return true;
}

}


