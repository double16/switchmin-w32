//
// DVCIRCAD.DLL
//
// CAD Objects for Logic design
//

#include "DVCIRCAD_Global.h"

#define DLLBLD_DVCIRCAD_LOGIC
#include "logic.h"
#undef DLLBLD_DVCIRCAD_LOGIC
#include "primitive.h"
#include "cadstatics.h"

#include <ctype.h>

//
// TCadBooleanLabel
//

TCadBooleanLabel::TCadBooleanLabel(const string& _s)
 : TCadLabel("")
{
  Parse(_s);
}

void TCadBooleanLabel::Parse(const string& s)
{
  if (s.length() < 1) {
    Complement=false;
    text=Suffix="";
    return;
  }

  string raw=s;
  // Extract complement
  if (raw[0]=='~') {
    Complement=true;
    raw.remove(0,1);
  }
  else Complement=false;

  // Extract suffix
  Suffix.remove(0);
  unsigned i=raw.length()-1;
  while (i && isdigit(raw[i])) i--;  // Get index of first non-numberical char
  if ( (i<(raw.length()-1)) && (!isdigit(raw[i])) ) { // Suffix only if there's at least one leading non-numerical char
    Suffix=raw.substr(i+1);
    raw.remove(i+1);
  }

  // Set name
  text=raw;
}

TRect TCadBooleanLabel::GetExtent(const TCadRenderInfo& render) const
{
  TSize sizeSuffix, sizeName=TCadLabel::GetExtent(render).Size();  // Get name size

  TFont* font=GetScaledFontSub(render.Scale);

  if (render.dc) {  // use DC calculations
    render.dc->SelectObject(*font);
    sizeSuffix = render.dc->GetTextExtent(Suffix.c_str(), Suffix.length());
  }
  else { // use worst-case font only calculations
    sizeSuffix.cx=font->GetMaxWidth() * Suffix.length();
    sizeSuffix.cy=font->GetHeight();
  }

  // Combine name and suffix sizes
  sizeName.cx += sizeSuffix.cx;
  if (sizeSuffix.cy > sizeName.cy) sizeName.cy=sizeSuffix.cy;


  return TRect(render.ScaleIt(pos), sizeName);
}

void TCadBooleanLabel::Paint(const TCadRenderInfo& render) const
{
  uint oldAlign=render.dc->SetTextAlign(TA_TOP | TA_LEFT);
  TColor oldColor=render.dc->SetTextColor(color);

  //
  // Output text
  TSize szName, szSuffix;
  TPoint p=render.ScaleIt(pos), ps=p;

  // Name
  render.dc->SelectObject(*GetScaledFontNorm(render.Scale));
  render.dc->TextOut(p, text.c_str(), text.length());
  szName=render.dc->GetTextExtent(text.c_str(), text.length());

  // Suffix
  ps.x += szName.cx;  // Move directly after name
  render.dc->SelectObject(*GetScaledFontSub(render.Scale));
  szSuffix=render.dc->GetTextExtent(Suffix.c_str(), Suffix.length());
  if (szSuffix.cy < szName.cy) ps.y += (szName.cy-szSuffix.cy);  // Move down
  render.dc->TextOut(ps, Suffix.c_str(), Suffix.length());

  // Complement
  if (Complement) {  // Draw bar above text
    render.dc->SelectObject(TPen(color));
    render.dc->MoveTo(p);
    render.dc->LineTo(TPoint(p.x+szName.cx+szSuffix.cx, p.y));
  }

  // Restore DC values
  render.dc->SetTextColor(oldColor);
  if (oldAlign!=GDI_ERROR) render.dc->SetTextAlign(oldAlign);
}


//
// TCadWire
//
void TCadWire::Add(const TPoint& p, bool connect)  // add point with connect value
{
  TCadMultiLine::Add(p);  // add point and fix extent
  connects.push_back(connect);
}

void TCadWire::Clear()
{
  connects.erase(connects.begin(), connects.end());
}

void TCadWire::Paint(const TCadRenderInfo& render) const
{
  if (points.size()<1) return;  // nothing to draw

  TCadObjectBase::Paint(render);
  render.dc->SelectObject(TBrush(color));

  TPoint p;
  TRect rDot=TRect(-1*render.Scale,-1*render.Scale,1*render.Scale,1*render.Scale);
  TPoint pDot=TPoint(-1*render.Scale,0);

  TPoints::size_type i=1;
  render.dc->MoveTo(p=render.ScaleIt(points[0], pos));
  if (connects.at(0))
   render.dc->Chord(rDot.OffsetBy(p.x,p.y), pDot.OffsetBy(p.x,p.y), pDot.OffsetBy(p.x,p.y));
  while (i<points.size()) {
    render.dc->LineTo(p=render.ScaleIt(points[i], pos));
    if (connects.at(i))
     render.dc->Chord(rDot.OffsetBy(p.x,p.y), pDot.OffsetBy(p.x,p.y), pDot.OffsetBy(p.x,p.y));
    i++;
  }
}


