#ifndef GBA_SPRITE_H
#define GBA_SPRITE_H

#ifndef WIN32
#include "hardware.h"
#endif
#include "spewgba.h"
#include "BitmapAsset.h"
#include "MapAsset.h"
#include "AABox2.h"

namespace gba
{
   /// 2d geometry asset - textured quad
   struct SpriteAsset
   {
      const gba::Vec<s32> m_oBox; //< defined within the bitmap's coordsys, affects collision and visual limits
      const gba::BitmapAsset& m_oBitmap;
   };

#ifndef WIN32
   /// The GBA supports 128 simultaneous sprites. These can be up to 64x64 pixels in size. 
   /// The OAM, which starts at 0x07000000, has one entry for each of the 128 sprites. 
   /// Intermixed with this data are the rotation/scaling attributes, of which there are 32 sets of 4 16 bit values. 

   class Sprite
   {
   public:
      Sprite() { release(); }
      
      /// perform animation, etc...
      inline void update()
      {
         // animation
         m_nCurentAnim = (m_nCurentAnim + 1) % m_nAnimFrames;
         m_oOamEntry.attribute2 &= ~0x03ff; // clear the field.
         m_oOamEntry.attribute2 |= 0x03ff & (m_nStartTile+m_nCurentAnim*m_nNextTile);  // index to my tile data (in 32 byte incr)

         // update the priority field so we can move the sprite between layers...
         m_oOamEntry.attribute2 &= ~(0x0003 << 10);               // clear the field.
         m_oOamEntry.attribute2 |= (0x0003 & m_nPriority) << 10;  // my priority
      }

      /// create from raw data.
      /// priority is 0-3, and orders sprite drawing in 4 layers, with the background
      inline void init( const void* tile, u32 tilesizex, u32 tilesizey, u32 bmpsizex, u32 bmpsizey, u32 nMemLocation, u32 priority, u32 mosaic = 0 )
      {
         m_nStartTile = nMemLocation / 32; //< index to my tile data (in 32 byte incr)
         m_nNextTile = (tilesizex * tilesizey) / 32; // tile indicies until the next tile
         m_nAnimFrames = (bmpsizex * bmpsizey) / (tilesizex * tilesizey); // size of total bmp / size of one tile.
         m_nPriority = priority;

         m_oOamEntry.clear();

         // start offscreen
         this->setPos( SCREENX, SCREENY );
         
         // set the size of the sprite
         m_oOamEntry.setSize( lookupsize( tilesizex, tilesizey ) );
         
         // other stuff...
         m_oOamEntry.attribute0 |= 0 << 10;       // 0 normal, 1 transparent, 2 window
         m_oOamEntry.attribute0 |= mosaic << 12;  // 1 use mosaic effect
         m_oOamEntry.attribute0 |= (1 << 13);       // 1 use 256 color, 0 use 16 color
         m_oOamEntry.attribute1 |= 0 << 12;       // 1 flip horiz
         m_oOamEntry.attribute1 |= 0 << 13;       // 1 flip vert
         m_oOamEntry.attribute2 |= (0x03ff & m_nStartTile) << 0;  // index to my tile data (in 32 byte incr)
         m_oOamEntry.attribute2 |= (0x0003 & m_nPriority) << 10;  // my priority
         m_oOamEntry.attribute2 |= (0x000f & 0) << 12;  // palette index (for 16bit pals)

         this->uploadTileData( tile, bmpsizex, bmpsizey, nMemLocation );
      }

      /// create from bitmap.
      inline void init( const BitmapAsset* const bmp, u32 nMemLocation, u32 priority, u32 mosaic = 0 )
      {
         mBitmap = bmp;
         init( (void*) bmp->tile( 0 ), bmp->tpx, bmp->tpy, bmp->px(), bmp->py(), nMemLocation, priority, mosaic );
      }

      inline void setPriority( u32 p )
      {
         m_nPriority = p;
      }

      inline void reset()
      {
         m_nAnimFrames = 0;
         m_nCurentAnim = 0;
         m_nStartTile = 0;
         m_nNextTile = 0;
         //m_oOamEntry.clear();
         m_oOamEntry.reset();
      }

      inline void release() { reset(); }

      void setPos( u16 x, u16 y )
      {
         m_oOamEntry.setX( x );
         m_oOamEntry.setY( y );
      }

      /// set the Object Attribute Memory (OAM) entry in the gba's sprite memory
      inline void uploadOamInfo( u16 nID ) const
      {
         *(OamEntry*)&((u16*)GBA_SPRITE_OAM)[0 + nID * 4] = m_oOamEntry;
         /*
	      ((u16*)GBA_SPRITE_OAM)[0 + nID * 4] = m_oOamEntry.attribute0;
         ((u16*)GBA_SPRITE_OAM)[1 + nID * 4] = m_oOamEntry.attribute1;
         ((u16*)GBA_SPRITE_OAM)[2 + nID * 4] = m_oOamEntry.attribute2;
         ((u16*)GBA_SPRITE_OAM)[3 + nID * 4] = m_oOamEntry.attribute3;
         */
      }

      const BitmapAsset* mBitmap; // changable pointer, points to const data.

   private:
      /// upload tile bitmap to sprite memory
      inline void uploadTileData( const void* tiledata, 
                                  const u32 sizex, const u32 sizey,
                                  const u32 nMemoryLocation    //< mem location is 0 based, and is the offset in bytes.
                                  )
      {
         // fast way...
         const u32 wordcount = (sizex * sizey) / 2;
         //DMA_CopyChannel3( tiledata, GBA_SPRITE_DATA, wordcount, DMA_16NOW);
         DMA_TRANSFER( &((u8*)GBA_SPRITE_DATA)[nMemoryLocation], tiledata, wordcount, 3, DMA_16NOW );
         
         // slow way
         /*
         const u32 size = (sizex * sizey) / 2;
         const u32 nMemoryLocation16bit = nMemoryLocation / 2;
         for (u32 loop = 0; loop < size; ++loop)
         {
            // copy 2 bytes at a time for better speed.
       	   ((u16*)GBA_SPRITE_DATA)[nMemoryLocation16bit + loop] = ((u16*)tiledata)[loop];
         }
         */
      }

      /// lookup the the size code, use 8, 16, 32, or 64 for sizes, 
      /// some combinations are not supported on gba.
      inline const u16 lookupsize( const u16 x, const u16 y ) const
      {
         if (x == 8 && y == 8) return 0x0000;
         if (x == 16 && y == 16) return 0x0001;
         if (x == 32 && y == 32) return 0x0002;
         if (x == 64 && y == 64) return 0x0003;
         if (x == 16 && y == 8) return 4;
         if (x == 32 && y == 8) return 5;
         if (x == 32 && y == 16) return 6;
         if (x == 64 && y == 32) return 7;
         if (x == 8 && y == 16) return 8;
         if (x == 8 && y == 32) return 9;
         if (x == 16 && y == 32) return 10;
         if (x == 32 && y == 64) return 11;
         spew::Assert( false, "unknown size combination" );
         return 15;
      }

      OamEntry m_oOamEntry;
      u16 m_nAnimFrames;
      u16 m_nCurentAnim;
      u16 m_nStartTile;
      u16 m_nNextTile;
      u8 m_nPriority;
   };
#endif
}



#endif
