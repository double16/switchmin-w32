//////// AB CLASSGEN Thu Mar 06 17:58:55 1997 ////////
// TBitField Implementation
//////////////////////////////////////////////////////
#include "SwitchMinDLL_Global.h"

#include "TBitField.HPP"

namespace swmin {

// default constructor
TBitField::TBitField()
 : data(NULL)
{
  bits=8;
  _commonCtorJob();
}

TBitField::TBitField(unsigned long _b)
 : data(NULL)
{
  bits=_b;
  _commonCtorJob();
}

// copy constructor
TBitField::TBitField(const TBitField& y)
 : data(NULL)
{
  bits=y.bits;
  _commonCtorJob();
  memcpy(data, y.data, bytes);
}

// assignment overload
TBitField& TBitField::operator=(const TBitField& y)
{
  invariant();
  if(this == &y) return *this;

  delete[] data;  // clear old data
  data=NULL;
  bits=y.bits;  // set new bit size
  _commonCtorJob();  // allocate new data area
  memcpy(data, y.data, bytes);  // copy data

  return *this;
}

// Sets/clears all bits
TBitField& TBitField::operator=(const int& i)
{
  memset(data, (i==0) ? 0:0xFF, bytes);
  if (i!=0) ClearUnused();  // must clear unused bits
  return *this;
}

// called from all constructors
void TBitField::_commonCtorJob()
{
  delete[] data;
  bytes=bits/8;
  if ( (bits%8) > 0) bytes++;
  data=new char[bytes];
  memset(data, 0, bytes);
}

// destructor
TBitField::~TBitField()
{
  delete[] data;
}



void TBitField::invariant()
{
  //...
}

bool TBitField::operator==(const TBitField& y) const
{
  if (y.bits!=bits) return false;
  return memcmp(data, y.data, bytes)==0;
}

bool TBitField::operator<(const TBitField& y) const
{
  if (y.bits!=bits) return bits<y.bits;
  return memcmp(data, y.data, bytes)<0;
}

bool TBitField::is_subset(const TBitField& y) const
{
  // every 1 bit in y must be 1 in this
  if (y.bits!=bits) return false;

  for(unsigned long i=0; i<bits; i++) {
    if (y[i]) {  // check for 1 bit in y
      if (bit(i)==0) return false;  // if no 1 bit in this, not a subset
    }
  }
  return true;
}

const int TBitField::operator[](unsigned long pos) const
{
  if (pos>=bits) return 0;
  return bit(pos);
}

int TBitField::bit(unsigned long pos) const
{
  return (data[pos/8]&(1<<(pos%8))) ? 1:0;
}

char TBitField::byte(unsigned long pos) const
{
  if (pos>=bytes) return 0;
  return data[pos];
}

TBitField& TBitField::and(const TBitField& b)
{
  unsigned long i=0;
  while (i<bytes) {
    data[i] &= b.byte(i);
    i++;
  }
  if (bits<b.getBits()) ClearUnused();
  return *this;
}

TBitField& TBitField::or(const TBitField& b)
{
  unsigned long i=0;
  while (i<bytes) {
    data[i] |= b.byte(i);
    i++;
  }
  if (bits<b.getBits()) ClearUnused();
  return *this;
}

TBitField& TBitField::not()
{
  unsigned long i=0;
  while (i<bytes) {
    data[i] = (char) ~data[i];
    i++;
  }
  ClearUnused();
  return *this;
}

void TBitField::set(unsigned long pos, int val)
{
  if (pos>=bits) return; // grow?
  if (val) data[pos/8] |= (char) 1<<(pos%8);
  else data[pos/8] &= (char) ~(1<<(pos%8));
}

int TBitField::truth() const
{
  unsigned long i=0;
  int val=0;
  while (i<bytes) val |= data[i++];
  return val;
}

unsigned long TBitField::index() const
{
  unsigned long c=0, i;
  int x;
  for(i=0; i<bytes; i++) {
    x=data[i];
    while (x) {  	// loop while x has bits
      if (x&1) c++; // if 1's place has a bit, count it
      x=x>>1;		// shift bits right and discard 1's place
    }
  }
  return c;
}

IMPLEMENT_STREAMABLE(TBitField);

void TBitField::Streamer::Write(opstream& out) const
{
  out << GetObject()->bits;
  out.writeBytes(GetObject()->data, GetObject()->bytes);
}

void *TBitField::Streamer::Read(ipstream& in, uint32 /*ver*/) const
{
  in >> GetObject()->bits;
  GetObject()->_commonCtorJob();  // allocate data
  in.readBytes(GetObject()->data, GetObject()->bytes);

  return GetObject();
}

void TBitField::ClearUnused()
{
    int unused=bits%8;
    if (unused>0) {
      char last=0;
      while (unused--) {
        last=last<<1;  // shift bits left once
        last |= 1;  // put a one bit in rightmost place
      }
      data[bytes-1] &= last;  // clear unused bits
    }

}

ostream& operator<<(ostream& os, const TBitField& y)
{
  os << '{';
  unsigned long bits=y.getBits(), i;
  for(i=0; i<bits; i++) os << (y[i] ? '1':'0');
  os << '}';

  return os;
}

}

