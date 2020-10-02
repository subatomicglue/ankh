#ifndef UI_SCREEN_ASSET
#define UI_SCREEN_ASSET

#include "BitmapAsset.h"
#include "DrawScreen.h"

struct UIScreenAsset
{
   const gba::BitmapAsset& m_oBitmap;
   const UIScreenAsset* m_poNext;
   inline void draw() const
   {
      drawScreen( m_oBitmap.palette, m_oBitmap.d );
   }
   inline void update() const {}
};

#endif
