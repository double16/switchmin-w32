//----------------------------------------------------------------------------
//  Project SwitchMin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    SwitchMin Application
//  FILE:         switchminmdiclient.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TSwitchMinMDIClient (TMDIClient).
//
//----------------------------------------------------------------------------
#if !defined(switchminmdiclient_h)              // Sentry, use file only if it's not already included.
#define switchminmdiclient_h

#include "switchminapp.rh"            // Definition of all resources.


//{{TMDIClient = TSwitchMinMDIClient}}
class TSwitchMinMDIClient : public TMDIClient {
  public:
    int      ChildCount;                 // Number of child window created.

    TSwitchMinMDIClient(TModule* module = 0);
    virtual ~TSwitchMinMDIClient();

    void OpenFile(const char* fileName = 0);

//{{TSwitchMinMDIClientVIRTUAL_BEGIN}}
  protected:
    virtual void SetupWindow();
//{{TSwitchMinMDIClientVIRTUAL_END}}

//{{TSwitchMinMDIClientRSP_TBL_BEGIN}}
  protected:
    void CmFilePrint();
    void CmFilePrintSetup();
    void CmFilePrintPreview();
    void CmPrintEnable(TCommandEnabler& tce);
    void EvDropFiles(TDropInfo);
//{{TSwitchMinMDIClientRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSwitchMinMDIClient);
};    //{{TSwitchMinMDIClient}}


#endif  // switchminmdiclient_h sentry.
