//////// AB CLASSGEN Thu Mar 06 17:58:55 1997 ////////
// TBitField Definition
//////////////////////////////////////////////////////
#if !defined(TBitField_HPP)
#define TBitField_HPP
#if !defined(RC_INVOKED)	// no Windows RC compiler

namespace swmin {

class DLLBLD_SWSYSTEM TBitField : public TStreamableBase
{
  char* data;
  unsigned long bits, bytes;

public:
   // create bit field with given bits
   TBitField();
	TBitField(unsigned long);
	TBitField(const TBitField&);
	TBitField& operator=(const TBitField&);
   // set/clear all bits
   TBitField& operator=(const int&);
	~TBitField();

   unsigned long getBits() const { return bits; }
   unsigned long getBytes() const { return bytes; }

	bool operator==(const TBitField&) const;
	bool operator<(const TBitField&) const;
   bool is_subset(const TBitField&) const;
   // return value of bit at given position
   const int operator[](unsigned long) const;

   char byte(unsigned long) const;

   TBitField& and(const TBitField&);		// bit-wise and with this
   TBitField& or(const TBitField&);			// bit-wise or and with this
   TBitField& not();						// bit-wise not

   // set given bit to value of int
   void set(unsigned long, int);
   // return non-zero if at least one bit is set
   int truth() const;
   unsigned long index() const;

   friend ostream& DLLBLD_SWSYSTEM operator<<(ostream&, const TBitField&);

protected:
	void invariant();

   void ClearUnused();
   int bit(unsigned long) const;

private:
	void _commonCtorJob();// called from all ctors

DECLARE_STREAMABLE(DLLBLD_SWSYSTEM, TBitField, 1);
};

}

#endif
#endif
