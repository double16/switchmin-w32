//
// DVCIRCAD.DLL
//
// CAD Object Primitives
//

#include "DVCIRCAD_Global.h"

#define DLLBLD_DVCIRCAD_PRIMITIVE
#include "primitive.h"
#undef DLLBLD_DVCIRCAD_PRIMITIVE

#include "cadstatics.h"

//
// TCadPageSet
//

TCadPageSet::TCadPageSet()
 : pgs()
{
}

TCadPageSet::~TCadPageSet()
{
}

uint32 TCadPageSet::Build(const TCadGroup* grp, TSize size, float Scale)
{
  Clear();

  TCadPage cadpage;
  TCadRenderInfo render(TPoint(0,0), Scale, TRect(TPoint(0,0), size), NULL);

  // Get extent of group at given scale
  TRect extent(grp->GetExtent(render));
  TRect page(extent.TopLeft(), size);

  cadpage.num=1;
  cadpage.row=1;
  cadpage.col=1;

  // Scan from top of drawing to bottom for rows
  while( ! (extent & page).IsEmpty() ) {

    // Scan from left of drawing to right for columns
    while( ! (extent & page).IsEmpty() ) {
    
      // Add this page if not empty
      render.Page=page;
      //render.Page=page;
      if (grp->ObjectsInPage(render) > 0) {
        cadpage.rect=page;
        Add(cadpage);
        cadpage.num++;
      }

      // Move page one column to the right
      page.MoveTo(page.left+size.cx, page.top);
      cadpage.col++;
    }  // while

    // Move page to next row, left side
    page.MoveTo(extent.left, page.top+size.cy);
    cadpage.row++;
    cadpage.col=1;
  }  // while

  return Pages();
}

uint32 TCadPageSet::Pages() const
{
 return pgs.size();
}

bool TCadPageSet::Add(const TCadPage& page)
{
  return pgs.insert(page).second;
}

bool TCadPageSet::Get(uint32 pagenum, TCadPage& page) const
{
  TCadPage p;
  p.num=pagenum;
  TCadPages::iterator i = pgs.find(p);

  if (i==pgs.end()) return false;

  page=*i;
  return true;
}

void TCadPageSet::Clear()
{
  pgs.erase(pgs.begin(), pgs.end());
}

//
// TCadGroup
//

TCadGroup::TCadGroup()
 : TCadObjectBase(), ExtentCache(0,0,0,0)
{
}

TCadGroup::~TCadGroup()
{
  Clear();
}

uint32 TCadGroup::ObjectsInPage(const TCadRenderInfo& render) const
{
  uint32 count=0;
  TCadObjectBasePtrVector::const_iterator i,e;
  // Modify render Origin with pos so all members are relative to pos
  TCadRenderInfo renderGrp(render.ScaleIt(pos), render.Scale, render.Page, render.dc);

  for(i=objects.begin(), e=objects.end(); i!=e; i++) {  // Check each object
    if ((*i)->InPage(renderGrp)) count++;  // Check if object is in the page
  }

  return count;
}

void TCadGroup::CalcExtent()
{
  TCadRenderInfo renderDummy;
  TRect r(0,0,0,0), s;
  TCadObjectBasePtrVector::iterator i,e;

  // Iterate through each object
  for(i=objects.begin(), e=objects.end(); i!=e; i++) {
    s=(*i)->GetExtent(renderDummy);  // Get size of this object

    if (s.left < r.left) r.left=s.left;
    if (s.right > r.right) r.right=s.right;
    if (s.top < r.top) r.top=s.top;
    if (s.bottom > r.bottom) r.bottom=s.bottom;
  }

  ExtentCache=r;
}

void TCadGroup::Add(TCadObjectBasePtr obj)
{
  objects.push_back(obj);
  if (objects.size()==1) ExtentCache=obj->GetExtent(TCadRenderInfo());
  else ExtentCache |= obj->GetExtent(TCadRenderInfo());
}

void TCadGroup::Del(TCadObjectBasePtr obj)
{
  TCadObjectBasePtrVector::iterator i,e;

  for(i=objects.begin(), e=objects.end(); i!=e; i++)
    if ( (*i) == obj) {  // check if this is the object
      delete (*i);  // delete the object
      objects.erase(i);  // delete the pointer
      break;  // exit loop
    }

  CalcExtent();
}

void TCadGroup::Clear()
{
  TCadObjectBasePtrVector::iterator i,e;
  for(i=objects.begin(), e=objects.end(); i!=e; i++) delete (*i);
  objects.erase(objects.begin(), objects.end());
  ExtentCache=TRect(0,0,0,0);
}

TRect TCadGroup::GetExtent(const TCadRenderInfo& render) const
{
  return render.ScaleIt(ExtentCache, pos);
}

