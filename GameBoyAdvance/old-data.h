#ifndef GAME_ASSETS
#define GAME_ASSETS

#include <assert.h>
#ifndef EDITOR
#include "Background.h"
#endif
//#include "gba/Sprite.h"

/////////////////////////////////////////////////////////
// readonly game data
///////////////////////////////////////////////////////////



// title screens.
extern const unsigned short title_Palette[256];
extern const unsigned char title5_Bitmap[];
extern const unsigned char title4_Bitmap[];
extern const unsigned char title3_Bitmap[];
extern const unsigned char title2_Bitmap[];
extern const unsigned char title_Bitmap[];

// palletes
extern const unsigned short sprite_Palette[];
extern const unsigned short bg_Palette[];

// level map
// bg (16x16 tiles, each is 8x8, 128x128 bitmap)
extern const unsigned char walls_Bitmap[];
const gba::BitmapAsset wallsBitmap = { 16, 16, 8, 8, walls_Bitmap };

// bg map
extern const unsigned short walls_Map[];
const gba::MapAsset wallsMap = { 96 * 2, 96 * 2, walls_Map };

//extern const unsigned char walls_Tiles[];

// sprites
//extern const unsigned char monster_Bitmap[];
//const gba::BitmapAsset monster( 1, 12, 16, 16, 16, 192, monster_Bitmap );

extern const unsigned char badguy_Bitmap[];
const gba::BitmapAsset badguy = { 1, 1, 8, 8, badguy_Bitmap };

extern const unsigned char benign_Bitmap[];
const gba::BitmapAsset benign = { 1, 2, 16, 16, benign_Bitmap };

extern const unsigned char bonus_Bitmap[];
const gba::BitmapAsset bonus = { 1, 1, 16, 16, bonus_Bitmap };

extern const unsigned char box_Bitmap[];
const gba::BitmapAsset box = { 1, 1, 16, 16, box_Bitmap };

extern const unsigned char chalice_Bitmap[];
const gba::BitmapAsset chalice = { 1, 1, 16, 16, chalice_Bitmap };

extern const unsigned char crown_Bitmap[];
const gba::BitmapAsset crown = { 1, 1, 32, 32, crown_Bitmap };

extern const unsigned char flit_Bitmap[];
const gba::BitmapAsset flit = { 1, 1, 16, 16, flit_Bitmap };

extern const unsigned char horzdoor_Bitmap[];
const gba::BitmapAsset horzdoor = { 1, 1, 32, 32, horzdoor_Bitmap };

extern const unsigned char illumina_Bitmap[];
const gba::BitmapAsset illumina = { 1, 1, 16, 16, illumina_Bitmap };

extern const unsigned char item_Bitmap[];
const gba::BitmapAsset item = { 1, 1, 8, 8, item_Bitmap };

extern const unsigned char key_Bitmap[];
const gba::BitmapAsset key = { 1, 1, 16, 16, key_Bitmap };

extern const unsigned char lock_Bitmap[];
const gba::BitmapAsset lock = { 1, 1, 8, 8, lock_Bitmap };

extern const unsigned char lockface_Bitmap[];
const gba::BitmapAsset lockface = { 1, 1, 16, 16, lockface_Bitmap };

extern const unsigned char musicbox_Bitmap[];
const gba::BitmapAsset musicbox = { 1, 1, 32, 32, musicbox_Bitmap };

extern const unsigned char other_Bitmap[];
const gba::BitmapAsset other = { 1, 1, 32, 32, other_Bitmap };

extern const unsigned char suck_Bitmap[];
const gba::BitmapAsset suck = { 1, 1, 32, 32, suck_Bitmap };

extern const unsigned char tele_Bitmap[];
const gba::BitmapAsset tele = { 1, 1, 16, 16, tele_Bitmap };

extern const unsigned char trap_Bitmap[];
const gba::BitmapAsset trap = { 1, 1, 16, 16, trap_Bitmap };

extern const unsigned char treasure_Bitmap[];
const gba::BitmapAsset treasure = { 1, 1, 32, 32, treasure_Bitmap };

extern const unsigned char vertdoor_Bitmap[];
const gba::BitmapAsset vertdoor = { 1, 1, 32, 32, vertdoor_Bitmap };




