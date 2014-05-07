//----------------------------------------------------------------------------
//  Project SwitchMin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    SwitchMin Application
//  FILE:         switchminapp.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TSwitchMinApp (TApplication).
//
//----------------------------------------------------------------------------

#include "SwitchMinEXE_Global.h"

#include "minview.h"
#include "primesview.h"
#include "funcspecview.h"
#include "switchdoc.h"
#include "costtypedialog.h"
#include "MinFnBuildDialog.h"
#include <owl/buttonga.h>
#include <owl/statusba.h>
#include <owl/docmanag.h>

#include <classlib/cmdline.h>
#include <classlib/filename.h>
#include <time.h>

#include "switchminapp.h"
#include "switchminmdiclient.h"
#include "switchminmdichild.h"
#include "switchminaboutdlg.h"                        // Definition of about dialog.

TImageListBuildIconData FnSpecImgData[] = {
  {IDI_DESC,0},
  {IDI_INPUTS,0},
  {IDI_OUTPUTS,0},
  {IDI_OPTIONS,0},
  {0,0},
};

TImageListBuildIconData QMTermsImgData[] = {
  {IDI_DNF,0},
  {IDI_CNF,0},
  {0,0},
};

//
// Generated help file.
//
const char HelpFileName[] = "SwitchMin.hlp";


//{{TSwitchMinApp Implementation}}



//{{DOC_VIEW}}
DEFINE_DOC_TEMPLATE_CLASS(TSwitchDoc, TMinView, DocType1);
DEFINE_DOC_TEMPLATE_CLASS(TSwitchDoc, TPrimesView, DocType2);
DEFINE_DOC_TEMPLATE_CLASS(TSwitchDoc, TFuncSpecView, DocType3);
//{{DOC_VIEW_END}}

//{{DOC_MANAGER}}
DocType1 __dvt1("Switching Functions", "*.swf", 0, "swf", dtAutoDelete | dtHidden);
DocType2 __dvt2("Switching Functions", "*.swf", 0, "swf", dtAutoDelete | dtHidden);
DocType3 __dvt3("Switching Functions", "*.swf", 0, "swf", dtAutoDelete | dtAutoOpen | dtOverwritePrompt | dtHideReadOnly | dtCreatePrompt);
//{{DOC_MANAGER_END}}


//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE2(TSwitchMinApp, TRecentFiles, TApplication)
//{{TSwitchMinAppRSP_TBL_BEGIN}}
  EV_OWLVIEW(dnCreate, EvNewView),
  EV_OWLVIEW(dnClose,  EvCloseView),
  EV_COMMAND(CM_FILESEND, CmFileSend),
  EV_COMMAND_ENABLE(CM_FILESEND, CeFileSend),
  EV_COMMAND(CM_HELPABOUT, CmHelpAbout),
  EV_COMMAND(CM_HELPCONTENTS, CmHelpContents),
  EV_COMMAND(CM_HELPUSING, CmHelpUsing),
  EV_WM_DROPFILES,
  EV_WM_WININICHANGE,
  EV_OWLDOCUMENT(dnCreate, EvOwlDocument),
  EV_OWLDOCUMENT(dnRename, EvOwlDocument),
  EV_REGISTERED(MruFileMessage, CmFileSelected),
  EV_COMMAND(CM_CALCTERMS, CmCalcTerms),
  EV_COMMAND_ENABLE(CM_CALCTERMS, CeCalcTerms),
  EV_COMMAND(CM_SWFMINIMIZE, CmSwfMinimize),
  EV_COMMAND_ENABLE(CM_SWFMINIMIZE, CeSwfMinimize),
  EV_COMMAND(CM_REGISTER, CmRegister),
  EV_COMMAND(CM_WHATSTHIS, CmWhatsthis),
//{{TSwitchMinAppRSP_TBL_END}}
END_RESPONSE_TABLE;