//
// TCadLogicGateBase
//
TCadLogicGateBase::TCadLogicGateBase(const string& _n, uint _i)  // name, inputs
 : name(_n), inputs(_i)
{
  TFont* font=GetScaledFontNorm(1.0);
  pinlength=font->GetAveWidth();
  pinsep=font->GetHeight();
  size.cx = (font->GetAveWidth() * ((name.length()>4) ? name.length()+1:5))+2*pinlength;
  size.cy = pinsep * inputs;
}

TPoint TCadLogicGateBase::OutputPin(const TCadRenderInfo& render) const
{
  return render.ScaleIt(TPoint(size.cx, size.cy/2), pos);
}

TPoint TCadLogicGateBase::InputPin(const TCadRenderInfo& render, uint i) const
{
  return render.ScaleIt(TPoint(0, i*pinsep + pinsep/2), pos);
}

void TCadLogicGateBase::Paint(const TCadRenderInfo& render) const
{
  TCadObjectBase::Paint(render);

  uint i;
  TPoint p;

  // Draw input pins
  for(i=0; i<inputs; i++) {
    p=InputPin(render, i);
    render.dc->MoveTo(p);
    render.dc->LineTo(TPoint(p.x+(pinlength*render.Scale), p.y));
  }

  // Draw output pin
  p=OutputPin(render);
  render.dc->MoveTo(p);
  render.dc->LineTo(TPoint(p.x-(pinlength*render.Scale), p.y));
}

void TCadLogicGateBase::ScalePoints(TPoint* pts, int count, const TCadRenderInfo& render)
{
  for(int i=0; i<count; i++) pts[i]=render.ScaleIt(pts[i], pos);
}

//
// TCadLogicGateAND
//
TCadLogicGateAND::TCadLogicGateAND(const string& _n, uint _i)
 : TCadLogicGateBase(_n, _i)
{
  pArcStart=TPoint(size.cx-2*pinlength, size.cy);
  pArcEnd=TPoint(size.cx-2*pinlength, 0);

  ptsBox[0]=pArcEnd;
  ptsBox[1]=TPoint(pinlength, 0);
  ptsBox[2]=TPoint(pinlength, size.cy);
  ptsBox[3]=pArcStart;

  rArc=TRect(size.cx-3*pinlength, 0, size.cx-pinlength, size.cy);
}

void TCadLogicGateAND::Paint(const TCadRenderInfo& render) const
{
  TCadLogicGateBase::Paint(render);

  if (inputs==1) {  // draw wire
    render.dc->MoveTo(InputPin(render, 0));
    render.dc->LineTo(OutputPin(render));
    return;
  }

  // scale points for box
  TPoint box[4];
  memmove(box, ptsBox, sizeof(box));
  ScalePoints(box, 4, render);

  // Draw rectangle portion, top-right -> top-left -> bottom-left -> top-right
  render.dc->Polyline(box, 4);

  // Draw arc at right
  render.dc->Arc(render.ScaleIt(rArc, pos),
                 render.ScaleIt(pArcStart, pos),
                 render.ScaleIt(pArcEnd, pos));
}


//
// TCadLogicGateOR
//
TCadLogicGateOR::TCadLogicGateOR(const string& _n, uint _i)
 : TCadLogicGateBase(_n, _i)

{
  pArcStart=TPoint(pinlength, size.cy);
  pArcEnd=TPoint(pinlength, 0);
  rArcLeft=TRect(0,0,2*pinlength,size.cy);
  rArcRight=TRect(3*pinlength-size.cx, 0, size.cx-pinlength, size.cy);
}

void TCadLogicGateOR::Paint(const TCadRenderInfo& render) const
{
  TCadLogicGateBase::Paint(render);

  if (inputs==1) {  // draw wire
    render.dc->MoveTo(InputPin(render, 0));
    render.dc->LineTo(OutputPin(render));
    return;
  }

  TPoint pStart, pEnd;

  // Draw arc for left side
  render.dc->Arc(render.ScaleIt(rArcLeft, pos),
                 pStart=render.ScaleIt(pArcStart, pos),
                 pEnd=render.ScaleIt(pArcEnd,pos));

  // Draw arc for top and bottom sides
  render.dc->Arc(render.ScaleIt(rArcRight, pos), pStart, pEnd);
}