void TCadGroup::Paint(const TCadRenderInfo& render) const
{
  TCadObjectBasePtrVector::const_iterator i,e;
  // Modify render Origin with pos so all members are relative to pos
  TCadRenderInfo renderGrp(render.ScaleIt(pos), render.Scale, render.Page, render.dc);

  for(i=objects.begin(), e=objects.end(); i!=e; i++) {  // Paint each object
    if ((*i)->InPage(renderGrp)) (*i)->Paint(renderGrp);  // Paint if in the page
  }

}

IMPLEMENT_STREAMABLE(TCadGroup);

void TCadGroup::Streamer::Write(opstream& out) const
{
  WriteBaseObject((TCadObjectBase*) this, out);

  out.writeWord32(GetObject()->objects.size());
  TCadObjectBasePtrVector::iterator i,e;
  for(i=GetObject()->objects.begin(), e=GetObject()->objects.end(); i!=e; i++)
   out << *(*i);
}

void *TCadGroup::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{
  ReadBaseObject((TCadObjectBase*) this, in);

  TCadObjectBasePtrVector::size_type count;
  count=in.readWord32();
  TCadObjectBasePtrVector* objs=&(GetObject()->objects);
  objs->erase(objs->begin(), objs->end());

  TCadObjectBasePtr p;
  while (count--) {
    p=new TCadObjectBase;
    in >> *p;
    objs->push_back(p);
  }

  return GetObject();
}

//
// TCadLabel
//
TCadLabel::TCadLabel(const string& _s)
 : text(_s)
{
}

TRect TCadLabel::GetExtent(const TCadRenderInfo& render) const
{
  TSize size;

  TFont* font=GetScaledFontNorm(render.Scale);

  if (render.dc) {  // use DC calculations
    render.dc->SelectObject(*font);
    size = render.dc->GetTextExtent(text.c_str(), text.length());
  }
  else { // use worst-case font only calculations
    size.cx=font->GetMaxWidth() * text.length();
    size.cy=font->GetHeight();
  }

  return TRect(render.ScaleIt(pos), size);
}

void TCadLabel::Paint(const TCadRenderInfo& render) const
{
  render.dc->SelectObject(*GetScaledFontNorm(render.Scale));
  uint oldAlign=render.dc->SetTextAlign(TA_TOP | TA_LEFT);
  TColor oldColor=render.dc->SetTextColor(color);

  // Output text
  render.dc->TextOut(render.ScaleIt(pos), text.c_str(), text.length());

  // Restore DC values
  render.dc->SetTextColor(oldColor);
  if (oldAlign!=GDI_ERROR) render.dc->SetTextAlign(oldAlign);
}

IMPLEMENT_STREAMABLE(TCadLabel);

void TCadLabel::Streamer::Write(opstream& out) const
{
  WriteBaseObject((TCadObjectBase*) this, out);
}

void *TCadLabel::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{
  ReadBaseObject((TCadObjectBase*) this, in);

  return GetObject();
}

//
// TCadRect
//
TCadRect::TCadRect(const TRect& _r)
 : r(_r)
{
}

void TCadRect::Paint(const TCadRenderInfo& render) const
{
  TCadObjectBase::Paint(render);
  render.dc->Rectangle(render.ScaleIt(r, pos));
}

IMPLEMENT_STREAMABLE(TCadRect);

void TCadRect::Streamer::Write(opstream& out) const
{
  WriteBaseObject((TCadObjectBase*) this, out);
  out << GetObject()->r;
}

void *TCadRect::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{
  ReadBaseObject((TCadObjectBase*) this, in);
  in >> GetObject()->r;
  return GetObject();
}

//
// TCadMultiLine
//

TCadMultiLine::TCadMultiLine()
 : ExtentCache(0,0,0,0)
{
}

void TCadMultiLine::CalcExtent()
{
  TRect r(0,0,0,0);
  TPoints::size_type i=0;

  while (i<points.size()) {
    if (points[i].x < r.left) r.left=points[i].x;
    else if (points[i].x > r.right) r.right=points[i].x;

    if (points[i].y < r.top) r.top=points[i].y;
    else if (points[i].y > r.bottom) r.bottom=points[i].y;

    i++;
  }

  ExtentCache=r;
}

void TCadMultiLine::Add(const TPoint& p)
{
  points.push_back(p);
  if (points.size()==1) ExtentCache=TRect(p,p);
  else {
    if (p.x < ExtentCache.left) ExtentCache.left=p.x;
    else if (p.x > ExtentCache.right) ExtentCache.right=p.x;

    if (p.y < ExtentCache.top) ExtentCache.top=p.y;
    else if (p.y > ExtentCache.bottom) ExtentCache.bottom=p.y;
  }
}

void TCadMultiLine::Clear()
{
  points.erase(points.begin(), points.end());
  ExtentCache=TRect(0,0,0,0);
}

TRect TCadMultiLine::GetExtent(const TCadRenderInfo& render) const
{
  TRect extent=ExtentCache;
  if (extent.left==extent.right) extent.right++;
  if (extent.top==extent.bottom) extent.bottom++;

  return render.ScaleIt(extent, pos);
}

