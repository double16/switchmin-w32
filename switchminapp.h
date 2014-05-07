//----------------------------------------------------------------------------
//  Project SwitchMin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    SwitchMin Application
//  FILE:         switchminapp.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TSwitchMinApp (TApplication).
//
//----------------------------------------------------------------------------
#if !defined(switchminapp_h)              // Sentry, use file only if it's not already included.
#define switchminapp_h

#include <owl/controlb.h>
#include <owl/docking.h>
#include <owl/mailer.h>
#include <owl/printer.h>
#include <owl/rcntfile.h>

#include <classlib/bags.h>

#include "switchminmdiclient.h"

#include "switchminapp.rh"            // Definition of all resources.
#include "regcop.h"

extern const char HelpFileName[];

//{{TApplication = TSwitchMinApp}}
class TSwitchMinApp : public TApplication, public TRecentFiles {
  private:
    bool            HelpState;                          // Has the help engine been used.

    void SetupSpeedBar(TDecoratedMDIFrame* frame);
    void AddFiles(TFileDropletList& files);
    void ProcessCmdLine(char * CmdLine);
    void RegisterInfo();
    void UnRegisterInfo();

  public:
    TRegCop RegCop;

    TSwitchMinApp();
    virtual ~TSwitchMinApp();

    TImageList *imglFnSpec, *imglQMTerms;
    bool IsStudentVersion() { return false; }
    bool CallHelp(uint command, uint32 data);
    int GetRegType() const { return RegCop.Type(); }

    void CreateGadgets(TDockableControlBar* cb, bool server = false);
    THarbor*        ApxHarbor;

    TSwitchMinMDIClient* MdiClient;

    // Public data members used by the print menu commands and Paint routine in MDIChild.
    //
    TPrinter*       Printer;                           // Printer support.
    int             Printing;                          // Printing in progress.

    TMailer*        ApxMail;

//{{TSwitchMinAppVIRTUAL_BEGIN}}
  public:
    virtual void InitMainWindow();
    virtual void InitInstance();
    virtual bool CanClose();
    virtual bool ProcessAppMsg(MSG& msg);
//{{TSwitchMinAppVIRTUAL_END}}

//{{TSwitchMinAppRSP_TBL_BEGIN}}
  protected:
    void EvNewView(TView& view);
    void EvCloseView(TView& view);
    void CeFileSend(TCommandEnabler& ce);
    void CmFileSend();
    void CmHelpAbout();
    void CmHelpContents();
    void CmHelpUsing();
    void EvDropFiles(TDropInfo drop);
    void EvWinIniChange(char far* section);
    void EvOwlDocument(TDocument& doc);
    int32 CmFileSelected(uint wp, int32 lp);
    void CmCalcTerms();
    void CeCalcTerms(TCommandEnabler &tce);
    void CmSwfMinimize();
    void CeSwfMinimize(TCommandEnabler &tce);
    void CmRegister();
    void CmWhatsthis();
//{{TSwitchMinAppRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSwitchMinApp);
};    //{{TSwitchMinApp}}


#endif  // switchminapp_h sentry.