//--------------------------------------------------------
// TSwitchMinApp
// ~~~~~
//
TSwitchMinApp::TSwitchMinApp() :
    TApplication("SwitchMin"), TRecentFiles(".\\SwitchMin.ini", 4),
    RegCop("Software\\DoubleVision\\Reg\\SwitchMin", "freeware", INT_MAX)
{
  HelpState = false;

  Printer = 0;
  Printing = 0;

  SetDocManager(new TDocManager(dmMDI, this));

  ApxMail = new TMailer();

  // INSERT>> Your constructor code here.

  // Image lists
  imglFnSpec=ImageListBuildIcon(FnSpecImgData, TSize(16,16), this);
  imglFnSpec->SetBkColor(TColor::Sys3dFace);
  imglQMTerms=ImageListBuildIcon(QMTermsImgData, TSize(16,16), this);
  imglQMTerms->SetBkColor(TColor::Sys3dFace);
}


TSwitchMinApp::~TSwitchMinApp()
{
  delete Printer;

  delete ApxMail;

  // INSERT>> Your destructor code here.
  delete imglFnSpec;
  delete imglQMTerms;
}


bool TSwitchMinApp::CanClose()
{
  bool result = TApplication::CanClose();

  // Close the help engine if we used it.
  //
  if (result && HelpState)
    GetMainWindow()->WinHelp(HelpFileName, HELP_QUIT, 0);

  return result;
}

void TSwitchMinApp::CreateGadgets(TDockableControlBar* cb, bool server)
{
  if (!server) {
    cb->Insert(*new TButtonGadget(CM_MDIFILENEW, CM_MDIFILENEW));
    cb->Insert(*new TButtonGadget(CM_MDIFILEOPEN, CM_MDIFILEOPEN));
    cb->Insert(*new TButtonGadget(CM_FILESAVE, CM_FILESAVE));
    cb->Insert(*new TSeparatorGadget(6));
  }

  cb->Insert(*new TButtonGadget(CM_EDITCUT, CM_EDITCUT));
  cb->Insert(*new TButtonGadget(CM_EDITCOPY, CM_EDITCOPY));
  cb->Insert(*new TButtonGadget(CM_EDITPASTE, CM_EDITPASTE));
  cb->Insert(*new TSeparatorGadget(6));
  cb->Insert(*new TButtonGadget(CM_EDITUNDO, CM_EDITUNDO));
  cb->Insert(*new TSeparatorGadget(6));
  cb->Insert(*new TButtonGadget(CM_EDITFIND, CM_EDITFIND));
  cb->Insert(*new TButtonGadget(CM_EDITFINDNEXT, CM_EDITFINDNEXT));
  cb->Insert(*new TSeparatorGadget(6));
  cb->Insert(*new TButtonGadget(IDB_TB_ADDFUNC, CM_ADDFUNC));
  cb->Insert(*new TButtonGadget(IDB_TB_DELFUNC, CM_DELFUNC));

  if (!server) {
    cb->Insert(*new TSeparatorGadget(6));
    cb->Insert(*new TButtonGadget(CM_FILEPRINT, CM_FILEPRINT));
    cb->Insert(*new TButtonGadget(CM_FILEPRINTPREVIEW, CM_FILEPRINTPREVIEW));
  }

  cb->Insert(*new TSeparatorGadget(6));
  cb->Insert(*new TButtonGadget(CM_HELPCONTENTS, CM_HELPCONTENTS));
  cb->Insert(*new TButtonGadget(CM_WHATSTHIS, CM_WHATSTHIS));

  // Add caption and fly-over help hints.
  //
  cb->SetCaption("Toolbar");
  cb->SetHintMode(TGadgetWindow::EnterHints);
}


void TSwitchMinApp::SetupSpeedBar(TDecoratedMDIFrame* frame)
{
  ApxHarbor = new THarbor(*frame);

  // Create default toolbar New and associate toolbar buttons with commands.
  //
  TDockableControlBar* cb = new TDockableControlBar(frame);
  CreateGadgets(cb);

  // Setup the toolbar ID used by OLE 2 for toolbar negotiation.
  //
  cb->Attr.Id = IDW_TOOLBAR;

  ApxHarbor->Insert(*cb, alTop);
}


