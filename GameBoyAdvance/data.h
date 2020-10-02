#ifndef GAME_ASSETS
#define GAME_ASSETS
#include <assert.h>
#include "Vec2.h"
#include "MapAsset.h"
//#include "gba/Sprite.h"
#include "Actor.h"
#include "UIScreenAsset.h"
class AnkhActor;
class PanPad;
/////////////////////////////////////////////////////////
// readonly game data
///////////////////////////////////////////////////////////
extern "C" { extern const unsigned short sprite_Palette[]; };
extern "C" { extern const unsigned short walls_Palette[]; };
extern "C" { extern const unsigned short title_Palette[]; };
extern "C" { extern const unsigned char title1_Bitmap[]; };
extern const gba::BitmapAsset title1;

extern "C" { extern const unsigned char title2_Bitmap[]; };
extern const gba::BitmapAsset title2;

extern "C" { extern const unsigned char title3_Bitmap[]; };
extern const gba::BitmapAsset title3;

extern "C" { extern const unsigned char title4_Bitmap[]; };
extern const gba::BitmapAsset title4;

extern "C" { extern const unsigned char title5_Bitmap[]; };
extern const gba::BitmapAsset title5;

extern "C" { extern const unsigned char title6_Bitmap[]; };
extern const gba::BitmapAsset title6;

extern "C" { extern const unsigned char title7_Bitmap[]; };
extern const gba::BitmapAsset title7;

extern "C" { extern const unsigned char title8_Bitmap[]; };
extern const gba::BitmapAsset title8;

extern "C" { extern const unsigned char title9_Bitmap[]; };
extern const gba::BitmapAsset title9;

extern const UIScreenAsset title9_ui;
extern const UIScreenAsset title8_ui;
extern const UIScreenAsset title7_ui;
extern const UIScreenAsset title6_ui;
extern const UIScreenAsset title5_ui;
extern const UIScreenAsset title4_ui;
extern const UIScreenAsset title3_ui;
extern const UIScreenAsset title2_ui;
extern const UIScreenAsset title1_ui;
extern "C" { extern const unsigned char badguy_Bitmap[]; };
extern const gba::BitmapAsset badguy;

extern "C" { extern const unsigned char benign_Bitmap[]; };
extern const gba::BitmapAsset benign;

extern "C" { extern const unsigned char bonus_Bitmap[]; };
extern const gba::BitmapAsset bonus;

extern "C" { extern const unsigned char box_Bitmap[]; };
extern const gba::BitmapAsset box;

extern "C" { extern const unsigned char chalice_Bitmap[]; };
extern const gba::BitmapAsset chalice;

extern "C" { extern const unsigned char crown_Bitmap[]; };
extern const gba::BitmapAsset crown;

extern "C" { extern const unsigned char flit_Bitmap[]; };
extern const gba::BitmapAsset flit;

extern "C" { extern const unsigned char horzdoor_Bitmap[]; };
extern const gba::BitmapAsset horzdoor;

extern "C" { extern const unsigned char illumina_Bitmap[]; };
extern const gba::BitmapAsset illumina;

extern "C" { extern const unsigned char item_Bitmap[]; };
extern const gba::BitmapAsset item;

extern "C" { extern const unsigned char key_Bitmap[]; };
extern const gba::BitmapAsset key;

extern "C" { extern const unsigned char lock_Bitmap[]; };
extern const gba::BitmapAsset lock;

extern "C" { extern const unsigned char lockface_Bitmap[]; };
extern const gba::BitmapAsset lockface;

extern "C" { extern const unsigned char musicbox_Bitmap[]; };
extern const gba::BitmapAsset musicbox;

extern "C" { extern const unsigned char other_Bitmap[]; };
extern const gba::BitmapAsset other;

extern "C" { extern const unsigned char suck_Bitmap[]; };
extern const gba::BitmapAsset suck;

extern "C" { extern const unsigned char tele_Bitmap[]; };
extern const gba::BitmapAsset tele;

extern "C" { extern const unsigned char trap_Bitmap[]; };
extern const gba::BitmapAsset trap;

extern "C" { extern const unsigned char treasure_Bitmap[]; };
extern const gba::BitmapAsset treasure;

extern "C" { extern const unsigned char vertdoor_Bitmap[]; };
extern const gba::BitmapAsset vertdoor;

extern "C" { extern const unsigned char locklisten_Bitmap[]; };
extern const gba::BitmapAsset locklisten;

extern "C" { extern const unsigned char bullet_Bitmap[]; };
extern const gba::BitmapAsset bullet;

extern "C" { extern const unsigned char touchvert_Bitmap[]; };
extern const gba::BitmapAsset touchvert;

extern "C" { extern const unsigned char touchhorz_Bitmap[]; };
extern const gba::BitmapAsset touchhorz;

extern "C" { extern const unsigned short walls_Map[]; };
const gba::MapAsset wallsMap = { 96 * 2, 96 * 2, walls_Map };

extern "C" { extern const unsigned short hud_Map[]; };
const gba::MapAsset hudMap = { 32, 32, hud_Map };

extern "C" { extern const unsigned char walls_Bitmap[]; };
extern const gba::BitmapAsset wallsBitmap;

extern "C" { extern const unsigned char hud_Bitmap[]; };
extern const gba::BitmapAsset hudBitmap;

extern "C" { extern const unsigned char quake3_Bitmap[]; };
extern const gba::BitmapAsset quake3;

extern "C" { extern const unsigned char nobitmap_Bitmap[]; };
extern const gba::BitmapAsset nobitmap;

