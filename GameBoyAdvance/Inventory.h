
#ifndef INVENTORY
#define INVENTORY

#include "AnkhActor.h"

class Item
{
public:
   inline Item() : occupied( true ) {}
   inline Item( AnkhActor* a, const gba::Vec<s32>& o ) : actor( a ), offset( o ), occupied( true ) {}
   AnkhActor* actor;
   gba::Vec<s32> offset;
   bool occupied; //< is the slot empty?
};

class PlayerInventory
{
public:
   PlayerInventory()
   {
      mItems[0].occupied = false;
      mItems[1].occupied = false;
      mItems[2].occupied = false;
      mItems[3].occupied = false;
   }
   void pickUpItem( int preferredSlot, AnkhActor* actor, AnkhActor* caller );
   void dropItem( int slot, const gba::Vec<s32>& droppoint );
private:
   s32 add( int preferredSlot, AnkhActor* actor, const gba::Vec<s32>& offset );
public:
   Item mItems[4];
};
extern PlayerInventory gPlayerInventory;

#endif

