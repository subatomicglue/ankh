
#ifndef INGAMESTATE
#define INGAMESTATE

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
#include "ActorDrawManager.h"
#include "MapRoom.h"

class FourStateDigital
{
public:
   enum State
   {
      UP, DOWN, EDGE_UP, EDGE_DOWN
   };

   FourStateDigital() : mState( UP ), mIsDown( false )
   {
   }

   void update( bool isDown )
   {
      mIsDown = isDown;
      switch (mState)
      {
      case DOWN: if (false == isDown) mState = EDGE_UP; break;
      case EDGE_DOWN: if (true == isDown) mState = DOWN; else mState = EDGE_UP; break;
      case EDGE_UP: if (true == isDown) mState = EDGE_DOWN; else mState = UP; break;
      case UP: if (true == isDown) mState = EDGE_DOWN; break;
      }
   }

   State getState() const { return mState; }
   bool isDown() const { return mIsDown; }
   bool isUp() const { return !mIsDown; }

private:
   State mState;
   bool mIsDown; // cache the state for isDown/isUp
};

class InGameControl
{
public:
   InGameControl() {}
   
   // dpad operation state...  these are separate to allow them to be mixed as the user needs.
   FourStateDigital useRun;   // dpad makes player run
   FourStateDigital useShoot; // dpad shoots in 4 directions
   FourStateDigital useTouch; // dpad touches in 4 directions
   FourStateDigital useWalk;  // dpad causes player to step in 4 directions
   FourStateDigital useDrop;  // dpad drops items from 4 slots

   FourStateDigital stepUp;   // take one step each press of the pad.
   FourStateDigital stepLeft;
   FourStateDigital stepRight;
   FourStateDigital stepDown;

   FourStateDigital runUp;    // runs by default, press back once to stop, again to change directions.
   FourStateDigital runLeft;
   FourStateDigital runRight;
   FourStateDigital runDown;

   FourStateDigital shootUp;   // shoot once for each press.  autorepeat after holding for a while.
   FourStateDigital shootLeft;
   FourStateDigital shootRight;
   FourStateDigital shootDown;

   FourStateDigital touchUp;  // reach out and touch, grab karma and pick up items for your inventory
   FourStateDigital touchLeft;
   FourStateDigital touchRight;
   FourStateDigital touchDown;

   FourStateDigital drop0;  // 4 inventory slots...
   FourStateDigital drop1;
   FourStateDigital drop2;
   FourStateDigital drop3;

   FourStateDigital pause;    // pause/unpause game
   FourStateDigital select;   // select button

   void update()
   {
      // action toggles (multiplexing the controller buttons to get many uses from the same buttons)
      useRun.update( !CTRLINPUT_R_PRESSED && 
                     !CTRLINPUT_L_PRESSED && 
                     !CTRLINPUT_A_PRESSED && 
                     !CTRLINPUT_B_PRESSED );
      useDrop.update( CTRLINPUT_L_PRESSED );
      useTouch.update( CTRLINPUT_R_PRESSED );
      useWalk.update( CTRLINPUT_A_PRESSED );
      useShoot.update( CTRLINPUT_B_PRESSED );

      // compute input triggers for all ingame actions
      runDown.update(  CTRLINPUT_DOWN_PRESSED  && useRun.isDown() );
      runUp.update(    CTRLINPUT_UP_PRESSED    && useRun.isDown() );
      runLeft.update(  CTRLINPUT_LEFT_PRESSED  && useRun.isDown() );
      runRight.update( CTRLINPUT_RIGHT_PRESSED && useRun.isDown() );

      stepDown.update(  CTRLINPUT_DOWN_PRESSED  && useWalk.isDown() );
      stepUp.update(    CTRLINPUT_UP_PRESSED    && useWalk.isDown() );
      stepLeft.update(  CTRLINPUT_LEFT_PRESSED  && useWalk.isDown() );
      stepRight.update( CTRLINPUT_RIGHT_PRESSED && useWalk.isDown() );

      shootDown.update( CTRLINPUT_DOWN_PRESSED  && useShoot.isDown() );
      shootUp.update(   CTRLINPUT_UP_PRESSED    && useShoot.isDown() );
      shootLeft.update( CTRLINPUT_LEFT_PRESSED  && useShoot.isDown() );
      shootRight.update(CTRLINPUT_RIGHT_PRESSED && useShoot.isDown() );

      touchDown.update( CTRLINPUT_DOWN_PRESSED  && useTouch.isDown() );
      touchUp.update(   CTRLINPUT_UP_PRESSED    && useTouch.isDown() );
      touchLeft.update( CTRLINPUT_LEFT_PRESSED  && useTouch.isDown() );
      touchRight.update(CTRLINPUT_RIGHT_PRESSED && useTouch.isDown() );

      drop3.update( CTRLINPUT_DOWN_PRESSED   && useDrop.isDown() );
      drop1.update( CTRLINPUT_UP_PRESSED     && useDrop.isDown() );
      drop0.update( CTRLINPUT_LEFT_PRESSED   && useDrop.isDown() );
      drop2.update( CTRLINPUT_RIGHT_PRESSED  && useDrop.isDown() );

      pause.update( CTRLINPUT_START_PRESSED );
      select.update( CTRLINPUT_SELECT_PRESSED );
   }
};


