
// sprite palette and graphics include
#include "assets.h"
#include "gba/hardware.h"
#include "gba/Sprite.h"
#include "gba/Background.h"
#include "AABox2.h" // aligned box.



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
      this->useRun.update( !CTRLINPUT_R_PRESSED && 
                           !CTRLINPUT_L_PRESSED && 
                           !CTRLINPUT_A_PRESSED && 
                           !CTRLINPUT_B_PRESSED );
      this->useDrop.update( CTRLINPUT_R_PRESSED );
      this->useWalk.update( CTRLINPUT_L_PRESSED );
      this->useShoot.update( CTRLINPUT_A_PRESSED );
      this->useTouch.update( CTRLINPUT_B_PRESSED );

      // compute input triggers for all ingame actions
      this->runDown.update(  CTRLINPUT_DOWN_PRESSED  && this->useRun.isDown() );
      this->runUp.update(    CTRLINPUT_UP_PRESSED    && this->useRun.isDown() );
      this->runLeft.update(  CTRLINPUT_LEFT_PRESSED  && this->useRun.isDown() );
      this->runRight.update( CTRLINPUT_RIGHT_PRESSED && this->useRun.isDown() );

      this->stepDown.update(  CTRLINPUT_DOWN_PRESSED  && this->useWalk.isDown() );
      this->stepUp.update(    CTRLINPUT_UP_PRESSED    && this->useWalk.isDown() );
      this->stepLeft.update(  CTRLINPUT_LEFT_PRESSED  && this->useWalk.isDown() );
      this->stepRight.update( CTRLINPUT_RIGHT_PRESSED && this->useWalk.isDown() );

      this->shootDown.update( CTRLINPUT_DOWN_PRESSED  && this->useShoot.isDown() );
      this->shootUp.update(   CTRLINPUT_UP_PRESSED    && this->useShoot.isDown() );
      this->shootLeft.update( CTRLINPUT_LEFT_PRESSED  && this->useShoot.isDown() );
      this->shootRight.update(CTRLINPUT_RIGHT_PRESSED && this->useShoot.isDown() );

      this->touchDown.update( CTRLINPUT_DOWN_PRESSED  && this->useTouch.isDown() );
      this->touchUp.update(   CTRLINPUT_UP_PRESSED    && this->useTouch.isDown() );
      this->touchLeft.update( CTRLINPUT_LEFT_PRESSED  && this->useTouch.isDown() );
      this->touchRight.update(CTRLINPUT_RIGHT_PRESSED && this->useTouch.isDown() );

      this->drop3.update( CTRLINPUT_DOWN_PRESSED   && this->useDrop.isDown() );
      this->drop1.update( CTRLINPUT_UP_PRESSED     && this->useDrop.isDown() );
      this->drop0.update( CTRLINPUT_LEFT_PRESSED   && this->useDrop.isDown() );
      this->drop2.update( CTRLINPUT_RIGHT_PRESSED  && this->useDrop.isDown() );

      this->pause.update( CTRLINPUT_START_PRESSED );
      this->select.update( CTRLINPUT_SELECT_PRESSED );
   }
};
InGameControl gInGameControl;

Camera gCamera;

// actors can trigger these actions in the ActionDispatcher
enum ActionTrigger
{
   trigSpecial, // a custom message was sent to the actor:   param, target  custom based on message type
   trigShoot,// The Actor was shot:     
   trigDrop, // The Actor was dropped:  target == worldspace drop point
   trigTouch,// Actor was touched:      param == ActorName, target == worldspace position intersected
   trigFrame // called each frame:      
};

// defines a set of actions for an actor.  we call this the actor's behaviour.
class Actor;
class Behavior
{
public:
   // run the action (see ActionTrigger for available actions)
   virtual void exec( Actor* actor, ActionTrigger id, s32 param = 0, const ankh::Vec<s32>& target = ankh::Vec<s32>( 0,0 ) ) = 0;
};

enum ActorName
{
   actBadguy11,   // roomX, roomY, guy# (optional)
   actBadguy12, 
   actBadguy131, 
   actBadguy132, 
   actBadguy241, 
   actBadguy242, 
   actBenign, 
   actSuck, 
   actOther, 
   actIllumina, 
   actTreasure, 
   actKey, 
   actLock, 
   actLockface, 
   actTele, 
   actBonus, 
   actBox, 
   actChalice, 
   actCrown, 
   actFlit, 
   actHorzdoor, 
   actItem11, 
   actItem23, 
   actMusicbox,
   actTrap, 
   actVertdoor,
   actTotalNumOfTypes // keep this last...
};

