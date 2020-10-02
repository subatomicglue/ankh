
#include "spewgba.h"
#include "Actor.h"
#include "AnkhActor.h"
#include "Inventory.h"
#include "commongame.h"

PlayerInventory gPlayerInventory;

s32 PlayerInventory::add( int preferredSlot, AnkhActor* actor, const gba::Vec<s32>& offset )
{
   // try the preferred slot first...
   if (!mItems[preferredSlot].occupied)
   {
      spew::Trace( "  adding to inventory:  slot[%d] at offset:%d,%d\n", preferredSlot, offset[0], offset[1] );
      mItems[preferredSlot] = Item( actor, offset );
      return preferredSlot;
   }

   for (int x = 0; x < 4; ++x)
   {
      if (!mItems[x].occupied)
      {
         spew::Trace( "  adding to inventory:  slot[%d] at offset:%d,%d\n", x, offset[0], offset[1] );
         mItems[x] = Item( actor, offset );
         return x;
      }
   }
   return -1;
}

void PlayerInventory::pickUpItem( int preferredSlot, AnkhActor* actor, AnkhActor* caller )
{
   if (actor && caller)
   {
      // add actor[name] to the inventory
      int slot = add( preferredSlot, actor, actor->pos() - caller->pos() );
      if (0 <= slot && slot < 4)
      {
         spew::Trace( "pickup slot:%d, %d,%d == %d,%d\n", slot, actor->cpos()[0], actor->cpos()[1], caller->cpos()[0], caller->cpos()[1] );

         // move actor to the inventory (screenspace location)
         actor->setScreenspace();
         switch (slot)
         {
         case 0: actor->setCPos( gba::Vec<s32>( 23*8 + 4, 3*8 - 4 ) ); break;
         case 1: actor->setCPos( gba::Vec<s32>( 27*8 + 4, 3*8 - 4 ) ); break;
         case 2: actor->setCPos( gba::Vec<s32>( 23*8 + 4, 7*8 - 4 ) ); break;
         case 3: actor->setCPos( gba::Vec<s32>( 27*8 + 4, 7*8 - 4 ) ); break;
         }
         spew::Trace( " inventory slot:%d, %d,%d\n", slot, actor->cpos()[0], actor->cpos()[1] );
         actor->setDepth( 0 ); //< draw the actor on top of the HUD...

         // let the actor process the pickup event
         actor->doThing( BehaviourMethod::PICKUP, caller );
      }
   }
}

void PlayerInventory::dropItem( int slot, const gba::Vec<s32>& droppoint )
{
   if (0 <= slot && slot < 4 && mItems[slot].occupied)
   {
      spew::GBAPrint( "  drop from inventory: slot[%d]", slot );
      spew::GBAPrint( " at loc:%d,%d offset:%d,%d\n", droppoint[0], droppoint[1], mItems[slot].offset[0], mItems[slot].offset[1] );
      
      gba::Vec<s32> oldPos = mItems[slot].actor->pos();

      // drop the item near the player, check if it intersected anything...
      mItems[slot].actor->setWorldspace();
      mItems[slot].actor->setPos( droppoint + mItems[slot].offset );
      AnkhActor* hitactor = NULL;
      if (!intersectAllActors(*mItems[slot].actor, hitactor, NULL, ACTOR_COLLISION_PHYSICS) && !intersectMap(*mItems[slot].actor))
      {   
         // let the actor process the drop event
         mItems[slot].actor->doThing( BehaviourMethod::DROP, &player );

         // remove from inventory
         mItems[slot].occupied = false;

         mItems[slot].actor->setDepth( 1 ); //< draw the actor under the HUD
      }
      else
      {
         spew::GBAPrint( "Can't drop, collision conflict... [%s]\n", hitactor?hitactor->name():"background" );

         // the item can't be dropped, put back into the inventory...
         mItems[slot].actor->setScreenspace();
         mItems[slot].actor->setPos( oldPos );
      }
   }
   else
   {
      spew::GBAPrint( "couldn't drop[%d] occupied=%d", slot, 0 <= slot && slot < 4 ? mItems[slot].occupied : false );
   }
}