//--------------------------------------------------------
// TSwitchMinApp
// ~~~~~
// Application main window construction & intialization.
//
void TSwitchMinApp::InitMainWindow()
{
  if (nCmdShow != SW_HIDE)
    nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWNORMAL : nCmdShow;

  MdiClient = new TSwitchMinMDIClient(this);
  TDecoratedMDIFrame* frame = new TDecoratedMDIFrame(Name, IDM_MDI, *MdiClient, true, this);

  // Enable acceptance of dropped files
  //
  frame->Attr.ExStyle |= WS_EX_ACCEPTFILES;

  nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWNORMAL : nCmdShow;

  // Assign icons for this application.
  //
  frame->SetIcon(this, IDI_MDIAPPLICATION);
  frame->SetIconSm(this, IDI_MDIAPPLICATION);

  // Associate with the accelerator table.
  //
  frame->Attr.AccelTable = IDM_MDI;

  TStatusBar* sb = new TStatusBar(frame, TGadget::Recessed,
                                  TStatusBar::CapsLock        |
                                  TStatusBar::NumLock         |
                                  TStatusBar::ScrollLock);
  frame->Insert(*sb, TDecoratedFrame::Bottom);

  SetupSpeedBar(frame);

  SetMainWindow(frame);

  frame->SetMenuDescr(TMenuDescr(IDM_MDI));

}


//--------------------------------------------------------
// TSwitchMinApp
// ~~~~~
// Application instance initialization.
//
void TSwitchMinApp::InitInstance()
{
  TApplication::InitInstance();

  ProcessCmdLine(lpCmdLine);
}


//--------------------------------------------------------
// TSwitchMinApp
// ~~~~~
// Process command line parameters.
//
void TSwitchMinApp::ProcessCmdLine(char * CmdLine)
{
  TCmdLine cmd(CmdLine);

  while (cmd.Kind != TCmdLine::Done) {
    if (cmd.Kind == TCmdLine::Option) {
      if (strnicmp(cmd.Token, "unregister", cmd.TokenLen) == 0) {
        UnRegisterInfo();
        return;
      }
    }
    cmd.NextToken();
  }

  RegisterInfo();
}


//--------------------------------------------------------
// TSwitchMinApp
// ~~~~~
// Register application info.
//
void TSwitchMinApp::RegisterInfo()
{
  TAPointer<char> buffer = new char[_MAX_PATH];
  TAPointer<char> ModuleFileName = new char[_MAX_PATH];

  GetModuleFileName(ModuleFileName, _MAX_PATH);

  TRegKey(TRegKey::ClassesRoot, "SwitchMin.Application\\DefaultIcon").SetDefValue(0, REG_SZ, ModuleFileName, strlen(ModuleFileName));

  //
  // Switching Function Document
   // Description
  strcpy(buffer, "Switching Function");
  TRegKey(TRegKey::ClassesRoot, "SwitchMin.Document.SwFunction").SetDefValue(0, REG_SZ, buffer, strlen(buffer));
   // Icon
  strcpy(buffer, ModuleFileName);
  strcat(buffer, ",1");
  TRegKey(TRegKey::ClassesRoot, "SwitchMin.Document.SwFunction\\DefaultIcon").SetDefValue(0, REG_SZ, buffer, strlen(buffer));
   // Open shell command
  sprintf(buffer, "\"%s\" \"%%1\"", ModuleFileName);
  TRegKey(TRegKey::ClassesRoot, "SwitchMin.Document.SwFunction\\shell\\Open\\command").SetDefValue(0, REG_SZ, buffer, strlen(buffer));
   // Extension link
  strcpy(buffer, "SwitchMin.Document.SwFunction");
  TRegKey(TRegKey::ClassesRoot, ".swf").SetDefValue(0, REG_SZ, buffer, strlen(buffer));
}


//--------------------------------------------------------
// TSwitchMinApp
// ~~~~~
// Unregister application info.
//
void TSwitchMinApp::UnRegisterInfo()
{
  TAPointer<char> buffer = new char[_MAX_PATH];

  GetModuleFileName(buffer, _MAX_PATH);

  TRegKey(TRegKey::ClassesRoot, "SwitchMin.Application").DeleteKey("DefaultIcon");
  TRegKey(TRegKey::ClassesRoot, "SwitchMin.Document.SwFunction").DeleteKey("DefaultIcon");

  TRegKey::ClassesRoot.DeleteKey("SwitchMin.Application");
  TRegKey::ClassesRoot.DeleteKey("SwitchMin.Document.SwFunction");
  TRegKey::ClassesRoot.DeleteKey(".swf");
}



