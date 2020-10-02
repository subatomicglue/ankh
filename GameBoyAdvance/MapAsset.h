#ifndef GBA_MAPASSET_H
#define GBA_MAPASSET_H

#include "common.h"
#include <assert.h>
#include <memory>

namespace gba
{
   class MapAsset
   {
   public:
      short getTile( u32 x, u32 y ) const
      {
         return d[x + y * tx];
      }

      const unsigned short tx;
      const unsigned short ty;
      const unsigned short* const d;
   };

   class MapAssetTemp
   {
   public:
      short getTile( u32 x, u32 y ) const
      {
         return d[x + y * tx];
      }

      unsigned short tx;
      unsigned short ty;
      unsigned short* d;
   };

   /*
   // warning, creates memory...
   inline void cloneMapAsset( MapAssetTemp& map, const MapAsset& map2 )
   {
      map.tx = map2.tx;
      map.ty = map2.ty;
      map.d = new unsigned short[map.tx * map.ty];
      memcpy( (void*)map.d, map2.d, map.tx * map.ty * sizeof(short) );
   }
   */
}

#endif
