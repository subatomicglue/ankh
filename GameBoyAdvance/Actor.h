#ifndef ACTOR_H
#define ACTOR_H

#ifndef WIN32
#include "hardware.h"
#else
#define SCREENX 240
#define SCREENY 160
#endif
#include "BitmapAsset.h"
#include "Sprite.h"
#include "AABox2.h"
#include "SortedList.h"
#include "Pool.h"

enum ActorFlags
{
   ACTOR_VISIBLE = 1,  // draw (does not disable other attributes)
   ACTOR_COLLISION = 2,  // if not set, no collision at all
   ACTOR_COLLISION_TRIGGER = 2|4, // fire collision event (does not invoke push back)
   ACTOR_COLLISION_PHYSICS = 2|8, // respond to object collision (push back)
   ACTOR_PICKUP = 16, // can this object be picked up
};

/** constant structure for actor's initial position information */
class ActorInit
{
	public:
      // TODO: remove this enum!!!  not needed?
		enum Type
		{
			candy,
			horzdoor,
			musicbox,
			doorlock,
			treasure,
			badguy,
			vertdoor,
			flit,
			crown,
			trap,
			player,
         benign,
         tele,
         illumina,
         redirect,
         suck,
         shooter,
         tonegen,
         key,
         chalice,
         osc, // for the candy that travels in 0,0
         none,
         panpad,
         interppad,
         playerdrone,
         trigger,
         touchpad, // pad that increases touch length
         triggerpad,  // triggers some other actor when you collide with it.
         statetrigger // send UP,LEFT,DOWN,RIGHT to trigger this one...
		};

      const char* const m_szName;
		const Type m_eType;
		const gba::SpriteAsset* const m_poSprite;
		const s32 m_oPos[2];
      const s32 m_oRoom[2];
      const s16 m_nKarmaFromTouching;
      const s16 m_nKarmaFromShooting;

      const s32 mFlags; // | together ActorFlags
};

class Actor
{
public:
   Actor( const ActorInit& rfoInit ) : m_rfoActorInit( rfoInit )
   {
      reset();
      //spew::Trace( "0x%x Actor() : m_rfoActorInit  %d %d collidable %d\n", this, m_rfoActorInit.m_oPos[0], m_rfoActorInit.m_oPos[1], m_bCollision );
   }
   ~Actor()
   {
      reset();
   }

   void init()
   {
      reset();
   }
   void reset()
   {
      m_oPos = startPosition();
      m_bIsDrawing = true;
      m_bIsOnScreen = false;
      m_bIsInWorldSpace = true;
      m_bIsEnabled = true;
      mDepth = 1;
      m_bHasBeenTouched = false;
      m_bHasBeenShot = false;
      m_bHasBeenCounted = false;
   }
   void release()
   {
      reset();
   }

   inline const char* const name() const { return m_rfoActorInit.m_szName; }
   inline const ActorInit::Type& type() const { return m_rfoActorInit.m_eType; }
   inline const gba::BitmapAsset* const bitmap() const { return &m_rfoActorInit.m_poSprite->m_oBitmap; }
   inline const gba::SpriteAsset* const sprite() const { return m_rfoActorInit.m_poSprite; }
   inline const gba::Vec<s32>& pos() const { return m_oPos; }

   inline const gba::Vec<s32> leftedge() const { return cpos() + gba::Vec<s32>( -bbox( 0 ) / 2, 0 ); }
   inline const gba::Vec<s32> upedge() const { return cpos() + gba::Vec<s32>( 1, -bbox( 1 ) / 2 ); }
   inline const gba::Vec<s32> rightedge() const { return cpos() + gba::Vec<s32>( bbox( 0 ) / 2 , 0 ); }
   inline const gba::Vec<s32> downedge() const { return cpos() + gba::Vec<s32>( 1, bbox( 1 ) / 2 ); }

   // 0-3 layers, affects draw order on the screen, and with backgrounds
   inline const u8 depth() const { return mDepth; }
   inline void setDepth( u8 d ) { mDepth = d; }

