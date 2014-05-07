//////// AB CLASSGEN Wed Apr 02 22:04:50 1997 ////////
// RdcTable Implementation
//////////////////////////////////////////////////////
#include "SwitchMinDLL_Global.h"
#define DLLBLD_SWSYSTEM_RDCTABLE
#include "RdcTable.h"

namespace swmin {


// default constructor
RdcTable::RdcTable()
{
  _commonCtorJob();
  //...
}

RdcTable::RdcTable(TermSet& _p, TermSet& _c)
 : Primes(_p), Cells(_c)
{
  _commonCtorJob();
  //...
}


// called from all constructors
void RdcTable::_commonCtorJob()
{
  //...

}

// destructor
RdcTable::~RdcTable()
{
  //...
}



void RdcTable::invariant()
{
  //...
}

bool RdcTable::GetEquivalentPrimes(TermSet& set)
{
  set.erase(set.begin(), set.end());  // clear result set

  if (noRow() || noCol()) return false;

  Term col(*(Cells.begin()));  // First column
  unsigned long f, outputs=col.funcs.getBits();
  for(f=0; f<outputs; f++) {  // Find first function in column
    if (col.funcs[f]) break;
  }
  if (f>=outputs) {  // No functions (shouldn't happen!), delete column and try again
    DeleteCol(col);
    return GetEquivalentPrimes(set);
  }

  // f has function to check in
  // create mask
  TBitField fn(outputs);
  fn=0;
  fn.set(f,1);
  col.funcs.and(fn);  // select only one function in column
  // build primes who cover this column
  BuildColumn(col, set);
  if (set.size()==0) {  // No rows cover this column (shouldn't happen!)
    DeleteCol(col);                   // Delete column
    return GetEquivalentPrimes(set); // and try again
  }
  // mask functions
  TermSet::iterator i, e;
  for(i=set.begin(), e=set.end(); i!=e; i++) (*i).funcs.and(fn);

  return true;
}

void RdcTable::DeleteRow(Term row)
{
  if (noRow()) return;

  TermSet::iterator i=Primes.find(row);
  if (i==Primes.end()) return;

  TBitField bf(row.funcs);
  bf.not();
  (*i).funcs.and(bf);
  if (!((*i).funcs.truth())) Primes.erase(i);

  //
  // Removal of a row may indirectly remove a column.  The following function
  // removes any columns in 'row' that are empty
  RemoveEmptyColumns(row);
}

bool RdcTable::RemoveEmptyColumns(Term row)
{
  bool removed=false;

  // Find all columns possibly affected by row removal
  TermSet col;
  BuildRow(row, col);

  TermSet::iterator i,e;
  Term t;
  TBitField fn(row.funcs.getBits());
  // Check each column in row (by function) for emptieness
  unsigned long fc=row.funcs.getBits(); // count of functions
  for(unsigned long f=0; f<fc; f++) {
    if (row.funcs[f]) {
      fn=0;
      fn.set(f,1);
      for(i=col.begin(), e=col.end(); i!=e; i++) {
        t=*i;
        t.funcs.and(fn);  // check only function we're checking
        if (t.funcs.truth()) {  // Check only if column exists in this funtion
          if (PrimesCovering(t)==0) { DeleteCol(t); removed=true; }
        } // if
      } // for
    } // if
  }  // for

  return removed;
}

void RdcTable::DeleteCol(Term col)
{
  if (noCol()) return;

  TermSet::iterator i=Cells.find(col);
  if (i==Cells.end()) return;

  TBitField bf(col.funcs);
  bf.not();
  (*i).funcs.and(bf);
  if (!((*i).funcs.truth())) Cells.erase(i);

  // Removal of a column may indirectly erase a row, either entirely or
  // locally within a subset of function.  The following function removes any
  // rows (or clear an output flag) that may have become empty.
  RemoveEmptyRows(col);
}

bool RdcTable::RemoveEmptyRows(Term col)
{
  bool removed=false;

  // Build rows possibly affected by column removal
  TermSet rows;
  BuildColumn(col, rows);

  // Check each row (by function) for emptieness
  TermSet::iterator i,e;
  Term t;
  unsigned long outputs=col.funcs.getBits();
  TBitField fn(outputs);
  for(unsigned long f=0; f<outputs; f++) {
    if (col.funcs[f]) {
      fn=0;
      fn.set(f,1);
      for(i=rows.begin(), e=rows.end(); i!=e; i++) {
        t=*i;
        t.funcs.and(fn);
        if (ColumnsInRow(t)==0) { DeleteRow(t); removed=true; }
      } // for
    } // if
  } // for

  return removed;
}


void RdcTable::DeleteRowAndCol(Term row)
{
  // Delete columns belonging to row
  if (!noCol()) {
    TermSet::iterator i,e;
    Term col;
    for(i=Cells.begin(), e=Cells.end(); i!=e; i++) {
      if ( row.is_partialcover(*i) ) {  // Delete column
        col=*i;
        col.funcs.and(row.funcs);  // Delete only those functions covered
        DeleteCol(col);
        if (noCol()) break;
        i=Cells.begin(); e=Cells.end();  // reset loop pointers
      }
    }
  } // if

  DeleteRow(row);
  
  /*
  // Delete columns belonging to row
  Term::iterator i,e;
  unsigned long outputs=row.funcs.getBits();
  for(i=row.begin(), e=row.end(); i!=e; i++) {
    Term t(outputs);
    t.funcs=row.funcs;
    t.insert(*i);
    DeleteCol(t);
  }

  // Delete row
  DeleteRow(row);
  */
}

void RdcTable::DeleteCommonCol(TermSet& rows)
{
  if (noCol()) return;

  TermSet::iterator ic, ec, ir, er;
  unsigned long outputs=(*rows.begin()).funcs.getBits();
  Term col(outputs);

  // Check each column in column set
  for(ic=Cells.begin(), ec=Cells.end(); ic!=ec; ic++) {
    col=*ic;  // get column
    // Check this column against all given rows
    for(ir=rows.begin(), er=rows.end(); ir!=er; ir++) {
      if ( (*ir).is_partialcover(col) ) {  // At least partially covers
        col.funcs.and( (*ir).funcs );  // take intersection of functions
      } // if
      else {  // Doesn't cover at all, skip this column
        col.funcs=0;  // mark as not covered
        break;  // exit loop checking rows
      } // if-else
    } // for
    // Determine if this is a common column
    if (col.funcs.truth()) {  // It's common to all rows, delete it!
      DeleteCol(col);
      if (noCol()) break;
      ic=Cells.begin(); // Reset loop pointers
      ec=Cells.end();
    }
  } // for
}

unsigned long RdcTable::ColumnsInRow(const Term& row) const
{
  if (noCol()) return 0;

  unsigned long c=0;
  unsigned long outputs=row.funcs.getBits();
  TermSet::iterator i=Cells.begin(), e=Cells.end();
  Term t;
  TBitField fn(outputs);
  while (i!=e) {
    t=*i;
    fn=t.funcs;
    for(unsigned long f=0; f<outputs; f++) {
      if (row.funcs[f] && fn[f]) {
        t.funcs=0;
        t.funcs.set(f,1);
        if (row.is_partialcover(t)) c++;
      } // if
    } // for
    i++;
  } // while

  return c;
}

unsigned long RdcTable::PrimesCovering(const Term& col) const
{
  if (noRow()) return 0;

  unsigned long c=0;
  TermSet::iterator i=Primes.begin(), e=Primes.end();
  while (i!=e) {
    if ( (*i).is_subset(col) ) c++;
    i++;
  }

  return c;
}

bool RdcTable::ColumnDominating(const Term& c1, const Term& c2) const
{
  if (c1==c2) return false;

  TermSet col1, col2;

  BuildColumn(c1, col1);
  BuildColumn(c2, col2);

  // return true if col2 is a subset of col1
  return col1.is_subset(col2);
}

bool RdcTable::RowDominating(const Term& r1, const Term& r2) const
{
  if (r1==r2) return false;

  TermSet row1, row2;

  BuildRow(r1, row1);
  BuildRow(r2, row2);

  // return true if row2 is a subset of row1
  if (!row1.is_subset(row2)) return false;

  // return true if r2 cost more than or equal to r1
  return PriceSheet[r2] >= PriceSheet[r1];
}

// Build set of primes who cover column c
void RdcTable::BuildColumn(const Term& c, TermSet& col) const
{
  if (noRow()) return;

  TermSet::iterator i, e;
  // Scan all primes
  for(i=Primes.begin(), e=Primes.end(); i!=e; i++) {
    // If column term is covered by prime, add to column set
    if ( (*i).is_partialcover(c) ) col.ins(*i);
  }
}

// Build set of columns who are in row
void RdcTable::BuildRow(const Term& r, TermSet& row) const
{
  if (noCol()) return;

  TermSet::iterator i,e;
  Term t;
  unsigned long fc=r.funcs.getBits(); // count of functions
  TBitField fn(fc);
  // Scan all columns
  for(i=Cells.begin(), e=Cells.end(); i!=e; i++) {
    t=*i;
    fn=t.funcs;
    // scan each function separately
    for(unsigned long f=0; f<fc; f++) {
      if (r.funcs[f] && fn[f]) {
        t.funcs=0;  // clear all function bits
        t.funcs.set(f, 1);  // set one function bit
        if ( r.is_subset(t) ) row.ins(t);
      }
    }
  }
}

bool RdcTable::GetEssential(Term& ess, Term& col) const
{
  if (noCol() || noRow()) return false;

  TermSet::iterator ic, ec;  // iterators for Cells
  Term t;
  unsigned long fc=(*Cells.begin()).funcs.getBits(); // count of functions
  TBitField fn(fc);
  // scan all columns
  for(ic=Cells.begin(), ec=Cells.end(); ic!=ec; ic++) {
    t=*ic;
    fn=t.funcs;
    // scan each function separately
    for(unsigned long f=0; f<fc; f++) {
      if (fn[f]) {  // If column exists in this function
        t.funcs=0;  // clear all function bits
        t.funcs.set(f, 1);  // set one function bit
        // check for essential
        if (PrimesCovering(t)==1) {
          // Find prime term and return it
          TermSet::iterator ip, ep;
          // Scan all primes
          for(ip=Primes.begin(), ep=Primes.end(); ip!=ep; ip++) {
            if ( (*ip).is_subset(t) ) {
              ess=*ip;
              col=t;
              return true;
            } // if
          } // for
        } // if
      } // if
    } // for
  } // for
  return false;
}

bool RdcTable::noCol() const
{
  return Cells.size()==0;
}

bool RdcTable::noRow() const
{
  return Primes.size()==0;
}

}

