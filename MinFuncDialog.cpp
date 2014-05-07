//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         minfuncdialog.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TMinFuncDialog (TDialog).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include "minfuncdialog.h"
#include "minview.h"
#include "switchminapp.h"

using namespace swmin;

//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TMinFuncDialog, TDialog)
//{{TMinFuncDialogRSP_TBL_BEGIN}}
  EV_BN_CLICKED(IDC_MINFUNCANDOR, Design_AndOR),
  EV_BN_CLICKED(IDC_MINFUNCBOOL, Design_BoolExpr),
  EV_BN_CLICKED(IDC_MINFUNCNAND, Design_NAND),
  EV_BN_CLICKED(IDC_MINFUNCNOR, Design_NOR),
  EV_BN_CLICKED(IDC_MINFUNCZIN, ZoomIn),
  EV_BN_CLICKED(IDC_MINFUNCZOUT, ZoomOut),
  EV_LVN_COLUMNCLICK(IDC_MINFUNCFNLIST, LVNColumnclick),
  EV_LVN_ITEMCHANGED(IDC_MINFUNCFNLIST, SelFnChanged),
  EV_WM_SIZE,
//{{TMinFuncDialogRSP_TBL_END}}
  EV_WM_HELP,
END_RESPONSE_TABLE;

#define COLNUM_ID       0
#define COLNUM_FORM     1
#define COLNUM_COSTTYPE 2
#define COLNUM_COST     3
#define COLNUM_DESC     4

//{{TMinFuncDialog Implementation}}


static TMinFuncDialogXfer TMinFuncDialogData;

TMinFuncDialog::TMinFuncDialog(TMinView* view, TWindow* parent, TResId resId, TModule* module)
:
    TDialog(parent, resId, module),
    minview(view)
{
//{{TMinFuncDialogXFER_USE}}
  AndOr = new TRadioButton(this, IDC_MINFUNCANDOR, 0);
  BooleanExpr = new TRadioButton(this, IDC_MINFUNCBOOL, 0);
  FnList = new TListWindow(this, IDC_MINFUNCFNLIST);
  NAND = new TRadioButton(this, IDC_MINFUNCNAND, 0);
  NOR = new TRadioButton(this, IDC_MINFUNCNOR, 0);

  SetTransferBuffer(&TMinFuncDialogData);
//{{TMinFuncDialogXFER_USE_END}}

  // INSERT>> Your constructor code here.

}


TMinFuncDialog::~TMinFuncDialog()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}

static DWORD aIDHs[] = {
  IDC_MINFUNCANDOR,IDH_dlg_MINFUNC_rb_ANDOR,
  IDC_MINFUNCBOOL,IDH_dlg_MINFUNC_rb_BOOLEXPR,
  IDC_MINFUNCFNLIST,IDH_dlg_MINFUNC_lv_FNLIST,
  IDC_MINFUNCNAND,IDH_dlg_MINFUNC_rb_NAND,
  IDC_MINFUNCNOR,IDH_dlg_MINFUNC_rb_NOR,
  IDC_MINFUNCZIN,IDH_dlg_MINFUNC_b_ZIN,
  IDC_MINFUNCZOUT,IDH_dlg_MINFUNC_b_ZOUT,
  0,0
};

void TMinFuncDialog::EvHelp(HELPINFO* pHelpInfo)
{
  ::WinHelp( (HWND) pHelpInfo->hItemHandle, HelpFileName, HELP_WM_HELP, (DWORD) aIDHs);
}


void TMinFuncDialog::SetFnList(const swmin::MinFunctionSet& fs)
{
  FnList->DeleteAllItems();
  FnListSort.Clear();

  swmin::MinFunctionSet::iterator i=fs.begin(), e=fs.end();
  while (i!=e) AddToList(*i++);

  FnList->SortItems(FnListSort, COLNUM_ID);
  //FnList->Update(-1);
}

void TMinFuncDialog::AddToList(const swmin::MinFunction& f)
{
  static char pch[1024];
  ostrstream os(pch, 1024);

  TListWindSorter::TItemData* sort = FnListSort.NewItem();

  TListWindItem lwiID(f.id.c_str(), COLNUM_ID, f.id.length());
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_ID, f.id));
  lwiID.SetItemData((uint32) sort);
  int i=FnList->InsertItem(lwiID);
  if (i<0) throw TXOwl("Unable to add function");

  char* pchForm = (f.GetMode()==swmin::dnf) ? "dnf":"cnf";
  TListWindItem lwiForm(pchForm, COLNUM_FORM, strlen(pchForm));
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_FORM, string(pchForm)));
  lwiForm.SetIndex(i);
  FnList->SetItem(lwiForm);

  TListWindItem lwiCostType(swmin::SwCostString[f.CostType], COLNUM_COSTTYPE, strlen(SwCostString[f.CostType]));
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_COSTTYPE, string(SwCostString[f.CostType])));
  lwiCostType.SetIndex(i);
  FnList->SetItem(lwiCostType);

  os.seekp(0);
  os << f.cost() << ends;
  TListWindItem lwiCost(pch, COLNUM_COST, strlen(pch));
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_COST, string(pch)));
  lwiCost.SetIndex(i);
  FnList->SetItem(lwiCost);

  TListWindItem lwiDesc(f.desc.c_str(), COLNUM_DESC, f.desc.length());
  sort->insert(TListWindSorter::TItemData::value_type(COLNUM_DESC, f.desc));
  lwiDesc.SetIndex(i);
  FnList->SetItem(lwiDesc);

}