   inline void setPos( const gba::Vec<s32>& p ) { m_oPos = p; }
   inline const s32& pos( u32 x ) const { return m_oPos[x]; }
   inline const gba::Vec<s32>& bbox() const { return m_rfoActorInit.m_poSprite->m_oBox; }
   inline const s32& bbox( u32 x ) const { return m_rfoActorInit.m_poSprite->m_oBox[x]; }
   inline gba::Vec<s32> startPosition() const { return gba::Vec<s32>( m_rfoActorInit.m_oPos[0], m_rfoActorInit.m_oPos[1] ); }
   inline gba::Vec<s32> startRoom() const { return gba::Vec<s32>( m_rfoActorInit.m_oRoom[0], m_rfoActorInit.m_oRoom[1] ); }

   // get the minimum/maximum extents in world space.
   inline s32 getWorldMin( u32 x ) const { return m_oPos[x]; }
   inline s32 getWorldMax( u32 x ) const { return m_oPos[x] + bbox(x); }

   /// compute centered position
   inline const gba::Vec<s32> cpos() const { return m_oPos + m_rfoActorInit.m_poSprite->m_oBox / 2; }
   inline const void setCPos( const gba::Vec<s32>& pos ) { m_oPos = pos - m_rfoActorInit.m_poSprite->m_oBox / 2; }

   /// get screen position of the actor
   inline gba::Vec<s32> getScreenPos() const 
   { 
      if (m_bIsInWorldSpace)
         return gba::Vec<s32>( m_oPos[0] - gCamera.posx(), m_oPos[1] - gCamera.posy() );
      else
         return m_oPos;   
   }

   /// get room coords of actor
   inline gba::Vec<s32> getRoomPos() const 
   { 
      const gba::Vec<s32> pos = cpos();
      return gba::Vec<s32>( pos[0] / roomsizex, pos[1] / roomsizey );
   }

   /// is this actor on the screen?
   inline bool isOnScreen( const s16 screen_width, const s16 screen_height ) const
   {
      gba::Vec<s32> local_pos = getScreenPos();

      s32 right  = local_pos[0] + bbox()[0];
      s32 bottom = local_pos[1] + bbox()[1];
      s32 left   = local_pos[0];
      s32 top    = local_pos[1];

      //spew::Trace( spew::ACTOR, 2, "0x%x pos %d %d cam %d %d initpos %d %d\n", this, m_oPos[0], m_oPos[1], gCamera.mPos[0], gCamera.mPos[1], m_rfoActorInit.m_oPos[0], m_rfoActorInit.m_oPos[1] );

      return !(right < 0 || screen_width < left ||
               bottom < 0 || screen_height < top);
   }

   inline bool isOnScreen() const
   {
      if (m_bIsInWorldSpace)
         return isOnScreen( gGameWindow.mWidth, gGameWindow.mHeight );
      else
         return isOnScreen( gScreenWindow.mWidth, gScreenWindow.mHeight );
   }

   /// get screen extents of the actor.
   inline gba::AABox2<s32> getScreenBox() const
   {
      gba::Vec<s32> screen_loc = getScreenPos();
      return gba::AABox2<s32>( screen_loc, screen_loc + bbox() );
   }

   /// get world extents of the actor.
   inline gba::AABox2<s32> getWorldBox() const
   {
      return gba::AABox2<s32>( m_oPos, m_oPos + bbox() );
   }


   inline void show() { m_bIsDrawing = true; }
   inline void hide() { m_bIsDrawing = false; }
   inline void disable() { m_bIsEnabled = false; }
   inline void enable() { m_bIsEnabled = true; }
   inline void setWorldspace() { m_bIsInWorldSpace = true; }
   inline void setScreenspace() { m_bIsInWorldSpace = false; }
   //inline void setCollidable( bool c ) { m_bCollision = c; }

   inline int karmaFromTouching() { return m_rfoActorInit.m_nKarmaFromTouching; }
   inline int karmaFromShooting() { return m_rfoActorInit.m_nKarmaFromShooting; }

   const gba::Vec<s32> getStartPos() const { return gba::Vec<s32>( m_rfoActorInit.m_oPos[0], m_rfoActorInit.m_oPos[1] ); }

   // don't reorder, we initialize using {}'s in data.cpp
   const ActorInit& m_rfoActorInit;
   gba::Vec<s32>    m_oPos;
   u8               mDepth;

   // set up some bitfields
   struct 
   {
      bool m_bIsDrawing : 1;
      bool m_bIsInWorldSpace : 1; // 0 for world space, 1 for screenspace
      bool m_bIsOnScreen : 1; // used by actor draw manager
      bool m_bIsEnabled : 1;
      bool m_bHasBeenTouched : 1;
      bool m_bHasBeenCounted : 1;
      bool m_bHasBeenShot : 1;
   };

