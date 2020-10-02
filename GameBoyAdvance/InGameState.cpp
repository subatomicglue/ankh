
#include "data.h"
#include "ShotManager.h"
#include "InGameState.h"
#include "commongame.h"

   InGameState::InGameState() : mMap( wallsBitmap, wallsMap, hudBitmap, hudMap )
   {
      reset();
   }
   void InGameState::reset()
   {
      player.m_oPos = player.startPosition();
      gCamera.setPos( 0,0 );
      
      mPlayerVelocity.set( 0, 0 );
      mRumbleCounter = 0;
      mRumbles[0].set( 1, 0 );
      mRumbles[1].set( 0, 1 );
      mRumbles[2].set( -1, 0 );
      mRumbles[3].set( 0, -1 );

      setKarmaScore( 15 );
      setObjectsScore( 1 );
      mLastRoom.set( 99, 99 ); // out of bounds... (basically not 0,0 which is the starting room)

      // treasure starts out hidden until player gets the illumina.
      hideAll( ActorInit::treasure );
      hideAll( ActorInit::none );

      //DissableInterupts( INT_VBLANK );
      //DissableInterupts( INT_HBLANK );

      extraLength = 0;
   }
   void InGameState::init()
   {
      reset();
      
      // init background
      gbaCopyBgPalette( walls_Palette, 256 );
      mMap.init();

      // change vid modes.
      REG_DISPCNT = BGMODE0 | BG0ENABLE | BG1ENABLE | OBJENABLE | OBJCHARMEMMAP1D;

      // init sprites.
      gbaCopySpritePalette( sprite_Palette, 256 );
      m_oActorDrawManager.init( (AnkhActor**)gAllActors, gActorCount );

      // setup interrupts
      int_init();
      // interrupt to call the update func:
      int_enable(INT_VBLANK);
      REG_DISPSTAT |= VID_IRQ_VB;
      IntrTable[II_VBLANK]= InGameState::vblFunc;
      REG_IME= 1;
   }

   // activate player touch
   void InGameState::touchItem( int which )
   {
      mLastTouchId = which;
      player.doThing( BehaviourMethod::TOUCH, NULL );
   }

   // activate player drop
   void InGameState::dropItem( int which )
   {
      mLastTouchId = which;
      player.doThing( BehaviourMethod::DROP, NULL );
   }

   void InGameState::update()
   {
      mMap.mRoom[0] = mLastRoom[0];
      mMap.mRoom[1] = mLastRoom[1];
      mMap.mRooms = mRooms;
      
      static bool m_bDebugMode = false;// for debugging...
      const int karma_bar_x = 20;
      const int karma_bar_height = 20;

      // slow-down feature : run 1/3 the speed...
      static s8 speed = 0;
      if (mSlow && speed <= 1)
      {
         ++speed;
         return;
      }
      speed = 0;

      // draw
      // first thing after vbl to prevent tearing...
      m_oActorDrawManager.uploadOamInfo();
      mMap.update();

      // update controller
      gInGameControl.update();

      // toggle pause
      static bool m_bPaused = false;
      if (gInGameControl.pause.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         m_bPaused = !m_bPaused;
      }

      // handle pause
      if (m_bPaused)
      {
         // make the karma-bar sparkle...
         static u8 karmabarID = 0;
         karmabarID = (karmabarID + 1) % 9; // cycle from [0..2]  3 times slower than framerate (/3 below)
         for (int y = 0; y <= karma_bar_height; ++y)
            mMap.setHudTile( karma_bar_x, y, 89 + karmabarID / 3 ); // 3 sparkle tiles starting at index 89

         // since we just messed up the karma meter,
         // we'll need to redraw it when unpaused...
         mScoreChanged = true;

         // print "paused" over the metareal text...
         mMap.print( karma_bar_x + 2, 18, "UNIVERSE" );
         mMap.print( karma_bar_x + 2, 19, " ON HOLD" );
         //mMap.print( karma_bar_x + 2, 19, " PAUSED " );

         // since we're paused, return early, don't process anything...
         return;
      }

      // debug mode toggle: (TODO: ifdef this out in release)
      if (gInGameControl.pause.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         // for debugging
         m_bDebugMode = !m_bDebugMode;
         if (m_bDebugMode)
         {
            // leave behind suggestive text (to be dissolved by "metareal")
            mMap.print( karma_bar_x + 2, 18, " DEBUG  " );
            mMap.print( karma_bar_x + 2, 19, " MODE   " );
         }
      }


      // METAREAL text effect on the HUD, random changing colors...
      // MMMMMEEEEETTTTTAAAAARRRRREEEEEAAAAALLLLL starts at index 96 in the hud.bmp
      // hud tile 20,19 is where we "print" it
      u32 changeMetaRealPos = rand() % 8;
      u32 changeTo = rand() % 5;
      static u32 metarealTimer = 0;
      static u32 metarealTimout = 30;
      ++metarealTimer;
      if (metarealTimout <= metarealTimer &&
          0 <= changeMetaRealPos &&
          0 < changeTo)
      {
         mMap.setHudTile( 22 + changeMetaRealPos, 19, 96 + 5*(0 + changeMetaRealPos) + changeTo );
         metarealTimer = 0;
         metarealTimout = rand() % 30;

         // HUD clock (TODO: implement this)
         mMap.print( 22 + changeMetaRealPos, 18, " " );
      }

      // redraw score elements
      if (mScoreChanged)
      {
         const int max_karma = 100;
         const int karma_bar_x = 20;
         const int karma_bar_y = 0;
         const int karma_bar_height = 20;
         int karma_tiles = (mKarma % max_karma) / (max_karma / karma_bar_height);
         int extra_tiles = ::clamp( mKarma / max_karma, 0, karma_bar_height ); // how many screens of karma (rollover)
         mMap.setHudTile( karma_bar_x, karma_bar_height, 1 );
         for (int x = 0; x <= karma_tiles; ++x)
            mMap.setHudTile( karma_bar_x, karma_bar_height - x, 2 );
         for (int x = karma_tiles+1; x <= karma_bar_height; ++x)
            mMap.setHudTile( karma_bar_x, karma_bar_height - x, 1 );
         for (int x = 0; x < extra_tiles; ++x)
         {
            spew::GBAPrint( "wow\n" );
            mMap.setHudTile( karma_bar_x, karma_bar_y + x*2 + 0, 87 ); // hardcoded to index 87 in hud.bmp
            mMap.setHudTile( karma_bar_x, karma_bar_y + x*2 + 1, 88 ); // hardcoded to index 88 in hud.bmp
         }
         clearScoreChanged();

         mMap.print( karma_bar_x + 2, 9, "ROOMS\n\nOBJECTS" );
         char buf[10];
         sprintf( buf, "%02d\n\n%04d", getRoomsScore(), getObjectsScore() );
         spew::Assert( strlen(buf) < sizeof( buf ), "overflow" );
         mMap.printnum( karma_bar_x + 2, 10, buf );
      }

      // attempt to touch something (does nothing if touch misses).
      if (gInGameControl.touchLeft.getState() == ::FourStateDigital::EDGE_DOWN) 
         touchItem( 0 );
      else if(gInGameControl.touchUp.getState() == ::FourStateDigital::EDGE_DOWN)
         touchItem( 1 );
      else if(gInGameControl.touchRight.getState() == ::FourStateDigital::EDGE_DOWN)
         touchItem( 2 );
      else if(gInGameControl.touchDown.getState() == ::FourStateDigital::EDGE_DOWN)
         touchItem( 3 );
      
      // drop item from inventory
      if (gInGameControl.drop0.getState() == ::FourStateDigital::EDGE_DOWN) 
         dropItem( 0 );
      else if(gInGameControl.drop1.getState() == ::FourStateDigital::EDGE_DOWN)
         dropItem( 1 );
      else if(gInGameControl.drop2.getState() == ::FourStateDigital::EDGE_DOWN)
         dropItem( 2 );
      else if(gInGameControl.drop3.getState() == ::FourStateDigital::EDGE_DOWN)
         dropItem( 3 );

      static const int shotSpeed = 2;
      if (gInGameControl.shootLeft.getState() == ::FourStateDigital::EDGE_DOWN)
         gShotManager.shoot( player.leftedge(), gba::Vec<s32>( -shotSpeed, 0 ), &player );
      else if(gInGameControl.shootUp.getState() == ::FourStateDigital::EDGE_DOWN)
         gShotManager.shoot( player.upedge(), gba::Vec<s32>( 0, -shotSpeed ), &player );
      else if(gInGameControl.shootRight.getState() == ::FourStateDigital::EDGE_DOWN)
         gShotManager.shoot( player.rightedge(), gba::Vec<s32>( shotSpeed, 0 ), &player );
      else if(gInGameControl.shootDown.getState() == ::FourStateDigital::EDGE_DOWN)
         gShotManager.shoot( player.downedge(), gba::Vec<s32>( 0, shotSpeed ), &player );
      

      // save current pos in case we have to undo
      gba::Vec<s32> old_pos = player.pos();
      
      // apply run controller input
      const int stepsize = 3;
      if(gInGameControl.runUp.getState() == ::FourStateDigital::EDGE_DOWN)
      {
	      mPlayerVelocity[0] = 0;
         mPlayerVelocity[1] = 0 != mPlayerVelocity[1] ? 0 : -stepsize;
      }
      if(gInGameControl.runDown.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         mPlayerVelocity[0] = 0;
         mPlayerVelocity[1] = 0 != mPlayerVelocity[1] ? 0 : stepsize;
      }
      if(gInGameControl.runLeft.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         mPlayerVelocity[0] = 0 != mPlayerVelocity[0] ? 0 : -stepsize;
         mPlayerVelocity[1] = 0;
      }
      if(gInGameControl.runRight.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         mPlayerVelocity[0] = 0 != mPlayerVelocity[0] ? 0 : stepsize;
         mPlayerVelocity[1] = 0;
      }

      // apply velocity
      player.m_oPos += mPlayerVelocity;


      // apply walk controller input
      bool stepped = false;
      if(gInGameControl.stepUp.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         player.m_oPos += gba::Vec<s32>( 0, -stepsize );
         mPlayerVelocity.set( 0, 0 );
         stepped = true;
      }
      if(gInGameControl.stepDown.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         player.m_oPos += gba::Vec<s32>( 0, stepsize );
         mPlayerVelocity.set( 0, 0 );
         stepped = true;
      }
      if(gInGameControl.stepLeft.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         player.m_oPos += gba::Vec<s32>( -stepsize, 0 );
         mPlayerVelocity.set( 0, 0 );
         stepped = true;
      }
      if(gInGameControl.stepRight.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         player.m_oPos += gba::Vec<s32>( stepsize, 0 );
         mPlayerVelocity.set( 0, 0 );
         stepped = true;
      }

      // collide player against world, if collided, step player back, until no collision...  
      // play a little rumble...
      bool collided = (!m_bDebugMode) && (intersectAllActors( player, ACTOR_COLLISION_PHYSICS ) || intersectMap( player ));
      if (collided)
      {
         const gba::Vec<s32> direction = (player.m_oPos - old_pos) / stepsize;
         int tries = 0;
         while (collided && tries < stepsize)
         {
            // step back...
            player.m_oPos = (player.m_oPos - direction);

            // see if we collided again...
            collided = (!m_bDebugMode) && (intersectAllActors( player, ACTOR_COLLISION_PHYSICS ) || intersectMap( player ));
            ++tries;
         }

         if ((stepsize - tries) <= 1 && player.m_oPos == old_pos)
         {
            // apply a rumble visual to the player to indicate they are bumping something.
            player.m_oPos += mRumbles[mRumbleCounter];
            mRumbleCounter = (mRumbleCounter+1) % 4;
            spew::GBAPrint( "collide\n" );


            // or... stop player velocity...
            //mPlayerVelocity.set( 0, 0 );
         }
      }


      // player's done moving, compute stats to move camera...
      s32 roomx = player.cpos()[0] / roomsizex,
          roomy = player.cpos()[1] / roomsizey;
      s32 roomlocalx = player.cpos()[0] % roomsizex,
          roomlocaly = player.cpos()[1] % roomsizey;
      bool roomchanged = announceRoom( roomx, roomy );
      if (roomchanged && !m_bDebugMode)
      {
         player.m_oPos += mPlayerVelocity * 5;
         mPlayerVelocity.set( 0, 0 );
      }

      if (stepped)
      {
         s32 tilex = player.cpos()[0] / tilesizex,
             tiley = player.cpos()[1] / tilesizey;
         s32 roomlocaltilex = tilex % tilesperroomx,
             roomlocaltiley = tiley % tilesperroomy;
         u8 i = getColorIndex( mMap, player.cpos()[0], player.cpos()[1] );
         u8 t = getTileIndex( mMap, player.cpos()[0], player.cpos()[1] );
         spew::GBAPrint( "- WORLD: pos:%03d,%03d tile:%02d,%02d", player.cpos()[0], player.cpos()[1], tilex, tiley );
         spew::GBAPrint( " room:%d,%d   mapcolor:%03d maptile:%03d\n", roomx, roomy, i, t );
         spew::GBAPrint( "- LOCAL: pos:%03d,%03d tile:%02d,%02d\n", roomlocalx, roomlocaly, roomlocaltilex, roomlocaltiley );
      }

      const s32 gameScreenX_div2 = gGameWindow.mWidth / 2;
      const s32 gameScreenY_div2 = gGameWindow.mHeight / 2;
      const gba::Vec<s32> playerPos = player.cpos() + gba::Vec<s32>( 0,4 ); // centered player position for centering the camera

      gba::Vec<s32> offset;
      {
         s32 mStartX = gameScreenX_div2, mEndX = -gameScreenX_div2, 
            mStartY = gameScreenY_div2, mEndY = -gameScreenY_div2;
         gba::Vec<s32> lerpval;
         gba::Vec<s32> lerprange;
         intersect01( lerpval, lerprange,
                     gba::AABox2<s32>( gba::Vec<s32>( 0,0 ),
                                       gba::Vec<s32>( roomsizex,roomsizey ) ),
                     gba::Vec<s32>( roomlocalx, roomlocaly ) );
         offset = lerp( lerpval, lerprange,
                        gba::Vec<s32>( mStartX, mStartY ),
                        gba::Vec<s32>( mEndX, mEndY ) );
      }

      // attach camera to player when player is far enough away from the map edges
      if ((mMap.getMinX() + gameScreenX_div2) <= player.pos(0) && 
          player.pos(0) < (mMap.getMaxX() * 8 - gameScreenX_div2))
      {
         gCamera.setPosX( playerPos[0] - gameScreenX_div2 + offset[0] );
      }

      // attach camera to player when player is far enough away from the map edges
      if ((mMap.getMinY() + gameScreenY_div2) <= player.pos(1) && 
          player.pos(1) < (mMap.getMaxY() * 8 - gameScreenY_div2))
      {
         gCamera.setPosY( playerPos[1] - gameScreenY_div2 + offset[1] );
      }

      mMapRoom.update( playerPos[0], playerPos[1] );

      gCamera.update();
      m_oActorDrawManager.updateBehaviours();
      gCamera.revert();
      gCamera.update();
      m_oActorDrawManager.update();

      // react
      gShotManager.update();
}

/// vsync interrupt
void InGameState::vblFunc()
{
   gInGameState.update();
}

/// hsync interrupt
void InGameState::hblFunc(void)
{
}
