import { Vec } from "./math.js";
import { intersectMap } from "./map.js";
import { intersectAllActors } from "./sprite.js";

function parseParams( behavior_params ) {
  if (behavior_params == undefined) return [];
  //console.log( "typeof behavior_params", typeof behavior_params );
  if (typeof behavior_params == "number") return [behavior_params];
  if (typeof behavior_params == "string") {
    let params = behavior_params == undefined ? [] : behavior_params
      .replace(/^\s+/,"") // data cleaning: strip whitespace at begining of string
      .replace(/\s+$/,"") // data cleaning: strip whitespace at end of string
      .split( /\s*,\s*/ ) // split string into an array (strip whitespace around the comma)
      .map( r =>          // convert each array element from string to number (if it is a number)
        (!isNaN(r)) &&          // use type coercion to parse the _entirety_ of the string (`parseFloat` alone does not do this)...
        (!isNaN(parseFloat(r))) // ...and ensure strings of whitespace fail
        ? parseFloat(r) : r     // if a number ? parse it, otherwise return the string as is
      );
    //console.log( `params "${behavior_params}" "${params.join(",")}"` );
    return params;
  }
  return behavior_params; // dont know what to do...
}

// the map of behavior names to behavior class types
let behaviors_lookup = {};
export function createBehavior( behavior_type, behavior_params ) {
  if (behaviors_lookup[behavior_type]) {
    //console.log( ` - Attaching behavior: ${behavior_type}` )
    let params = parseParams( behavior_params );
    //console.log( params );
    return new behaviors_lookup[behavior_type]( ...params )
  }
  console.log( ` - ERROR: behavior ${behavior_type} not found, using "none"` )
  return new NoneMethod();
}

// things:
export const BEHAVIOR_THINGS = {
  UPDATE: "UPDATE",
  TOUCH: "TOUCH",
}


/*
class BadguyMethod : public BehaviourMethod 
{
public:
   BadguyMethod()
   {
      mVelocity.set( 2, 2 );
      mRandDir.set( 0,0 );
      mUpdateFreq = 0;
      mUpdateCount = 0;
   }
   BadguyMethod( s32 velx, s32 vely )
   {
      mVelocity.set( velx, vely );
      mRandDir.set( 0,0 );
      mUpdateFreq = 0;
      mUpdateCount = 0;
   }
   gba::Vec<s8> mVelocity;
   gba::Vec<s8> mRandDir;
   s8 mUpdateFreq, mUpdateCount;

   virtual void doThing( BehaviourMethod::Thing thing, AnkhActor* parent, AnkhActor* caller )
   {
      switch (thing)
      {
      case BehaviourMethod::UPDATE:
         {
            // run update every nth time
            ++mUpdateCount;
            if (mUpdateCount < mUpdateFreq)
            {
               return;
            }
            mUpdateFreq = rand( 1, 2 ); // change from fast to slow
            mUpdateCount = 0;

            // save current pos in case we have to undo
            gba::Vec<s32> old_pos = parent->pos();

            if (rand(1,30) == 1) // 1 in n chance
            {
               gba::Vec<s32> dir2player = player.cpos() - parent->cpos();
               s32 dir2playerLength = dir2player.length();
               if (50 < dir2playerLength)
               {
                  // go toward player
                  //mRandDir = (dir2player*2) / dir2playerLength;
               }
               else
               {
                  // go away from player
                  mRandDir = -(dir2player*2) / dir2playerLength;
               }
               mRandDir[0] = clamp( mRandDir[0], (s8)-1, (s8)1 );
               mRandDir[1] = clamp( mRandDir[1], (s8)-1, (s8)1 );
            }
            else
            {
               // generate smooth wandering travel (better than just setting dir = rand each frame)...
               int which = rand(0,1);
               if (mRandDir[which] == 0) mRandDir[which] = rand(-1, 1);
               else if (mRandDir[1 - which] != 0) mRandDir[1 - which] = 0;
            }

            const s32 distFromScreenEdge = 20;
            if ((mRandDir[0] < 0 && parent->getScreenPos()[0] < (gGameWindow.mX+distFromScreenEdge)) ||
                (0 < mRandDir[0] && (gGameWindow.mX + gGameWindow.mWidth-distFromScreenEdge) < parent->getScreenPos()[0]))
            {
               spew::GBAPrint( "badguy: near edge of screen, reversing[0]\n" );
               mRandDir[0] = -mRandDir[0];
            }
            if ((mRandDir[1] < 0 && parent->getScreenPos()[1] < (gGameWindow.mY+distFromScreenEdge)) ||
                (0 < mRandDir[1] && (gGameWindow.mY + gGameWindow.mHeight-distFromScreenEdge) < parent->getScreenPos()[1]))
            {
               spew::GBAPrint( "badguy: near edge of screen, reversing[1]\n" );
               mRandDir[1] = -mRandDir[1];
            }
            gba::Vec<s32> offset = mRandDir * mVelocity;
            parent->m_oPos += offset;
            

            if (intersectAllActors( *parent, ACTOR_COLLISION_PHYSICS ) || intersectMap( *parent ))
            {
               // undo, can't go there...
               parent->m_oPos = old_pos;
               mRandDir = -mRandDir;
            }

            // shoot periodically
            static s8 timer = 0;
            static s8 timeout = 30;
            ++timer;
            if (timeout < timer)
            {
               // periodically shoot directly at player 1/6 the time
               if (50 < timeout) // increase value to make it less likely...
               {
                  spew::GBAPrint( "shoot directly\n" );
                  offset = player.cpos() - parent->cpos();
                  s32 l = offset.length();
                  offset = (offset * 3) / (l == 0 ? 1 : l); // normalize to 3x
               }
               while (0 == offset.lengthSqr())
               {
                  spew::GBAPrint( "shoot random\n" );
                  offset.set( rand( -1, 1 ), rand( -1, 1 ) );
               }
               gShotManager.shoot( parent->pos(), offset, parent );
               timeout = rand( 0, 60 );
               timer = 0;
            }
         }
         break;
      case BehaviourMethod::SHOOT:
         {
            spew::GBAPrint( "badguy:  i got shot! arggghhhh\n" );
            parent->hide();
            parent->disable();
         }
         break;
      case BehaviourMethod::TOUCH:
         //gPlayerInventory.pickUpItem( gInGameState.mLastTouchId, parent, caller );
         break;
      }
   }
};
*/
export class NoneMethod { doThing( thing, parent_actor, calling_actor ) {} }
behaviors_lookup["none"] = NoneMethod;
export let TeleMethod = NoneMethod;
behaviors_lookup["tele"] = TeleMethod;
//export let BenignMethod = BadguyMethod;
//behaviors_lookup["benign"] = BenignMethod;

