//
// TCadWindow
//
//   Window holds a TCadGroup.  Handles drawing, scrolling, sizing, scaling
//   and some printing.
//
#ifndef TCadWindow_h
#define TCadWindow_h

#include <owl/window.h>
#include "primitive.h"

class DLLBLD_DVCIRCAD TCadWindow : public TWindow
{
    TCadGroup obj;
    TPoint Origin;
    float Scale;
    TSize scrollDelta;
    TCadPageSet Pages;
    TRect ObjExtent;
    bool UpdateLocked;

  public:
    TCadWindow(TWindow* parent, const char far* title = 0, TModule* module = 0);
    virtual ~TCadWindow();

    void BuildPages(TRect);
    int GetPages() const;
    bool GetPage(uint32, TCadPage&) const;
    void Print(TDC&, TRect, int);

    void Add(TCadObjectBasePtr);
    void Del(TCadObjectBasePtr);
    void Clear();
    void LockUpdateBegin();  // Doesn't update window with add/del/clear
    void LockUpdateEnd();    // Updates window on add/del/clear

    void SetScale(float);
    float GetScale() const { return Scale; }
    void ScaleScale(float);

    //bool ForEachObj(callback);
    //bool QueryObj(callback);

  protected:
    void FixOrigin(bool = true);			// Adjusts origin and scroll bars if necessary
    void FixObjExtent(bool = true);		// Adjusts object extent
    void SetScrollDelta(bool = true);	// Adjusts scrollbar deltas
    void SetScrollBars();					// Sets scrollbar ranges and thumb positions

  public:
    virtual void Paint(TDC& dc, bool erase, TRect& rect);

  protected:
    void EvHScroll(uint scrollCode, uint thumbPos, THandle hWndCtl);
    void EvVScroll(uint scrollCode, uint thumbPos, THandle hWndCtl);
    void EvSize(uint sizeType, TSize& size);

DECLARE_RESPONSE_TABLE(TCadWindow);

//DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadWindow, 1);
};


#endif

// EOF
