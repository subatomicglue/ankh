

#ifndef BACKGROUND
#define BACKGROUND

#include "BitmapAsset.h"
#include "MapAsset.h"
#include "Vec2.h"
#include "AABox2.h"
#include "common.h"

class Background
{
public:
   Background( const gba::BitmapAsset& bitmap, const gba::MapAsset& maptiles,
               const gba::BitmapAsset& hudbitmap, const gba::MapAsset& hudtiles ) : 
      mBitmap( bitmap ), mMap( maptiles ), mHudBitmap( hudbitmap ), mHudMap( hudtiles )
   {
   }

   void print( const int xpos, int ypos, const char* const text )
   {
      const int size = strlen( text );
      int xinc = 0;
      for (int x = 0; x < size; ++x)
      {
         if (text[x] == '\n')
         {
            ++ypos;
            xinc = 0;
         }
         else
         {
            spew::Assert( ' ' <= text[x] && text[x] <= 'Z', "no lower case allowed..." ); 
            setHudTile( xpos + xinc, ypos, 17 + (text[x] - ' ') ); // alphabet starts at ' ' at index 17 in hud.bmp
            ++xinc;
         }
      }
   }
   // print special font, numbers only, no newlines
   void printnum( const int xpos, int ypos, const char* const text )
   {
      const int size = strlen( text );
      int xinc = 0;
      for (int x = 0; x < size; ++x)
      {
         if (text[x] == '\n')
         {
            ++ypos;
            xinc = 0;
         }
         else
         {
            spew::Assert( ' ' <= text[x] && text[x] <= 'Z', "no lower case allowed..." ); 
            setHudTile( xpos + xinc, ypos, 7 + (text[x] - '0') ); // numbers starts at '0' at index 7 in hud.bmp
            ++xinc;
         }
      }
   }

   void init()
   {
      mPos[0] = 0;
      mPos[1] = 0;
      old_cell[0] = 78; // garbage
      old_cell[1] = 23; // garbage

      gCamera.setExtents( s32(mMap.tx * mBitmap.tpx) - gGameWindow.mWidthTiles,
                          s32(mMap.ty * mBitmap.tpy) - gGameWindow.mHeightTiles );
      
      REG_BG0CNT = BGPRIORITY1 | BGCHARBASEBLOCK(0) | BGCOLORMODE1X256 | BGSCREENBASEBLOCK(24) | BGTEXTSCREENSIZE256x256;
      REG_BG1CNT = BGPRIORITY0 | BGCHARBASEBLOCK(2) | BGCOLORMODE1X256 | BGSCREENBASEBLOCK(31) | BGTEXTSCREENSIZE256x256;

      // copy tile bitmaps to the video hardware

      {
         const u32 size = mBitmap.ps() / 2; // size can be large, must use 32bit (256x256 exceeds u16 range)
         const u16* const pSrc = (const u16* const)mBitmap.d; // copy 2 bytes at a time (either hardware req, or faster, don't remember which)
         for(u32 i = 0; i < size; ++i)
         {
            GBA_BG_DATA_S(0)[i] = pSrc[i];
         }
      }
      {
         const u32 size = mHudBitmap.ps() / 2; // size can be large, must use 32bit (256x256 exceeds u16 range)
         const u16* const pSrc = (const u16* const)mHudBitmap.d; // copy 2 bytes at a time (either hardware req, or faster, don't remember which)
         for(u32 i = 0; i < size; ++i)
         {
            GBA_BG_DATA_S(2)[i] = pSrc[i];
         }
      }

      /*
      // what's this do?
      GBA_BG_DATA[0] = 0;
      GBA_BG_DATA[1] = 1;
      GBA_BG_DATA[2] = 2;
      GBA_BG_DATA[3] = 3;
  */
      // copy tile indexes to the video mem...
      gbaCopyBgMap( GBA_BG_DATA_M(24), mMap.d, mMap.tx, mMap.ty, 0, 0, 32, 32 );
      gbaCopyBgMap( GBA_BG_DATA_M(31), mHudMap.d, mHudMap.tx, mHudMap.ty, 0, 0, 32, 32 );
   }

   inline void setHudTile( int x, int y, int index )
   {
      // 32 = tilemem_width
      // 31 = where we have the hud tile indexes in video mem...
      GBA_BG_DATA_M(31)[x + y * 32] = index;
   }

   void kill()
   {
   }

   void updateStats()
   {
   }