/*
/// this plays the music sequence needed to get passed the piano room.
class TonegenMethod : public BehaviourMethod
{
public:
   virtual void doThing( BehaviourMethod::Thing id, AnkhActor* parent, AnkhActor* caller )
   {
      if (id == SHOOT)
      {
         // play sound
         spew::GBAPrint( "play key sound!\n" );
      }
   }
};

/// this plays the music sequence needed to get past the piano room.
class SuckMethod : public BehaviourMethod
{
public:
   SuckMethod( int side = 1, int damage = -4 ) : mSide( side ), mDamage( damage ), mAttached( false ) {}
   virtual void doThing( BehaviourMethod::Thing id, AnkhActor* parent, AnkhActor* caller )
   {
      if (id == UPDATE)
      {
         // save current pos in case we have to undo
         gba::Vec<s32> old_pos = parent->pos();
         gba::Vec<s32> dir( 0, 0 );

         // go directly to the player
         if (!mAttached)
         {
            dir = player.pos() - parent->pos();
            dir = dir / (dir.length()/3);
            parent->setPos( parent->pos() + dir );
            mPlayerPos = player.pos();
         }

         // suck the life out of the player.
         if (mAttached || intersect( parent->getWorldBox(), player.getWorldBox() ))
         {
            // freeze player
            player.setPos( mPlayerPos );
            gInGameState.mPlayerVelocity.set( 0,0 );

            // todo: detect the mSide instead of hardcoded...

            // snap sucks to player.
            parent->setCPos( player.cpos() + gba::Vec<s32>( 0, mSide * (player.bbox( 1 )-2) ) );
            gInGameState.setKarmaScore( gInGameState.getKarmaScore() + mDamage );
            mAttached = true;
         }

         // collision detect
         AnkhActor* hitactor = NULL;
         if (intersectAllActors( *parent, hitactor, &player, ACTOR_COLLISION_PHYSICS ) ||
             intersectMap( *parent ))
         {
            // undo, can't go there...
            parent->m_oPos = old_pos;
            mAttached = false;

            // try one step in a direction we think may work (vertical towards player)
            dir.set( 0, dir.length() * (-mSide) );
            parent->setPos( parent->pos() + dir );
            
            if (intersectAllActors( *parent, hitactor, &player, ACTOR_COLLISION_PHYSICS ) ||
                intersectMap( *parent ))
            {
               // undo, still can't go there...
               parent->m_oPos = old_pos;
            }
         }
      }
      else if (id == TOUCH || id == OFFSCREEN)
      {
         mAttached = false;
         parent->setPos( parent->getStartPos() );
      }
   }
   s8 mSide, mDamage, mAttached;
   gba::Vec<s32> mPlayerPos;
};



class PanpadMethod : public BehaviourMethod
{
public:
   PanpadMethod()
   {
      reset();
   }
   PanpadMethod( s32 x, s32 y )
   {
      reset();
      mX = x;
      mY = y;
   }
   void reset()
   {
      mX = 0;
      mY = 0;
      mState = out;
   }
   void doThing( BehaviourMethod::Thing thing, AnkhActor* parent, AnkhActor* caller )
   {
      switch (thing)
      {
      case BehaviourMethod::UPDATE:
         if (intersect( parent->getWorldBox(), player.cpos() ))
         {
            if (mState == enter) mState = in;
            if (mState == out) mState = enter;
         }
         else
         {
            if (mState == exit) mState = out;
            if (mState == in) mState = exit;
         }
         if (mState == enter)
         {
            spew::GBAPrint( "enter pan pad\n" );
            gCamera.setOffset( mX, mY );
         }
         if (mState == exit)
         {
            spew::GBAPrint( "exit pan pad\n" );
            gCamera.setOffset( 0, 0 );
         }
         break;
      }
   }
private:
   s8 mX, mY;
   enum State { in, out, enter, exit };
   s8 mState;
};


class InterppadMethod : public BehaviourMethod
{
public:
   InterppadMethod()
   {
      reset();
   }
   InterppadMethod( s32 startx, s32 endx, s32 starty, s32 endy )
   {
      reset();
      mStartX = startx;
      mStartY = starty;
      mEndX = endx;
      mEndY = endy;
   }
   void reset()
   {
      mStartX = 0;
      mStartY = 0;
      mEndX = 0;
      mEndY = 0;
      mState = out;
   }
   void doThing( BehaviourMethod::Thing thing, AnkhActor* parent, AnkhActor* caller )
   {
      switch (thing)
      {
      case BehaviourMethod::UPDATE:
         {
            // compute in/out state
            gba::Vec<s32> lerpval;
            gba::Vec<s32> lerprange;
            if (intersect01( lerpval, lerprange, parent->getWorldBox(), player.cpos() ))
            {
               if (mState == enter) mState = in;
               if (mState == out) mState = enter;
            }
            else
            {
               if (mState == exit) mState = out;
               if (mState == in) mState = exit;
            }
            
            // handle in/out state
            if (mState == enter || mState == in)
            {
               if (mState == enter) { spew::GBAPrint( "enter interp pad\n" ); }
               gba::Vec<s32> offset = lerp( lerpval, lerprange,
                                          gba::Vec<s32>( mStartX, mStartY ),
                                          gba::Vec<s32>( mEndX, mEndY ) );
               //spew::GBAPrint( "interp:  %d %d %d %d", mStartX, mStartY, mEndX, mEndY );
               //spew::GBAPrint( " | %d %d\n", offset[0], offset[1] );
               gCamera.setOffset( offset[0], offset[1] );
            }
            if (mState == exit)
            {
               spew::GBAPrint( "exit interp pad\n" );
               gCamera.setOffset( 0, 0 );
            }
         }
         break;
      }
   }
private:
   s8 mStartX, mStartY;
   s8 mEndX, mEndY;
   enum State { in, out, enter, exit };
   s8 mState;
};

/// extends the length of the tractorbeam
class TouchpadMethod : public BehaviourMethod
{
public:
   TouchpadMethod()
   {
      reset();
   }
   TouchpadMethod( s32 extraLength )
   {
      reset();
      mLength = extraLength;
   }
   void reset()
   {
      mState = out;
      mLength = 6;
   }
   void doThing( BehaviourMethod::Thing thing, AnkhActor* parent, AnkhActor* caller )
   {
      switch (thing)
      {
      case BehaviourMethod::UPDATE:
         if (intersect( parent->getWorldBox(), player.cpos() ))
         {
            if (mState == enter) mState = in;
            if (mState == out) mState = enter;
         }
         else
         {
            if (mState == exit) mState = out;
            if (mState == in) mState = exit;
         }
         if (mState == enter)
         {
            spew::GBAPrint( "enter touchextender pad\n" );
            gInGameState.extraLength = mLength;
         }
         if (mState == exit)
         {
            spew::GBAPrint( "exit touchextender pad\n" );
            gInGameState.extraLength = 0;
         }
         break;
      }
   }
private:
   enum State { in, out, enter, exit };
   s8 mState;
   s8 mLength;
};



class ShooterMethod : public BehaviourMethod
{
public:
   ShooterMethod() : BehaviourMethod()
   {
      reset();
   }
   /// delay == -1 : turn off auto shoot
   /// 0 <= delay   : autoshoot
   /// delayrange > 0 to generate new delay randomly between [0..delayrange]
   ShooterMethod( s32 x, s32 y, s32 delay, s32 delayrange = -1, s32 spread = 0, bool enabled = true )
   {
      reset();
      mVelocity.set( x, y );
      mDelay = delay;
      mEnabled = enabled;
      mDelayRange = delayrange;
      mSpread = spread;
   }
   void reset()
   {
      mTimer = 0;
      mDelay = 30;
      mVelocity.set( 0, -1 );
      mDoShot = false;
      mEnabled = true;
      mDelayRange = -1;
   }
   void doThing( BehaviourMethod::Thing thing, AnkhActor* parent, AnkhActor* caller )
   {
      switch (thing)
      {
      case BehaviourMethod::UPDATE:
         if (gShotManager.canShoot() && (mDoShot || (mEnabled && 0 <= mDelay && (++mTimer % mDelay) == 0)))
         {
            mDoShot = false;
            mTimer = 0;
            gba::Vec<s32> sidevel( rand( -mSpread * mVelocity[1], mSpread * mVelocity[1] ),
                                   rand( -mSpread * mVelocity[0], mSpread * mVelocity[0] ) );
            gba::Vec<s32> newvel = mVelocity + sidevel;
            gShotManager.shoot( parent->cpos() + (newvel / newvel.abs()) * parent->bbox()/2, newvel, parent );
            spew::GBAPrint( "%s shooting sidevel:[%d %d] spr:%d ", (int)parent->name(), sidevel[0], sidevel[1], mSpread );
            spew::GBAPrint( "  vel:[%d %d]\n", mVelocity[0], mVelocity[1] );

            if (0 < mDelayRange)
            {
               mDelay = rand( 0, mDelayRange );
            }
         }
         break;
      case BehaviourMethod::TRIGGER:
         // delay the shot, so that we see the shot actually happen when we enter the room...
         // (because update only runs when the actor is on screen)
         if (0 < mDelay)
            mEnabled = !mEnabled;
         else
            mDoShot = true;
         spew::GBAPrint( "%s triggered enabled %d\n", (int)parent->name(), (int)mEnabled );
         break;
      }
   }
private:
   u32 mTimer;
   gba::Vec<s32> mVelocity;
   s32 mDelay;
   s32 mDelayRange;
   s32 mSpread;
   bool mEnabled;
   bool mDoShot;
};


class MoverMixin
{
public:
   MoverMixin()
   {
      reset();
   }
   void reset()
   {
      mVelocity.set( 0,0 );
   }
   void doThing( BehaviourMethod::Thing thing, AnkhActor* parent, AnkhActor* caller )
   {
      const int stepsize = 3;
      switch (thing)
      {
      case BehaviourMethod::UPDATE:
      {
         if (mVelocity.lengthSqr())
         {
            // save current pos in case we have to undo
            gba::Vec<s32> old_pos = parent->pos();       

            // clamp velocity
            mVelocity[0] = ::clamp( mVelocity[0], -stepsize, stepsize );
            mVelocity[1] = ::clamp( mVelocity[1], -stepsize, stepsize );

            // apply velocity
            parent->m_oPos += mVelocity;


            // collide player against world, if collided, step player back, until no collision...  
            // play a little rumble...
            bool collided = (intersectAllActors( *parent, ACTOR_COLLISION_PHYSICS ) || intersectMap( *parent ));
            if (collided)
            {
               const gba::Vec<s32> direction = (parent->m_oPos - old_pos) / stepsize;
               int tries = 0;
               while (collided && tries <= stepsize)
               {
                  // step back...
                  parent->m_oPos = (parent->m_oPos - direction);

                  // see if we collided again...
                  collided = (intersectAllActors( *parent, ACTOR_COLLISION_PHYSICS ) || intersectMap( *parent ));
                  ++tries;
               }

               if ((stepsize - tries) <= 1 && parent->m_oPos == old_pos)
               {
                  spew::GBAPrint( "%s collide (stopping)\n", parent->name() );
                  mVelocity.set( 0, 0 );
               }
            }
         }
      }
      break;

      // apply run controller input
      case BehaviourMethod::LEFT:
         mVelocity[0] -= stepsize;
         mVelocity[1] = 0;
         spew::GBAPrint( "%s LEFT\n", parent->name() );
         break;
      case BehaviourMethod::UP:
         mVelocity[0] = 0;
         mVelocity[1] -= stepsize;
         spew::GBAPrint( "%s UP\n", parent->name() );
         break;
      case BehaviourMethod::DOWN:
         mVelocity[0] = 0;
         mVelocity[1] += stepsize;
         spew::GBAPrint( "%s DOWN\n", parent->name() );
         break;
      case BehaviourMethod::RIGHT:
         mVelocity[0] += stepsize;
         mVelocity[1] = 0;
         spew::GBAPrint( "%s RIGHT\n", parent->name() );
         break;
      }
   }

   gba::Vec<s32> mVelocity;
};


class TrapMethod : public ShooterMethod, public MoverMixin
{
public:
   TrapMethod() : ShooterMethod(), MoverMixin()
   {
      reset();
   }
   /// delay == -1 : turn off auto shoot
   /// 0 <= delay   : autoshoot
   TrapMethod( s32 x, s32 y, s32 delay ) : ShooterMethod( x, y, delay ), MoverMixin()
   {
      reset();
   }
   void reset()
   {
   }
   void doThing( BehaviourMethod::Thing thing, AnkhActor* parent, AnkhActor* caller )
   {
      switch (thing)
      {
      case TOUCH: gPlayerInventory.pickUpItem( gInGameState.mLastTouchId, parent, caller ); break;
      }
      MoverMixin::doThing( thing, parent, caller );
      ShooterMethod::doThing( thing, parent, caller );
   }
};


class ObjectMethod : public BehaviourMethod
{
public:
   void doThing( BehaviourMethod::Thing thing, AnkhActor* parent, AnkhActor* caller )
   {
      switch (thing)
      {
      case BehaviourMethod::TOUCH:
         //pick up
         gPlayerInventory.pickUpItem( gInGameState.mLastTouchId, parent, caller );
         break;
      }
   }
private:
};
typedef ObjectMethod BonusMethod;
typedef ObjectMethod CandyMethod;
typedef ObjectMethod ChaliceMethod;


class FlitMethod : public ObjectMethod
{
public:
   FlitMethod() {}
   virtual void doThing( BehaviourMethod::Thing action, AnkhActor* parent, AnkhActor* caller )
   {
      switch (action)
      {
      case TOUCH:
         gPlayerInventory.pickUpItem( gInGameState.mLastTouchId, parent, caller );
         break;
      case PICKUP: gInGameState.slow(); break;
      case DROP:   gInGameState.fast(); break;
      }
   }
};

class TreasureMethod : public ObjectMethod 
{
public:
   TreasureMethod()
   {
      mHitPoits = 1;
   }
   TreasureMethod( s32 h )
   {
      mHitPoits = h;
   }
   virtual void doThing( BehaviourMethod::Thing action, AnkhActor* parent, AnkhActor* caller )
   {
      switch (action)
      {
      case SHOOT:
         --mHitPoits;
         if (0 == mHitPoits)
         {
            parent->hide();
            parent->disable();
         }
         break;
      }
      ObjectMethod::doThing( action, parent, caller );
   }

   u8 mHitPoits;
};

class RedirectMethod : public ObjectMethod 
{
public:
   RedirectMethod()
   {
      mVelocity.set( 0, -1 );
   }
   RedirectMethod( s32 x, s32 y )
   {
      mVelocity.set( x, y );
   }
   virtual void doThing( BehaviourMethod::Thing action, AnkhActor* parent, AnkhActor* caller )
   {
      switch (action)
      {
      case SHOOT:
         spew::GBAPrint( "redirect!\n" );
         gShotManager.shoot( parent->cpos() + (mVelocity / mVelocity.abs()) * parent->bbox()/2, mVelocity, parent );
         break;
      }
      ObjectMethod::doThing( action, parent, caller );
   }

   gba::Vec<s32> mVelocity;
};

class KeyMethod : public ObjectMethod
{
public:
   inline KeyMethod() { reset(); }
   inline KeyMethod( AnkhActor& whoToTrigger ) { reset(); init( &whoToTrigger ); }

   inline void reset()
   {
      mDestination = NULL;
   }

   inline KeyMethod& init( AnkhActor* whoToTrigger )
   {
      mDestination = whoToTrigger;
      return *this;
   }

   virtual void doThing( BehaviourMethod::Thing id, AnkhActor* parent, AnkhActor* caller )
   {
      switch (id)
      {
      case TOUCH:
         //pick up
         gPlayerInventory.pickUpItem( gInGameState.mLastTouchId, parent, caller );
         break;

      case SHOOT:
         // play tone?? (check game).
         break;

      case DROP:
         if (mDestination && (mDestination->getRoomPos() == parent->getRoomPos()))
            mDestination->doThing( TRIGGER, parent );
         else
            spew::GBAPrint( "no destination for this trigger\n" );
         break;
      }
   }

private:
   AnkhActor* mDestination;
};


class IlluminaMethod : public ObjectMethod
{
public:
   inline IlluminaMethod() { reset(); }
   inline IlluminaMethod( ActorInit::Type t ) { reset(); mType = t; }

   inline void reset()
   {
      mType = ActorInit::treasure;
   }

   virtual void doThing( BehaviourMethod::Thing id, AnkhActor* parent, AnkhActor* caller )
   {
      switch (id)
      {
      case TOUCH:
         gPlayerInventory.pickUpItem( gInGameState.mLastTouchId, parent, caller );

         spew::GBAPrint( "illuminating...\n" );
         showAll( mType );
         break;

      case DROP:
         spew::GBAPrint( "darkness consumes...\n" );
         hideAll( mType );
      }
   }

private:
   ActorInit::Type mType;
};


class TriggerMethod : public BehaviourMethod
{
public:
   inline TriggerMethod() { reset(); }

   /// send [send] to [dest] when this method recieves a [recv]
   inline TriggerMethod( AnkhActor& dest, Thing send = TRIGGER, Thing recv = SHOOT, bool inSameRoom = false )
   {
      reset();
      init( &dest, send, recv, inSameRoom );
   }

   inline void reset()
   {
      mDestination = NULL;
      mSend = TRIGGER;
      mRecv = SHOOT;
      mInSameRoom = false;
   }

   /// send [send] to [dest] when this method recieves a [recv]
   inline TriggerMethod& init( AnkhActor* dest, Thing send = TRIGGER, Thing recv = SHOOT, bool inSameRoom = false )
   {
      mDestination = dest;
      mSend = send;
      mRecv = recv;
      mInSameRoom = inSameRoom;
      return *this;
   }

   virtual void doThing( BehaviourMethod::Thing id, AnkhActor* parent, AnkhActor* caller )
   {
      if (id == mRecv)
      {
         if ((mDestination && mInSameRoom && parent->getRoomPos() == mDestination->getRoomPos()) ||
             (mDestination && !mInSameRoom))
         {
            spew::GBAPrint( "Trigger: %s sending signal %d to %s\n", (int)parent->name(), (int)mSend, (int)mDestination->name() );
            mDestination->doThing( mSend, parent );
         }
         else if (!mDestination)
            spew::GBAPrint( "Trigger: got %d, no destination for %s's trigger\n", (int)id, (int)parent->name() );
         else if (mDestination && mInSameRoom)
            spew::GBAPrint( "Trigger: %s not in same room as %s\n", (int)parent->name(), (int)mDestination->name() );
      }
      else
      {
         if (id != UPDATE)
         {
            spew::GBAPrint( "Trigger: unhandled %s -> %s (got:%d, send:%d,    ", (int)parent->name(),
                  mDestination ? (int)mDestination->name() : (int)"NULL", mSend );
            spew::GBAPrint( "recv:%d)\n", mRecv );
         }
      }
   }
   Thing getRecv() const { return mRecv; }

private:
   AnkhActor* mDestination;
   Thing mSend, mRecv;
   bool mInSameRoom;
};
typedef TriggerMethod DoorlockMethod;
//typedef TriggerMethod CrownMethod;

// sends trigger to dest after UP, LEFT, DOWN, RIGHT
class StatetriggerMethod : public BehaviourMethod
{
public:
   inline StatetriggerMethod() : mState( 0 ), mSend( TRIGGER ), mDestination( NULL ), mDestination2( NULL ) {}

   inline StatetriggerMethod( AnkhActor& dest, Thing send = TRIGGER ) : 
         BehaviourMethod(), mState( 0 ), mSend( send ), mDestination( &dest ), mDestination2( NULL )
   {
   }
   inline StatetriggerMethod( AnkhActor& dest, AnkhActor& dest2, Thing send = TRIGGER ) : 
         BehaviourMethod(), mState( 0 ), mSend( send ), mDestination( &dest ), mDestination2( &dest2 )
   {
   }
   virtual void doThing( BehaviourMethod::Thing action, AnkhActor* parent, AnkhActor* caller )
   {
      if (action != UPDATE)
      {
         switch (action)
         {
         case UP:    mState = 1; spew::GBAPrint( "got UP\n" ); break;
         case LEFT:  if (mState == 1) mState = 2; else mState = 0; spew::GBAPrint( "got LEFT\n" );break;
         case DOWN:  if (mState == 2) mState = 3; else mState = 0; spew::GBAPrint( "got DOWN\n" );break;
         case RIGHT: if (mState == 3) mState = 4; else mState = 0; spew::GBAPrint( "got RIGHT\n" );break;
         }
         spew::GBAPrint( "StatetriggerMethod:  state == %d\n", (int)mState );
         if (mState == 4)
         {
            if (mDestination)
            {
               spew::GBAPrint( "StatetriggerMethod:  %s sending %d signal to %s\n", (int)parent->name(), (int)mSend, (int)mDestination->name() );
               mDestination->doThing( mSend, parent );
            }
            if (mDestination2)
            {
               spew::GBAPrint( "StatetriggerMethod:  %s sending %d signal to %s\n", (int)parent->name(), (int)mSend, (int)mDestination2->name() );
               mDestination2->doThing( mSend, parent );
            }
            mState = 0;
         }
      }
   }
   AnkhActor* mDestination;
   AnkhActor* mDestination2;
   Thing mSend;
   u8 mState;
};

class MusicboxMethod : public TriggerMethod
{
public:
   inline MusicboxMethod() : TriggerMethod() {}

   /// send [send] to [dest] when this method recieves a [recv]
   inline MusicboxMethod( AnkhActor& dest, Thing send = TRIGGER, Thing recv = SHOOT ) :
      TriggerMethod( dest, send, recv, true )
   {
   }
   virtual void doThing( BehaviourMethod::Thing action, AnkhActor* parent, AnkhActor* caller )
   {
      switch (action)
      {
      case TOUCH:
         // TODO: play music
         gPlayerInventory.pickUpItem( gInGameState.mLastTouchId, parent, caller );
         return;
      case DROP:
         // TODO: stop music
         break;
      }
      TriggerMethod::doThing( action, parent, caller );
   }
};


/// sends an event to dest when player collides with it...
class TriggerpadMethod : public TriggerMethod
{
public:
   TriggerpadMethod() : TriggerMethod()
   {
      reset();
   }
   TriggerpadMethod( AnkhActor& dest, Thing send ) : TriggerMethod( dest, send, ENTER )
   {
      reset();
   }
   void reset()
   {
      mState = out;
   }
   void doThing( BehaviourMethod::Thing thing, AnkhActor* parent, AnkhActor* caller )
   {
      switch (thing)
      {
      case BehaviourMethod::UPDATE:
         if (intersect( parent->getWorldBox(), player.cpos() ))
         {
            if (mState == enter) mState = in;
            if (mState == out) mState = enter;
         }
         else
         {
            if (mState == exit) mState = out;
            if (mState == in) mState = exit;
         }
         if (mState == enter)
         {
            spew::GBAPrint( "entered trigger pad\n" );
            TriggerMethod::doThing( TriggerMethod::getRecv(), parent, caller );
         }
         if (mState == exit)
         {
            spew::GBAPrint( "exited trigger pad\n" );
         }
         break;
      }
   }
private:
   enum State { in, out, enter, exit };
   s8 mState;
};

template <unsigned DIR> // 0 for horiz, 1 for vert
class DoorMethod : public BehaviourMethod
{
public:
   DoorMethod() { reset(); }

   /// state false = closed (no change)
   /// direction -1 or 1
   /// distance -1 or [0..n]
   DoorMethod( bool state, int direction = 1, int distance = -1 ) 
   { 
      reset();
      mOpen = state;
      mDir = direction;
      mDist = distance;
   }

   void reset()
   {
      mOpen = false;
      mCount = 0;
      mDir = 1;
      mDist = -1;
   }

   void doThing( BehaviourMethod::Thing id, AnkhActor* parent, AnkhActor* caller )
   {
      switch (id)
      {
      case TRIGGER:
         // trigger open animation
         mOpen = !mOpen; // move to special...
         break;
      case UPDATE:
         int dist = mDist == -1 ? parent->bbox( DIR ) : mDist;
         // handle open animation
         if (mOpen && -dist < mCount)
         {
            --mCount;
            parent->m_oPos[DIR] -= mDir;
         }
         if (!mOpen && mCount <= 0)
         {
            ++mCount;
            parent->m_oPos[DIR] += mDir;
         }
         break;
      }
   }

   s16 mCount;
   s16 mDir;
   s16 mDist;
   bool mOpen;
};

typedef DoorMethod<0> HorzdoorMethod;
typedef DoorMethod<1> VertdoorMethod;
*/


