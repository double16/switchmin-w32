//
// Copyable DIB class
//
#ifndef PRINTABLEDIB
#define PRINTABLEDIB

#include <owl/applicat.h>
#include <owl/dibitmap.h>
#include <owl/dc.h>
#include <owl/gdiobjec.h>

class TPrintableDIB
{
  protected:
     TDib* dib;
     TDibDC* dc;
     TDiBitmap* bitmap;

  public:
    TPrintableDIB(TApplication*, TResId);
    ~TPrintableDIB();

    TDib* DIB() { return dib; }
    TDibDC* DC() { return dc; }
    TDiBitmap* BMP() { return bitmap; }
};

#endif

// EOF