void TCadMultiLine::Paint(const TCadRenderInfo& render) const
{
  if (points.size()<1) return;  // nothing to draw

  TCadObjectBase::Paint(render);

  TPoints::size_type i=1;
  render.dc->MoveTo(render.ScaleIt(points[0], pos));
  while (i<points.size()) render.dc->LineTo(render.ScaleIt(points[i++], pos));
}

IMPLEMENT_STREAMABLE(TCadMultiLine);

void TCadMultiLine::Streamer::Write(opstream& out) const
{
  WriteBaseObject((TCadObjectBase*) this, out);
  out.writeWord32(GetObject()->points.size());
  TPoints::size_type i=0;
  while (i<GetObject()->points.size()) out << GetObject()->points[i++];
}

void *TCadMultiLine::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{
  ReadBaseObject((TCadObjectBase*) this, in);
  TPoints::size_type count;
  count=in.readWord32();
  TPoints* pts=&(GetObject()->points);
  pts->erase(pts->begin(), pts->end());
  TPoint p;
  while (count--) {
    in >> p;
    pts->push_back(p);
  }

  return GetObject();
}


//
// TCadLine
//
TCadLine::TCadLine(const TPoint& _p1, const TPoint& _p2)
 : p1(_p1), p2(_p2)
{
  size=TRect(_p1, _p2).Normalized().Size();
  if (size.cx==0) size.cx=1;
  if (size.cy==0) size.cy=1;
}

void TCadLine::Paint(const TCadRenderInfo& render) const
{
  TCadObjectBase::Paint(render);
  render.dc->MoveTo(render.ScaleIt(p1, pos));
  render.dc->LineTo(render.ScaleIt(p2, pos));
}

IMPLEMENT_STREAMABLE(TCadLine);

void TCadLine::Streamer::Write(opstream& out) const
{
  WriteBaseObject((TCadObjectBase*) this, out);
  out << GetObject()->p1 << GetObject()->p2;
}

void *TCadLine::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{

  ReadBaseObject((TCadObjectBase*) this, in);
  in >> GetObject()->p1 >> GetObject()->p2;
  return GetObject();
}

//
// TCadObjectBase
//

TCadObjectBase::TCadObjectBase()
 : pos(TPoint(0,0)), layer(0), size(TSize(0,0)), color(TColor::Black)
{

}

TCadObjectBase::~TCadObjectBase()
{
}


TRect TCadObjectBase::GetExtent(const TCadRenderInfo& render) const
{
  return render.ScaleIt(TRect(pos, size));
}

bool TCadObjectBase::InPage(const TCadRenderInfo& render) const
{
  return !((GetExtent(render) & render.Page).IsEmpty());
}

void TCadObjectBase::Paint(const TCadRenderInfo& render) const
{
  render.dc->SelectObject(TPen(color));
}

bool TCadObjectBase::operator==(const TCadObjectBase& y) const
{
  return ( (pos==y.pos) && (size==y.size) && (layer==y.layer) );
}

IMPLEMENT_STREAMABLE(TCadObjectBase);

void TCadObjectBase::Streamer::Write(opstream& out) const
{
  out << GetObject()->pos << GetObject()->color << GetObject()->layer << GetObject()->size;
}

void *TCadObjectBase::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{

  in >> GetObject()->pos >> GetObject()->color >> GetObject()->layer >> GetObject()->size;
  return GetObject();
}

//
// TCadRenderInfo
//

TCadRenderInfo::TCadRenderInfo()
 : Origin(TPoint(0,0)), Scale(1.0), Page(TRect(0,0,0,0)), dc(NULL)
{
}

TCadRenderInfo::TCadRenderInfo(TPoint _o, float _s, TRect _p, TDC* _dc)
 : Origin(_o), Scale(_s), Page(_p), dc(_dc)
{
}

TPoint TCadRenderInfo::ScaleIt(const TPoint& _p, const TPoint& off) const
{
  return TPoint(((_p.x+off.x)*Scale)+Origin.x, ((_p.y+off.y)*Scale)+Origin.y);
}

TRect TCadRenderInfo::ScaleIt(const TRect& _r, const TPoint& off) const
{
  return TRect(((_r.left+off.x)*Scale)+Origin.x, ((_r.top+off.y)*Scale)+Origin.y,
               ((_r.right+off.x)*Scale)+Origin.x, ((_r.bottom+off.y)*Scale)+Origin.y);
}

IMPLEMENT_STREAMABLE(TCadRenderInfo);

void TCadRenderInfo::Streamer::Write(opstream& out) const
{
  out << GetObject()->Origin << GetObject()->Scale << GetObject()->Page;
}

void *TCadRenderInfo::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{

  in >> GetObject()->Origin >> GetObject()->Scale >> GetObject()->Page;
  GetObject()->dc=NULL;
  return GetObject();
}


// EOF

