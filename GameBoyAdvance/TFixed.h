
#include <iostream>
#include <assert.h>

#if defined(_MSC_VER) && !defined(__MWERKS__) && \
    !defined(__ICL) && !defined(__COMO__) && \
    !defined(__BORLANDC__)
  #define MSVC
# endif


typedef unsigned int TUInt32;
typedef unsigned short TUInt16;
typedef unsigned char TUInt8;
#ifdef MSVC
typedef __int64 TUInt64;
typedef __int64 Int64;
#else
typedef unsigned long long TUInt64;
typedef long long TInt64;
#endif
typedef int TInt32;
typedef short TInt16;
typedef char TInt8;

template <bool x> struct _STATIC_ASSERTION_FAILURE;
template <> struct _STATIC_ASSERTION_FAILURE<true>{};
#define STATIC_ASSERTION_FAILURE( arg, msg ) \
		_STATIC_ASSERTION_FAILURE<arg && msg>()

template <typename TYPE> struct NextSizeUpType { };
//template <> struct NextSizeUpType<TInt64> { typedef TInt64 Type; }; // this should be a warning... instead the user will just have to be smart and know why things are overflowing...
template <> struct NextSizeUpType<TInt32> { typedef TInt64 Type; };
template <> struct NextSizeUpType<TInt16> { typedef TInt32 Type; };
template <> struct NextSizeUpType<TInt8> { typedef TInt16 Type; };
template <typename TYPE> struct NextSizeDownType { };
template <> struct NextSizeDownType<TInt64> { typedef TInt32 Type; };
template <> struct NextSizeDownType<TInt32> { typedef TInt16 Type; };
template <> struct NextSizeDownType<TInt16> { typedef TInt8 Type; };

/** output 010101 representation of the number */
template <typename T>
inline void outputBin( std::ostream& out, T a )
{
	long long bytes = (*((long long*)(&a)));
	int size = (sizeof( T ) * 8);
	for (int x = size-1; x >= 0; --x)
		out << ((bytes & ((long long)(1) << x)) >> x);
}

/** fixed point type. 
 *  represent real numbers using fixed point representation
 *  this type behaves like built-in types such as float/int/double, etc... 
 *  with full auto conversion by the compiler (be careful, no warnings)...
 *  It is templated, so that you may specify the precision of the fixed point representation.
 */
template <unsigned NUM_INTEGER_BITS, unsigned NUM_FRACTIONAL_BITS, typename INTEGER_TYPE>
class TFixed
{
	public:
		typedef INTEGER_TYPE Type;
		enum
      {
         IntBits = NUM_INTEGER_BITS,
		   FracBits = NUM_FRACTIONAL_BITS,
		   Bits = sizeof( Type ) * 8,
		   ShiftAmount = FracBits,
		   ShiftMult = 1 << ShiftAmount
      };
		Type m_nData; // fixedpoint data representation.

		/** default constructor */
		TFixed()
		{
			STATIC_ASSERTION_FAILURE( (IntBits + FracBits) == sizeof( Type ) * 8 && 
											  (IntBits + FracBits) == Bits, 
											  "invalid template args, sizeof(type) * 8 == isize + fsize" ); 
		}
		
		struct DataConstructor {};

		/** data constructor */
		TFixed( int nData, const DataConstructor& type )
		{
			// todo:  assert on size of integer overflowing nData
			m_nData = nData;
		}
		
		/** integer constructor */
		TFixed( int nInteger )
		{
			// todo:  assert on size of integer overflowing nData
			m_nData = nInteger << FracBits;
		}

		/** float constructor */
		TFixed( float rReal )
		{
			m_nData = Type(rReal * float(ShiftMult));
		}

		/** double constructor */
		TFixed( double rReal )
		{
			m_nData = Type(rReal * double(ShiftMult));
		}

		/** copy constructor */	
		TFixed( const TFixed& rValue )
		{
			m_nData = rValue.m_nData;
		}

      /** equality operator */
      TFixed& operator=( const TFixed& rValue )
      {
         m_nData = rValue.m_nData;
         return *this;
      }

      /** you can convert this class type to an integer type */
      inline operator Type() const
      {
         return m_nData >> FracBits;
      }