   /// check ActorFlags
   inline s32 flags() const { return m_rfoActorInit.mFlags; }
   inline s32 hasFlags( s32 t ) const { return t == (t & m_rfoActorInit.mFlags); }
   inline s32 hasAnyFlags( s32 t ) const { return 0 != (t & m_rfoActorInit.mFlags); }
};

/// aabb intersection
inline bool intersect( const gba::AABox2<s32>& act1, const Actor& act2 )
{  
   // do pixel perfect collision
   // lookup worldspace actor pixel (transform to actorspace to do the lookup)...
   // traverse the box pixels in worldspace... (wx/y)
   gba::AABox2<s32> isectBox;
   if (gba::intersection( isectBox, act1, act2.getWorldBox() ))
   {
      for (int wy = isectBox.mMin[1]; wy <= isectBox.mMax[1]; ++wy)
      {
         for (int wx = isectBox.mMin[0]; wx <= isectBox.mMax[0]; ++wx)
         {
            s32 a2x = wx - act2.pos(0);
            s32 a2y = wy - act2.pos(1);
            u8 actor2Pixel = act2.bitmap()->pixelST( a2x, a2y );
            if (isCollidableActorPixel( actor2Pixel ))
            {
               return true;
            }
         }
      }
   }

   return false;
}

/// aabb intersection
inline bool intersect( const Actor& act1, const Actor& act2 )
{
   // Look for a separating axis on each box for each axis
   /*if (act1.getWorldMin( 0 ) > act2.getWorldMax( 0 ) || 
       act1.getWorldMin( 1 ) > act2.getWorldMax( 1 ) ||   
       act2.getWorldMin( 0 ) > act1.getWorldMax( 0 ) ||
       act2.getWorldMin( 1 ) > act1.getWorldMax( 1 ))
       return false; // no intersection
   // No separating axis ... they must intersect
   */
   
   // do pixel perfect collision
   // lookup worldspace actor pixel (transform to actorspace to do the lookup)...
   // traverse the box pixels in worldspace... (wx/y)
   gba::AABox2<s32> isectBox;
   if (gba::intersection( isectBox, act1.getWorldBox(), act2.getWorldBox() ))
   {
      for (int wy = isectBox.mMin[1]; wy <= isectBox.mMax[1]; ++wy)
      {
         for (int wx = isectBox.mMin[0]; wx <= isectBox.mMax[0]; ++wx)
         {
            s32 a1x = wx - act1.pos(0);
            s32 a1y = wy - act1.pos(1);
            s32 a2x = wx - act2.pos(0);
            s32 a2y = wy - act2.pos(1);
            u8 actor1Pixel = act1.bitmap()->pixelST( a1x, a1y );
            u8 actor2Pixel = act2.bitmap()->pixelST( a2x, a2y );
            if (isCollidableActorPixel( actor1Pixel ) && isCollidableActorPixel( actor2Pixel ))
            {
               return true;
            }
         }
      }
   }

   return false;
}


inline void printActor( const Actor& act )
{
   for (int y = 0; y < act.bbox(1); ++y)
   {
      for (int x = 0; x < act.bbox(0); ++x)
      {
         u8 actorPixel = act.bitmap()->pixelST( 0, x, y );
         if (actorPixel)
         spew::GBAPrint( "%02d,", actorPixel );
         else
         spew::GBAPrint( "  ," );
      }
      spew::GBAPrint( "\n" );
      for (int x = 0; x < act.bbox(0); ++x)
      {
         u8 actorPixel = act.bitmap()->pixelST( 0, x, y );
         if (actorPixel)
         spew::GBAPrint( "%02d,", actorPixel );
         else
         spew::GBAPrint( "  ," );
      }
      spew::GBAPrint( "\n" );
   }
   spew::GBAPrint( "===================\n" );
   spew::GBAPrint( "actor:  tx/y :%d,%d\n", act.bitmap()->tx, act.bitmap()->ty );
   spew::GBAPrint( "     :  tpx/y:%d,%d\n", act.bitmap()->tpx, act.bitmap()->tpy );
   spew::GBAPrint( "     :  bbox :%d,%d\n", act.bbox(0), act.bbox(1) );
   spew::Assert( false, "" );
}


#endif