class Visual
{
public:
   Visual()
   {
      this->reset();
   }

   // sprite must be create-ed first.
   Visual( gba::Sprite* a )
   {
      this->init( a, 0, 0 );
   }

   void reset()
   {
   }

   // setup
   // a   == sprite that this actor uses for drawing
   // x/y == actor position (world space)
   // w/h == bounding geometry used for collisions (local space)
   // if no w/h given, take it from the bitmap
   // render area taken from sprite (no need to give)
   void init( gba::Sprite* a, s32 x = 0, s32 y = 0, s32 w = 0, s32 h = 0 )
   {
      this->reset();
      mSprite = a;
      mPos[0] = x;
      mPos[1] = y;

      // if no width given, take it from the bitmap
      if (w == 0)
      {
         mLocalBox.min()[0] = -(s32)(mSprite->mBitmap->tpx / 2);
         mLocalBox.max()[0] = (s32)mSprite->mBitmap->tpx / 2;
      }
      else
      {
         mLocalBox.min()[0] = (-w) / 2;
         mLocalBox.max()[0] = w / 2;
      }

      // if no height given, take it from the bitmap
      if (h == 0)
      {
         mLocalBox.min()[1] = -(s32)(mSprite->mBitmap->tpy / 2);
         mLocalBox.max()[1] = (s32)mSprite->mBitmap->tpy / 2;
      }
      else
      {
         mLocalBox.min()[1] = (-h) / 2;
         mLocalBox.max()[1] = h / 2;
      }
   }

   virtual void kill()
   {
   }

   // get the width/height
   inline s32 getSize( u16 x ) const 
   { 
      return mLocalBox.getMax()[x] - mLocalBox.getMin()[x];
   }

   // get the width/height
   inline s32 getSpriteSize( u16 x ) const 
   { 
      if (x == 0) 
         return (s32)mSprite->mBitmap->tpx; 
      else
         return (s32)mSprite->mBitmap->tpy;
   }
   
   // get the minimum/maximum extents in world space.
   inline s32 getMin( u16 x ) const { return mPos[x] + mLocalBox.getMin()[x]; }
   inline s32 getMax( u16 x ) const { return mPos[x] + mLocalBox.getMax()[x]; }

   // is this actor on the screen?
   inline bool isOnScreen() const
   {
      const s32 localx = mPos[0] - gCamera.mPos[0], 
                localy = mPos[1] - gCamera.mPos[1];

      s32 right  = (localx + this->getSpriteSize( 0 ) / 2);
      s32 bottom = (localy + this->getSpriteSize( 1 ) / 2);
      s32 left   = (localx - this->getSpriteSize( 0 ) / 2);
      s32 top    = (localy - this->getSpriteSize( 1 ) / 2);
      //return 0 <= right && left <= (s32)SCREENX &&
      //       0 <= bottom && top <= (s32)SCREENY;

      return !(right < 0 || (s32)SCREENX < left ||
               bottom < 0 || (s32)SCREENY < top);
   }

   inline void setPos( ankh::Vec<s32> p )
   {
      mPos = p;
      mPosChanged = true;
   }
   inline void setPos( s32 x, s32 y )
   {
      mPos[0] = x; 
      mPos[1] = y;
      mPosChanged = true;
   }

   ankh::AABox2<s32> worldBox() const
   {
      return ankh::xform( mLocalBox, mPos );
   }

   virtual void update()
   {
      // get local screen position
      const s32 localx = mPos[0] - gCamera.mPos[0], 
                  localy = mPos[1] - gCamera.mPos[1];

      // only set the sprite position while on-screen.
      // this allows sprites to be shared between Actors.
      // while Actor is off-screen, another Actor on-screen may be using it.
      // the art needs to be set up so no two actors sharing a sprite are on screen at once.
      if (this->isOnScreen() || mPosChanged)
      {
         mSprite->setPos( localx - this->getSpriteSize( 0 ) / 2, localy - this->getSpriteSize( 1 ) / 2);
         mPosChanged = false;
      }

      mSprite->update();
   }

   // world coord sys.
   ankh::Vec<s32> mPos;
   bool mPosChanged;

   // in local object coord sys
   ankh::AABox2<s32> mLocalBox;

   // my sprite.
   gba::Sprite* mSprite;
};

class Actor : public Visual
{
public:
   Actor()
   {
      this->reset();
   }