   s32 mRoom[2];
   u64 mRooms; // bitmask representing the rooms...
   void update()
   {
      // get local screen position of the bg (inverted)
      s32 localx = -(mPos[0] - gCamera.posx()), 
          localy = -(mPos[1] - gCamera.posy());
      
      // Page map data to video memory:
      // we've divided the map into cells so we can handle huge maps that exceed the GBA video memory
      // when we enter/exit a cell, we load the new cell's map over the old 

      // what cell are we in?
      // cells have nxn tiles, and exist at kxp intervals.
      const u16 tile_size = 8;
      const gba::Vec<u16> num_cells_on_screen( gGameWindow.mWidthTiles + 2, 28 ); // visible
      const gba::Vec<u16> interval_size( 2, 8 );
      
      // set the position of the map
      const u16 current_pxx = localx % (interval_size[0] * tile_size);
      const u16 current_pxy = localy % (interval_size[1] * tile_size);
      REG_BG0SCRLX = current_pxx;
      REG_BG0SCRLY = current_pxy;

      // dynamically page in/out the map as needed.

      // get what tile/cell we're in
      const gba::Vec<u16> current_tile( localx / tile_size, localy / tile_size );
      const gba::Vec<u16> current_cell( current_tile[0] / interval_size[0], 
                                        current_tile[1] / interval_size[1] );
      const gba::Vec<u16> current_tile_adjusted( current_cell[0] * interval_size[0],
                                                 current_cell[1] * interval_size[1] );

      // if we're in a new cell, page in the new map data
      if (current_cell[0] != old_cell[0] ||
          current_cell[1] != old_cell[1])
      {
         gbaCopyBgMap( GBA_BG_DATA_M(24), mMap.d, mMap.tx, mMap.ty,
                        current_tile_adjusted[0],
                        current_tile_adjusted[1],
                        num_cells_on_screen[0], 
                        num_cells_on_screen[1] );

         if (mRoom[0] == 1 && mRoom[1] == 2)
         {
            s32 x = 27 - current_tile_adjusted[0];
            s32 y = 51 - current_tile_adjusted[1];
            spew::GBAPrint( "la\n" );
            gbaCopyBgMap8x8( GBA_BG_DATA_M(24), (u64)mRooms, x, y, mMap.d, mMap.tx, mMap.ty );
            spew::GBAPrint( "dropping bg map tiles at [%d, %d]\n", x, y );
         }
      }

      // save the cell info for next time.
      old_cell = current_cell;
   }

   inline s32 getMinX() const { return 0; }
   inline s32 getMaxX() const { return mMap.tx; }
   inline s32 getMinY() const { return 0; }
   inline s32 getMaxY() const { return mMap.ty; }

   inline const gba::MapAsset& getMap() const { return mMap; }
   inline const gba::BitmapAsset& getBitmap() const { return mBitmap; }

private:
   gba::Vec<u16> old_cell;
   gba::Vec<s32> mPos;
   const gba::BitmapAsset& mBitmap;
   const gba::MapAsset& mMap;

   // hud
   const gba::BitmapAsset& mHudBitmap;
   const gba::MapAsset& mHudMap;
};



/// get the tile index at location x,y in world space (0,0 is top corner of map)
inline u8 getTileIndex( const Background& bg, s32 x, s32 y )
{
   // to get color, do a couple lookups...
   // worldspace -> tileindex -> tilebitmap -> palette
   //spew::Assert( x < tileWidth() && y < tileHeight(), "out of bounds" );
   u32 tile = bg.getMap().getTile( x / bg.getBitmap().tpx, y / bg.getBitmap().tpy );
   return tile;
}

/// get the pixel at location x,y in world space (0,0 is top corner of map)
inline u8 getColorIndex( const Background& bg, s32 x, s32 y )
{
   // to get color, do a couple lookups...
   // worldspace -> tileindex -> tilebitmap -> palette
   //spew::Assert( x < tileWidth() && y < tileHeight(), "out of bounds" );
   u32 tile = bg.getMap().getTile( x / bg.getBitmap().tpx, y / bg.getBitmap().tpy );
   return bg.getBitmap().pixelT( tile, x % bg.getBitmap().tpx, y % bg.getBitmap().tpy );
}

