//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         funcspecfuncsdialog.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TFuncSpecFuncsDialog (TDialog).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include <mem.h>
#include "funcspecfuncsdialog.h"
#include <vector.h>
#include "FuncSpecView.h"
#include "switchminapp.h"

using namespace swmin;

//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TFuncSpecFuncsDialog, TPropWinDialog)
//{{TFuncSpecFuncsDialogRSP_TBL_BEGIN}}
  EV_WM_SIZE,
  EV_LVN_COLUMNCLICK(IDC_FSFUNCS, LVNColumnClick),
  EV_NM_DBLCLK(IDC_FSFUNCS, LBNDblClk),
  EV_NM_RETURN(IDC_FSFUNCS, LBNDblClk),
//{{TFuncSpecFuncsDialogRSP_TBL_END}}
  EV_WM_HELP,
END_RESPONSE_TABLE;


// Column numbers for list window
#define COLNUM_ID 0
#define COLNUM_DESC 1
#define COLNUM_FORM 2
#define COLNUM_TERMS 3
#define COLNUM_DC 4
//#define COLNUM_INPUTS 5
//#define COLNUM_HAZ 6


//{{TFuncSpecFuncsDialog Implementation}}


static TFuncSpecFuncsDialogXfer TFuncSpecFuncsDialogData;

TFuncSpecFuncsDialog::TFuncSpecFuncsDialog(TWindow* parent, TResId resId, TModule* module)
:
    TPropWinDialog(parent, resId, module)
{
//{{TFuncSpecFuncsDialogXFER_USE}}
  Functions = new TListWindow(this, IDC_FSFUNCS);

  SetTransferBuffer(&TFuncSpecFuncsDialogData);
//{{TFuncSpecFuncsDialogXFER_USE_END}}

  // INSERT>> Your constructor code here.
  view = TYPESAFE_DOWNCAST(parent, TFuncSpecView);

}


TFuncSpecFuncsDialog::~TFuncSpecFuncsDialog()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.
}


void TFuncSpecFuncsDialog::EvHelp(HELPINFO* pHelpInfo)
{
  ((TSwitchMinApp*) GetApplication())->CallHelp(HELP_CONTEXT, IDH_vw_FS_MAIN);
}


void TFuncSpecFuncsDialog::EvSize(uint sizeType, TSize& size)
{
    TDialog::EvSize(sizeType, size);

  // INSERT>> Your code here.
  Functions->SetWindowPos(0, TRect(TPoint(0,0), size), SWP_SHOWWINDOW | SWP_NOZORDER);
}


void TFuncSpecFuncsDialog::SetupWindow()
{
  TDialog::SetupWindow();

  // INSERT>> Your code here.

  // Add columns to list window
  TListWindColumn colID("Name", 40);
  TListWindColumn colDesc("Description",80);
  TListWindColumn colForm("Form",40);
  TListWindColumn colTerms("Terms",100);
  TListWindColumn colDC("Don't Cares",100);
//  TListWindColumn colInputs("Inputs",50);
//  TListWindColumn colHazardFree("Haz (Hazard Free)",35);

  Functions->InsertColumn(COLNUM_ID, colID);
  Functions->InsertColumn(COLNUM_DESC, colDesc);
  Functions->InsertColumn(COLNUM_FORM, colForm);
  Functions->InsertColumn(COLNUM_TERMS, colTerms);
  Functions->InsertColumn(COLNUM_DC, colDC);
//  Functions->InsertColumn(COLNUM_INPUTS, colInputs);
//  Functions->InsertColumn(COLNUM_HAZ, colHazardFree);

}

void TFuncSpecFuncsDialog::SetFunctions(const FunctionSet& fs)
{
  Functions->DeleteAllItems();
  Sorter.Clear();

  FunctionSet::iterator i=fs.begin(), e=fs.end();
  while (i!=e) AddFunctionToList(*i++);
  Functions->Update(-1);
}

void TFuncSpecFuncsDialog::AddFunction(const Function& f)
{
  AddFunctionToList(f);
  Functions->Update(-1);
}

