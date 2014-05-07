//
// Circuit CAD Library
//

#include "DVCIRCAD_Global.h"
#include "dvcircad.h"

TModule* ResMod = NULL;
TFont *CadFontNorm = NULL, *CadFontSub=NULL;
TFont *CadScaledFontNorm = NULL, *CadScaledFontSub = NULL;
float CadScaledFontValue=0.0;

void ScaleFonts(float scale)
{
  if (scale==CadScaledFontValue) return;

  // Delete old fonts
  delete CadScaledFontNorm;
  delete CadScaledFontSub;

  LOGFONT logfont;

  // Create normal scaled font
  CadFontNorm->GetObject(logfont);
  logfont.lfWidth = (float) logfont.lfWidth * scale;
  logfont.lfHeight = (float) logfont.lfHeight * scale;
  CadScaledFontNorm=new TFont(&logfont);

  // Create subscript scaled font
  CadFontSub->GetObject(logfont);
  logfont.lfWidth = (float) logfont.lfWidth * scale;
  logfont.lfHeight = (float) logfont.lfHeight * scale;
  CadScaledFontSub=new TFont(&logfont);

  CadScaledFontValue=scale;
}

TFont* GetScaledFontNorm(float scale)
{
  ScaleFonts(scale);
  return CadScaledFontNorm;
}

TFont* GetScaledFontSub(float scale)
{
  ScaleFonts(scale);
  return CadScaledFontSub;
}


BOOL WINAPI
  DllEntryPoint(HINSTANCE instance, DWORD reason, LPVOID)
{
  switch (reason) {
    case DLL_PROCESS_ATTACH:
      ResMod=new TModule("DVCIRCAD", instance, NULL);
      CadFontNorm=new TFont("Courier New",  8, 5);
      CadFontSub=new TFont("Courier New", 4, 3);
      break;

    case DLL_PROCESS_DETACH:
      delete ResMod;
      delete CadFontNorm;
      delete CadFontSub;
      delete CadScaledFontNorm;
      delete CadScaledFontSub;
      break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
      break;
  }

  return true;
}


// EOF
