#ifndef ANKH_COMMON_H
#define ANKH_COMMON_H

#include <stdlib.h> // abs
#include "spewgba.h"
#include "Vec2.h"

// types
#ifndef __cplusplus
typedef     unsigned char           bool;
#endif
typedef void (*fp)(void);

typedef     unsigned char           u8;
typedef     unsigned short int      u16;
typedef     unsigned int            u32;
typedef     unsigned long long int  u64;
typedef     signed char             s8;
typedef     signed short int        s16;
typedef     signed int              s32;
typedef     signed long long int    s64;
typedef     volatile unsigned char           vu8;
typedef     volatile unsigned short int      vu16;
typedef     volatile unsigned int            vu32;
typedef     volatile unsigned long long int  vu64;
typedef     volatile signed char             vs8;
typedef     volatile signed short int        vs16;
typedef     volatile signed int              vs32;
typedef     volatile signed long long int    vs64;
typedef     s16     sfp16;  //1:7:8 fixed point
typedef     s32     sfp32;  //1:19:8 fixed point
typedef     u16     ufp16;  //8:8 fixed point
typedef     u32     ufp32;  //24:8 fixed point

template <typename T>
inline T clamp( T n, T low, T high )
{
   if (n < low) return low;
   else if (high < n) return high;
   else return n;
}

template <typename T>
inline T Min( T low, T high ) { return low < high ? low : high; }

template <typename T>
inline T Max( T low, T high ) { return low <= high ? high : low; }

/// lerpval is from lerprange to 1
template <typename T>
inline T lerp( T lerpval, T lerprange, T lo, T hi )
{
   T size = hi - lo;
   return (lerpval * size / lerprange) + lo;
}

inline s32 rand( s32 from, s32 to )
{
   if (to < from)
   {
      // swap
      s32 temp = from;
      from = to;
      to = temp;
   }
   s32 r = ((unsigned int)rand()) % (1+to-from);
   r += from;
   return r;
}

class Camera
{
public:
   Camera()
   {
      //mPosRevert.set( 0, 0 );
      //mOffsetRevert.set( 0, 0 );

      mPos.set( 0, 0 );
      mPosActual.set( 0, 0 );
      mOffset.set( 0, 0 );
      mOffsetActual.set( 0, 0 );
      mExtentsX = 999999;
      mExtentsY = 999999;
   }
   void revert()
   {
      //spew::GBAPrint( "r lsqr:%02d  rev:%d,%d ", (mOffsetActual - mOffset).lengthSqr(), mOffsetRevert[0], mOffsetRevert[1] );
      //spew::GBAPrint( "off:%d,%d  oa:%d,%d  | ", mOffset[0], mOffset[1], mOffsetActual[0], mOffsetActual[1] );
      mOffsetActual = mOffsetRevert;
      mPosActual = mPosRevert;
      //spew::GBAPrint( "off:%d,%d  oa:%d,%d\n", mOffset[0], mOffset[1], mOffsetActual[0], mOffsetActual[1] );
   }
   void update()
   {
      //spew::GBAPrint( "u lsqr:%02d  rev:%d,%d ", (mOffsetActual - mOffset).lengthSqr(), mOffsetRevert[0], mOffsetRevert[1] );
      //spew::GBAPrint( "off:%d,%d  oa:%d,%d  | ", mOffset[0], mOffset[1], mOffsetActual[0], mOffsetActual[1] );
      mOffsetRevert = mOffsetActual;
      mPosRevert = mPosActual;
      if ((mOffsetActual - mOffset).lengthSqr())
      {
         //spew::GBAPrint( "[a]" );
         gba::Vec<s32> m0ve = (mOffset - mOffsetActual);
         gba::Vec<s32> m1ve = (mPos - mPosActual);
         gba::Vec<s32> d0r = (m0ve / m0ve.abs());
         gba::Vec<s32> m1ve_abs = m1ve.abs();
         gba::Vec<s32> d1r;// = (m1ve / m1ve.abs());
         d1r[0] = 4 <= m1ve_abs[0] ? (m1ve[0] / m1ve_abs[0])*(m1ve_abs[0]/2) : (m1ve[0] / m1ve_abs[0] );
         d1r[1] = 4 <= m1ve_abs[1] ? (m1ve[1] / m1ve_abs[1])*(m1ve_abs[1]/2) : (m1ve[1] / m1ve_abs[1] );
         mOffsetActual += d0r;
         mPosActual += d1r;
      }
      else
      {
         //spew::GBAPrint( "[b]" );
         mPosActual = mPos;
      }
      //spew::GBAPrint( "off:%d,%d  oa:%d,%d\n", mOffset[0], mOffset[1], mOffsetActual[0], mOffsetActual[1] );
   }
   s32 posx() const { return clamp( mPosActual[0] + mOffsetActual[0], 0, mExtentsX ); }
   s32 posy() const { return clamp( mPosActual[1] + mOffsetActual[1], 0, mExtentsY ); }

   void setExtents( s32 w, s32 h ) { mExtentsX = w; mExtentsY = h; }

   void setPos( s32 x, s32 y ) { mPos[0] = x; mPos[1] = y; }
   void setPosX( s32 x ) { mPos[0] = x; }
   void setPosY( s32 y ) { mPos[1] = y; }
   void setOffset( s8 x, s8 y ) { spew::GBAPrint( "setoff: %d %d\n", x, y ); mOffset[0] = x; mOffset[1] = y; }
private:
   gba::Vec<s32> mPos;
   gba::Vec<s32> mPosActual;
   gba::Vec<s32> mOffsetActual;
   gba::Vec<s32> mOffset;
   s32 mExtentsX;
   s32 mExtentsY;
   gba::Vec<s32> mPosRevert;
   gba::Vec<s32> mOffsetRevert;
};
extern Camera gCamera;
class WindowDimensions
{
public:
   WindowDimensions( s32 x, s32 y, s32 w, s32 h ) : 
         mX( x ), mY( y ), mWidth( w ), mHeight( h ), mWidthTiles( w / 8 ), mHeightTiles( h / 8 )
   {}
   s32 mX, mY, mWidth, mHeight;
   s32 mWidthTiles, mHeightTiles;
};
extern WindowDimensions gGameWindow;
extern WindowDimensions gScreenWindow;

static const s32 tilesizex = 8, tilesizey = 8;
static const s32 tilesperroomx = 24, tilesperroomy = 24;
static const s32 roomsizex = tilesizex * tilesperroomx,
                 roomsizey = tilesizey * tilesperroomy;


// tile number we can pass through.
inline bool isCollidableTile( u32 tile )
{
   return !(
            (tile == 346) ||
            (tile == 347) ||
            (tile == 348) ||
            (tile == 349) ||
            (tile == 378) ||
            (tile == 379) ||
            (tile == 380) ||
            (tile == 381)
          );
}

// pixel number we can pass through.
inline bool isCollidableBgPixel( u32 pixel )
{
   return pixel != 200;
}
inline bool isCollidableActorPixel( u32 pixel )
{
   return pixel != 0;
}


#endif
