
#ifndef SHOTMANAGER
#define SHOTMANAGER

#include "FixedSizeArray.h"
#include "AnkhActor.h"
#include "data.h"
#include "commongame.h"

class ShotManager
{
public:
   enum { NUM_BULLETS = 12 };
   ShotManager() { reset(); }
   void reset()
   {
      for (s32 x = 0; x < NUM_BULLETS; ++x)
      {
         mShot[x].reset();
      }
      setShotActor( 0, &bullet000 );
      setShotActor( 1, &bullet001 );
      setShotActor( 2, &bullet002 );
      setShotActor( 3, &bullet003 );
      setShotActor( 4, &bullet004 );
      setShotActor( 5, &bullet005 );
      setShotActor( 6, &bullet006 );
      setShotActor( 7, &bullet007 );
      setShotActor( 8, &bullet008 );
      setShotActor( 9, &bullet009 );
      setShotActor( 10, &bullet010 );
      setShotActor( 11, &bullet011 );
   }
   void init() { this->reset(); }
   void kill() {}

   void shoot( const gba::Vec<s32>& pos, const gba::Vec<s32>& velocity, const AnkhActor* dontCollideWithThis )
   {
      spew::GBAPrint( "shoot pos:%d,%d  vel:%d,%d\n", pos[0], pos[1], velocity[0], velocity[1] );
      for (s32 x = 0; x < NUM_BULLETS; ++x)
      {
         if (false == mShot[x].active)
         {
            mShot[x].start( pos, velocity, dontCollideWithThis );
            return;
         }
      }
      // otherwise, steal one
      //mShot[0].start( pos, velocity, dontCollideWithThis );
      spew::GBAPrint( "no bullets left, wait for reload\n" );
   }

   inline bool canShoot() const
   {
      for (s32 x = 0; x < NUM_BULLETS; ++x)
      {
         if (false == mShot[x].active)
         {
            return true;
         }
      }
      return false;
   }

   // todo: detect off screen, and remove the bullet... or distance... or something...

   void update()
   {
      for (s32 x = 0; x < NUM_BULLETS; ++x)
      {
         if (true == mShot[x].active)
         {
            AnkhActor* hitactor = NULL;
            bool touchedWall = intersectMap( *mShot[x].bulletActor );
            bool touchedActor = intersectAllActors( *mShot[x].bulletActor, hitactor, mShot[x].dontCollideWithThis, ACTOR_COLLISION_PHYSICS );
            if (touchedActor || touchedWall)
            {
               // stop collided bullet first...
               mShot[x].stop();
            }
            if (touchedActor &&
               // filter out same type shooting same type...
                ((mShot[x].dontCollideWithThis && mShot[x].dontCollideWithThis->type() != hitactor->type()) || 
                  NULL == mShot[x].dontCollideWithThis))
            {
               // ...then do action (which may trigger another shot to take it's place)
               hitactor->doThing( BehaviourMethod::SHOOT, mShot[x].bulletActor );

               // game scoring for objects.
               if (!hitactor->m_bHasBeenCounted)
               {
                  hitactor->m_bHasBeenCounted = true;
                  gInGameState.setObjectsScore( gInGameState.getObjectsScore() + 1 );
                  spew::GBAPrint( "increasing object score to %d\n",
                                 gInGameState.getObjectsScore() );
               }
               if (!hitactor->m_bHasBeenShot)
               {
                  hitactor->m_bHasBeenShot = true;
                  gInGameState.setKarmaScore( gInGameState.getKarmaScore() + hitactor->karmaFromShooting() );
                  spew::GBAPrint( "shot karma %d  == %d\n",
                                    hitactor->karmaFromShooting(),
                                    gInGameState.getKarmaScore() );
               }
            }
         }
         mShot[x].update();
      }
   }

   struct Shot
   {
      Shot() { reset(); }
      void reset()
      {
         bulletActor = NULL;
         active = false;
      }
      bool active;
      gba::Vec<s32> position, velocity;
      AnkhActor* bulletActor;
      const AnkhActor* dontCollideWithThis;
      inline void start( const gba::Vec<s32>& p, const gba::Vec<s32>& v, const AnkhActor* excludeFromCollision )
      {
         spew::Trace( "shoot start\n" );
         active = true;
         velocity = v;
         bulletActor->show();
         bulletActor->setCPos( p );
         dontCollideWithThis = excludeFromCollision;
      }
      inline void stop()
      {
         spew::Trace( "shoot stop\n" );
         bulletActor->hide();
         active = false; // disable me
      }

      inline void update()
      {
         if (true == active)
         {
            bulletActor->setPos( bulletActor->pos() + velocity );

            spew::Trace( "shoot update\n" );
            if (!bulletActor->isOnScreen())
            {
               stop();
            }
         }
      }
   };

   inline void setShotActor( int x, AnkhActor* a )
   {
      spew::Assert( 0 <= x && x < NUM_BULLETS, "num bullets" );
      mShot[x].bulletActor = a;
   }

private:

   // max number of shots in the game at one time...
   FixedSizeArray<Shot, NUM_BULLETS> mShot;
};

extern ShotManager gShotManager;

#endif
