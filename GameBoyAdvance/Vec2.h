#ifndef _VEC2_H_
#define _VEC2_H_

#include <math.h>
//#include "swi.h" // bios calls.

namespace gba
{
   template< class DATA_TYPE >
   class Vec
   {
   public:
      Vec()
      {
      }
      
      Vec( DATA_TYPE x, DATA_TYPE y ) : mX( x ), mY( y )
      {
      }

      template <typename DT>
      Vec( const Vec<DT>& d ) : mX( d[0] ), mY( d[1] )
      {
      }

      DATA_TYPE* getData() { return &mX; }
      const DATA_TYPE* const getData() const { return &mX; }
      DATA_TYPE& operator[]( unsigned int x ) { return getData()[x]; }
      const DATA_TYPE& operator[]( unsigned int x ) const { return getData()[x]; }
      inline void set( DATA_TYPE x, DATA_TYPE y )
      {
         mX = x;
         mY = y;
      }

      inline void set( const Vec<DATA_TYPE>& d )
      {
         mX = d.mX;
         mY = d.mY;
      }

      inline Vec<DATA_TYPE> abs() const
      {
         return Vec<DATA_TYPE>( ::abs( mX ), ::abs( mY ) );
      }
      

      inline Vec<DATA_TYPE> operator/( const Vec<DATA_TYPE>& rhs ) const
      {
         return Vec<DATA_TYPE>( mX / rhs[0], mY / rhs[1] );
      }
      inline Vec<DATA_TYPE> operator*( const Vec<DATA_TYPE>& rhs ) const
      {
         return Vec<DATA_TYPE>( mX * rhs[0], mY * rhs[1] );
      }
      inline Vec<DATA_TYPE> operator*( DATA_TYPE rhs ) const
      {
         return Vec<DATA_TYPE>( mX * rhs, mY * rhs );
      }
      inline Vec<DATA_TYPE> operator/( DATA_TYPE rhs ) const
      {
         return Vec<DATA_TYPE>( mX / rhs, mY / rhs );
      }
      DATA_TYPE lengthSqr() const
      {
         return mX * mX + mY * mY;
      }
      DATA_TYPE length() const
      {
         return (DATA_TYPE)sqrt( mX * mX + mY * mY ); // TODO, int version of sqrt?
      }

   private:
      DATA_TYPE mX, mY;
   };

   template <typename DATA_TYPE, typename DATA_TYPE1, typename DATA_TYPE2>
   inline Vec<DATA_TYPE> operator+( const Vec<DATA_TYPE1>& lhs, const Vec<DATA_TYPE2>& rhs )
   {
      return Vec<DATA_TYPE>( lhs[0] + rhs[0], lhs[1] + rhs[1] );
   }


   /** add 2 point/vec */
   template< class DATA_TYPE >
   inline DATA_TYPE operator+( const DATA_TYPE& pnt1, const DATA_TYPE& pnt2 )
   {
      return DATA_TYPE( pnt1[0] + pnt2[0], pnt1[1] + pnt2[1] );
   }

   /** add 2 point/vec */
   template< class DATA_TYPE >
   inline DATA_TYPE operator-( const DATA_TYPE& pnt1, const DATA_TYPE& pnt2 )
   {
      return DATA_TYPE( pnt1[0] - pnt2[0], pnt1[1] - pnt2[1] );
   }

   /** add 2 point/vec */
   template< class DATA_TYPE >
   inline DATA_TYPE operator-( const DATA_TYPE& pnt1 )
   {
      return DATA_TYPE( -pnt1[0], -pnt1[1] );
   }

   /** div by scalar */
   template< typename DATA_TYPE, typename DATA_TYPE2 >
   inline Vec<DATA_TYPE> operator/( const Vec<DATA_TYPE>& lhs, DATA_TYPE2 rhs )
   {
      return Vec<DATA_TYPE>( lhs[0] / rhs, lhs[1] / rhs );
   }

   /** -= point/vec */
   template< class DATA_TYPE >
   inline DATA_TYPE operator+=( DATA_TYPE& pnt1, const DATA_TYPE& pnt2 )
   {
      pnt1[0] += pnt2[0];
      pnt1[1] += pnt2[1];
      return pnt1;
   }

   /** -= point/vec */
   template< class DATA_TYPE >
   inline DATA_TYPE operator-=( DATA_TYPE& pnt1, const DATA_TYPE& pnt2 )
   {
      pnt1[0] -= pnt2[0];
      pnt1[1] -= pnt2[1];
      return pnt1;
   }

   template< class DATA_TYPE >
   inline bool operator==( const DATA_TYPE& pnt1, const DATA_TYPE& pnt2 )
   {
      return pnt1[0] == pnt2[0] && pnt1[1] == pnt2[1];
   }

   template< class DATA_TYPE >
   inline bool operator!=( const DATA_TYPE& pnt1, const DATA_TYPE& pnt2 )
   {
      return pnt1[0] != pnt2[0] || pnt1[1] != pnt2[1];
   }

}

#endif
