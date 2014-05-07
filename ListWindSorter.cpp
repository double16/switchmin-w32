//
// TListWindSorter
//
//   Generic class for sorting columns of a TListWindow
//   using strings for each column
//

#include "SwitchMinEXE_Global.h"

#include "ListWindSorter.h"

TListWindSorter::TItemData::TItemData()
 : std::map<int, string, std::less<int> >()
{

}

bool TListWindSorter::TItemData::add(int col, string& s)
{
  return insert(value_type(col, s)).second;
}

TListWindSorter::TListWindSorter()
{
  LastColumn=-1;
  Ascending=true;
}

TListWindSorter::~TListWindSorter()
{
  Clear();
}

TListWindSorter::TItemData* TListWindSorter::NewItem()
{
  TItemData* item=new TItemData;
  AddItem(item);
  return item;
}

uint32 TListWindSorter::AddItem(TItemData* item)
{
  SortData.push_back(item);
  return (uint32) item;
}

void TListWindSorter::Clear()
{
  TSortData::iterator i,e;
  for(i=SortData.begin(), e=SortData.end(); i!=e; i++)
    delete (*i);  // Delete data objects
  SortData.erase(SortData.begin(), SortData.end());  // clear SortData
}

int TListWindSorter::Compare(uint32 item1, uint32 item2, uint32 lParam) const
{
  TItemData* data1 = (TItemData*) item1;
  TItemData* data2 = (TItemData*) item2;

  // Verify data1 and data2 in SortData here

  return AltCompare(DoCompare(data1, data2, lParam));
}

int TListWindSorter::DoCompare(TItemData* item1, TItemData* item2, uint32 column) const
{
  string s1,s2;
  if (!GetString(s1, item1, column)) return 0;
  if (!GetString(s2, item2, column)) return 0;
  return StringCompare(s1, s2);
}


int TListWindSorter::AltCompare(int result) const
{
  if (Ascending) return result;  // Ascending sort
  return result*(-1);  // Descending sort
}

void TListWindSorter::Alternate(uint32 column)
{
  // Implement alternate ascending/descending sort
  if (column==LastColumn) Ascending=!Ascending;  // Alternate ascending/descending
  else {
    LastColumn=column; // save last column
    Ascending=true;    // force ascending
  }
}

bool TListWindSorter::GetString(string& s, TItemData* data, int column) const
{
  if (data->find(column)==data->end()) {
    s.remove(0);
    return false;
  }
  s=(*data)[column];
  return true;
}

int TListWindSorter::StringCompare(string& s1, string& s2) const
{
  if (s1<s2) return -1;
  else if (s1==s2) return 0;
  return 1;
}

int TListWindSorter::LongCompare(long l1, long l2) const
{
  if (l1<l2) return -1;
  else if (l1==l2) return 0;
  return 1;
}

// EOF