   // sprite must be create-ed first.
   Actor( gba::Sprite* a ) : Visual( a )
   {
   }

   virtual ~Actor() {}

   // reset
   virtual void reset()
   {
      Visual::reset();
      mInventorySlot = -1;
   }

   // setup
   // a   == sprite that this actor uses for drawing
   // x/y == actor position (world space)
   // w/h == bounding geometry used for collisions (local space)
   // if no w/h given, take it from the bitmap
   // render area taken from sprite (no need to give)
   void init( gba::Sprite* a, Behavior* b, s32 x = 0, s32 y = 0, s32 w = 0, s32 h = 0 )
   {
      this->reset();
      Visual::init( a, x, y, w, h );
      mBehavior = b;
   }

   virtual void kill()
   {
   }

   virtual void update()
   {
      if (-1 == mInventorySlot)
      {
         Visual::update();
      }
      else 
      {
         switch (mInventorySlot)
         {
            case 0: mSprite->setPos( 180 - this->getSpriteSize( 0 ) / 2, 30 - this->getSpriteSize( 1 ) / 2 ); break;
            case 1: mSprite->setPos( 200 - this->getSpriteSize( 0 ) / 2, 15 - this->getSpriteSize( 1 ) / 2 ); break;
            case 2: mSprite->setPos( 230 - this->getSpriteSize( 0 ) / 2, 30 - this->getSpriteSize( 1 ) / 2 ); break;
            case 3: mSprite->setPos( 200 - this->getSpriteSize( 0 ) / 2, 60 - this->getSpriteSize( 1 ) / 2 ); break;
         }
         mSprite->update();
      }

      // service my behaviour if i'm on screen.
      if (this->isOnScreen())
      {
         this->exec( trigFrame );
      }
   }

   void exec( ActionTrigger id, s32 param = 0, const ankh::Vec<s32>& target = ankh::Vec<s32>( 0,0 ) )
   {
      mBehavior->exec( this, id, param, target );
   }

   Behavior* mBehavior;
   s32 mInventorySlot; // -1 == not in inventory, 0 - 3 == in inventory
};
Actor* gActorDatabase = NULL;

inline bool intersect( const Actor& box1, const Actor& box2 )
{
   // Look for a separating axis on each box for each axis
   if (box1.getMin( 0 ) >= box2.getMax( 0 ))  return false;
   if (box1.getMin( 1 ) >= box2.getMax( 1 ))  return false;
  
   if (box2.getMin( 0 ) >= box1.getMax( 0 ))  return false;
   if (box2.getMin( 1 ) >= box1.getMax( 1 ))  return false;
  
   // No separating axis ... they must intersect
   return true;
}


Actor* gPlayer = NULL;

class PlayerInventory
{
   struct Item
   {
      s32 id;
      ankh::Vec<s32> pickupDistance;
      bool slotfull;
      void reset()
      {
         slotfull = false;
      }
   };
public:
   PlayerInventory()
   {
      for (s32 x = 0; x < 4; ++x)
         mItem[x].reset();
   }
   
   // try to pick up an item at the target position (world coords)
   // return -1 if none intersected.  return actor name if intersected.
   void pickUpItem( s32 actorName, const ankh::Vec<s32>& target )
   {
      // add actor[name] to the inventory
      this->add( actorName, target - gPlayer->mPos ); 
      
      // take this object somewhere off the map (this value is arbitrary)
      gActorDatabase[actorName].mPos.set( 0, 0 );
   }

   // takes item out of inventory, and drops it on the right place on the map
   void dropItem( s32 slot )
   {
      if (true == this->item( slot ).slotfull)
      {
         // move the item near the player (it was offscreen)
         ankh::Vec<s32> dropPoint = gPlayer->mPos + this->item( slot ).pickupDistance;
         gActorDatabase[ this->item( slot ).id ].mPos = dropPoint;
         
         // let the actor process the drop trigger
         gActorDatabase[ this->item( slot ).id ].exec( trigDrop, 0, dropPoint );

         // remove from inventory
         this->drop( slot );
      }
   }
private:

   bool isFull() const
   {
      for (s32 x = 0; x < 4; ++x)
         if (mItem[x].slotfull == false) 
            return false;
      return true;
   }

   // ret -1 if full
   s32 getNextSlot() const
   {
      for (s32 x = 0; x < 4; ++x)
         if (mItem[x].slotfull == false) 
            return x;
      return -1;
   }

