//
// DVCIRCAD.DLL
//
// CAD Object Primitives
//

#ifndef DVCIRCAD_Primitive_h
#define DVCIRCAD_Primitive_h

#include "DVCIRCAD_Global.h"

class DLLBLD_DVCIRCAD TCadRenderInfo : public TStreamableBase
{
  public:
    TPoint Origin;
    float Scale;
    TRect Page;
    TDC* dc;

    TCadRenderInfo();
    TCadRenderInfo(TPoint, float, TRect, TDC*);

    TPoint ScaleIt(const TPoint&, const TPoint& = TPoint(0,0)) const;
    TRect ScaleIt(const TRect&, const TPoint& = TPoint(0,0)) const;

DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadRenderInfo, 1);
};

class DLLBLD_DVCIRCAD TCadPage
{
  public:
    uint32 num, row, col;
    TRect rect;

    bool operator==(const TCadPage& y) const { return num==y.num; }
    bool operator<(const TCadPage& y) const { return num<y.num; }
};


#if defined(DLLUSE_DVCIRCAD) || defined(DLLBLD_DVCIRCAD_PRIMITIVE)
#pragma option -vi-
   DLLBLD_STLSET(DLLBLD_DVCIRCAD, TCadPage, std::less<TCadPage>);
#pragma option -vi
#endif

class DLLBLD_DVCIRCAD TCadPageSet
{
 public:
   typedef std::set<TCadPage, std::less<TCadPage> > TCadPages;

 protected:
   TCadPages pgs;

 public:
   TCadPageSet();
   ~TCadPageSet();

   bool Add(const TCadPage&);
   bool Get(uint32, TCadPage&) const;  // retrieve page data
   void Clear();
   uint32 Pages() const;

   uint32 Build(const class TCadGroup*, TSize, float);  // build set from CAD group
};

class DLLBLD_DVCIRCAD TCadObjectBase : public TStreamableBase
{
  public:
    TPoint pos;
    TColor color;
    uint32 layer;

  protected:
    TSize size;

  public:
    TCadObjectBase();
    virtual ~TCadObjectBase();

    virtual TRect GetExtent(const TCadRenderInfo&) const;
    virtual bool InPage(const TCadRenderInfo&) const;
    virtual void Paint(const TCadRenderInfo&) const;
    virtual bool operator==(const TCadObjectBase&) const;

DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadObjectBase, 1);
};

typedef TCadObjectBase* TCadObjectBasePtr;

class DLLBLD_DVCIRCAD TCadLine : public TCadObjectBase
{
  protected:
    TPoint p1, p2;

  public:
    TCadLine(const TPoint&, const TPoint&);

    void Paint(const TCadRenderInfo&) const;

DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadLine, 1);
};

#if defined(DLLUSE_DVCIRCAD) || defined(DLLBLD_DVCIRCAD_PRIMITIVE)
#pragma option -vi-
    DLLBLD_STLVECTOR(DLLBLD_DVCIRCAD, TPoint);
#pragma option -vi
#endif

class DLLBLD_DVCIRCAD TCadMultiLine : public TCadObjectBase
{
  public:
    typedef std::vector<TPoint> TPoints;

  protected:
    TPoints points;
    TRect ExtentCache;

    void CalcExtent();

  public:
    TCadMultiLine();

    void Add(const TPoint&);
    void Clear();
    TRect GetExtent(const TCadRenderInfo&) const;
    void Paint(const TCadRenderInfo&) const;

DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadMultiLine, 1);
};


class DLLBLD_DVCIRCAD TCadRect : public TCadObjectBase
{
  protected:
    TRect r;

  public:
    TCadRect(const TRect&);

    void Paint(const TCadRenderInfo&) const;

DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadRect, 1);
};
/*
class DLLBLD_DVCIRCAD TCadCircle : public TCadObjectBase
{
  protected:
    TPoint center;
    uint32 radius;

  public:
    TCadCircle(const TPoint&, uint32);

    void Paint(const TCadRenderInfo&) const;

DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadCircle, 1);
};

class DLLBLD_DVCIRCAD TCadArc : public TCadCircle
{
  protected:
    int angle1, angle2;

  public:
    TCadArc(const TPoint&, uint32, int, int);

    void Paint(const TCadRenderInfo&) const;

DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadArc, 1);
};


*/

class DLLBLD_DVCIRCAD TCadLabel : public TCadObjectBase
{
  protected:
    string text;

  public:
    TCadLabel(const string&);

    TRect GetExtent(const TCadRenderInfo&) const;
    void Paint(const TCadRenderInfo&) const;

DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadLabel, 1);
};

#if defined(DLLUSE_DVCIRCAD) || defined(DLLBLD_DVCIRCAD_PRIMITIVE)
#pragma option -vi-
    DLLBLD_STLVECTOR(DLLBLD_DVCIRCAD, TCadObjectBasePtr);
#pragma option -vi
#endif

class DLLBLD_DVCIRCAD TCadGroup : public TCadObjectBase
{
  public:
    typedef std::vector<TCadObjectBasePtr> TCadObjectBasePtrVector;

  protected:
    TCadObjectBasePtrVector objects;
    TRect ExtentCache;

    void CalcExtent();

  public:
    TCadGroup();
    ~TCadGroup();

    void Add(TCadObjectBasePtr);
    void Del(TCadObjectBasePtr);
    void Clear();
    uint32 ObjectsInPage(const TCadRenderInfo&) const;

    TRect GetExtent(const TCadRenderInfo&) const;
    void Paint(const TCadRenderInfo&) const;

DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadGroup, 1);
};

#endif

