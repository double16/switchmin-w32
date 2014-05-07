//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         termviewdialog.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~
//  Source file for implementation of TTermViewDialog (TDialog).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include "termviewdialog.h"
#include "switchminapp.h"


//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TTermViewDialog, TPropWinDialog)
//{{TTermViewDialogRSP_TBL_BEGIN}}
  EV_BN_CLICKED(IDC_PRIMESONLY, BNClicked),
  EV_WM_SIZE,
  EV_LVN_COLUMNCLICK(IDC_TERMVIEWLIST, LVNColumnclick),
  EV_LVN_DELETEITEM(IDC_TERMVIEWLIST, LVNDeleteitem),
  EV_LVN_DELETEALLITEMS(IDC_TERMVIEWLIST, LVNDeleteallitems),
//{{TTermViewDialogRSP_TBL_END}}
  EV_WM_HELP,
END_RESPONSE_TABLE;

#define COLNUM_CELL  0
#define COLNUM_FUNC  1
#define COLNUM_PRIME 2

//{{TTermViewDialog Implementation}}


static TTermViewDialogXfer TTermViewDialogData;

TTermViewDialog::TTermViewDialog(TWindow* parent, TResId resId, TModule* module)
:
    TPropWinDialog(parent, resId, module)
{
//{{TTermViewDialogXFER_USE}}
  Terms = new TListWindow(this, IDC_TERMVIEWLIST);
  ShowPrimesOnly = new TCheckBox(this, IDC_PRIMESONLY, 0);

  SetTransferBuffer(&TTermViewDialogData);
//{{TTermViewDialogXFER_USE_END}}

  // INSERT>> Your constructor code here.

}


TTermViewDialog::~TTermViewDialog()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}

static DWORD aIDHs[] = {
  IDC_TERMVIEWLIST,IDH_dlg_TERMVIEW_lv_TERMS,
  IDC_PRIMESONLY, IDH_dlg_TERMVIEW_cb_PRIMES,
  0,0
};

void TTermViewDialog::EvHelp(HELPINFO* pHelpInfo)
{
  ::WinHelp( (HWND) pHelpInfo->hItemHandle, HelpFileName, HELP_WM_HELP, (DWORD) aIDHs);
}

void TTermViewDialog::BNClicked()
{
  // INSERT>> Your code here.
  ShowPrimesOnly->Toggle();
  LoadData(ShowPrimesOnly->GetCheck()==BF_CHECKED);
}


void TTermViewDialog::SetupWindow()
{
  TDialog::SetupWindow();

  // INSERT>> Your code here.
  // Add columns to list window
  TListWindColumn colCell("Cells", 100);
  TListWindColumn colFunc("Functions",80);
  TListWindColumn colPrime("Prime?",50);

  Terms->InsertColumn(COLNUM_CELL, colCell);
  Terms->InsertColumn(COLNUM_FUNC, colFunc);
  Terms->InsertColumn(COLNUM_PRIME, colPrime);

  ShowPrimesOnly->Check();
}

void TTermViewDialog::SetTerms(const swmin::TermSet& t, const StringVector& fname)
{
  TermCache=t;
  FuncsCache=fname;
  LoadData(ShowPrimesOnly->GetCheck()==BF_CHECKED);
}

void TTermViewDialog::LoadData(bool PrimeOnly)
{
  Terms->DeleteAllItems();

  swmin::TermSet::reverse_iterator i = TermCache.rbegin(), e = TermCache.rend();
  while (i!=e) {
    if ( (!PrimeOnly) || (*i).GetPrime() )
      if (!AddTerm(*i, FuncsCache)) throw TXOwl("Unable to add term");
    i++;
  }
  Terms->Update(-1);
}

bool TTermViewDialog::AddTerm(const swmin::Term& y, const StringVector& fname)
{
  TTermViewDisplay disp;

  GetTermString(y, fname, disp);

  TListWindSorter::TItemData* sort = Sorter.NewItem();

  // Cell terms
  //
  TListWindItem lwiCell(disp.cells.c_str(), COLNUM_CELL, disp.cells.length());
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_CELL, disp.cells));
  lwiCell.SetItemData((uint32) sort);
  int x=Terms->InsertItem(lwiCell);
  if (x<0) return false;

  // Functions term is included in
  //
  TListWindItem lwiFunc(disp.funcs.c_str(), COLNUM_FUNC, disp.funcs.length());
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_FUNC, disp.funcs));
  lwiFunc.SetIndex(x);
  Terms->SetItem(lwiFunc);

  // Prime indicator
  //
  TListWindItem lwiPrime(disp.prime.c_str(), COLNUM_PRIME, disp.prime.length());
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_PRIME, disp.prime));
  lwiPrime.SetIndex(x);
  Terms->SetItem(lwiPrime);

  return true;
}

void TTermViewDialog::GetTermString(const swmin::Term& y, const StringVector& fname, TTermViewDisplay& disp)
{
  char pch[128];
  ostrstream s(pch, sizeof(pch));

  // Cell terms
  //
  s.seekp(0);
  s << y << ends;
  disp.cells=pch;

  // Functions term is included in
  //
  s.seekp(0);
  unsigned long bits=y.funcs.getBits();
  for(unsigned int j=0; j<bits; j++) {
    if (y.funcs[j]) s << fname[j] << ',';
  }
  if (s.tellp()>0) s.seekp(s.tellp()-1);  // remove trailing comma
  s << ends;
  disp.funcs=pch;

  // Prime indicator
  //
  disp.prime = (y.Prime) ? "Yes":"No";
}

void TTermViewDialog::EvSize(uint sizeType, TSize& size)
{
    TDialog::EvSize(sizeType, size);

  // INSERT>> Your code here.

  // Resize terms list window
  TRect rscrTerms = Terms->GetWindowRect(), rDlg = GetClientRect();
  TPoint pTermsTopLeft=rscrTerms.TopLeft();
  ScreenToClient(pTermsTopLeft);
  TRect rTerms(pTermsTopLeft, rDlg.BottomRight());
  Terms->SetWindowPos(0, rTerms, SWP_NOZORDER | SWP_SHOWWINDOW);

}


void TTermViewDialog::LVNColumnclick(TLwNotify& lwn)
{
  // INSERT>> Your code here.
  Sorter.Alternate(lwn.iSubItem);
  Terms->SortItems(Sorter, lwn.iSubItem);
}


void TTermViewDialog::LVNDeleteitem(TLwNotify& /*lwn*/)
{
  // INSERT>> Your code here.
  /*
  TListWindItem item;
  item.mask=LVIF_PARAM;

  for(unsigned int i=0; i<=1; i++) {
    if (!Terms->GetItem(item, lwn.iItem, i)) throw TXOwl("Unable to delete item data.");
    delete[] (char*) item.lParam;
  }
  */
}


void TTermViewDialog::LVNDeleteallitems(TLwNotify& /*lwn*/)
{
  // INSERT>> Your code here.
  /*
  unsigned int count=Terms->GetItemCount();
  TLwNotify l;
  for(unsigned int i=0; i<count; i++) {
    l.iItem=i;
    LVNDeleteitem(l);
  }
  */
}