export class OscMethod {
  constructor( dirx, diry ) {
    // “this” represents the new object {} that you get back
    this.dir.x = dirx;
    this.dir.y = diry;
  }
  doThing( thing, parent_actor, calling_actor ) {
    switch (thing) {
      case BEHAVIOR_THINGS.UPDATE: {
        parent_actor.setVelocity( this.dir.x, this.dir.y );

        if (parent_actor.sprite.collided)
        {
          console.log( "osc behavior: bump! inverting direction" );
          this.dir.x = -this.dir.x;
          this.dir.y = -this.dir.y;
          parent_actor.setVelocity( this.dir.x, this.dir.y ); // invert direction
        }
      }
      break;

      case BEHAVIOR_THINGS.TOUCH: {
        //gPlayerInventory.pickUpItem( gInGameState.mLastTouchId, parent_actor, calling_actor );
      }
      break;
    }
  }
  dir = new Vec();
}
behaviors_lookup["osc"] = OscMethod;

/*
class PlayerdroneMethod : public BehaviourMethod
{
public:
   PlayerdroneMethod()
   {
      m_oDir.set( 0,0 );
   }
   virtual void doThing( BehaviourMethod::Thing thing, AnkhActor* parent, AnkhActor* caller )
   {
      switch (thing)
      {
      case BehaviourMethod::UP:
         spew::GBAPrint( "i've been told to go up\n" );
         m_oDir = gba::Vec<s32>( 0, -1 );
         break;
      case BehaviourMethod::DOWN:
         spew::GBAPrint( "i've been told to go down\n" );
         m_oDir = gba::Vec<s32>( 0, 1 );
         break;
      case BehaviourMethod::RIGHT:
         spew::GBAPrint( "i've been told to go right\n" );
         m_oDir = gba::Vec<s32>( -1, 0 );
         break;
      case BehaviourMethod::LEFT:
         spew::GBAPrint( "i've been told to go left\n" );
         m_oDir = gba::Vec<s32>( 1, 0 );
         break;
      case BehaviourMethod::STOP:
         spew::GBAPrint( "i've been told to stop\n" );
         m_oDir = gba::Vec<s32>( 0, 0 );
         break;

      case BehaviourMethod::SHOOT:
         {
            spew::GBAPrint( "i've been told to shoot\n" );
            gba::Vec<s32> velocity(1,0);
            gShotManager.shoot( parent->cpos() + (velocity / velocity.abs()) * parent->bbox()/2, velocity, parent );
            break;
         }

      case BehaviourMethod::EXIT:
         spew::GBAPrint( "i've been told to reset\n" );
         parent->setPos( gba::Vec<s32>( parent->m_rfoActorInit.m_oPos[0], parent->m_rfoActorInit.m_oPos[1] ) );
         break;

      case BehaviourMethod::UPDATE:
         {
            // save current pos in case we have to undo
            gba::Vec<s32> old_pos = parent->pos();

            parent->m_oPos += m_oDir;

            if (intersectAllActors( *parent, ACTOR_COLLISION_PHYSICS ) || intersectMap( *parent ))
            {
               // undo, can't go there...
               parent->m_oPos = old_pos;

               // stop
               m_oDir = gba::Vec<s32>( 0, 0 );
            }
         }
      }
   }
   gba::Vec<s32> m_oDir;
};


class PlayerMethod : public BehaviourMethod
{
public:
   PlayerMethod() : mTouchBusy( false )
   {
   }

   virtual void doThing( BehaviourMethod::Thing thing, AnkhActor* parent, AnkhActor* caller )
   {
      switch (thing)
      {
      case BehaviourMethod::SHOOT:
         // players loose karma everytime they're shot.
         gInGameState.setKarmaScore( gInGameState.getKarmaScore() + parent->karmaFromTouching() );
         break;

      case BehaviourMethod::TOUCH:
         // special case, usually this case is called when the object _gets_ touched.
         spew::GBAPrint( "player touch\n" );
         {
            if (!mDropBusy && !mTouchBusy)
            {
               mTouchID = gInGameState.mLastTouchId;
               mDropBusy = false;
               mTouchBusy = true;
               mTouched = false;
               mTouchAnim = 0;
               mTouchDir = 1;
               touchvert000.hide();
               touchhorz000.hide();
               touchvert000.setWorldspace();
               touchhorz000.setWorldspace();
               const int singleLength = Max( Max( touchvert000.bbox(0), touchvert000.bbox(1) ),
                                       Max( touchhorz000.bbox(0), touchhorz000.bbox(1) ) );
               mTotalLength = singleLength + singleLength/2 + gInGameState.extraLength;
            }
         }
         break;
      case BehaviourMethod::DROP:
         // special case, usually this case is called when the object _gets_ touched.
         spew::GBAPrint( "player drop[%d]\n", gInGameState.mLastTouchId );
         {
            if (!mDropBusy && !mTouchBusy && gPlayerInventory.mItems[gInGameState.mLastTouchId].occupied)
            {
               mTouchID = gInGameState.mLastTouchId;
               mDropBusy = true;
               mTouchBusy = false;
               mTouched = false;
               mTouchAnim = 0;
               mTouchDir = 1;
               touchvert000.hide();
               touchhorz000.hide();
               touchvert000.setWorldspace();
               touchhorz000.setWorldspace();
               switch (mTouchID)
               {// left, up, right, down
               case 0: spew::GBAPrint( "[0]:" ); mTotalLength = abs( gPlayerInventory.mItems[mTouchID].offset[0] ) - gPlayerInventory.mItems[mTouchID].actor->bbox(0) - 2;  break;
               case 1: spew::GBAPrint( "[1]:" ); mTotalLength = abs( gPlayerInventory.mItems[mTouchID].offset[1] ) - gPlayerInventory.mItems[mTouchID].actor->bbox(1) - 2;  break;
               case 2: spew::GBAPrint( "[2]:" ); mTotalLength = abs( gPlayerInventory.mItems[mTouchID].offset[0] ) - player.bbox(0) - 3;  break;
               case 3: spew::GBAPrint( "[3]:" ); mTotalLength = abs( gPlayerInventory.mItems[mTouchID].offset[1] ) - player.bbox(1) - 3;  break;
               }
               spew::GBAPrint( "dropping %d units away\n", mTotalLength );
            }
         }
         break;
      case BehaviourMethod::UPDATE:
         {
            // detect triggers...
            {
               AnkhActor* hitactor;
               bool collided = intersectAllActors( *parent, hitactor, parent, ACTOR_COLLISION_TRIGGER );
               if (collided && hitactor)
               {
                  ((AnkhActor*)hitactor)->doThing( BehaviourMethod::COLLIDE, parent );
               }
            }

            // do touch/drop
            if (mTouchBusy || mDropBusy)
            {
               mTouchAnim += mTouchDir;
               //spew::Trace( "player touch anim: %d dir:%d\n", mTouchAnim, mTouchDir );

               AnkhActor* graphic = &touchvert000;
               AnkhActor* graphic2 = &touchvert001;
               gba::AABox2<s32> collidebox;
               gba::Vec<s32> location;
               gba::Vec<s32> collisionLocation;
               gba::Vec<s32> start0, location1, anim;// location1 is where to draw the graphic... when mTouchAnim=singleLength
               const int singleLength = Max( Max( touchvert000.bbox(0), touchvert000.bbox(1) ),
                                             Max( touchhorz000.bbox(0), touchhorz000.bbox(1) ) );
               switch (mTouchID)
               {
               // left
               case 0:
                  graphic = &touchhorz000;
                  graphic2 = &touchhorz001;
                  start0 = parent->cpos() + gba::Vec<s32>( -parent->bbox(0)/2, 0 );
                  anim = gba::Vec<s32>( -mTouchAnim, 0 );
                  collisionLocation = start0 + anim;
                  location = collisionLocation;
                  location1 = start0 + gba::Vec<s32>( -singleLength, 0 );
                  {
                     gba::Vec<s32> collideBoxOffset( 0, graphic->bbox(1)/2 );
                     collidebox.mMin = collisionLocation - collideBoxOffset;
                     collidebox.mMax.set( parent->getWorldBox().mMin[0], collisionLocation[1] );
                     collidebox.mMax += collideBoxOffset;
                  }
                  break;
               // up
               case 1: 
                  graphic = &touchvert000;
                  graphic2 = &touchvert001;
                  start0 = parent->cpos() + gba::Vec<s32>( 0, -parent->bbox(1)/2 );
                  anim = gba::Vec<s32>( 0, -mTouchAnim );
                  collisionLocation = start0 + anim;
                  location = collisionLocation;
                  location1 = start0 + gba::Vec<s32>( 0, -singleLength );
                  {
                     gba::Vec<s32> collideBoxOffset( graphic->bbox(0)/2, 0 );
                     collidebox.mMin = collisionLocation - collideBoxOffset;
                     collidebox.mMax.set( collisionLocation[0], parent->getWorldBox().mMin[1] );
                     collidebox.mMax += collideBoxOffset;
                  }
                  break;
               // right
               case 2: 
                  graphic = &touchhorz000;
                  graphic2 = &touchhorz001;
                  start0 = parent->cpos() + gba::Vec<s32>( parent->bbox(0)/2 + 2, 0 );
                  anim = gba::Vec<s32>( mTouchAnim, 0 );
                  collisionLocation = start0 + anim;
                  location = collisionLocation + gba::Vec<s32>( -graphic->bbox(0), 0 );
                  location1 = start0 + gba::Vec<s32>( singleLength -graphic->bbox(0), 0 );
                  {
                     gba::Vec<s32> collideBoxOffset( 0, graphic->bbox(1)/2 );
                     collidebox.mMin.set( parent->getWorldBox().mMax[0], collisionLocation[1] );
                     collidebox.mMin -= collideBoxOffset;
                     collidebox.mMax = collisionLocation + collideBoxOffset;
                  }
                  break;
               // down
               case 3: 
                  graphic = &touchvert000; 
                  graphic2 = &touchvert001; 
                  start0 = parent->cpos() + gba::Vec<s32>( 0, 2 + parent->bbox(1)/2 );
                  anim = gba::Vec<s32>( 0, mTouchAnim );
                  collisionLocation = start0 + anim;
                  location = collisionLocation + gba::Vec<s32>( 0, -graphic->bbox(1) );
                  location1 = start0 + gba::Vec<s32>( 0, -graphic->bbox(1) + singleLength );
                  {
                     gba::Vec<s32> collideBoxOffset( graphic->bbox(0)/2, 0 );
                     collidebox.mMin.set( collisionLocation[0], parent->getWorldBox().mMax[1] );
                     collidebox.mMin -= collideBoxOffset;
                     collidebox.mMax = collisionLocation + collideBoxOffset;
                  }
                  break;
               }

               //spew::GBAPrint( "%d %d %d %d\n", collidebox.mMin[0], collidebox.mMin[1], collidebox.mMax[0], collidebox.mMax[1] );

               // do our own collision of tractorbeams...

               graphic->setPos( location ); 
               graphic->show();
               if (singleLength < mTouchAnim)
               {
                  //spew::GBAPrint( "LONG!! using 2nd touch sprite | singlesize:%d anim:%d\n", singleLength, mTouchAnim );
                  graphic2->setPos( location1 );
                  graphic2->show();
               }
               else
               {
                  graphic2->hide();
               }
               AnkhActor* resultActor = NULL;
               intersectAllActors( collidebox, graphic, resultActor, parent, ACTOR_COLLISION_PHYSICS );

               //spew::Trace( "touching location... %d %d  0x%x exclude:0x%x\n", collisionLocation[0], collisionLocation[1], result.actor, parent );
               if (mTouchBusy && !mTouched && resultActor)
               {
                  spew::GBAPrint( "Player touching actor:  %s\n", resultActor->name() );
                  resultActor->doThing( BehaviourMethod::TOUCH, parent );
                  mTouched = true; // only one touch per animation cycle
                  mTouchDir = -1;  // retract the touch...

                  // game scoring for objects.
                  if (!resultActor->m_bHasBeenCounted)
                  {
                     resultActor->m_bHasBeenCounted = true;
                     gInGameState.setObjectsScore( gInGameState.getObjectsScore() + 1 );
                     spew::GBAPrint( "increasing object score to %d\n",
                                    gInGameState.getObjectsScore() );
                  }
                  if (!resultActor->m_bHasBeenTouched)
                  {
                     resultActor->m_bHasBeenTouched = true;
                     gInGameState.setKarmaScore( gInGameState.getKarmaScore() + resultActor->karmaFromTouching() );
                     spew::GBAPrint( "touch karma %d  == %d\n",
                                       resultActor->karmaFromTouching(),
                                       gInGameState.getKarmaScore() );
                  }
               }

               bool touchedWall = intersect( gInGameState.mMap, collidebox );
               if (touchedWall)
               {
                  spew::GBAPrint( "player 'touched' wall\n" );
               }

               // state control
               switch (mTouchDir)
               {
               case 1:
                  if (mTotalLength < mTouchAnim || touchedWall)
                  {
                     // change direction of touch animation
                     mTouchDir = -1;

                     // process drop
                     if (mDropBusy)
                     {
                        gPlayerInventory.dropItem( mTouchID, player.pos() );
                     }
                  }
                  break;
               case -1:
                  if (mTouchAnim <= 1)
                  {
                     // done with touch animation
                     mTouchBusy = false;
                     mDropBusy = false;
                     graphic->hide();
                     graphic2->hide();
                  }
                  break;
               }
            }
         }
         break;
      }
   }

   int mTouchID; //< which touch is happening
   int mTouchDir; //< touch velocity, 1 is out, -1 is back in...
   int mTouchAnim; //< touch animation
   bool mTouchBusy, mDropBusy; //< true during animation
   bool mTouched;
   int mTotalLength;
};
*/

