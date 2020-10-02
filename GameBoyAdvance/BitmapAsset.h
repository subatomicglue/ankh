#ifndef GBA_BITMAPASSET_H
#define GBA_BITMAPASSET_H

#include "common.h"
#include "spewgba.h"

namespace gba
{
   // a bitmap.
   // tiles x,y, pixels x,y, data pointer.
   //
   // construction:
   // const BitmapAsset bmp = {x,y,xpx,ypx,data};
   struct BitmapAsset
   {
      inline const unsigned char* const tile( int t ) const
      {
         spew::Assert( t < (tx * ty), "out of bounds" );
         return &d[tpx * tpy * t];
      }

      /// number of tiles
      inline u32 ts() const { return tx * ty; }

      /// how many pixels in one tile
      inline u32 tps() const { return tpx * tpy; }
      
      /// total width of bitmap
      inline u32 px() const { return tpx * tx; }
      inline u32 py() const { return tpy * ty; }

      /// read one pixel from location x,y in the bitmap
      inline u8 pixel( u32 x, u32 y ) const
      {
         return d[y * px() + x];
      }

      /// read one pixel from the tile at location x,y (relative to the tile).
      inline u8 pixelT( u32 t, u32 x, u32 y ) const
      {
         return tile( t )[y * tpx + x];
      }

      /// read one pixel from the sprite tile at location x,y (relative to the tile).
      /// sprites are stored in 8x8 blocks, in book reading order (left to right, top to bottom)
      inline u8 pixelST( u32 t, u32 x, u32 y ) const
      {
         const int numTilesX = tpx / 8;
         const int pixelsPerTileX = 8;
         const int pixelsPerTileY = 8;
         const int pixelsPerTile = 64; // pixelsPerTileX * pixelsPerTileY
         const int subTile = (x / pixelsPerTileX)  +  (numTilesX * (y / pixelsPerTileY));
         return tile( t )[subTile * pixelsPerTile +  (y % pixelsPerTileY) * pixelsPerTileX + (x % pixelsPerTileX)];
      }

      /// read one pixel from the sprite tile at location x,y (relative to the tile).
      /// sprites are stored in 8x8 blocks, in book reading order (left to right, top to bottom)
      inline u8 pixelST( u32 x, u32 y ) const
      {
         const int numTilesX = tpx / 8;
         const int pixelsPerTileX = 8;
         const int pixelsPerTileY = 8;
         const int pixelsPerTile = 64; // pixelsPerTileX * pixelsPerTileY
         const int subTile = (x / pixelsPerTileX)  +  (numTilesX * (y / pixelsPerTileY));
         return d[subTile * pixelsPerTile +  (y % pixelsPerTileY) * pixelsPerTileX + (x % pixelsPerTileX)];
      }


      /// total num pixels
      inline u32 ps() const { return tpx * tx * tpy * ty; }

      /// get position of tile t (in pixels) relative to 0,0 upper/left corner of bitmap
      //inline s32 getTilePosX( u32 t ) const { return tpx * t; }
      //inline s32 getTilePosY( u32 t ) const { return tpy * t/tx; }

      // needs to match memory setup
      // init the struct with {} initialization so this stuff goes into ROM.
      const u32 tx,  ty;  // x, y (how many tiles across)
      const u32 tpx, tpy; // x, y (num pixels in one tile...)
      const u8* const d;        // the bitmap data
      const u16* const palette; // the bitmap palette
   };
}


#endif
