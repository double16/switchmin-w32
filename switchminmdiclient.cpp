//----------------------------------------------------------------------------
//  Project SwitchMin
//  DoubleVision, LTD
//  Copyright � 1997. All Rights Reserved.
//
//  SUBSYSTEM:    SwitchMin Application
//  FILE:         switchminmdiclient.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TSwitchMinMDIClient (TMDIClient).
//
//----------------------------------------------------------------------------

#include "SwitchMinEXE_Global.h"

#include <owl/docmanag.h>
#include <owl/listbox.h>
#include <stdio.h>

#include "switchminapp.h"
#include "switchminmdichild.h"
#include "switchminmdiclient.h"
#include "apxprint.h"
#include "apxprev.h"


//{{TSwitchMinMDIClient Implementation}}


//
// Build a response table for all messages/commands handled
// by TSwitchMinMDIClient derived from TMDIClient.
//
DEFINE_RESPONSE_TABLE1(TSwitchMinMDIClient, TMDIClient)
//{{TSwitchMinMDIClientRSP_TBL_BEGIN}}
  EV_COMMAND(CM_FILEPRINT, CmFilePrint),
  EV_COMMAND(CM_FILEPRINTERSETUP, CmFilePrintSetup),
  EV_COMMAND(CM_FILEPRINTPREVIEW, CmFilePrintPreview),
  EV_COMMAND_ENABLE(CM_FILEPRINT, CmPrintEnable),
  EV_COMMAND_ENABLE(CM_FILEPRINTERSETUP, CmPrintEnable),
  EV_COMMAND_ENABLE(CM_FILEPRINTPREVIEW, CmPrintEnable),
  EV_WM_DROPFILES,
//{{TSwitchMinMDIClientRSP_TBL_END}}
END_RESPONSE_TABLE;


//--------------------------------------------------------
// TSwitchMinMDIClient
// ~~~~~~~~~~~
// Construction/Destruction handling.
//
TSwitchMinMDIClient::TSwitchMinMDIClient(TModule* module)
:
  TMDIClient(module)
{
  ChildCount = 0;

  // INSERT>> Your constructor code here.

}


TSwitchMinMDIClient::~TSwitchMinMDIClient()
{
  Destroy();

  // INSERT>> Your destructor code here.

}


//--------------------------------------------------------
// TSwitchMinMDIClient
// ~~~~~~~~~~~
// MDIClient site initialization.
//
void TSwitchMinMDIClient::SetupWindow()
{
  // Default SetUpWindow processing.
  //
  TMDIClient::SetupWindow();

  // Accept files via drag/drop in the client window.
  //
  DragAcceptFiles(true);
}


//--------------------------------------------------------
// TSwitchMinMDIClient
// ~~~~~~~~~~
// Menu File Print command
//
void TSwitchMinMDIClient::CmFilePrint()
{
  // Create Printer object if not already created.
  //
  TSwitchMinApp* theApp = TYPESAFE_DOWNCAST(GetApplication(), TSwitchMinApp);
  if (theApp) {
    if (!theApp->Printer)
      theApp->Printer = new TPrinter(theApp);

    TAPointer<char> docName = new char[_MAX_PATH];

    TDocument* currentDoc = theApp->GetDocManager()->GetCurrentDoc();
    if (currentDoc->GetTitle())
      strcpy(docName, currentDoc->GetTitle());
    else
      strcpy(docName, "Document");

    // Create Printout window and set characteristics.
    //
    TApxPrintout printout(theApp->Printer, docName, GetActiveMDIChild()->GetClientWindow(), true);

    theApp->Printing++;

    // Bring up the Print dialog and print the document.
    //
    theApp->Printer->Print(GetWindowPtr(GetActiveWindow()), printout, true);

    theApp->Printing--;
  }
}


//--------------------------------------------------------
// TSwitchMinMDIClient
// ~~~~~~~~~~
// Menu File Print Setup command
//
void TSwitchMinMDIClient::CmFilePrintSetup()
{
  TSwitchMinApp* theApp = TYPESAFE_DOWNCAST(GetApplication(), TSwitchMinApp);
  if (theApp) {
    if (!theApp->Printer)
      theApp->Printer = new TPrinter(theApp);

    // Bring up the Print Setup dialog.
    //
    theApp->Printer->Setup(this);
  }
}


//--------------------------------------------------------
// TSwitchMinMDIClient
// ~~~~~~~~~~
// Menu File Print Preview command
//
void TSwitchMinMDIClient::CmFilePrintPreview()
{
  TSwitchMinApp* theApp = TYPESAFE_DOWNCAST(GetApplication(), TSwitchMinApp);
  if (theApp) {
    if (!theApp->Printer)
      theApp->Printer = new TPrinter(GetApplication());

    theApp->Printing++;

    TApxPreviewWin* prevW = new TApxPreviewWin(Parent, theApp->Printer, GetActiveMDIChild()->GetClientWindow(), "Print Preview", new TLayoutWindow(0));
    prevW->Create();

    // Here we resize the preview window to take the size of the MainWindow, then
    // hide the MainWindow.
    //
    TFrameWindow * mainWindow =  GetApplication()->GetMainWindow();
    TRect r = mainWindow->GetWindowRect();
    prevW->MoveWindow(r);
    prevW->ShowWindow(SW_SHOWNORMAL);
    mainWindow->ShowWindow(SW_HIDE);

    GetApplication()->BeginModal(GetApplication()->GetMainWindow());

    // After the user closes the preview Window, we take its size and use it
    // to size the MainWindow, then show the MainWindow again.
    //
    r = prevW->GetWindowRect();
    mainWindow->MoveWindow(r);
    mainWindow->ShowWindow(SW_SHOWNORMAL);

    // We must destroy the preview window explicitly.  Otherwise, the window will
    // not be destroyed until it's parent the MainWindow is destroyed.
    //
    prevW->Destroy();
    delete prevW;

    theApp->Printing--;
  }
}


//--------------------------------------------------------
// TSwitchMinMDIClient
// ~~~~~~~~~~
// Menu enabler used by Print, Print Setup and Print Preview.
//
void TSwitchMinMDIClient::CmPrintEnable(TCommandEnabler& tce)
{
  if (GetActiveMDIChild()) {
    TSwitchMinApp* theApp = TYPESAFE_DOWNCAST(GetApplication(), TSwitchMinApp);
    if (theApp) {
      // If we have a Printer already created just test if all is okay.
      // Otherwise create a Printer object and make sure the printer really
      // exists and then delete the Printer object.
      //
      if (!theApp->Printer) {
        theApp->Printer = new TPrinter(theApp);
        tce.Enable(!theApp->Printer->GetSetup().Error);
      }
      else
        tce.Enable(!theApp->Printer->GetSetup().Error);
    }
  }
  else
    tce.Enable(false);
}


void TSwitchMinMDIClient::EvDropFiles(TDropInfo)
{
  Parent->ForwardMessage();
}