   void add( s32 actorName, const ankh::Vec<s32>& pickupDistance )
   {
      s32 slot = this->getNextSlot();
      if (-1 != slot)  // ensure we don't screw the player by overwritting an inventory slot.
      {
         mItem[slot].id = actorName;
         mItem[slot].pickupDistance = pickupDistance;
         gActorDatabase[actorName].mInventorySlot = slot;
         mItem[slot].slotfull = true;
      }
   }

   const Item& item( s32 slot ) const 
   {
      return mItem[slot];
   }

   void drop( s32 slot )
   {
      if (true == mItem[slot].slotfull)
      {
         gActorDatabase[ mItem[slot].id ].mInventorySlot = -1;
         mItem[slot].slotfull = false;
      }
   }

private:
   Item mItem[4];
};

PlayerInventory gPlayerInventory;


// give world coordinate position, returns true if on screen.
inline bool isOnScreen( const ankh::Vec<s32>& pos )
{
   const s32 localx = pos[0] - gCamera.mPos[0], 
             localy = pos[1] - gCamera.mPos[1];
   return 0 <= localx && localx <= SCREENX && 0 <= localy && localy <= SCREENY; 
}

// test intersection with all actors
struct IntersectResult
{
   ActorName id;
};
bool intersectActors( const ankh::Vec<s32>& target, IntersectResult& result )
{
   for (s32 x = 0; x < actTotalNumOfTypes; ++x)
   {
      if (gActorDatabase[x].isOnScreen() &&
         ankh::intersect( gActorDatabase[x].worldBox(), target ))
      {
         result.id = (ActorName)x;
         return true;
      }
   }
   return false;
}

class ShotManager
{
public:
   ShotManager() { this->reset(); }
   void reset() 
   {
      for (s32 x = 0; x < 10; ++x)
      {
         mShot[x].reset();
      }
   }
   void init() { this->reset(); }
   void kill() {}

   void shoot( const ankh::Vec<s32>& pos, const ankh::Vec<s32>& velocity )
   {
      for (s32 x = 0; x < 10; ++x)
      {
         if (false == mShot[x].active)
         {
            mShot[x].start( pos, velocity );
            return;
         }
      }
   }

   void update()
   {
      for (s32 x = 0; x < 10; ++x)
      {
         if (true == mShot[x].active)
         {
            IntersectResult result;
            if (::intersectActors( mShot[x].position, result ) && result.id != actItem23)
            {
               mShot[x].stop();
               gActorDatabase[result.id].exec( trigShoot, 0, mShot[x].position );
            }
         }
         mShot[x].update();
      }
   }

   struct Shot
   {
      void reset()
      {
         active = false;
      }
      bool active;
      ankh::Vec<s32> position, velocity;
      inline void start( const ankh::Vec<s32>& pos, const ankh::Vec<s32>& velocity )
      {
         this->active = true;
         this->position = pos;
         this->velocity = velocity;
      }
      inline void stop()
      {
         gActorDatabase[actItem23].setPos( ankh::Vec<s32>( -23, -23 ) );
         active = false; // disable me
      }

      inline void update()
      {
         if (true == active)
         {
            position += velocity;

            // todo: make this a dot....
            gActorDatabase[actItem23].setPos( position );

            if (!::isOnScreen( position ) )
            {
               this->stop();
            }
         }
      }
   };

private:
   // max number of shots in the game at one time...
   Shot mShot[10];
};

ShotManager gShotManager;


// behaviour types enumed:
//           normal grabbable object (enters inventory)
//           static object (gives karma)
//           shot redirector (absorbs shot, emits another in some direction)
//           trigger object (triggers another object when touched, shot, or dropped)

class ObjectBehavior : public Behavior
{
public:
   void exec( Actor* actor, ActionTrigger id, s32 param = 0, const ankh::Vec<s32>& target = ankh::Vec<s32>( 0,0 ) )
   {
      switch (id)
      {
      case trigShoot:
         // remove karma
         break;
      case trigTouch:
         // add karma
         gPlayerInventory.pickUpItem( param, target );
         break;
      }
   }
private:
   
   
};

class TriggerBehavior : public Behavior
{
public:
   TriggerBehavior() { this->reset(); }

   void reset()
   {
      mDestination = (ActorName)0;
   }

   TriggerBehavior& init( ActorName whoToTrigger )
   {
      mDestination = whoToTrigger;
      return *this;
   }

