//----------------------------------------------------------------------------
//  Project SwitchMin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    SwitchMin Application
//  FILE:         switchminaboutdlg.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TSwitchMinAboutDlg (TDialog).
//
//----------------------------------------------------------------------------
#if !defined(switchminaboutdlg_h)              // Sentry, use file only if it's not already included.
#define switchminaboutdlg_h

#include <owl/static.h>

#include "switchminapp.rh"                  // Definition of all resources.


//{{TDialog = TSwitchMinAboutDlg}}
class TSwitchMinAboutDlg : public TDialog {
  public:
    TSwitchMinAboutDlg(TWindow* parent, TResId resId = IDD_ABOUT, TModule* module = 0);
    virtual ~TSwitchMinAboutDlg();

//{{TSwitchMinAboutDlgVIRTUAL_BEGIN}}
  public:
    void SetupWindow();
//{{TSwitchMinAboutDlgVIRTUAL_END}}
};    //{{TSwitchMinAboutDlg}}


// Reading the VERSIONINFO resource.
//
class TProjectRCVersion {
  public:
    TProjectRCVersion(TModule* module);
    virtual ~TProjectRCVersion();

    bool GetProductName(LPSTR& prodName);
    bool GetProductVersion(LPSTR& prodVersion);
    bool GetCopyright(LPSTR& copyright);
    bool GetDebug(LPSTR& debug);

  protected:
    uint8 far*  TransBlock;
    void far*   FVData;

  private:
    // Don't allow this object to be copied.
    //
    TProjectRCVersion(const TProjectRCVersion&);
    TProjectRCVersion& operator = (const TProjectRCVersion&);
};


#endif  // switchminaboutdlg_h sentry.