//
// TCadLogicGateNAND
//
TCadLogicGateNAND::TCadLogicGateNAND(const string& _n, uint _i)
 : TCadLogicGateBase(_n, _i)
{
  pArcStart=TPoint(size.cx-3*pinlength, size.cy);
  pArcEnd=TPoint(size.cx-3*pinlength, 0);

  ptsBox[0]=pArcEnd;
  ptsBox[1]=TPoint(pinlength, 0);
  ptsBox[2]=TPoint(pinlength, size.cy);
  ptsBox[3]=pArcStart;

  rArc=TRect(size.cx-4*pinlength, 0, size.cx-2*pinlength, size.cy);

  pInvArc=TPoint(size.cx-2*pinlength, size.cy/2);
  rInvArc=TRect(size.cx-2*pinlength, (size.cy-pinlength)/2, size.cx-pinlength, (size.cy+pinlength)/2);
}


void TCadLogicGateNAND::Paint(const TCadRenderInfo& render) const
{
  TCadLogicGateBase::Paint(render);

  // scale points for box
  TPoint box[4];
  memmove(box, ptsBox, sizeof(box));
  ScalePoints(box, 4, render);

  // Draw rectangle portion, top-right -> top-left -> bottom-left -> top-right
  render.dc->Polyline(box, 4);

  // Draw arc at right
  render.dc->Arc(render.ScaleIt(rArc, pos),
                 render.ScaleIt(pArcStart, pos),
                 render.ScaleIt(pArcEnd, pos));

  // Draw negate circle
  TPoint p=render.ScaleIt(pInvArc, pos);
  render.dc->Arc(render.ScaleIt(rInvArc, pos), p, p);
}


//
// TCadLogicGateNOR
//
TCadLogicGateNOR::TCadLogicGateNOR(const string& _n, uint _i)
 : TCadLogicGateBase(_n, _i)
{
  pArcStart=TPoint(pinlength, size.cy);
  pArcEnd=TPoint(pinlength, 0);
  rArcLeft=TRect(0,0,2*pinlength,size.cy);
  rArcRight=TRect(4*pinlength-size.cx, 0, size.cx-2*pinlength, size.cy);

  pInvArc=TPoint(size.cx-2*pinlength, size.cy/2);
  rInvArc=TRect(size.cx-2*pinlength, (size.cy-pinlength)/2, size.cx-pinlength, (size.cy+pinlength)/2);
}

void TCadLogicGateNOR::Paint(const TCadRenderInfo& render) const
{
  TCadLogicGateBase::Paint(render);

  TPoint pStart, pEnd;

  // Draw arc for left side
  render.dc->Arc(render.ScaleIt(rArcLeft, pos),
                 pStart=render.ScaleIt(pArcStart, pos),
                 pEnd=render.ScaleIt(pArcEnd,pos));

  // Draw arc for top and bottom sides
  render.dc->Arc(render.ScaleIt(rArcRight, pos), pStart, pEnd);

  // Draw negate circle
  TPoint p=render.ScaleIt(pInvArc, pos);
  render.dc->Arc(render.ScaleIt(rInvArc, pos), p, p);
}


//
// TCadLogicGateINV
//
TCadLogicGateINV::TCadLogicGateINV(const string& _n)
 : TCadLogicGateBase(_n, 1)
{
  ptsTri[0] = TPoint(size.cx-2*pinlength, size.cy/2);
  ptsTri[1] = TPoint(pinlength, 0);
  ptsTri[2] = TPoint(pinlength, size.cy);
  ptsTri[3] = ptsTri[0];

  pInvArc=TPoint(size.cx-2*pinlength, size.cy/2);
  rInvArc=TRect(size.cx-2*pinlength, (size.cy-pinlength)/2, size.cx-pinlength, (size.cy+pinlength)/2);
}

void TCadLogicGateINV::Paint(const TCadRenderInfo& render) const
{
  TCadLogicGateBase::Paint(render);

  // scale points for triangle
  TPoint tri[4];
  memmove(tri, ptsTri, sizeof(tri));
  ScalePoints(tri, 4, render);
  // Draw rectangle portion, top-right -> top-left -> bottom-left -> top-right
  render.dc->Polyline(tri, 4);

  // Draw negate circle
  TPoint p=render.ScaleIt(pInvArc, pos);
  render.dc->Arc(render.ScaleIt(rInvArc, pos), p, p);
}

//
// TCadLogicGateXOR
//
TCadLogicGateXOR::TCadLogicGateXOR(const string& _n, uint _i)
 : TCadLogicGateBase(_n, _i)
{
}

void TCadLogicGateXOR::Paint(const TCadRenderInfo& render) const
{
  TCadLogicGateBase::Paint(render);
}


