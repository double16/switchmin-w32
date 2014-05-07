//
// DVCIRCAD.DLL
//
// CAD Object Primitives
//

#ifndef DVCIRCAD_Logic_h
#define DVCIRCAD_Logic_h

#include "primitive.h"

class DLLBLD_DVCIRCAD TCadBooleanLabel : public TCadLabel
{
  protected:
    bool Complement;
    string Suffix;  // output after name in subscript

    void Parse(const string&);  // extracts name, suffix & complement

  public:
    TCadBooleanLabel(const string&);

    TRect GetExtent(const TCadRenderInfo&) const;
    void Paint(const TCadRenderInfo&) const;

//DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadBooleanLabel, 1);
};

#if defined(DLLUSE_DVCIRCAD) || defined(DLLBLD_DVCIRCAD_LOGIC)
#pragma option -vi-
    DLLBLD_STLVECTOR(DLLBLD_DVCIRCAD, bool);
#pragma option -vi
#endif

class DLLBLD_DVCIRCAD TCadWire : public TCadMultiLine
{
  protected:
    typedef std::vector<bool> TCadWireBoolVector;
    TCadWireBoolVector connects;

  public:
    void Add(const TPoint&, bool);  // add point with connect value
    void Clear();

    void Paint(const TCadRenderInfo&) const;

//DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadWire, 1);
};

class DLLBLD_DVCIRCAD  TCadLogicGateBase : public TCadObjectBase
{
  protected:
    string name;
    uint inputs;
    int pinlength, pinsep;  // length of pins (horz) and separation (vert)

    void ScalePoints(TPoint*, int, const TCadRenderInfo&);

  public:
    TCadLogicGateBase(const string&, uint);  // name, inputs

    TPoint OutputPin(const TCadRenderInfo&) const;
    TPoint InputPin(const TCadRenderInfo&, uint) const;

    void Paint(const TCadRenderInfo&) const;

//DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadLogicGateBase, 1);
};

class DLLBLD_DVCIRCAD  TCadLogicGateAND : public TCadLogicGateBase
{
  protected:
    // All geometry is relative to pos, Scale=1.0
    TPoint ptsBox[4];  // Points for open-rectangle
    TRect rArc;		  // Rectangle for arc at right end
    TPoint pArcStart, pArcEnd;  // Start and end points for arc

  public:
    TCadLogicGateAND(const string&, uint);  // name, inputs

    void Paint(const TCadRenderInfo&) const;
//DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadLogicGateAND, 1);
};

class DLLBLD_DVCIRCAD  TCadLogicGateOR : public TCadLogicGateBase
{
  protected:
    // All geometry is relative to pos, Scale=1.0
    TPoint pArcStart, pArcEnd;  // Points for arcs
    TRect rArcLeft, rArcRight;  // Rectangles for arcs

  public:
    TCadLogicGateOR(const string&, uint);  // name, inputs

    void Paint(const TCadRenderInfo&) const;
//DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadLogicGateOR, 1);
};

class DLLBLD_DVCIRCAD  TCadLogicGateNAND : public TCadLogicGateBase
{
  protected:
    // All geometry is relative to pos, Scale=1.0
    TPoint ptsBox[4];  // Points for open-rectangle
    TRect rArc;		  // Rectangle for arc at right end
    TPoint pArcStart, pArcEnd;  // Start and end points for arc
    TRect rInvArc;  // Rectangle for inverter arc
    TPoint pInvArc;    // Start and end point for arc

  public:
    TCadLogicGateNAND(const string&, uint);  // name, inputs

    void Paint(const TCadRenderInfo&) const;
//DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadLogicGateNAND, 1);
};

class DLLBLD_DVCIRCAD  TCadLogicGateNOR : public TCadLogicGateBase
{
  protected:
    // All geometry is relative to pos, Scale=1.0
    TPoint pArcStart, pArcEnd;  // Points for arcs
    TRect rArcLeft, rArcRight;  // Rectangles for arcs
    TRect rInvArc;  // Rectangle for inverter arc
    TPoint pInvArc;    // Start and end point for arc

  public:
    TCadLogicGateNOR(const string&, uint);  // name, inputs

    void Paint(const TCadRenderInfo&) const;
//DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadLogicGateNOR, 1);
};

class DLLBLD_DVCIRCAD  TCadLogicGateINV : public TCadLogicGateBase
{
  protected:
    // All geometry is relative to pos, Scale=1.0
    TPoint ptsTri[4];  // Points for triangle
    TRect rInvArc;  // Rectangle for inverter arc
    TPoint pInvArc;    // Start and end point for arc

  public:
    TCadLogicGateINV(const string&);  // name, inputs

    void Paint(const TCadRenderInfo&) const;
//DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadLogicGateINV, 1);
};

class DLLBLD_DVCIRCAD TCadLogicGateXOR : public TCadLogicGateBase
{
  protected:
    // All geometry is relative to pos, Scale=1.0
    TPoint pArcStart, pArcEnd;  // Points for arcs
    TRect rArcLeft, rArcRight;  // Rectangles for arcs

  public:
    TCadLogicGateXOR(const string&, uint);  // name, inputs

    void Paint(const TCadRenderInfo&) const;
//DECLARE_STREAMABLE(DLLBLD_DVCIRCAD, TCadLogicGateXOR, 1);
};

#endif

// EOF