   void exec( Actor* actor, ActionTrigger id, s32 param = 0, const ankh::Vec<s32>& target = ankh::Vec<s32>( 0,0 ) )
   {
      switch (id)
      {
      case trigTouch:
      case trigShoot:
         // add karma
         gActorDatabase[mDestination].exec( trigSpecial, param, target );
         break;
      }
   }

private:
   ActorName mDestination;
};

class DoorBehavior : public Behavior
{
public:
   DoorBehavior() { this->reset(); }

   void reset()
   {
      mOpen = false;
      mCount = 0;
   }

   void exec( Actor* actor, ActionTrigger id, s32 param = 0, const ankh::Vec<s32>& target = ankh::Vec<s32>( 0,0 ) )
   {
      switch (id)
      {
      case trigSpecial:
         mOpen = !mOpen; // move to special...
         break;
      case trigTouch: // TODO: move me to base class..
         // add karma
         this->exec( actor, trigSpecial, param, target );    // hack...
         break;
      case trigFrame:
         if (mOpen && -actor->getSize( 0 ) < mCount)
         {
            --mCount;
            actor->mPos[0]--;
         }
         if (!mOpen && mCount < 0)
         {
            ++mCount;
            actor->mPos[0]++;
         }
         break;
      }
   }

   bool mOpen;
   s32 mCount;
};

class BadguyBehavior : public Behavior
{
public:
   BadguyBehavior()
   {
      this->reset();
   }

   void reset()
   {
      mCount = 0;
      mRumbles[0].set( 1, 0 );
      mRumbles[1].set( 0, 1 );
      mRumbles[2].set( -1, 0 );
      mRumbles[3].set( 0, -1 );
   }

   void exec( Actor* actor, ActionTrigger id, s32 param = 0, const ankh::Vec<s32>& target = ankh::Vec<s32>( 0,0 ) )
   {
      switch (id)
      {
      case trigShoot:
         // todo: explode me... :)
         actor->setPos( -23, -23 ); // hide me, im dead
         break;
      case trigFrame:
         mCount = (mCount + 1) % 4;
         actor->mPos += mRumbles[mCount];
         break;
      case trigTouch:
         gPlayerInventory.pickUpItem( param, target );
         break;
      }
   }

private:
   ankh::Vec<s32> mRumbles[4];
   s32 mCount;
};

BadguyBehavior* oBadGuyBeh;
TriggerBehavior* oTriggerBehavior;
ObjectBehavior* oObjectBehavior;
DoorBehavior* oDoorBehavior;

void initActorDatabase()
{
   // NPCs
   gActorDatabase[actBadguy11].init(  &gSpriteDatabase[sprBadguy1], &oBadGuyBeh[0], 100, 60, 6, 4 );
   gActorDatabase[actBadguy12].init(  &gSpriteDatabase[sprBadguy1], &oBadGuyBeh[1], 100, 260, 6, 4 );
   gActorDatabase[actBadguy131].init(  &gSpriteDatabase[sprBadguy1], &oBadGuyBeh[2], 90, 460, 6, 4 );
   gActorDatabase[actBadguy132].init(  &gSpriteDatabase[sprBadguy2], &oBadGuyBeh[3], 150, 460, 6, 4 );
   gActorDatabase[actBadguy241].init(  &gSpriteDatabase[sprBadguy1], &oBadGuyBeh[4], 290, 660, 6, 4 );
   gActorDatabase[actBadguy242].init(  &gSpriteDatabase[sprBadguy2], &oBadGuyBeh[5], 350, 660, 6, 4 );
   gActorDatabase[actBenign].init(  &gSpriteDatabase[sprBenign], &oObjectBehavior[0], -150, 100 );
   gActorDatabase[actSuck].init(    &gSpriteDatabase[sprSuck], &oObjectBehavior[1], -200, 100 );

   // PC
   gActorDatabase[actOther].init(   &gSpriteDatabase[sprOther], &oObjectBehavior[2], 120, 100, 22, 14 );

   // treasure!
   gActorDatabase[actIllumina].init(&gSpriteDatabase[sprIllumina], &oObjectBehavior[3], -250, 100 );
   gActorDatabase[actTreasure].init(&gSpriteDatabase[sprTreasure], &oObjectBehavior[4], -300, 100, 17, 6 );

   // lock/key/transport
   gActorDatabase[actKey].init(     &gSpriteDatabase[sprKey], &oObjectBehavior[5], -100, 150 );
   gActorDatabase[actLock].init(    &gSpriteDatabase[sprLock], &oObjectBehavior[6], -150, 150 );
   gActorDatabase[actLockface].init(&gSpriteDatabase[sprLockface], &oTriggerBehavior[0].init( actHorzdoor ), 139, 140, 14, 6 );
   gActorDatabase[actTele].init(    &gSpriteDatabase[sprTele], &oObjectBehavior[7], -250, 150 );

   // active items (shoot them, and they respond positively)
   gActorDatabase[actMusicbox].init(&gSpriteDatabase[sprMusicbox], &oObjectBehavior[8], 50, 75, 20, 8 );
   gActorDatabase[actBox].init(     &gSpriteDatabase[sprBox], &oObjectBehavior[9], -100, 200 );
   gActorDatabase[actTrap].init(    &gSpriteDatabase[sprTrap], &oObjectBehavior[10], -150, 200 );

   // misc pickups
   gActorDatabase[actBonus].init(   &gSpriteDatabase[sprBonus], &oObjectBehavior[11], -200, 200 );
   gActorDatabase[actChalice].init( &gSpriteDatabase[sprChalice], &oObjectBehavior[12], -250, 200 );
   gActorDatabase[actCrown].init(   &gSpriteDatabase[sprCrown], &oObjectBehavior[13], -300, 200 );
   gActorDatabase[actItem11].init(    &gSpriteDatabase[sprItem], &oObjectBehavior[14], 95, 35, 6, 4 );
   gActorDatabase[actItem23].init(    &gSpriteDatabase[sprItem], &oObjectBehavior[15], 208, 560, 6, 4 );
   gActorDatabase[actFlit].init(    &gSpriteDatabase[sprFlit], &oObjectBehavior[16], -150, 250 );

   // doors
   gActorDatabase[actHorzdoor].init(&gSpriteDatabase[sprHorzdoor], &oDoorBehavior[0], 95, 160, 30, 4 );
   gActorDatabase[actVertdoor].init(&gSpriteDatabase[sprVertdoor], &oDoorBehavior[1], -250, 250, 6, 32 );
}