//--------------------------------------------------------
// TSwitchMinApp
// ~~~~~
// Response Table handlers:
//
void TSwitchMinApp::EvNewView(TView& view)
{
  TMDIClient* mdiClient = TYPESAFE_DOWNCAST(GetMainWindow()->GetClientWindow(), TMDIClient);
  if (mdiClient) {
    TSwitchMinMDIChild* child = new TSwitchMinMDIChild(*mdiClient, 0, view.GetWindow());

    // Set the menu descriptor for this view.
    //
    if (view.GetViewMenu())
      child->SetMenuDescr(*view.GetViewMenu());

    // Assign icons with this child window.
    //
    child->SetIcon(this, IDI_DOC);
    child->SetIconSm(this, IDI_DOC);

    child->Create();
  }
}


void TSwitchMinApp::EvCloseView(TView&)
{
}


void TSwitchMinApp::CeFileSend(TCommandEnabler& ce)
{
  ce.Enable((GetDocManager()->GetCurrentDoc() != 0)
            && ApxMail->IsMAPIAvailable());
}


void TSwitchMinApp::CmFileSend ()
{
  // Check to see if a document exists
  //
  TDocument* currentDoc = GetDocManager()->GetCurrentDoc();

  if (currentDoc) {
    TAPointer<char> savedPath = new char[_MAX_PATH];
    TAPointer<char> docName = new char[_MAX_PATH];

    bool dirtyState = currentDoc->IsDirty();

    if (currentDoc->GetDocPath())
      strcpy(savedPath, currentDoc->GetDocPath());
    else
      strcpy(savedPath, "");

    if (currentDoc->GetTitle())
      strcpy(docName, currentDoc->GetTitle());
    else
      strcpy(docName, "Document");

    TFileName tempFile(TFileName::TempFile);

    currentDoc->SetDocPath(tempFile.Canonical().c_str());
    currentDoc->Commit(true);

    currentDoc->SetDocPath(savedPath);
    currentDoc->SetDirty(dirtyState);

    ApxMail->SendDocuments(GetMainWindow(), tempFile.Canonical().c_str(), docName, false);

    tempFile.Remove();
  }
}


//--------------------------------------------------------
// TSwitchMinApp
// ~~~~~
// Menu Help Contents command
//
void TSwitchMinApp::CmHelpContents()
{
  // Show the help table of contents.
  //
  HelpState = GetMainWindow()->WinHelp(HelpFileName, HELP_FINDER, 0);
}


//--------------------------------------------------------
// TSwitchMinApp
// ~~~~~
// Menu Help Using Help command
//
void TSwitchMinApp::CmHelpUsing()
{
  // Display the contents of the Windows help file.
  //
  HelpState = GetMainWindow()->WinHelp(HelpFileName, HELP_HELPONHELP, 0);
}


//--------------------------------------------------------
// TSwitchMinApp
// ~~~~~~~~~~~
// Menu Help About SwitchMin command
void TSwitchMinApp::CmHelpAbout()
{
  // Show the modal dialog.
  //
  TSwitchMinAboutDlg(GetMainWindow()).Execute();
}


void TSwitchMinApp::EvDropFiles(TDropInfo drop)
{
  TFileDropletList files;

  // Iterate thru the entries in drop and create FileDrops objects for each
  // one.
  //
  int fileCount = drop.DragQueryFileCount();  // Number of files dropped.
  for (int i = 0; i < fileCount; i++)
    files.Add(new TFileDroplet(drop, i));

  // Open the files that were dropped.
  //
  AddFiles(files);

  // Release the memory allocated for this handle with DragFinish.
  //
  drop.DragFinish();
}


void TSwitchMinApp::AddFiles(TFileDropletList& files)
{
  // Open all files dragged in.
  //
  for (TFileDropletListIter fileIter(files); fileIter; fileIter++) {
    TDocTemplate* tpl = GetDocManager()->MatchTemplate(fileIter.Current()->GetName());
    if (tpl)
      GetDocManager()->CreateDoc(tpl, fileIter.Current()->GetName());
  }
}


