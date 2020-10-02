
#include "game.h"


int main(void)
{
   
   GameFramework::instance().init();
   EnableInterupts( INT_VBLANK );
   while (1)
   {
      GameFramework::instance().update();
      gbaVsync();
   }

   return 0;
}

int __gba_multiboot;      // don't use this for large roms