inline bool intersect( Background& bg, Actor& act )
{
   // get all the tiles the actor is intersecting with.
   u32 tile_lt[2];
   tile_lt[0] = act.getMin(0) / bg.getBitmap().tpx;
   tile_lt[1] = act.getMin(1) / bg.getBitmap().tpy;
   
   u32 tile_rb[2];
   tile_rb[0] = act.getMax(0) / bg.getBitmap().tpx;
   tile_rb[1] = act.getMax(1) / bg.getBitmap().tpy;

   // return if any of them are not black (assume can only move through black tiles)
   for (u32 x = tile_lt[0]; x <= tile_rb[0]; ++x)
   for (u32 y = tile_lt[1]; y <= tile_rb[1]; ++y)
   {
      u32 tile = bg.getMap().getTile( x, y );
      if (tile != cBlackTileID) return true;
   }
   
   return false;
}

class RoomsMap : public Background
{
public:
   RoomsMap() : Background( walls, wallsMap ) {}
};

// base class for all gamestates.
class GameState
{
public:
};

// this represents/drives the main gameplay mode of the game
class InGameState : public GameState
{
public:
   InGameState()
   {
      this->reset();
   }

   void reset()
   {
      // init globals...
      gActorDatabase = mActorDatabase;
      gPlayer = &gActorDatabase[actOther];
      gPlayer->setPos( 120, 100 );


      mPlayerVelocity.set( 0, 0 );
      mRumbleCounter = 0;
      mRumbles[0].set( 1, 0 );
      mRumbles[1].set( 0, 1 );
      mRumbles[2].set( -1, 0 );
      mRumbles[3].set( 0, -1 );
   }

   void init()
   {
      this->reset();
      gba::Sprite::initAll();


      REG_DISPCNT = BGMODE0 | BG0ENABLE | OBJENABLE | OBJCHARMEMMAP1D;

      // load the palettes for sprites and for backgrounds
      gbaCopySpritePalette( sprite_Palette, 256 );
      gbaCopyBgPalette( bg_Palette, 256 );
      oBadGuyBeh = moBadGuyBeh;
      oTriggerBehavior = moTriggerBehavior;
      oObjectBehavior = moObjectBehavior;
      oDoorBehavior = moDoorBehavior;

      // init game objects...
      initSpriteDatabase();
      initActorDatabase();
      gShotManager.init();
      mMap.init();
   }

   void kill()
   {
      for (u32 x = 0; x < actTotalNumOfTypes; ++x)
      {
         gActorDatabase[x].kill();
      }
      gPlayer->kill();
      mMap.kill();
      gShotManager.kill();
   }

