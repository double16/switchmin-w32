//
// Function to Text (and vice versa) class
//
// Allows a text representation of Function
//

#include "SwitchMinDLL_Global.h"
#include "Function.HPP"

namespace swmin {

class DLLBLD_SWSYSTEM FuncText
{
 public:
  string id, desc, inputs, mode, terms, dc, HazardFree;
  bool Set(const Function&);  // converts function to a text representation
  bool Get(Function&);  // converts a text rep to a function object
 protected:
  bool TextToTerms(const string&, TermSet&);
  bool TermsToText(const TermSet&, string&);
};

}

// EOF