      /** you may convert this class type to a single precision floating point type */
      inline operator float() const
      {
         // assume mult is faster than div
         // assume 1 / shiftmult is computed at compile time.
         // assumes return value optimization (see s.meyars)
         return float( m_nData * (float(1) / float(ShiftMult)) );
      }

      /** you may convert this class type to a double precision floating point type */
      inline operator double() const
      {
         // assume mult is faster than div
         // assume 1 / shiftmult is computed at compile time.
         // assumes return value optimization (see s.meyars)
         return double( m_nData * (double(1) / double(ShiftMult)) );
      }

		/** multiply two fixed point numbers, return result as a temporary */
		inline const TFixed operator*( const TFixed& rhs )
		{
         // Xbit * Xbit yields a (X*2)bit quantity
         const typename NextSizeUpType<Type>::Type nLhs( m_nData ), nRhs( rhs.m_nData );

         // also truncates the larger size back down to Type size.
		   // assumes return value optimization (see s.meyars)
         return TFixed( (nLhs * nRhs) >> FracBits, DataConstructor() );
		}

		/** divide two fixed point numbers, return result as a temporary */
		inline const TFixed operator/( const TFixed& rhs )
		{
         // (X*2)bit / Xbit yields a 32bit quantity
		   typename NextSizeUpType<Type>::Type nRhs = rhs.m_nData, nLhs = m_nData;
			nLhs <<= FracBits;

         // also truncates the larger size back down to Type size.
		   // assumes return value optimization (see s.meyars)
         return TFixed( (nLhs / nRhs), DataConstructor() );
		}

		/** add two fixed point numbers, return result as a temporary */
		inline const TFixed operator+( const TFixed& rhs )
		{
		   // assumes return value optimization (see s.meyars)
         return TFixed( m_nData + rhs.m_nData, DataConstructor() );
		}

		/** subtract two fixed point numbers, return result as a temporary */
		inline const TFixed operator-( const TFixed& rhs )
		{
		   // assumes return value optimization (see s.meyars)
         return TFixed( m_nData - rhs.m_nData, DataConstructor() );
		}

      inline TFixed& operator+=( const TFixed& rhs )
      {
         m_nData += rhs.m_nData;
         return *this;
      }

      inline TFixed& operator-=( const TFixed& rhs )
      {
         m_nData -= rhs.m_nData;
         return *this;
      }

      inline TFixed& operator*=( const TFixed& rhs )
      {
         // Xbit * Xbit yields a (X*2)bit quantity
         const typename NextSizeUpType<Type>::Type nLhs( m_nData ), nRhs( rhs.m_nData );

         // also truncates the larger size back down to Type size.
		   // assumes return value optimization (see s.meyars)
         m_nData = Type( (nLhs * nRhs) >> FracBits );
         return *this;
      }

      inline TFixed& operator/=( const TFixed& rhs )
      {
         // (X*2)bit / Xbit yields a 32bit quantity
		   typename NextSizeUpType<Type>::Type nRhs = rhs.m_nData, nLhs = m_nData;
			nLhs <<= FracBits;

         // truncates the larger size back down to Type size.
         m_nData = Type( nLhs / nRhs );
         return *this;
      }

      /** prefix++ add one to this, then return new value */
      inline TFixed& operator++()
      {
         m_nData += Type(1) << FracBits;
         return *this;
      }

      /** prefix-- subtract one from this, then return new value */
      inline TFixed& operator--()
      {
         m_nData -= Type(1) << FracBits;
         return *this;
      }

      /** postfix++ add one to this, but returns the original value before subtraction */
      inline const TFixed operator++( int notused )
      {
         TFixed temporary( *this );
         m_nData += Type(1) << FracBits;
         return temporary;
      }

      /** postfix-- subtract one from this, but returns the original value before subtraction */
      inline const TFixed operator--( int notused )
      {
         TFixed temporary( *this );
         m_nData -= Type(1) << FracBits;
         return temporary;
      }
};

template <unsigned A, unsigned B, typename Type>
inline std::ostream& operator<<( std::ostream& out, const TFixed<A,B,Type>& rhs )
{
	out << float( rhs );
}