void TFuncSpecFuncsDialog::AddFunction(const FuncText& ft)
{
  AddFunctionToList(ft);
  Functions->Update(-1);
}

void TFuncSpecFuncsDialog::DelFunction(const Function& f)
{
  FuncText t;
  t.Set(f);
  DelFunction(t);
}

void TFuncSpecFuncsDialog::DelFunction(const FuncText& ft)
{
  string ID;
  int sel;

  for (sel=Functions->GetNextItem(-1, TListWindow::All);
       sel>=0;
       sel=Functions->GetNextItem(sel, TListWindow::All) ) {

    if (GetIDFromListWind(sel, ID)) {
      if (ID==ft.id) {
        Functions->DeleteAnItem(sel);
        Functions->Update(-1);
        return;
      } // if
    } // if

  } // for
}

void TFuncSpecFuncsDialog::ReplaceFunction(const Function& f)
{
  FuncText t;
  t.Set(f);
  ReplaceFunction(t);
}

void TFuncSpecFuncsDialog::ReplaceFunction(const FuncText& ft)
{
  DelFunction(ft);
  AddFunction(ft);
}


void TFuncSpecFuncsDialog::AddFunctionToList(const Function& f)
{
  FuncText t;
  t.Set(f);
  AddFunctionToList(t);
}

void TFuncSpecFuncsDialog::AddFunctionToList(const FuncText& t)
{
  TListWindSorter::TItemData* sort=Sorter.NewItem();
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_ID, t.id));
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_DESC, t.desc));
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_FORM, t.mode));
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_TERMS, t.terms));
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_DC, t.dc));


  TListWindItem lwiID(t.id.c_str(), COLNUM_ID, t.id.length());
  lwiID.SetItemData((uint32) sort);
  int i=Functions->InsertItem(lwiID);
  if (i<0) throw TXOwl("Unable to add function");

  TListWindItem lwiDesc(t.desc.c_str(), COLNUM_DESC, t.desc.length());
  lwiDesc.SetIndex(i);
  Functions->SetItem(lwiDesc);

  TListWindItem lwiMode(t.mode.c_str(), COLNUM_FORM, t.mode.length());
  lwiMode.SetIndex(i);
  Functions->SetItem(lwiMode);

  TListWindItem lwiTerms(t.terms.c_str(), COLNUM_TERMS, t.terms.length());
  lwiTerms.SetIndex(i);
  Functions->SetItem(lwiTerms);

  TListWindItem lwiDC(t.dc.c_str(), COLNUM_DC, t.dc.length());
  lwiDC.SetIndex(i);
  Functions->SetItem(lwiDC);

  /*
  TListWindItem lwiInputs(t.inputs.c_str(), COLNUM_INPUTS, t.inputs.length());
  lwiInputs.SetIndex(i);
  Functions->SetItem(lwiInputs);

  TListWindItem lwiHaz(t.HazardFree.c_str(), COLNUM_HAZ, t.HazardFree.length());
  lwiHaz.SetIndex(i);
  Functions->SetItem(lwiHaz);
  */
}


void TFuncSpecFuncsDialog::LVNColumnClick(TLwNotify& lwn)
{
  // INSERT>> Your code here.
  Sorter.Alternate(lwn.iSubItem);
  Functions->SortItems(Sorter, lwn.iSubItem);
}

bool TFuncSpecFuncsDialog::GetIDFromListWind(int sel, string& s)
{
  TAPointer<char> buffer = new char[255];

  TListWindItem item;
  item.mask=LVIF_TEXT;
  item.pszText=buffer;
  item.cchTextMax=255;
  item.iItem=sel;
  item.iSubItem=0;
  if (!Functions->GetItem(item)) return false;
  s=(char*) buffer;
  return true;
}

void TFuncSpecFuncsDialog::LBNDblClk()
{
  if (view)
    view->PostMessage(WM_COMMAND, MAKEWPARAM(CM_EDITFUNC, 0), NULL);
}



