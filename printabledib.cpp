//
// Copyable DIB class
//

#include "SwitchMinEXE_Global.h"

#include "PrintableDIB.h"


TPrintableDIB::TPrintableDIB(TApplication* app, TResId resid)
{
  dib=   new TDib(*app, resid);
  dc=    new TDibDC;
  bitmap=new TDiBitmap(*dib);
  dc->SelectObject(*bitmap);
  dc->StretchDIBits(TRect(0, 0, bitmap->Width(), bitmap->Height()),
                          TRect(0, 0, bitmap->Width(), bitmap->Height()),
                          *dib);
}

TPrintableDIB::~TPrintableDIB()
{
  delete bitmap;
  delete dc;
  delete dib;
}

// EOF
