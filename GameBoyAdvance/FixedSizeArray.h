
#ifndef FIXED_SIZE_ARRAY
#define FIXED_SIZE_ARRAY

#include "spewgba.h"

template <typename T, unsigned SIZE>
class FixedSizeArray
{
public:
   inline FixedSizeArray() : mSize( 0 )
   {
   }
   inline void push_back( T val )
   {
      spew::Assert( mSize < SIZE, "out of bounds" );
      mData[mSize] = val;
   }
   inline void pop_back()
   {
      spew::Assert( 0 < mSize, "out of bounds" );
      --mSize;
   }
   inline void erase( int x )
   {
      spew::Assert( 0 < mSize, "out of bounds" );
      mData[x] = mData[mSize-1];
      --mSize;
   }
   inline T& operator[]( int x )
   { 
      spew::Assert( 0 <= mSize && mSize < SIZE, "out of bounds" );
      return mData[x];
   }
   inline const T& operator[]( int x ) const
   { 
      spew::Assert( 0 <= mSize && mSize < SIZE, "out of bounds" );
      return mData[x];
   }
   inline s16 size() const { return mSize; }
private:
   T mData[SIZE];
   s16 mSize;
};

#endif