class InGameState
{
public:
   InGameState();
   void reset();
   void init();

   /// provide the id of the touch... corresponds to the inventory slot (if free) if item is picked up.
   void touchItem( int touchID );
   /// activate player drop
   void dropItem( int which );

   void update();

   /// vsync interrupt
   static void vblFunc();

   /// hsync interrupt
   static void hblFunc(void);

   ActorDrawManager m_oActorDrawManager;
   Background mMap;
   u8 mRumbleCounter;
   gba::Vec<s32> mRumbles[4];
   gba::Vec<s32> mPlayerVelocity;
   int mLastTouchId; //< which touch happened last...

   inline int getKarmaScore() const { return mKarma; }
   inline int getObjectsScore() const { return mObjects; }
   inline int getRoomsScore() const
   {
      const unsigned int numbits = sizeof( mRooms ) * 8;
      unsigned int count = 0;
      for (unsigned int x = 0; x < numbits; ++x)
      {
         count += ((mRooms>>x) & 0x1);
      }
      return count;
   }
   inline void setKarmaScore( int k )
   {
      spew::GBAPrint( "setting karma to %d\n", k );
      mScoreChanged = true;
      mKarma = clamp( k, 0, 99999 );
   }
   inline void setObjectsScore( int k )
   {
      spew::GBAPrint( "setting objects to %d\n", k );
      mScoreChanged = true;
      mObjects = clamp( k, 0, 99999 );
   }
   inline bool announceRoom( s32 x, s32 y )
   {
      gba::Vec<u8> roomxy( x, y );
      if (mLastRoom != roomxy)
      {
         mLastRoom = roomxy;
         mScoreChanged = true;
         u64 mask = (u64(0x1) << u64(x + 8*y));
         mRooms |= mask;
         spew::GBAPrint( "announcing room %d %d rooms:0x%08x%08x", x, y, (mRooms>>32)&0xffffffff, mRooms );
         spew::GBAPrint( " mask:0x%08x%08x\n", (mask>>32)&0xffffffff, mask );
         return true;
      }
      return false;
   }
   inline bool hasScoreChanged() const { return mScoreChanged; }
   inline void clearScoreChanged() { mScoreChanged = false; }
   int extraLength; //< adds to the length of the touch...

   void slow() { mSlow = true; }
   void fast() { mSlow = false; }
private:
   bool mScoreChanged;
   int mKarma;
   u64 mRooms;
   gba::Vec<u8> mLastRoom;
   int mObjects;
   MapRoom mMapRoom;
   bool mSlow;
};




extern Camera gCamera;
extern InGameControl gInGameControl;
extern InGameState gInGameState;
extern WindowDimensions gGameWindow;
extern WindowDimensions gScreenWindow;





#endif