   // try to touch an item at the target position (world coords)
   void touchItem( const ankh::Vec<s32>& target )
   {
      for (s32 x = 0; x < actTotalNumOfTypes; ++x)
      {
         if (gActorDatabase[x].isOnScreen() &&
            ankh::intersect( ankh::xform( gActorDatabase[x].mLocalBox, gActorDatabase[x].mPos ), target ))
         {
            gActorDatabase[x].exec( trigTouch, x, target );
            return;
         }
      }
   }

   void update()
   {
      // update controller
      gInGameControl.update();

      // attempt to touch something (does nothing if touch misses).
      if (gInGameControl.touchLeft.getState() == ::FourStateDigital::EDGE_DOWN) 
         this->touchItem( gPlayer->mPos + ankh::Vec<s32>( -30, 0 ) );
      else if(gInGameControl.touchUp.getState() == ::FourStateDigital::EDGE_DOWN)
         this->touchItem( gPlayer->mPos + ankh::Vec<s32>( 0, -30 ) );
      else if(gInGameControl.touchRight.getState() == ::FourStateDigital::EDGE_DOWN)
         this->touchItem( gPlayer->mPos + ankh::Vec<s32>( 30, 0 ) );
      else if(gInGameControl.touchDown.getState() == ::FourStateDigital::EDGE_DOWN)
         this->touchItem( gPlayer->mPos + ankh::Vec<s32>( 0, 30 ) );

      // drop item from inventory
      if (gInGameControl.drop0.getState() == ::FourStateDigital::EDGE_DOWN) 
         gPlayerInventory.dropItem( 0 );
      else if(gInGameControl.drop1.getState() == ::FourStateDigital::EDGE_DOWN)
         gPlayerInventory.dropItem( 1 );
      else if(gInGameControl.drop2.getState() == ::FourStateDigital::EDGE_DOWN)
         gPlayerInventory.dropItem( 2 );
      else if(gInGameControl.drop3.getState() == ::FourStateDigital::EDGE_DOWN)
         gPlayerInventory.dropItem( 3 );

      s32 dist = 2;
      if (gInGameControl.shootLeft.getState() == ::FourStateDigital::EDGE_DOWN) 
         gShotManager.shoot( gPlayer->mPos + ankh::Vec<s32>( -gPlayer->getSize( 0 ) / 2 - dist, 0 ), ankh::Vec<s32>( -4, 0 ) );
      else if(gInGameControl.shootUp.getState() == ::FourStateDigital::EDGE_DOWN)
         gShotManager.shoot( gPlayer->mPos + ankh::Vec<s32>( 0, -gPlayer->getSize( 1 ) / 2 - dist ), ankh::Vec<s32>( 0, -4 ) );
      else if(gInGameControl.shootRight.getState() == ::FourStateDigital::EDGE_DOWN)
         gShotManager.shoot( gPlayer->mPos + ankh::Vec<s32>( gPlayer->getSize( 0 ) / 2 + dist, 0 ), ankh::Vec<s32>( 4, 0 ) );
      else if(gInGameControl.shootDown.getState() == ::FourStateDigital::EDGE_DOWN)
         gShotManager.shoot( gPlayer->mPos + ankh::Vec<s32>( 0, gPlayer->getSize( 1 ) / 2 + dist ), ankh::Vec<s32>( 0, 4 ) );

      
      if (gInGameControl.pause.getState() == ::FourStateDigital::EDGE_DOWN) 
         this->reset();

      // save current pos in case we have to undo
      ankh::Vec<s32> old_pos = gPlayer->mPos;
      
      // apply run controller input
      if(gInGameControl.runUp.getState() == ::FourStateDigital::EDGE_DOWN)
      {
	      mPlayerVelocity[0] = 0;
         mPlayerVelocity[1] -= 3;
      }
      if(gInGameControl.runDown.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         mPlayerVelocity[0] = 0;
         mPlayerVelocity[1] += 3;
      }
      if(gInGameControl.runLeft.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         mPlayerVelocity[0] -= 3;
         mPlayerVelocity[1] = 0;
      }
      if(gInGameControl.runRight.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         mPlayerVelocity[0] += 3;
         mPlayerVelocity[1] = 0;
      }

      // clamp velocity
      mPlayerVelocity[0] = ::clamp( mPlayerVelocity[0], -3, 3 );
      mPlayerVelocity[1] = ::clamp( mPlayerVelocity[1], -3, 3 );

      // apply velocity
      gPlayer->mPos += mPlayerVelocity;


      // apply walk controller input
      if(gInGameControl.stepUp.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         gPlayer->mPos += ankh::Vec<s32>( 0, -3 );
         mPlayerVelocity.set( 0, 0 );
      }
      if(gInGameControl.stepDown.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         gPlayer->mPos += ankh::Vec<s32>( 0, 3 );
         mPlayerVelocity.set( 0, 0 );
      }
      if(gInGameControl.stepLeft.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         gPlayer->mPos += ankh::Vec<s32>( -3, 0 );
         mPlayerVelocity.set( 0, 0 );
      }
      if(gInGameControl.stepRight.getState() == ::FourStateDigital::EDGE_DOWN)
      {
         gPlayer->mPos += ankh::Vec<s32>( 3, 0 );
         mPlayerVelocity.set( 0, 0 );
      }

      if (intersectAllActors() || intersectMap())
      {
         // undo, can't go there...
         gPlayer->mPos = old_pos;
         
         // apply a rumble visual to the player to indicate they are bumping something.
         gPlayer->mPos += mRumbles[mRumbleCounter];
         mRumbleCounter = (mRumbleCounter+1) % 4;
      }


      // attach camera to player when player is far enough away from the map edges
      if ((mMap.getMinX() + SCREENX/2) <= gPlayer->mPos[0] && 
          gPlayer->mPos[0] <= (mMap.getMaxX() * 8 - SCREENX/2))
      {
         gCamera.mPos[0] = gPlayer->mPos[0] - SCREENX/2;
      }

      // attach camera to player when player is far enough away from the map edges
      if ((mMap.getMinY() + SCREENY/2) <= gPlayer->mPos[1] && 
          gPlayer->mPos[1] <= (mMap.getMaxY() * 8 - SCREENY/2))
      {
         gCamera.mPos[1] = gPlayer->mPos[1] - SCREENY/2;
      }

      // draw everyone based on the camera xform
      for (u32 x = 0; x < actTotalNumOfTypes; ++x)
      {
       gActorDatabase[x].update();
         gActorDatabase[x].exec( trigFrame, x ); // update all behaviors 
      }
      gShotManager.update();
      gPlayer->update();
      mMap.update();
   }

