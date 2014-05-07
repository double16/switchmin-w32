//
// TCadWindow
//
//   Window holds a TCadGroup.  Handles drawing, scrolling, sizing, scaling
//   and some printing.
//
#include "DVCIRCAD_Global.h"

#include <math.h>

#include "CadWindow.h"

#include "primitive.h"
#include "cadstatics.h"


// Following determines how much blank space is allowed to right and
// below of CAD drawing when scrolling.  An insanly large value will allow
// the drawing to just barely scroll completely off the screen.  A value of
// 1 will make the rightmost and bottommost parts be at the edge of the window.
const unsigned long ScrollLimitFactor = 2;


//
// Build a response table for all messages/commands handled by the application.
//
DEFINE_RESPONSE_TABLE1(TCadWindow, TWindow)
  EV_WM_HSCROLL,
  EV_WM_VSCROLL,
  EV_WM_SIZE,
END_RESPONSE_TABLE;


void TCadWindow::BuildPages(TRect rect)
{
  Pages.Build(&obj, rect.Size(), Scale);
}

bool TCadWindow::GetPage(uint32 pagenum, TCadPage& page) const
{
  return Pages.Get(pagenum, page);
}

int TCadWindow::GetPages() const
{
  return Pages.Pages();
}

void TCadWindow::Print(TDC& dc, TRect rect, int pagenum)
{
  TCadPage pageinfo;
  if (!Pages.Get(pagenum, pageinfo)) return;  // Page doesn't exist

  TPoint org(rect.left-pageinfo.rect.left, rect.top-pageinfo.rect.top);
  TCadRenderInfo render(org, Scale, rect, &dc);

  // Save old clipping region
  TRegion oldClip;
  dc.GetClipRgn(oldClip);
  //  Create clipping path for drawing area
  dc.IntersectClipRect(rect);
  TRect clipBox=dc.GetClipBox();

  obj.Paint(render);

  // Restore clipping region
  dc.SelectClipRgn(oldClip);
}


TCadWindow::TCadWindow(TWindow* parent, const char far* title, TModule* module)
:
    TWindow(parent, title, module)
{
  // INSERT>> Your constructor code here.

  // Override the default window style for TWindow.
  Attr.Style |= WS_HSCROLL | WS_VSCROLL;

  Origin=TPoint(0,0);
  Scale=1.0;
  scrollDelta=TSize(0,0);
  ObjExtent=TRect(0,0,0,0);
  UpdateLocked=false;
}


TCadWindow::~TCadWindow()
{
  Destroy(IDCANCEL);

  // INSERT>> Your destructor code here.

}

void TCadWindow::SetScale(float _s)
{
  Scale=_s;
  FixObjExtent(true);
  if (!UpdateLocked) Invalidate(true);
}

void TCadWindow::ScaleScale(float _s)
{
  SetScale(Scale*_s);
}

void TCadWindow::Add(TCadObjectBasePtr pObj)
{
  obj.Add(pObj);
  FixObjExtent(true);
  if (!UpdateLocked) Invalidate(false);
}

void TCadWindow::Del(TCadObjectBasePtr pObj)
{
  obj.Del(pObj);
  FixObjExtent(true);
  if (!UpdateLocked) Invalidate(true);
}

void TCadWindow::Clear()
{
  obj.Clear();
  ObjExtent=TRect(0,0,0,0);
  Origin=TPoint(0,0);

  if (!UpdateLocked)  {
    SetScrollBars();
    Invalidate(true);
  }
}

void TCadWindow::FixOrigin(bool AdjustSB)
{
  if (!UpdateLocked) {
    if (Origin.x < -ObjExtent.right) Origin.x=-ObjExtent.right;
    else if (Origin.x > -ObjExtent.left) Origin.x=-ObjExtent.left;

    if (Origin.x < -ObjExtent.bottom) Origin.x=-ObjExtent.bottom;
    else if (Origin.x > -ObjExtent.top) Origin.x=-ObjExtent.top;

    if (AdjustSB) SetScrollBars();
  }
}

void TCadWindow::FixObjExtent(bool AdjustSB)
{
  if (!UpdateLocked) {
    ObjExtent=obj.GetExtent(TCadRenderInfo(TPoint(0,0), Scale, TRect(0,0,0,0), NULL));
    FixOrigin(false);
    SetScrollDelta(false);
    if (AdjustSB) SetScrollBars();
  }
}

void TCadWindow::SetScrollDelta(bool AdjustSB)
{
  TRect rtClient=GetClientRect();

  if (rtClient.IsEmpty()) {
    scrollDelta=TSize(0,0);
    return;
  }

  // Calc scrolling size of drawing
  TSize scrollArea(ObjExtent.Size().cx - (rtClient.Size().cx/ScrollLimitFactor),
                   ObjExtent.Size().cy - (rtClient.Size().cy/ScrollLimitFactor));

  if (ObjExtent.Size().cx <= rtClient.Size().cx ) scrollDelta.cx=0;
  else
   scrollDelta.cx=scrollArea.cx / ceil( (double) scrollArea.cx / ((double) rtClient.Size().cx / 4.0));

  if (ObjExtent.Size().cy <= rtClient.Size().cy ) scrollDelta.cy=0;
   else scrollDelta.cy=scrollArea.cy / ceil( (double) scrollArea.cy / ((double) rtClient.Size().cy / 4.0));

  if (AdjustSB) SetScrollBars();
}