extern const gba::SpriteAsset badguySprite;
extern const gba::SpriteAsset benignSprite;
extern const gba::SpriteAsset bonusSprite;
extern const gba::SpriteAsset boxSprite;
extern const gba::SpriteAsset chaliceSprite;
extern const gba::SpriteAsset crownSprite;
extern const gba::SpriteAsset flitSprite;
extern const gba::SpriteAsset horzdoorSprite;
extern const gba::SpriteAsset illuminaSprite;
extern const gba::SpriteAsset itemSprite;
extern const gba::SpriteAsset keySprite;
extern const gba::SpriteAsset lockSprite;
extern const gba::SpriteAsset lockfaceSprite;
extern const gba::SpriteAsset musicboxSprite;
extern const gba::SpriteAsset otherSprite;
extern const gba::SpriteAsset suckSprite;
extern const gba::SpriteAsset teleSprite;
extern const gba::SpriteAsset trapSprite;
extern const gba::SpriteAsset treasureSprite;
extern const gba::SpriteAsset vertdoorSprite;
extern const gba::SpriteAsset locklistenSprite;
extern const gba::SpriteAsset bulletSprite;
extern const gba::SpriteAsset touchvertSprite;
extern const gba::SpriteAsset touchhorzSprite;
extern const ActorInit playerInit; // (6.375 * 16 + 0 * 192 - 16), (6.5 * 16 + 0 * 192 - 16)
extern AnkhActor player;
extern const ActorInit bullet000Init; // (6 * 16 + 0 * 192 - 16), (6.6 * 16 + 0 * 192 - 16)
extern AnkhActor bullet000;
extern const ActorInit bullet001Init; // (6.1 * 16 + 0 * 192 - 16), (6.7 * 16 + 0 * 192 - 16)
extern AnkhActor bullet001;
extern const ActorInit bullet002Init; // (6 * 16 + 0 * 192 - 16), (6.8 * 16 + 0 * 192 - 16)
extern AnkhActor bullet002;
extern const ActorInit bullet003Init; // (6.1 * 16 + 0 * 192 - 16), (6.9 * 16 + 0 * 192 - 16)
extern AnkhActor bullet003;
extern const ActorInit bullet004Init; // (6 * 16 + 0 * 192 - 16), (7 * 16 + 0 * 192 - 16)
extern AnkhActor bullet004;
extern const ActorInit bullet005Init; // (6.1 * 16 + 0 * 192 - 16), (7.1 * 16 + 0 * 192 - 16)
extern AnkhActor bullet005;
extern const ActorInit bullet006Init; // (6 * 16 + 0 * 192 - 16), (7.2 * 16 + 0 * 192 - 16)
extern AnkhActor bullet006;
extern const ActorInit bullet007Init; // (6.1 * 16 + 0 * 192 - 16), (7.3 * 16 + 0 * 192 - 16)
extern AnkhActor bullet007;
extern const ActorInit bullet008Init; // (6 * 16 + 0 * 192 - 16), (7 * 16 + 0 * 192 - 16)
extern AnkhActor bullet008;
extern const ActorInit bullet009Init; // (6.1 * 16 + 0 * 192 - 16), (7.1 * 16 + 0 * 192 - 16)
extern AnkhActor bullet009;
extern const ActorInit bullet010Init; // (6 * 16 + 0 * 192 - 16), (7.2 * 16 + 0 * 192 - 16)
extern AnkhActor bullet010;
extern const ActorInit bullet011Init; // (6.1 * 16 + 0 * 192 - 16), (7.3 * 16 + 0 * 192 - 16)
extern AnkhActor bullet011;
extern const ActorInit touchvert000Init; // (5 * 16 + 0 * 192 - 16), (6.625 * 16 + 0 * 192 - 16)
extern AnkhActor touchvert000;
extern const ActorInit touchhorz000Init; // (5.5 * 16 + 0 * 192 - 16), (6.625 * 16 + 0 * 192 - 16)
extern AnkhActor touchhorz000;
extern const ActorInit touchvert001Init; // (5 * 16 + 0 * 192 - 16), (6.625 * 16 + 0 * 192 - 16)
extern AnkhActor touchvert001;
extern const ActorInit touchhorz001Init; // (5.5 * 16 + 0 * 192 - 16), (6.625 * 16 + 0 * 192 - 16)
extern AnkhActor touchhorz001;
extern const ActorInit candy000Init; // (5 * 16 + 0 * 192 - 16), (3.375 * 16 + 0 * 192 - 16)
extern AnkhActor candy000;
extern const ActorInit candy001Init; // (7.625 * 16 + 0 * 192 - 16), (4.625 * 16 + 0 * 192 - 16)
extern AnkhActor candy001;
extern const ActorInit candy002Init; // (7.75 * 16 + 0 * 192 - 16), (6 * 16 + 0 * 192 - 16)
extern AnkhActor candy002;
extern const ActorInit door000Init; // (6 * 16 + 0 * 192 - 16), (11 * 16 + 0 * 192 - 16)
extern AnkhActor door000;
extern const ActorInit musicbox000Init; // (3 * 16 + 0 * 192 - 16), (6.625 * 16 + 0 * 192 - 16)
extern AnkhActor musicbox000;
extern const ActorInit doorlock000Init; // (9 * 16 + 0 * 192 - 16), (9.25 * 16 + 0 * 192 - 16)
extern AnkhActor doorlock000;
extern const ActorInit treasure000Init; // (6.5 * 16 + 0 * 192 - 16), (3.375 * 16 + 0 * 192 - 16)
extern AnkhActor treasure000;
extern const ActorInit enemy010Init; // (7 * 16 + 0 * 192 - 16), (7 * 16 + 1 * 192 - 16)
extern AnkhActor enemy010;
extern const ActorInit enemy020Init; // (5 * 16 + 0 * 192 - 16), (3.375 * 16 + 2 * 192 - 16)
extern AnkhActor enemy020;
extern const ActorInit enemy021Init; // (11 * 16 + 0 * 192 - 16), (6 * 16 + 2 * 192 - 16)
extern AnkhActor enemy021;
extern const ActorInit treasure030Init; // (4 * 16 + 0 * 192 - 16), (9.5 * 16 + 3 * 192 - 16)
extern AnkhActor treasure030;
extern const ActorInit treasure031Init; // (8.875 * 16 + 0 * 192 - 16), (9.5 * 16 + 3 * 192 - 16)
extern AnkhActor treasure031;
extern const ActorInit doorlock040Init; // (9.25 * 16 + 0 * 192 - 16), (5.375 * 16 + 4 * 192 - 16)
extern AnkhActor doorlock040;
extern const ActorInit doorlock041Init; // (5.25 * 16 + 0 * 192 - 16), (9.375 * 16 + 4 * 192 - 16)
extern AnkhActor doorlock041;
extern const ActorInit door040Init; // (9 * 16 + 0 * 192 - 16), (6 * 16 + 4 * 192 - 16)
extern AnkhActor door040;
extern const ActorInit door041Init; // (6 * 16 + 0 * 192 - 16), (9 * 16 + 4 * 192 - 16)
extern AnkhActor door041;
extern const ActorInit doorlock060Init; // (8 * 16 + 0 * 192 - 16), (9.25 * 16 + 6 * 192 - 16)
extern AnkhActor doorlock060;
extern const ActorInit door060Init; // (5.625 * 16 + 0 * 192 - 16), (5 * 16 + 6 * 192 - 16)
extern AnkhActor door060;
extern const ActorInit doorlock061Init; // (4.625 * 16 + 0 * 192 - 16), (5.875 * 16 + 6 * 192 - 16)
extern AnkhActor doorlock061;
extern const ActorInit benign100Init; // (6.5 * 16 + 1 * 192 - 16), (6.75 * 16 + 0 * 192 - 16)
extern AnkhActor benign100;
extern const ActorInit candy120Init; // (2.125 * 16 + 1 * 192 - 16), (11.5 * 16 + 2 * 192 - 16)
extern AnkhActor candy120;
extern const ActorInit doorlock130Init; // (3.25 * 16 + 1 * 192 - 16), (3.375 * 16 + 3 * 192 - 16)
extern AnkhActor doorlock130;
extern const ActorInit door130Init; // (6 * 16 + 1 * 192 - 16), (2.75 * 16 + 3 * 192 - 16)
extern AnkhActor door130;
extern const ActorInit enemy130Init; // (9.125 * 16 + 1 * 192 - 16), (5.25 * 16 + 3 * 192 - 16)
extern AnkhActor enemy130;
extern const ActorInit enemy131Init; // (7 * 16 + 1 * 192 - 16), (8 * 16 + 3 * 192 - 16)
extern AnkhActor enemy131;
extern const ActorInit door140Init; // (6 * 16 + 1 * 192 - 16), (8 * 16 + 4 * 192 - 16)
extern AnkhActor door140;
extern const ActorInit musicbox140Init; // (6.5 * 16 + 1 * 192 - 16), (10.375 * 16 + 4 * 192 - 16)
extern AnkhActor musicbox140;
extern const ActorInit doorlock150Init; // (7 * 16 + 1 * 192 - 16), (5.25 * 16 + 5 * 192 - 16)
extern AnkhActor doorlock150;
extern const ActorInit door150Init; // (5.625 * 16 + 1 * 192 - 16), (8 * 16 + 5 * 192 - 16)
extern AnkhActor door150;
extern const ActorInit doorlock151Init; // (4.625 * 16 + 1 * 192 - 16), (8.875 * 16 + 5 * 192 - 16)
extern AnkhActor doorlock151;
extern const ActorInit enemy150Init; // (10.375 * 16 + 1 * 192 - 16), (3.625 * 16 + 5 * 192 - 16)
extern AnkhActor enemy150;
extern const ActorInit doorlock160Init; // (5.125 * 16 + 1 * 192 - 16), (2.25 * 16 + 6 * 192 - 16)
extern AnkhActor doorlock160;
extern const ActorInit door160Init; // (4 * 16 + 1 * 192 - 16), (9 * 16 + 6 * 192 - 16)
extern AnkhActor door160;
extern const ActorInit doorlock161Init; // (4.875 * 16 + 1 * 192 - 16), (10 * 16 + 6 * 192 - 16)
extern AnkhActor doorlock161;
extern const ActorInit crown200Init; // (6.375 * 16 + 2 * 192 - 16), (3.25 * 16 + 0 * 192 - 16)
extern AnkhActor crown200;
extern const ActorInit trap200Init; // (6.625 * 16 + 2 * 192 - 16), (9.75 * 16 + 0 * 192 - 16)
extern AnkhActor trap200;
extern const ActorInit teleswitch210Init; // (2.25 * 16 + 2 * 192 - 16), (10.25 * 16 + 1 * 192 - 16)
extern AnkhActor teleswitch210;
extern const ActorInit teleport210Init; // (10.25 * 16 + 2 * 192 - 16), (10.375 * 16 + 1 * 192 - 16)
extern AnkhActor teleport210;
extern const ActorInit lock220Init; // (5.375 * 16 + 2 * 192 - 16), (8.25 * 16 + 2 * 192 - 16)
extern AnkhActor lock220;
extern const ActorInit lock221Init; // (8.375 * 16 + 2 * 192 - 16), (5.5 * 16 + 2 * 192 - 16)
extern AnkhActor lock221;
extern const ActorInit lock222Init; // (9.25 * 16 + 2 * 192 - 16), (8.125 * 16 + 2 * 192 - 16)
extern AnkhActor lock222;
extern const ActorInit lock223Init; // (3.375 * 16 + 2 * 192 - 16), (5.5 * 16 + 2 * 192 - 16)
extern AnkhActor lock223;
extern const ActorInit face220Init; // (11.125 * 16 + 2 * 192 - 16), (9.125 * 16 + 2 * 192 - 16)
extern AnkhActor face220;
extern const ActorInit enemy230Init; // (8.25 * 16 + 2 * 192 - 16), (4 * 16 + 3 * 192 - 16)
extern AnkhActor enemy230;
extern const ActorInit enemy231Init; // (4.125 * 16 + 2 * 192 - 16), (4.75 * 16 + 3 * 192 - 16)
extern AnkhActor enemy231;
extern const ActorInit doorlock240Init; // (2 * 16 + 2 * 192 - 16), (8.25 * 16 + 4 * 192 - 16)
extern AnkhActor doorlock240;
extern const ActorInit door240Init; // (6 * 16 + 2 * 192 - 16), (5.75 * 16 + 4 * 192 - 16)
extern AnkhActor door240;
extern const ActorInit doorlock241Init; // (6.75 * 16 + 2 * 192 - 16), (4.875 * 16 + 4 * 192 - 16)
extern AnkhActor doorlock241;
extern const ActorInit doorlock250Init; // (8 * 16 + 2 * 192 - 16), (8.25 * 16 + 5 * 192 - 16)
extern AnkhActor doorlock250;
extern const ActorInit door250Init; // (4 * 16 + 2 * 192 - 16), (5.75 * 16 + 5 * 192 - 16)
extern AnkhActor door250;
extern const ActorInit key251Init; // (4.625 * 16 + 2 * 192 - 16), (4.625 * 16 + 5 * 192 - 16)
extern AnkhActor key251;
extern const ActorInit doorlock260Init; // (11 * 16 + 2 * 192 - 16), (8.25 * 16 + 6 * 192 - 16)
extern AnkhActor doorlock260;
extern const ActorInit door260Init; // (5.625 * 16 + 2 * 192 - 16), (3.875 * 16 + 6 * 192 - 16)
extern AnkhActor door260;
extern const ActorInit doorlock261Init; // (4.625 * 16 + 2 * 192 - 16), (4.875 * 16 + 6 * 192 - 16)
extern AnkhActor doorlock261;
extern const ActorInit shooter300Init; // (2.5 * 16 + 3 * 192 - 16), (3.25 * 16 + 0 * 192 - 16)
extern AnkhActor shooter300;
extern const ActorInit leftdoor300Init; // (4.25 * 16 + 3 * 192 - 16), (2.5 * 16 + 0 * 192 - 16)
extern AnkhActor leftdoor300;
extern const ActorInit rightdoor300Init; // (9.25 * 16 + 3 * 192 - 16), (2.5 * 16 + 0 * 192 - 16)
extern AnkhActor rightdoor300;
extern const ActorInit face300Init; // (10.75 * 16 + 3 * 192 - 16), (3.25 * 16 + 0 * 192 - 16)
extern AnkhActor face300;
extern const ActorInit musicbox301Init; // (6.75 * 16 + 3 * 192 - 16), (7 * 16 + 0 * 192 - 16)
extern AnkhActor musicbox301;
extern const ActorInit entrydoor300Init; // (6 * 16 + 3 * 192 - 16), (11 * 16 + 0 * 192 - 16)
extern AnkhActor entrydoor300;
extern const ActorInit lock300Init; // (2.625 * 16 + 3 * 192 - 16), (6 * 16 + 0 * 192 - 16)
extern AnkhActor lock300;
extern const ActorInit lock301Init; // (11.375 * 16 + 3 * 192 - 16), (6 * 16 + 0 * 192 - 16)
extern AnkhActor lock301;
extern const ActorInit treasure310Init; // (6.5 * 16 + 3 * 192 - 16), (6.75 * 16 + 1 * 192 - 16)
extern AnkhActor treasure310;
extern const ActorInit treasure320Init; // (10.75 * 16 + 3 * 192 - 16), (9.125 * 16 + 2 * 192 - 16)
extern AnkhActor treasure320;
extern const ActorInit benign320Init; // (8.125 * 16 + 3 * 192 - 16), (10 * 16 + 2 * 192 - 16)
extern AnkhActor benign320;
extern const ActorInit benign321Init; // (4.25 * 16 + 3 * 192 - 16), (9.375 * 16 + 2 * 192 - 16)
extern AnkhActor benign321;
extern const ActorInit door320Init; // (6 * 16 + 3 * 192 - 16), (8.75 * 16 + 2 * 192 - 16)
extern AnkhActor door320;
extern const ActorInit illumina320Init; // (2 * 16 + 3 * 192 - 16), (11.5 * 16 + 2 * 192 - 16)
extern AnkhActor illumina320;
extern const ActorInit enemy330Init; // (5.375 * 16 + 3 * 192 - 16), (3.75 * 16 + 3 * 192 - 16)
extern AnkhActor enemy330;
extern const ActorInit enemy331Init; // (9.375 * 16 + 3 * 192 - 16), (3.125 * 16 + 3 * 192 - 16)
extern AnkhActor enemy331;
extern const ActorInit enemy332Init; // (8.25 * 16 + 3 * 192 - 16), (10.25 * 16 + 3 * 192 - 16)
extern AnkhActor enemy332;
extern const ActorInit doorlock340Init; // (8 * 16 + 3 * 192 - 16), (8.25 * 16 + 4 * 192 - 16)
extern AnkhActor doorlock340;
extern const ActorInit door340Init; // (4 * 16 + 3 * 192 - 16), (5.75 * 16 + 4 * 192 - 16)
extern AnkhActor door340;
extern const ActorInit doorlock341Init; // (4.875 * 16 + 3 * 192 - 16), (4.875 * 16 + 4 * 192 - 16)
extern AnkhActor doorlock341;
extern const ActorInit doorlock350Init; // (5 * 16 + 3 * 192 - 16), (4.25 * 16 + 5 * 192 - 16)
extern AnkhActor doorlock350;
extern const ActorInit door350Init; // (5.625 * 16 + 3 * 192 - 16), (5.875 * 16 + 5 * 192 - 16)
extern AnkhActor door350;
extern const ActorInit doorlock351Init; // (4.75 * 16 + 3 * 192 - 16), (6.875 * 16 + 5 * 192 - 16)
extern AnkhActor doorlock351;
extern const ActorInit doorlock360Init; // (3 * 16 + 3 * 192 - 16), (5.25 * 16 + 6 * 192 - 16)
extern AnkhActor doorlock360;
extern const ActorInit door360Init; // (7.875 * 16 + 3 * 192 - 16), (3.875 * 16 + 6 * 192 - 16)
extern AnkhActor door360;
extern const ActorInit doorlock361Init; // (8.75 * 16 + 3 * 192 - 16), (4.875 * 16 + 6 * 192 - 16)
extern AnkhActor doorlock361;
extern const ActorInit suck400Init; // (6.25 * 16 + 4 * 192 - 16), (11.5 * 16 + 0 * 192 - 16)
extern AnkhActor suck400;
extern const ActorInit suck401Init; // (6.25 * 16 + 4 * 192 - 16), (2.125 * 16 + 0 * 192 - 16)
extern AnkhActor suck401;
extern const ActorInit enemy430Init; // (8.625 * 16 + 4 * 192 - 16), (9.625 * 16 + 3 * 192 - 16)
extern AnkhActor enemy430;
extern const ActorInit enemy431Init; // (5.625 * 16 + 4 * 192 - 16), (9.5 * 16 + 3 * 192 - 16)
extern AnkhActor enemy431;
extern const ActorInit enemy432Init; // (7 * 16 + 4 * 192 - 16), (8 * 16 + 3 * 192 - 16)
extern AnkhActor enemy432;
extern const ActorInit enemy440Init; // (10.125 * 16 + 4 * 192 - 16), (10.375 * 16 + 4 * 192 - 16)
extern AnkhActor enemy440;
extern const ActorInit enemy441Init; // (4.375 * 16 + 4 * 192 - 16), (2.375 * 16 + 4 * 192 - 16)
extern AnkhActor enemy441;
extern const ActorInit enemy442Init; // (11.125 * 16 + 4 * 192 - 16), (5.125 * 16 + 4 * 192 - 16)
extern AnkhActor enemy442;
extern const ActorInit enemy443Init; // (2.125 * 16 + 4 * 192 - 16), (4.25 * 16 + 4 * 192 - 16)
extern AnkhActor enemy443;
extern const ActorInit enemy450Init; // (9.25 * 16 + 4 * 192 - 16), (11.125 * 16 + 5 * 192 - 16)
extern AnkhActor enemy450;
extern const ActorInit enemy451Init; // (9.25 * 16 + 4 * 192 - 16), (2.25 * 16 + 5 * 192 - 16)
extern AnkhActor enemy451;
extern const ActorInit enemy452Init; // (3.25 * 16 + 4 * 192 - 16), (3.25 * 16 + 5 * 192 - 16)
extern AnkhActor enemy452;
extern const ActorInit enemy453Init; // (10.75 * 16 + 4 * 192 - 16), (4.125 * 16 + 5 * 192 - 16)
extern AnkhActor enemy453;
extern const ActorInit door460Init; // (6 * 16 + 4 * 192 - 16), (10 * 16 + 6 * 192 - 16)
extern AnkhActor door460;
extern const ActorInit door461Init; // (6 * 16 + 4 * 192 - 16), (8 * 16 + 6 * 192 - 16)
extern AnkhActor door461;
extern const ActorInit doorlock460Init; // (5 * 16 + 4 * 192 - 16), (7.25 * 16 + 6 * 192 - 16)
extern AnkhActor doorlock460;
extern const gba::SpriteAsset piano461Sprite;
extern const ActorInit piano461Init; // (5 * 16 + 4 * 192 - 16), (7.25 * 16 + 6 * 192 - 16)
extern AnkhActor piano461;
extern const ActorInit treasure470Init; // (9.875 * 16 + 4 * 192 - 16), (6.5 * 16 + 7 * 192 - 16)
extern AnkhActor treasure470;
extern const ActorInit treasure471Init; // (9.875 * 16 + 4 * 192 - 16), (6 * 16 + 7 * 192 - 16)
extern AnkhActor treasure471;
extern const ActorInit treasure472Init; // (9.875 * 16 + 4 * 192 - 16), (5.5 * 16 + 7 * 192 - 16)
extern AnkhActor treasure472;
extern const ActorInit enemy500Init; // (7.25 * 16 + 5 * 192 - 16), (10.375 * 16 + 0 * 192 - 16)
extern AnkhActor enemy500;
extern const ActorInit enemy501Init; // (7.25 * 16 + 5 * 192 - 16), (2.25 * 16 + 0 * 192 - 16)
extern AnkhActor enemy501;
extern const ActorInit up540Init; // (11.375 * 16 + 5 * 192 - 16), (10.75 * 16 + 4 * 192 - 16)
extern AnkhActor up540;
extern const ActorInit down541Init; // (9.125 * 16 + 5 * 192 - 16), (10.75 * 16 + 4 * 192 - 16)
extern AnkhActor down541;
extern const ActorInit stop542Init; // (10.25 * 16 + 5 * 192 - 16), (11.625 * 16 + 4 * 192 - 16)
extern AnkhActor stop542;
extern const ActorInit shoot543Init; // (11.375 * 16 + 5 * 192 - 16), (10.375 * 16 + 4 * 192 - 16)
extern AnkhActor shoot543;
extern const ActorInit face544Init; // (6.875 * 16 + 5 * 192 - 16), (6.75 * 16 + 4 * 192 - 16)
extern AnkhActor face544;
extern const ActorInit player540Init; // (2 * 16 + 5 * 192 - 16), (2 * 16 + 4 * 192 - 16)
extern AnkhActor player540;
extern const gba::SpriteAsset triggerpad540Sprite;
extern const ActorInit triggerpad540Init; // (1152px), (847px)
extern AnkhActor triggerpad540;
extern const ActorInit bonus550Init; // (7 * 16 + 5 * 192 - 16), (7.75 * 16 + 5 * 192 - 16)
extern AnkhActor bonus550;
extern const ActorInit down550Init; // (9 * 16 + 5 * 192 - 16), (10.75 * 16 + 5 * 192 - 16)
extern AnkhActor down550;
extern const ActorInit up551Init; // (11.5 * 16 + 5 * 192 - 16), (10.75 * 16 + 5 * 192 - 16)
extern AnkhActor up551;
extern const ActorInit shoot552Init; // (11.5 * 16 + 5 * 192 - 16), (10.375 * 16 + 5 * 192 - 16)
extern AnkhActor shoot552;
extern const ActorInit face550Init; // (6.875 * 16 + 5 * 192 - 16), (6.75 * 16 + 5 * 192 - 16)
extern AnkhActor face550;
extern const ActorInit player550Init; // (2 * 16 + 5 * 192 - 16), (2.125 * 16 + 5 * 192 - 16)
extern AnkhActor player550;
extern const gba::SpriteAsset triggerpad550Sprite;
extern const ActorInit triggerpad550Init; // (1152px), (1039px)
extern AnkhActor triggerpad550;
extern const ActorInit down560Init; // (9 * 16 + 5 * 192 - 16), (10.75 * 16 + 6 * 192 - 16)
extern AnkhActor down560;
extern const ActorInit shoot561Init; // (11.5 * 16 + 5 * 192 - 16), (10.25 * 16 + 6 * 192 - 16)
extern AnkhActor shoot561;
extern const ActorInit face560Init; // (6.875 * 16 + 5 * 192 - 16), (6.75 * 16 + 6 * 192 - 16)
extern AnkhActor face560;
extern const ActorInit player560Init; // (2 * 16 + 5 * 192 - 16), (2 * 16 + 6 * 192 - 16)
extern AnkhActor player560;
extern const gba::SpriteAsset triggerpad560Sprite;
extern const ActorInit triggerpad560Init; // (1152px), (1231px)
extern AnkhActor triggerpad560;
extern const ActorInit trigger570Init; // (11.5 * 16 + 5 * 192 - 16), (9.375 * 16 + 7 * 192 - 16)
extern AnkhActor trigger570;
extern const ActorInit trigger571Init; // (11.375 * 16 + 5 * 192 - 16), (10 * 16 + 7 * 192 - 16)
extern AnkhActor trigger571;
extern const ActorInit trigger572Init; // (11.5 * 16 + 5 * 192 - 16), (10.25 * 16 + 7 * 192 - 16)
extern AnkhActor trigger572;
extern const ActorInit trigger573Init; // (11.375 * 16 + 5 * 192 - 16), (10.625 * 16 + 7 * 192 - 16)
extern AnkhActor trigger573;
extern const ActorInit trigger574Init; // (10.25 * 16 + 5 * 192 - 16), (11.625 * 16 + 7 * 192 - 16)
extern AnkhActor trigger574;
extern const ActorInit trigger575Init; // (9.125 * 16 + 5 * 192 - 16), (10.5 * 16 + 7 * 192 - 16)
extern AnkhActor trigger575;
extern const ActorInit trigger576Init; // (9.25 * 16 + 5 * 192 - 16), (9.5 * 16 + 7 * 192 - 16)
extern AnkhActor trigger576;
extern const ActorInit trigger577Init; // (9 * 16 + 5 * 192 - 16), (9.25 * 16 + 7 * 192 - 16)
extern AnkhActor trigger577;
extern const ActorInit face570Init; // (6.75 * 16 + 5 * 192 - 16), (6.75 * 16 + 7 * 192 - 16)
extern AnkhActor face570;
extern const ActorInit enemy570Init; // (2.125 * 16 + 5 * 192 - 16), (2.125 * 16 + 7 * 192 - 16)
extern AnkhActor enemy570;
extern const ActorInit door570Init; // (5.625 * 16 + 5 * 192 - 16), (6 * 16 + 7 * 192 - 16)
extern AnkhActor door570;
extern const ActorInit door571Init; // (3.625 * 16 + 5 * 192 - 16), (2 * 16 + 7 * 192 - 16)
extern AnkhActor door571;
extern const ActorInit door572Init; // (3.625 * 16 + 5 * 192 - 16), (6 * 16 + 7 * 192 - 16)
extern AnkhActor door572;
extern const ActorInit door573Init; // (5.75 * 16 + 5 * 192 - 16), (2 * 16 + 7 * 192 - 16)
extern AnkhActor door573;
extern const ActorInit door574Init; // (4 * 16 + 5 * 192 - 16), (4 * 16 + 7 * 192 - 16)
extern AnkhActor door574;
extern const ActorInit door575Init; // (6 * 16 + 5 * 192 - 16), (5.875 * 16 + 7 * 192 - 16)
extern AnkhActor door575;
extern const ActorInit door576Init; // (6 * 16 + 5 * 192 - 16), (7.875 * 16 + 7 * 192 - 16)
extern AnkhActor door576;
extern const ActorInit door600Init; // (5.625 * 16 + 6 * 192 - 16), (5.875 * 16 + 0 * 192 - 16)
extern AnkhActor door600;
extern const ActorInit door610Init; // (6 * 16 + 6 * 192 - 16), (5 * 16 + 1 * 192 - 16)
extern AnkhActor door610;
extern const ActorInit doorlock610Init; // (3.125 * 16 + 6 * 192 - 16), (9.875 * 16 + 1 * 192 - 16)
extern AnkhActor doorlock610;
extern const ActorInit musicbox610Init; // (3 * 16 + 6 * 192 - 16), (6.625 * 16 + 1 * 192 - 16)
extern AnkhActor musicbox610;
extern const gba::SpriteAsset touchpad610Sprite;
extern const ActorInit touchpad610Init; // (1200px), (193px)
extern AnkhActor touchpad610;
extern const ActorInit door620Init; // (8.625 * 16 + 6 * 192 - 16), (1.875 * 16 + 2 * 192 - 16)
extern AnkhActor door620;
extern const ActorInit doorlock620Init; // (2.875 * 16 + 6 * 192 - 16), (11.625 * 16 + 2 * 192 - 16)
extern AnkhActor doorlock620;
extern const ActorInit key620Init; // (2.875 * 16 + 6 * 192 - 16), (7.125 * 16 + 2 * 192 - 16)
extern AnkhActor key620;
extern const ActorInit candy630Init; // (2.875 * 16 + 6 * 192 - 16), (5.25 * 16 + 3 * 192 - 16)
extern AnkhActor candy630;
extern const ActorInit candy631Init; // (2 * 16 + 6 * 192 - 16), (5.5 * 16 + 3 * 192 - 16)
extern AnkhActor candy631;
extern const ActorInit candy632Init; // (2 * 16 + 6 * 192 - 16), (5 * 16 + 3 * 192 - 16)
extern AnkhActor candy632;
extern const ActorInit candy633Init; // (2.875 * 16 + 6 * 192 - 16), (5 * 16 + 3 * 192 - 16)
extern AnkhActor candy633;
extern const ActorInit candy634Init; // (2.375 * 16 + 6 * 192 - 16), (5.5 * 16 + 3 * 192 - 16)
extern AnkhActor candy634;
extern const ActorInit candy635Init; // (2 * 16 + 6 * 192 - 16), (5.25 * 16 + 3 * 192 - 16)
extern AnkhActor candy635;
extern const ActorInit candy636Init; // (2.375 * 16 + 6 * 192 - 16), (5 * 16 + 3 * 192 - 16)
extern AnkhActor candy636;
extern const ActorInit candy637Init; // (2.875 * 16 + 6 * 192 - 16), (5.5 * 16 + 3 * 192 - 16)
extern AnkhActor candy637;
extern const ActorInit doorlock630Init; // (1.25 * 16 + 6 * 192 - 16), (8.375 * 16 + 3 * 192 - 16)
extern AnkhActor doorlock630;
extern const ActorInit door630Init; // (9 * 16 + 6 * 192 - 16), (6 * 16 + 3 * 192 - 16)
extern AnkhActor door630;
extern const ActorInit doorlock631Init; // (9.25 * 16 + 6 * 192 - 16), (8.25 * 16 + 3 * 192 - 16)
extern AnkhActor doorlock631;
extern const ActorInit door631Init; // (6 * 16 + 6 * 192 - 16), (9 * 16 + 3 * 192 - 16)
extern AnkhActor door631;
extern const gba::SpriteAsset panpad630Sprite;
extern const ActorInit panpad630Init; // (1086px), (1423px)
extern AnkhActor panpad630;
extern const ActorInit enemy650Init; // (3.75 * 16 + 6 * 192 - 16), (6.75 * 16 + 5 * 192 - 16)
extern AnkhActor enemy650;
extern const ActorInit enemy651Init; // (8.375 * 16 + 6 * 192 - 16), (7.25 * 16 + 5 * 192 - 16)
extern AnkhActor enemy651;
extern const ActorInit door650Init; // (5.625 * 16 + 6 * 192 - 16), (6 * 16 + 5 * 192 - 16)
extern AnkhActor door650;
extern const ActorInit door651Init; // (8 * 16 + 6 * 192 - 16), (6 * 16 + 5 * 192 - 16)
extern AnkhActor door651;
extern const ActorInit enemy660Init; // (3.75 * 16 + 6 * 192 - 16), (6.5 * 16 + 6 * 192 - 16)
extern AnkhActor enemy660;
extern const ActorInit enemy661Init; // (9.75 * 16 + 6 * 192 - 16), (6.75 * 16 + 6 * 192 - 16)
extern AnkhActor enemy661;
extern const ActorInit door660Init; // (5.625 * 16 + 6 * 192 - 16), (6 * 16 + 6 * 192 - 16)
extern AnkhActor door660;
extern const ActorInit door661Init; // (8 * 16 + 6 * 192 - 16), (6 * 16 + 6 * 192 - 16)
extern AnkhActor door661;
extern const ActorInit enemy670Init; // (2.75 * 16 + 6 * 192 - 16), (6.5 * 16 + 7 * 192 - 16)
extern AnkhActor enemy670;
extern const ActorInit enemy671Init; // (9.25 * 16 + 6 * 192 - 16), (6.875 * 16 + 7 * 192 - 16)
extern AnkhActor enemy671;
extern const ActorInit door670Init; // (5.625 * 16 + 6 * 192 - 16), (6 * 16 + 7 * 192 - 16)
extern AnkhActor door670;
extern const ActorInit door671Init; // (8 * 16 + 6 * 192 - 16), (6 * 16 + 7 * 192 - 16)
extern AnkhActor door671;
extern const ActorInit redirect710Init; // (6.625 * 16 + 7 * 192 - 16), (8.75 * 16 + 1 * 192 - 16)
extern AnkhActor redirect710;
extern const ActorInit redirect711Init; // (6.625 * 16 + 7 * 192 - 16), (4.75 * 16 + 1 * 192 - 16)
extern AnkhActor redirect711;
extern const ActorInit shooter710Init; // (9.875 * 16 + 7 * 192 - 16), (8.875 * 16 + 1 * 192 - 16)
extern AnkhActor shooter710;
extern const ActorInit doorlock710Init; // (3.75 * 16 + 7 * 192 - 16), (4.875 * 16 + 1 * 192 - 16)
extern AnkhActor doorlock710;
extern const ActorInit trigger760Init; // (4.625 * 16 + 7 * 192 - 16), (11.75 * 16 + 6 * 192 - 16)
extern AnkhActor trigger760;
extern const ActorInit trigger761Init; // (2.75 * 16 + 7 * 192 - 16), (4.125 * 16 + 6 * 192 - 16)
extern AnkhActor trigger761;
extern const ActorInit door760Init; // (5 * 16 + 7 * 192 - 16), (9.875 * 16 + 6 * 192 - 16)
extern AnkhActor door760;
extern const ActorInit doorlock760Init; // (7.25 * 16 + 7 * 192 - 16), (2.125 * 16 + 6 * 192 - 16)
extern AnkhActor doorlock760;
extern const ActorInit randshooter760Init; // (7.25 * 16 + 7 * 192 - 16), (8 * 16 + 6 * 192 - 16)
extern AnkhActor randshooter760;
extern const ActorInit redirect760Init; // (5.125 * 16 + 7 * 192 - 16), (7.375 * 16 + 6 * 192 - 16)
extern AnkhActor redirect760;
extern const ActorInit redirect761Init; // (5.375 * 16 + 7 * 192 - 16), (4.125 * 16 + 6 * 192 - 16)
extern AnkhActor redirect761;
extern const ActorInit redirect762Init; // (8.875 * 16 + 7 * 192 - 16), (9.125 * 16 + 6 * 192 - 16)
extern AnkhActor redirect762;
extern const ActorInit mover762Init; // (11.125 * 16 + 7 * 192 - 16), (2.125 * 16 + 6 * 192 - 16)
extern AnkhActor mover762;
extern const ActorInit door770Init; // (10 * 16 + 7 * 192 - 16), (10 * 16 + 7 * 192 - 16)
extern AnkhActor door770;
extern const ActorInit door771Init; // (10 * 16 + 7 * 192 - 16), (3.75 * 16 + 7 * 192 - 16)
extern AnkhActor door771;
extern const ActorInit door772Init; // (5 * 16 + 7 * 192 - 16), (3.75 * 16 + 7 * 192 - 16)
extern AnkhActor door772;
extern const ActorInit enemy770Init; // (6.875 * 16 + 7 * 192 - 16), (5 * 16 + 7 * 192 - 16)
extern AnkhActor enemy770;
extern const ActorInit enemy771Init; // (8.25 * 16 + 7 * 192 - 16), (7.75 * 16 + 7 * 192 - 16)
extern AnkhActor enemy771;
extern const ActorInit trigger770Init; // (9.625 * 16 + 7 * 192 - 16), (9.75 * 16 + 7 * 192 - 16)
extern AnkhActor trigger770;
extern const ActorInit trigger771Init; // (9.625 * 16 + 7 * 192 - 16), (4 * 16 + 7 * 192 - 16)
extern AnkhActor trigger771;
extern const ActorInit trigger772Init; // (4.5 * 16 + 7 * 192 - 16), (2 * 16 + 7 * 192 - 16)
extern AnkhActor trigger772;
extern const ActorInit chalice772Init; // (10.75 * 16 + 7 * 192 - 16), (10.875 * 16 + 7 * 192 - 16)
extern AnkhActor chalice772;
extern const ActorInit face770Init; // (10.5 * 16 + 7 * 192 - 16), (2.75 * 16 + 7 * 192 - 16)
extern AnkhActor face770;
extern const ActorInit flit770Init; // (5.125 * 16 + 7 * 192 - 16), (8.75 * 16 + 7 * 192 - 16)
extern AnkhActor flit770;


const gba::BitmapAsset* const gAllBitmaps[] = { &title1, &title2, &title3, &title4, &title5, &title6, &title7, &title8, &title9, &badguy, &benign, &bonus, &box, &chalice, &crown, &flit, &horzdoor, &illumina, &item, &key, &lock, &lockface, &musicbox, &other, &suck, &tele, &trap, &treasure, &vertdoor, &locklisten, &bullet, &touchvert, &touchhorz, &wallsBitmap, &hudBitmap, &quake3, &nobitmap,  };
const unsigned int gBitmapCount = 37;


extern AnkhActor* gAllActors[212];
const unsigned int gActorCount = 212;
#endif