   bool intersectMap()
   {
      // keep us in bounds of the map extents. 
      // not needed when we have collision detection on the map walls... :)
      //gPlayer->mPos[0] = ::clamp( gPlayer->mPos[0], (mMap.getMinX() * 8) + 1, (mMap.getMaxX() * 8) - 1 );
      //gPlayer->mPos[1] = ::clamp( gPlayer->mPos[1], (mMap.getMinY() * 8) + 1, (mMap.getMaxY() * 8) - 1 );

      return intersect( mMap, *gPlayer );
   }

   bool intersectAllActors()
   {
      for (int x = 0; x < actTotalNumOfTypes; ++x)
      {
         if (x != actOther && gPlayer->mSprite != gActorDatabase[x].mSprite && gActorDatabase[x].isOnScreen())
         {
            if (intersect( *gPlayer, gActorDatabase[x] ))
            {
               return true;
            }
         }
      }
      return false;
   }                                                                      

private:
   Actor mActorDatabase[actTotalNumOfTypes];
   BadguyBehavior moBadGuyBeh[20];
TriggerBehavior moTriggerBehavior[20];
ObjectBehavior moObjectBehavior[20];
DoorBehavior moDoorBehavior[20];

   RoomsMap mMap;
   ankh::Vec<s32> mPlayerVelocity;
   u8 mRumbleCounter;
   ankh::Vec<s32> mRumbles[4];
};

class GameFramework
{
public:
   GameFramework() : mMap(wallsMap), mBitmap( walls )
   {
      
   }
   static GameFramework* gf = NULL;
   static GameFramework& instance() { if (!gf) gf = new GameFramework; return gf; }
    
   void init()
   {
      
      print( "starting app...\n" );
      mInGameState.init();
      
      // start VBL interrupt handler
      mRunning = true;
   }

   void kill()
   {
      mRunning = false;
      mInGameState.kill();
   }

   bool running()
   {
      return mRunning;
   }

   void update()
   {
      mInGameState.update();
   }

private:
   const gba::BitmapAsset& mBitmap;
   const gba::MapAsset& mMap;
   
   bool mRunning;
   InGameState mInGameState;

   static void vblFunc()
   {   
      GameFramework::instance().update();
   }

   static void hblFunc()
   {   
   }
};