void TCadWindow::SetScrollBars()
{
  if (UpdateLocked) return;

  int hMax, vMax;
  TRect rtClient=GetClientRect();
  TSize delta=scrollDelta, szObj=ObjExtent.Size();

  if (delta.cx==0) delta.cx=1;
  if (delta.cy==0) delta.cy=1;

  // Set max of scroll bar ranges
  if (szObj.cx <= rtClient.Size().cx) {
    hMax=0;
    Origin.x=-ObjExtent.left;
  }
  else hMax=(szObj.cx-(rtClient.Size().cx/ScrollLimitFactor))/delta.cx;

  if (szObj.cy <= rtClient.Size().cy) {
    vMax=0;
    Origin.y=-ObjExtent.top;
  }
  else vMax=(szObj.cy-(rtClient.Size().cy/ScrollLimitFactor))/delta.cy;

  // Set scroll bar ranges and thumb positions
  SetScrollRange(SB_HORZ, 0, hMax);
  SetScrollPos(SB_HORZ, abs(Origin.x+ObjExtent.left)/delta.cx);

  SetScrollRange(SB_VERT, 0, vMax);
  SetScrollPos(SB_VERT, abs(Origin.y+ObjExtent.top)/delta.cy);
}

void TCadWindow::EvHScroll(uint scrollCode, uint thumbPos, THandle hWndCtl)
{
    TWindow::EvHScroll(scrollCode, thumbPos, hWndCtl);

  // INSERT>> Your code here.
  TRect rtcl=GetClientRect();
  int leftMax=(rtcl.Size().cx/ScrollLimitFactor)-ObjExtent.right;

  switch (scrollCode) {
    case SB_TOP: Origin.x=-ObjExtent.left; break;
    case SB_BOTTOM: Origin.x=leftMax; break;
    case SB_LINELEFT: Origin.x += scrollDelta.cx; break;
    case SB_LINERIGHT: Origin.x -= scrollDelta.cx; break;
    case SB_PAGELEFT: Origin.x += rtcl.Size().cx; break;
    case SB_PAGERIGHT: Origin.x -= rtcl.Size().cx; break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
      Origin.x = (-1)*thumbPos*scrollDelta.cx + ObjExtent.left;
      break;
    default: return;
  }

  // Validate new Origin
  if (Origin.x>(-ObjExtent.left)) Origin.x=-ObjExtent.left;
  if (Origin.x<leftMax) Origin.x=leftMax;

  // Set new thumb position
  if (scrollDelta.cx)
   SetScrollPos(SB_HORZ, abs(Origin.x+ObjExtent.left)/scrollDelta.cx);

  // Redraw window
  Invalidate(true);
}


void TCadWindow::EvVScroll(uint scrollCode, uint thumbPos, THandle hWndCtl)
{
    TWindow::EvVScroll(scrollCode, thumbPos, hWndCtl);

  // INSERT>> Your code here.
  TRect rtcl=GetClientRect();
  int topMax=(rtcl.Size().cy/ScrollLimitFactor)-ObjExtent.bottom;

  switch (scrollCode) {
    case SB_TOP: Origin.y=-ObjExtent.top; break;
    case SB_BOTTOM: Origin.y=topMax; break;
    case SB_LINEUP: Origin.y += scrollDelta.cy; break;
    case SB_LINEDOWN: Origin.y -= scrollDelta.cy; break;
    case SB_PAGEUP: Origin.y += rtcl.Size().cy; break;
    case SB_PAGEDOWN: Origin.y -= rtcl.Size().cy; break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
      Origin.y = (-1)*thumbPos*scrollDelta.cy + ObjExtent.top;
      break;
    default: return;
  }

  // Validate new Origin
  if (Origin.y>(-ObjExtent.top)) Origin.y=-ObjExtent.top;
  if (Origin.y<topMax) Origin.y=topMax;

  // Set new thumb position
  if (scrollDelta.cy)
   SetScrollPos(SB_VERT, abs(Origin.y+ObjExtent.top)/scrollDelta.cy);

  // Redraw window
  Invalidate(true);
}


void TCadWindow::EvSize(uint sizeType, TSize& size)
{
    TWindow::EvSize(sizeType, size);

  // INSERT>> Your code here.
  SetScrollDelta(true);
}


void TCadWindow::Paint(TDC& dc, bool erase, TRect& rect)
{
  TWindow::Paint(dc, erase, rect);

  // INSERT>> Your code here.
  obj.Paint(TCadRenderInfo(Origin, Scale, rect, &dc));
}

void TCadWindow::LockUpdateBegin()  // Doesn't update window with add/del/clear
{
  UpdateLocked=true;
}

void TCadWindow::LockUpdateEnd()    // Updates window on add/del/clear
{
  UpdateLocked=false;
  FixObjExtent(true);
  Invalidate(true);
}