/// does pixel accurate intersection between bg and actor.
/// BEWARE: (duplicate of below code, adds actor pixel collision, keep in sync!)
inline bool intersect( const Background& bg, const Actor& act )
{
   // actor world rect:
   const gba::AABox2<s32> actorBox = act.getWorldBox();

   // get all the tiles the actor is intersecting with.
   const u32 tile_lt0 = u32(actorBox.mMin[0]) / bg.getBitmap().tpx;
   const u32 tile_lt1 = u32(actorBox.mMin[1]) / bg.getBitmap().tpy;
   const u32 tile_rb0 = u32(actorBox.mMax[0]) / bg.getBitmap().tpx;
   const u32 tile_rb1 = u32(actorBox.mMax[1]) / bg.getBitmap().tpy;

   // for each bg tile that intersects the actor...
   for (u32 tx = tile_lt0; tx <= tile_rb0; ++tx)
   {
      // tile world rect:
      gba::AABox2<s32> tileBox;
      tileBox.mMin[0] = tx * bg.getBitmap().tpx;
      tileBox.mMax[0] = tileBox.mMin[0] + bg.getBitmap().tpx;

      for (u32 ty = tile_lt1; ty <= tile_rb1; ++ty)
      {
         u32 tile = bg.getMap().getTile( tx, ty );
         if (isCollidableTile( tile ))
         {
            //return true; // disable the expensive per pixel collision

            // tile world rect (cont):
            tileBox.mMin[1] = ty * bg.getBitmap().tpy;
            tileBox.mMax[1] = tileBox.mMin[1] + bg.getBitmap().tpy;

            // intersection of actor and tile rects:
            gba::AABox2<s32> isectBox;
            if (gba::intersection( isectBox, actorBox, tileBox ))
            {
               // traverse the box pixels in worldspace... (wx/y)
               for (int wy = isectBox.mMin[1]; wy <= isectBox.mMax[1]; ++wy)
               {
                  for (int wx = isectBox.mMin[0]; wx <= isectBox.mMax[0]; ++wx)
                  {
                     // lookup worldspace actor pixel (transform to actorspace to do the lookup)...
                     s32 ax = wx - act.pos(0);
                     s32 ay = wy - act.pos(1);
                     u8 actorPixel = act.bitmap()->pixelST( ax, ay );

                     // lookup bg pixel
                     u8 bgPixel = getColorIndex( bg, wx, wy );
                     if (isCollidableActorPixel( actorPixel ) && isCollidableBgPixel( bgPixel ))
                     {
                        return true;
                     }
                  }
               }
            }
         }
      }
   }

   return false;
}


/// does pixel accurate intersection between bg and _box_ 
/// BEWARE: (duplicate of above code minus actor pixel collision, keep in sync!)
inline bool intersect( const Background& bg, const gba::AABox2<s32>& actorBox )
{
   // actor world rect:
   //const gba::AABox2<s32> actorBox = act.getWorldBox();

   // get all the tiles the actor is intersecting with.
   const u32 tile_lt0 = u32(actorBox.mMin[0]) / bg.getBitmap().tpx;
   const u32 tile_lt1 = u32(actorBox.mMin[1]) / bg.getBitmap().tpy;
   const u32 tile_rb0 = u32(actorBox.mMax[0]) / bg.getBitmap().tpx;
   const u32 tile_rb1 = u32(actorBox.mMax[1]) / bg.getBitmap().tpy;

   // for each bg tile that intersects the actor...
   for (u32 tx = tile_lt0; tx <= tile_rb0; ++tx)
   {
      // tile world rect:
      gba::AABox2<s32> tileBox;
      tileBox.mMin[0] = tx * bg.getBitmap().tpx;
      tileBox.mMax[0] = tileBox.mMin[0] + bg.getBitmap().tpx;

      for (u32 ty = tile_lt1; ty <= tile_rb1; ++ty)
      {
         u32 tile = bg.getMap().getTile( tx, ty );
         if (isCollidableTile( tile ))
         {
            //return true; // disable the expensive per pixel collision

            // tile world rect (cont):
            tileBox.mMin[1] = ty * bg.getBitmap().tpy;
            tileBox.mMax[1] = tileBox.mMin[1] + bg.getBitmap().tpy;

            // intersection of actor and tile rects:
            gba::AABox2<s32> isectBox;
            if (gba::intersection( isectBox, actorBox, tileBox ))
            {
               // traverse the box pixels in worldspace... (wx/y)
               for (int wy = isectBox.mMin[1]; wy <= isectBox.mMax[1]; ++wy)
               {
                  for (int wx = isectBox.mMin[0]; wx <= isectBox.mMax[0]; ++wx)
                  {
                     // lookup bg pixel
                     u8 bgPixel = getColorIndex( bg, wx, wy );
                     if (isCollidableBgPixel( bgPixel ))
                     {
                        return true;
                     }
                  }
               }
            }
         }
      }
   }

   return false;
}

#endif
