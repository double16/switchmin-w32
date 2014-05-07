//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         inputsgrid.cpp
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Source file for implementation of TInputsGrid (TControl).
//
//----------------------------------------------------------------------------
#include "SwitchMinEXE_Global.h"

#include "inputsgrid.h"

//
// TInputsGridData  -  Transfer Buffer
//
typedef struct { string name, desc; } TInputsStruct;
typedef std::vector<TInputsStruct> TInputsGridDataImp;

#define GRIDDATA ((TInputsGridDataImp*) Data)

TInputsGridData::TInputsGridData()
{
  GRIDDATA=new TInputsGridDataImp();
}

TInputsGridData::~TInputsGridData()
{
  delete GRIDDATA;
}

int TInputsGridData::size() const
{
  return GRIDDATA->size();
}

bool TInputsGridData::Get(int index, string& name, string& desc) const
{
  if ( (index<0) || (index>=GRIDDATA->size()) ) return false;
  name=GRIDDATA->at(index).name;
  desc=GRIDDATA->at(index).desc;
  return true;
}

bool TInputsGridData::GetName(int index, string& name) const
{
  if ( (index<0) || (index>=GRIDDATA->size()) ) return false;
  name=GRIDDATA->at(index).name;
  return true;
}

bool TInputsGridData::GetDesc(int index, string& desc) const
{
  if ( (index<0) || (index>=GRIDDATA->size()) ) return false;
  desc=GRIDDATA->at(index).desc;
  return true;
}

bool TInputsGridData::Set(int index, const string& name, const string& desc)
{
  if (index<0) return false;
  if (index>=GRIDDATA->size()) {
    int elements=(index - GRIDDATA->size())+1;
    TInputsStruct empty;
    while (elements--) GRIDDATA->push_back(empty);
  }

  GRIDDATA->at(index).name=name;
  GRIDDATA->at(index).desc=desc;
  return true;
}

bool TInputsGridData::SetName(int index, const string& name)
{
  if (index<0) return false;
  if (index>=GRIDDATA->size()) {
    int elements=(index - GRIDDATA->size())+1;
    TInputsStruct empty;
    while (elements--) GRIDDATA->push_back(empty);
  }

  GRIDDATA->at(index).name=name;
  return true;
}

bool TInputsGridData::SetDesc(int index, const string& desc)
{
  if (index<0) return false;
  if (index>=GRIDDATA->size()) {
    int elements=(index - GRIDDATA->size())+1;
    TInputsStruct empty;
    while (elements--) GRIDDATA->push_back(empty);
  }

  GRIDDATA->at(index).desc=desc;
  return true;
}

TInputsGridData& TInputsGridData::operator=(const TInputsGridData& y)
{
  if (&y==this) return *this;

  // remove current data
  TInputsGridDataImp *dat1 = (TInputsGridDataImp*) Data,
                     *dat2 = (TInputsGridDataImp*) y.Data;
  dat1->erase(dat1->begin(), dat1->end());
  TInputsGridDataImp::iterator i=dat2->begin(), e=dat2->end();
  for( ; i!=e; i++) {
    if ( (*i).name.length() > 0) dat1->push_back(*i);
  }

  //dat1->insert(dat1->begin(), dat2->begin(), dat2->end());

  return *this;
}



//{{TInputsGrid Implementation}}

int InputGridAttr = gAllBorders | gSizeColumn | gDisableScroll | gMultipleSelection | gLineSwapOn;

TInputsGrid::TInputsGrid(TWindow* parent, int resourceID, int max, TModule* module)
:
    MaxInputs(max),
    TGrid(parent, resourceID, NextLine, TabNextColumn, ReverseColor, -1,
       InputGridAttr, aGrid, module)
{
  SetVBarText(2);
}

TInputsGrid::TInputsGrid(TWindow* parent, int id, const char far* /*title*/, int x, int y, int w, int h, int max, TModule* module)
:
    MaxInputs(max),
    TGrid(parent, id, x, y, w, h, NextLine, TabNextColumn, ReverseColor, -1,
       InputGridAttr, aGrid, module)
{
  // INSERT>> Your constructor code here.
  SetVBarText(2);
}


TInputsGrid::~TInputsGrid()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}

const void *TInputsGrid::TransferItem(int line, int id)
{
  static string s;
  static char pch[4];

  bool result;
  s.remove(0);
  switch (id) {
    case 0:  // Line Label
      sprintf(pch, "%d", line);
      return pch;
    case IDC_INPUTGRIDNAME:
      result=Data.GetName(line, s);
      break;
    case IDC_INPUTGRIDDESC:
      result=Data.GetDesc(line, s);
      break;
  }
  if (result) return s.c_str();
  else return NULL;
}

void TInputsGrid::TransferItem(int line, int id, const void *data)
{
  if (!data) return;
  switch (id) {
    case IDC_INPUTGRIDNAME:
      Data.SetName(line, string((const char*) data));
      break;
    case IDC_INPUTGRIDDESC:
      Data.SetDesc(line, string((const char*) data));
      break;
  }
}

uint TInputsGrid::Transfer(void* buffer, TTransferDirection direction)
{
  switch (direction) {
    case tdGetData:
      *((TInputsGridData*) buffer) = Data;
      break;
    case tdSetData:
      Data = *((TInputsGridData*) buffer);
      SetModify(false);
      SetNLines(Data.size());
      Refresh();
      break;
    case tdSizeData:
      break;
  }

  return sizeof(TInputsGridData);
}

bool TInputsGrid::AppendNewLine(int &newline)
{
  if (newline >= MaxInputs) return false;
  else {  // Check if previous name is empty
    if (newline == 0) return true;  // Can always add one input
    string s;
    Data.GetName(newline-1, s);
    return (s.length()!=0);
  }
}

bool TInputsGrid::VkEsc(bool /*ctrl*/, bool /*shift*/, int /*repeat*/)
{
  // Ignore escape key so parent doesn't close
  return true;
}