void TSwitchMinApp::EvOwlDocument(TDocument& doc)
{
  if (doc.GetDocPath())
    SaveMenuChoice(doc.GetDocPath());
}


int32 TSwitchMinApp::CmFileSelected(uint wp, int32)
{
  TAPointer<char> text = new char[_MAX_PATH];

  GetMenuText(wp, text, _MAX_PATH);
  TDocTemplate* tpl = GetDocManager()->MatchTemplate(text);
  if (tpl)
    GetDocManager()->CreateDoc(tpl, text);
  return 0;
}


bool TSwitchMinApp::CallHelp(uint command, uint32 data)
{
  bool result = GetMainWindow()->WinHelp(HelpFileName, command, data);
  if (command != HELP_CONTEXTPOPUP) HelpState=result;
  return result;
}


//
// Process application messages to provide context sensitive help
//
bool TSwitchMinApp::ProcessAppMsg(MSG& msg)
{
  /*
  if (msg.message == WM_KEYDOWN) {
    if (msg.wParam == VK_F1) {
      // If the Shift/F1 then set the context help cursor.
      //
      if (::GetKeyState(VK_SHIFT) < 0) {
        GetMainWindow()->SendMessage(WM_SYSCOMMAND, SC_CONTEXTHELP);
        return true;        // Gobble up the message.
      }
      else {
        // If F1 w/o the Shift key then bring up help's main index.
        //
        CallHelp(HELP_FINDER, 0);
        return true;        // Gobble up the message.
      }
    }
  }
  */

  // Continue normal processing.
  //
  return TApplication::ProcessAppMsg(msg);
}


void TSwitchMinApp::EvWinIniChange(char far* section)
{
  if (strcmp(section, "windows") == 0) {
    // If the device changed in the WIN.INI file then the printer
    // might have changed.  If we have a TPrinter(Printer) then
    // check and make sure it's identical to the current device
    // entry in WIN.INI.
    //
    if (Printer) {
      const int bufferSize = 255;
      char printDBuffer[bufferSize];
      LPSTR printDevice = printDBuffer;
      LPSTR devName;
      LPSTR driverName = 0;
      LPSTR outputName = 0;
      if (::GetProfileString("windows", "device", "", printDevice, bufferSize)) {
        // The string which should come back is something like:
        //
        //      HP LaserJet III,hppcl5a,LPT1:
        //
        // Where the format is:
        //
        //      devName,driverName,outputName
        //
        devName = printDevice;
        while (*printDevice) {
          if (*printDevice == ',') {
            *printDevice++ = 0;
            if (!driverName)
              driverName = printDevice;
            else
              outputName = printDevice;
          }
          else
            printDevice = ::AnsiNext(printDevice);
        }

        if (Printer->GetSetup().Error != 0                ||
            strcmp(devName, Printer->GetSetup().GetDeviceName()) != 0  ||
            strcmp(driverName, Printer->GetSetup().GetDriverName()) != 0 ||
            strcmp(outputName, Printer->GetSetup().GetOutputName()) != 0 ) {
          // New printer installed so get the new printer device now.
          //
          delete Printer;
          Printer = new TPrinter(this);
        }
      }
      else {
        // No printer installed(GetProfileString failed).
        //
        delete Printer;
        Printer = new TPrinter(this);
      }
    }
  }
}

/*
const char* TimeLockMsg =
 "This beta version of SwitchMin has expired. "
 "You may download the latest version of SwitchMin at http://incolor.inetnebr.com/double. ";
*/

int OwlMain(int , char* [])
{
  /*
  // check time lock
  struct tm LastDay = {0, 0, 0, 14, 6, 97};
  if (time(NULL) > mktime(&LastDay) ) { // time lock expired
    MessageBox(NULL, TimeLockMsg, "SwitchMin", MB_OK | MB_ICONEXCLAMATION);
    return 0;
  }
  */
  TSwitchMinApp   app;

  // If a valid reg code has not been entered, ask for code
  /*
  if ( (app.RegCop.Info.Type == ProductRegInfo::Invalid) ||
       (app.GetRegType() == ProductRegInfo::Invalid) ) {
    app.RegCop.Register();
    //if (app.RegCop.Register()==2) WinHelp(0, HelpFileName, HELP_CONTEXT, IDH_REGISTER);
  }

  if (app.GetRegType() == ProductRegInfo::Invalid) return 0;  // Cannot run
  */

  return app.Run();
}

