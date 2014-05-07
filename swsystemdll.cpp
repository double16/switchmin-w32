//
//  DLL startup/shutdown code
//

#include "SwitchMinDLL_Global.H"
#include <windows.h>
#include "SWSYSTEMDLL.H"

//
// DLL Entry Point function
//
BOOL WINAPI
  DllEntryPoint(HINSTANCE /*instance*/, DWORD reason, LPVOID)
{
  switch (reason) {
    case DLL_PROCESS_ATTACH:
    case DLL_PROCESS_DETACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
      break;
  }

  return true;

}

// EOF

