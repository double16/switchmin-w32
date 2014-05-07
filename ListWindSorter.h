//
// TListWindSorter
//
//   Generic class for sorting columns of a TListWindow
//   using strings for each column
//
#ifndef ListWindSorter_h
#define ListWindSorter_h

#include <owl/listwind.h>
#include <cstring.h>
#include <vector.h>
#include <map.h>

class TListWindSorter : public TLwComparator
{
  //
  // Public type definitions
  public:
    class TItemData : public std::map<int, string, std::less<int> > {
     public:
      TItemData();
      bool add(int, string&);
    };

    typedef std::vector<TItemData*> TSortData;

    // This structure is used to save all the pointers that are stored in
    // the list window items.  When the Clear() method is called (or the
    // object is destructed) all pointers are deleted.  This saves the
    // programmer from having to delete the pointers in response to a
    // LVN_DELETE or LVN_DELETEALLITEMS message.
    TSortData SortData;

    // Saves the last column sorted.  Used to alternate ascending/descending sort.
    int LastColumn;
    // True if last sort was ascending.
    bool Ascending;

  //
  // Public methods
  public:
    TListWindSorter();
    ~TListWindSorter();

    // Creates a new item data structure and adds it to SortData.  Just add
    // the column strings and set the TListWindItem's ItemData member.
    TItemData* NewItem();

    // Adds an item to the sorter.  Returns the pointer casted as an uint32
    // for easy use in the TListWindItem::SetItemData() method.
    // The TItemData* passed will belong to this object and will be deleted
    // in the destructor.  DO NOT delete the parameter!
    uint32 AddItem(TItemData*);

    // Removes all item data from the sorter.  The item data in the
    // TListWindItems will no longer be valid.  DO NOT call
    // TListWindow::SortItems after a Clear() until you reconstruct the items.
    void Clear();

    // Use to enable alternate ascending/descending sort.  Call with
    // column number before each TListWindow::SortItems() call.
    // Not required if you don't wish to alternate when a column is
    // clicked repeatedly.
    void Alternate(uint32 column);

    // Called to compare two items in the TListWindow::SortItems method.
    // Verifies item1 and item2 are TItemData pointers, calls protected
    // DoCompare.  Calls AltCompare to implement alternate asc/desc sorting.
    // It is not recommended to override this function.
    virtual int Compare(uint32 item1, uint32 item2, uint32 lParam) const;

  protected:
    // Called to compare two items during sorting.  Calls GetStrings to extract
    // string data for the column and StringCompare to compare them.  Override
    // this method to handle custom sorting.
    virtual int DoCompare(TItemData* item1, TItemData* item2, uint32 column) const;

    // Extract a string from the item data using the column number given.
    // Returns true if column was found in the item data, false otherwise.
    // Unfound string defaults to empty.
    bool GetString(string& s, TItemData* item, int column) const;

    // Compares two strings according to the convention Compare adheres to.
    int StringCompare(string&, string&) const;
    // Compares two longs
    int LongCompare(long, long) const;
    
    // Inverts compare results if using alternate sorting. MUST call this
    // to get final result if Compare() is overriden
    int AltCompare(int) const;
};


#endif

// EOF