void TSwitchMinApp::CmCalcTerms()
{
  TDocManager* docman=GetDocManager();
  TSwitchDoc* doc=TYPESAFE_DOWNCAST(docman->GetCurrentDoc(), TSwitchDoc);
  if (doc) docman->CreateView(*doc, &__dvt2);
}


void TSwitchMinApp::CeCalcTerms(TCommandEnabler &tce)
{
  TDocManager* docman=GetDocManager();
  TSwitchDoc* doc=TYPESAFE_DOWNCAST(docman->GetCurrentDoc(), TSwitchDoc);
  tce.Enable(doc!=NULL);
}


void TSwitchMinApp::CmSwfMinimize()
{
  TDocManager* docman=GetDocManager();
  TSwitchDoc* doc=TYPESAFE_DOWNCAST(docman->GetCurrentDoc(), TSwitchDoc);
  if (doc) {
   memset(&TCostTypeDialogData, 0, sizeof(TCostTypeDialogXfer));
   TCostTypeDialogData.Inputs=
    TCostTypeDialogData.InputsDNF=
    TCostTypeDialogData.InputsCNF=
    TCostTypeDialogData.LiteralsDNF=
    TCostTypeDialogData.LiteralsCNF=
    TCostTypeDialogData.TermsDNF=
    TCostTypeDialogData.TermsCNF=BF_CHECKED;
   if (TCostTypeDialog(GetMainWindow()).Execute()==IDOK) {
     TMinFnBuildDialog dlg(GetMainWindow());
     dlg.Create();

     if (TCostTypeDialogData.Inputs) {
       if (TCostTypeDialogData.InputsDNF) doc->MakeMinFn(swmin::Cost_Inputs, swmin::dnf, (swmin::SwSystemMinDisp*) &dlg);
       if (TCostTypeDialogData.InputsCNF) doc->MakeMinFn(swmin::Cost_Inputs, swmin::cnf, (swmin::SwSystemMinDisp*) &dlg);
     }
     if (TCostTypeDialogData.Literals) {
       if (TCostTypeDialogData.LiteralsDNF) doc->MakeMinFn(swmin::Cost_Literals, swmin::dnf, (swmin::SwSystemMinDisp*) &dlg);
       if (TCostTypeDialogData.LiteralsCNF) doc->MakeMinFn(swmin::Cost_Literals, swmin::cnf, (swmin::SwSystemMinDisp*) &dlg);
     }
     if (TCostTypeDialogData.Terms) {
       if (TCostTypeDialogData.TermsDNF) doc->MakeMinFn(swmin::Cost_Term, swmin::dnf, (swmin::SwSystemMinDisp*) &dlg);
       if (TCostTypeDialogData.TermsCNF) doc->MakeMinFn(swmin::Cost_Term, swmin::cnf, (swmin::SwSystemMinDisp*) &dlg);
     }

     if (!doc->MinViewActive()) docman->CreateView(*doc, &__dvt1);
   } // if
  } // if
}


void TSwitchMinApp::CeSwfMinimize(TCommandEnabler &tce)
{
  TDocManager* docman=GetDocManager();
  TSwitchDoc* doc=TYPESAFE_DOWNCAST(docman->GetCurrentDoc(), TSwitchDoc);
  tce.Enable(doc!=NULL);
}


void TSwitchMinApp::CmRegister()
{
  // INSERT>> Your code here.
  //if (RegCop.Register()==2) CallHelp(HELP_CONTEXT, IDH_REGISTER);
}


void TSwitchMinApp::CmWhatsthis()
{
  // INSERT>> Your code here.
  GetMainWindow()->SendMessage(WM_SYSCOMMAND, SC_CONTEXTHELP);
}

