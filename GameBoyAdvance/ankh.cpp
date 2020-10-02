
#include "common.h"
#include "hardware.h"
#include "data.h"
#include "Background.h"
#include "UIScreenAsset.h"
#include "BitmapAsset.h"
extern "C"
{
   #include "interrupt.h"
}
#include "Sprite.h"
#include "Actor.h"
#include "Vec2.h"
#include "AABox2.h"

#define MULTIBOOT volatile const int __gba_multiboot = 1;
MULTIBOOT

// a step  lshoulder
// s drop  rshoulder
// z shoot  A
// x touch  B
// enter start start

#include "InGameState.h"

/////////////////////////////////////////////////////////
// game state:
Camera gCamera;
InGameControl gInGameControl;
InGameState gInGameState;
WindowDimensions gGameWindow( 0, 0, 20*8, 21*8 );
WindowDimensions gScreenWindow( 0, 0, 240, 160 );
/////////////////////////////////////////////////////////




int main()
{
   spew::SetLevel( 0 );
   spew::SetFilter( spew::APP | spew::ACTOR );
   spew::Trace( spew::APP, "starting frontend mode!!!\n" );

   // cycle through screens until button press.
   const UIScreenAsset* poCurrentUIScreen = &title1_ui;
   u32 nTime = 0;

   // draw crack screen
   poCurrentUIScreen->draw();
   poCurrentUIScreen = poCurrentUIScreen->m_poNext;
   while (!CTRLINPUT_START_PRESSED && !CTRLINPUT_A_PRESSED)
   {
      poCurrentUIScreen->update(); // update the screen.
      while (REG_VCOUNT != 160);    // wait for vblank
   }
   while (CTRLINPUT_START_PRESSED || CTRLINPUT_A_PRESSED);    // wait for button to be let go

   // draw other screens.
   while (!CTRLINPUT_START_PRESSED)
   {
      // draw one screen
      poCurrentUIScreen->draw();
      poCurrentUIScreen = poCurrentUIScreen->m_poNext;

      // wait (can skip into the game with B, or skip to the next screen with A)
      nTime = 0;
      while (nTime < 3000 && !CTRLINPUT_START_PRESSED && !CTRLINPUT_A_PRESSED)
      {
         poCurrentUIScreen->update(); // update the screen.
         while (REG_VCOUNT != 160 && !CTRLINPUT_START_PRESSED);    // wait for vblank
         ++nTime;
      }
      while (CTRLINPUT_A_PRESSED);    // wait for button to be let go
   }
   while (CTRLINPUT_START_PRESSED || CTRLINPUT_A_PRESSED);    // wait for button to be let go
   spew::Trace( spew::APP, "starting game mode!!!\n" );

   gInGameState.init();
   while(1)
   {
      //gInGameState.update();
      //gbaVsync();    // wait for vblank
      //while (REG_VCOUNT != 160 );
   }

   return 0;
}