void TMinFuncDialog::Design_AndOR()
{
  minview->DesignChange(TGateWindow::ANDOR);
}


void TMinFuncDialog::Design_BoolExpr()
{
  minview->DesignChange(TGateWindow::BoolExpr);
}


void TMinFuncDialog::Design_NAND()
{
  minview->DesignChange(TGateWindow::NAND);
}


void TMinFuncDialog::Design_NOR()
{
  minview->DesignChange(TGateWindow::NOR);
}


void TMinFuncDialog::ZoomIn()
{
  minview->ZoomIn();
}


void TMinFuncDialog::ZoomOut()
{
  minview->ZoomOut();
}

bool TMinFuncDialog::GetSelID(string& id) const
{
  int sel=FnList->GetNextItem(-1, TListWindow::Selected);
  if (sel<0) return false;

  TAPointer<char> buffer = new char[255];

  TListWindItem item;
  item.mask=LVIF_TEXT;
  item.pszText=buffer;
  item.cchTextMax=255;
  item.iItem=sel;
  item.iSubItem=0;
  if (!FnList->GetItem(item)) return false;
  id=(char*) buffer;
  return true;
}

int TMinFuncDialog::GetSelDesign() const
{
  if (AndOr->GetCheck()==BF_CHECKED) return (int) TGateWindow::ANDOR;
  if (NAND->GetCheck()==BF_CHECKED) return (int) TGateWindow::NAND;
  if (NOR->GetCheck()==BF_CHECKED) return (int) TGateWindow::NOR;
  return (int) TGateWindow::BoolExpr;
}


int TMinFuncSorter::DoCompare(TItemData* item1, TItemData* item2, uint32 column) const
{
  string s1, s2, CostType1, CostType2;
  unsigned long c1, c2;
  int result, pos1, pos2;

  switch (column) {
    // For COLNUM_ID, if there is an attached '#', sorted by following number
    case COLNUM_ID:
      GetString(s1, item1, COLNUM_ID);
      GetString(s2, item2, COLNUM_ID);
      pos1=s1.find('#'); pos2=s2.find('#');
      if ((pos1==NPOS) || (pos2==NPOS) || (pos1!=pos2))  // no matching '#'
       return TListWindSorter::DoCompare(item1, item2, column);
      if (s1.compare(s2, 0, pos1))						// not same ID w/o '#'
       return TListWindSorter::DoCompare( item1, item2, column);

      c1=atol(s1.substr(pos1+1).c_str());
      c2=atol(s2.substr(pos2+1).c_str());
      return LongCompare(c1,c2);

    // For COLNUM_COST sort by numerical value
    case COLNUM_COST:
      GetString(s1, item1, COLNUM_COST); c1=atol(s1.c_str());
      GetString(s2, item2, COLNUM_COST); c2=atol(s2.c_str());
      return LongCompare(c1,c2);

    // For COLNUM_COSTTYPE sort by cost type first and then cost
    case COLNUM_COSTTYPE:
      GetString(CostType1, item1, COLNUM_COSTTYPE);
      GetString(CostType2, item2, COLNUM_COSTTYPE);
      if (CostType1 != CostType2) return StringCompare(CostType1, CostType2);

      GetString(s1, item1, COLNUM_COST); c1=atol(s1.c_str());
      GetString(s2, item2, COLNUM_COST); c2=atol(s2.c_str());
      return LongCompare(c1,c2);

    default:
      return TListWindSorter::DoCompare(item1, item2, column);
  }
}

void TMinFuncDialog::LVNColumnclick(TLwNotify& lwn)
{
  // INSERT>> Your code here.
  FnListSort.Alternate(lwn.iSubItem);
  FnList->SortItems(FnListSort, lwn.iSubItem);
}


void TMinFuncDialog::SelFnChanged(TLwNotify& /*lwn*/)
{
  string id;
  if (!GetSelID(id)) return;
  minview->FunctionChange(id);
}


void TMinFuncDialog::SetupWindow()
{
  TDialog::SetupWindow();

  // INSERT>> Your code here.
  // Add columns to list window
  TListWindColumn colID("Name", 80);
  TListWindColumn colForm("Form",40);
  TListWindColumn colCostType("Cost Type",60);
  TListWindColumn colCost("Cost", 40);
  TListWindColumn colDesc("Description", 150);

  FnList->InsertColumn(COLNUM_ID, colID);
  FnList->InsertColumn(COLNUM_FORM, colForm);
  FnList->InsertColumn(COLNUM_COSTTYPE, colCostType);
  FnList->InsertColumn(COLNUM_COST, colCost);
  FnList->InsertColumn(COLNUM_DESC, colDesc);

  BooleanExpr->Check();
}


void TMinFuncDialog::EvSize(uint sizeType, TSize& size)
{
    TDialog::EvSize(sizeType, size);

  // INSERT>> Your code here.
  TRect rscrFnList = FnList->GetWindowRect(), rDlg = GetClientRect();

  TPoint pFnListTopLeft=rscrFnList.TopLeft();
  ScreenToClient(pFnListTopLeft);
  TPoint pFnListBottomRight=rscrFnList.BottomRight();
  ScreenToClient(pFnListBottomRight);

  TRect rFnList(pFnListTopLeft, pFnListBottomRight);
  rFnList.right=rDlg.right;

  FnList->SetWindowPos(0, rFnList, SWP_SHOWWINDOW | SWP_NOZORDER);
}

