//----------------------------------------------------------------------------
//  Project switchmin
//  DoubleVision, LTD
//  Copyright © 1997. All Rights Reserved.
//
//  SUBSYSTEM:    switchmin.apx Application
//  FILE:         inputsgrid.h
//  AUTHOR:       Pat B. Double
//
//  OVERVIEW
//  ~~~~~~~~
//  Class definition for TInputsGrid (TControl).
//
//----------------------------------------------------------------------------
#if !defined(inputsgrid_h)              // Sentry, use file only if it's not already included.
#define inputsgrid_h

#include <owl/control.h>
#include "griddll.h"

#include "switchminapp.rh"            // Definition of all resources.

class TInputsGridData {
    void* Data;

  public:
    TInputsGridData();
    ~TInputsGridData();

    TInputsGridData& operator=(const TInputsGridData&);

    int size() const;
    
    bool Get(int index, string& name, string& desc) const;  // Returns true if successful
    bool GetName(int index, string& name) const;
    bool GetDesc(int index, string& desc) const;
    bool Set(int index, const string& name, const string& desc);  // Returns true if successful
    bool SetName(int index, const string& name);
    bool SetDesc(int index, const string& desc);
};

const int IDC_INPUTGRIDNAME = 101;
const int IDC_INPUTGRIDDESC = 102;

//{{TControl = TInputsGrid}}
class TInputsGrid : public TGrid {
    int MaxInputs;
    TInputsGridData Data;

  public:
    TInputsGrid(TWindow* parent, int resourceID, int max, TModule* module = 0);
    TInputsGrid(TWindow* parent, int id, const char far* title, int x, int y, int w, int h, int max, TModule* module = 0);
    virtual ~TInputsGrid();

    const void *TransferItem(int line, int id);
    void TransferItem(int line, int id, const void *data);

    uint Transfer(void* buffer, TTransferDirection direction);

  protected:
    bool AppendNewLine(int &newline);
    bool VkEsc(bool ctrl, bool shift, int repeat);
};    //{{TInputsGrid}}


#endif  // inputsgrid_h sentry.