/*
#include "gba/Sprite.h"
enum SpriteTypes
{
   Other, Item, MusicBox
};

// all the sprites we'll need in the game
s32 gSpriteConfig[] = 
{
   3,
   Other,    // 0 other
   Item,     // 1 item
   MusicBox  // 2 music box
};

// all game sprites
gba::Sprite** gSpriteTable;

// factory function for creating sprites.
inline gba::Sprite* createSprite( SpriteTypes id )
{
   switch (id)
   {
   case Other: return new gba::Sprite( other.d, other.tpx, other.tpy );
   case Item: return new gba::Sprite( item.d, item.tpx, item.tpy );
   case MusicBox: return new gba::Sprite( musicbox.d, musicbox.tpx, musicbox.tpy );
   default: assert( false ); return 0;
   }
}

inline void initSpriteTable()
{
   gSpriteTable = new gba::Sprite*[gSpriteConfig[0]];
   for (int x = 0; x < gSpriteConfig[0]; ++x)
   {
      gSpriteTable[x] = createSprite( gSpriteConfig[1 + x * 1 + 0] );
   }
}
*/


/*
enum SpriteNames
{
   sprBadguy1, 
   sprBadguy2, 
   sprBadguy3, 
   sprBenign, 
   sprSuck, 
   sprOther, 
   sprIllumina, 
   sprTreasure, 
   sprKey, 
   sprLock, 
   sprLockface, 
   sprTele, 
   sprBonus, 
   sprBox, 
   sprChalice, 
   sprCrown, 
   sprFlit, 
   sprHorzdoor, 
   sprItem, 
   sprMusicbox,
   sprTrap, 
   sprVertdoor,
   sprShot0,sprShot1,sprShot2,sprShot3,sprShot4,sprShot5,sprShot6,sprShot7,sprShot8,sprShot9,sprShot10,
   sprTotalNumOfTypes // keep this last...
};

extern gba::Sprite gSpriteDatabase[];




inline void initSpriteDatabase()
{
   gSpriteDatabase[sprBadguy1].create( &badguy ); 
   gSpriteDatabase[sprBadguy2].create( &badguy ); 
   gSpriteDatabase[sprBadguy3].create( &badguy ); 
   gSpriteDatabase[sprBenign].create( &benign ); 
   gSpriteDatabase[sprSuck].create( &suck ); 
   gSpriteDatabase[sprOther].create( &other ); 
   gSpriteDatabase[sprIllumina].create( &illumina ); 
   gSpriteDatabase[sprTreasure].create( &treasure ); 
   gSpriteDatabase[sprKey].create( &key ); 
   gSpriteDatabase[sprLock].create( &lock ); 
   gSpriteDatabase[sprLockface].create( &lockface ); 
   gSpriteDatabase[sprTele].create( &tele ); 
   gSpriteDatabase[sprBonus].create( &bonus ); 
   gSpriteDatabase[sprBox].create( &box ); 
   gSpriteDatabase[sprChalice].create( &chalice ); 
   gSpriteDatabase[sprCrown].create( &crown ); 
   gSpriteDatabase[sprFlit].create( &flit ); 
   gSpriteDatabase[sprHorzdoor].create( &horzdoor ); 
   gSpriteDatabase[sprItem].create( &item ); 
   gSpriteDatabase[sprMusicbox].create( &musicbox );
   gSpriteDatabase[sprTrap].create( &trap );

   gSpriteDatabase[sprShot0 + 0].create( &item );
   gSpriteDatabase[sprShot0 + 1].create( &item );
   gSpriteDatabase[sprShot0 + 2].create( &item );
   gSpriteDatabase[sprShot0 + 3].create( &item );
   gSpriteDatabase[sprShot0 + 4].create( &item );
   gSpriteDatabase[sprShot0 + 5].create( &item );
   gSpriteDatabase[sprShot0 + 6].create( &item );
   gSpriteDatabase[sprShot0 + 7].create( &item );
   gSpriteDatabase[sprShot0 + 8].create( &item );
   gSpriteDatabase[sprShot0 + 9].create( &item );

   gSpriteDatabase[sprVertdoor].create( &vertdoor );
}
*/


#endif
