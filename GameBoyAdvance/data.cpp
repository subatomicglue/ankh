#include "AnkhActor.h"
#include "Behaviours.h"
#include "PanPad.h"
#include "data.h"

const gba::BitmapAsset title1 = { 1, 1, 240, 160, title1_Bitmap, title_Palette };
const gba::BitmapAsset title2 = { 1, 1, 240, 160, title2_Bitmap, title_Palette };
const gba::BitmapAsset title3 = { 1, 1, 240, 160, title3_Bitmap, title_Palette };
const gba::BitmapAsset title4 = { 1, 1, 240, 160, title4_Bitmap, title_Palette };
const gba::BitmapAsset title5 = { 1, 1, 240, 160, title5_Bitmap, title_Palette };
const gba::BitmapAsset title6 = { 1, 1, 240, 160, title6_Bitmap, title_Palette };
const gba::BitmapAsset title7 = { 1, 1, 240, 160, title7_Bitmap, title_Palette };
const gba::BitmapAsset title8 = { 1, 1, 240, 160, title8_Bitmap, title_Palette };
const gba::BitmapAsset title9 = { 1, 1, 240, 160, title9_Bitmap, title_Palette };
const UIScreenAsset title9_ui = { title9, &title2_ui };
const UIScreenAsset title8_ui = { title8, &title9_ui };
const UIScreenAsset title7_ui = { title7, &title8_ui };
const UIScreenAsset title6_ui = { title6, &title7_ui };
const UIScreenAsset title5_ui = { title5, &title6_ui };
const UIScreenAsset title4_ui = { title4, &title5_ui };
const UIScreenAsset title3_ui = { title3, &title4_ui };
const UIScreenAsset title2_ui = { title2, &title3_ui };
const UIScreenAsset title1_ui = { title1, &title2_ui };
const gba::BitmapAsset badguy = { 1, 1, 8, 8, badguy_Bitmap, sprite_Palette };
const gba::BitmapAsset benign = { 1, 2, 16, 16, benign_Bitmap, sprite_Palette };
const gba::BitmapAsset bonus = { 1, 1, 16, 16, bonus_Bitmap, sprite_Palette };
const gba::BitmapAsset box = { 1, 1, 16, 16, box_Bitmap, sprite_Palette };
const gba::BitmapAsset chalice = { 1, 1, 16, 16, chalice_Bitmap, sprite_Palette };
const gba::BitmapAsset crown = { 1, 1, 32, 32, crown_Bitmap, sprite_Palette };
const gba::BitmapAsset flit = { 1, 1, 16, 16, flit_Bitmap, sprite_Palette };
const gba::BitmapAsset horzdoor = { 1, 1, 32, 32, horzdoor_Bitmap, sprite_Palette };
const gba::BitmapAsset illumina = { 1, 1, 16, 16, illumina_Bitmap, sprite_Palette };
const gba::BitmapAsset item = { 1, 1, 8, 8, item_Bitmap, sprite_Palette };
const gba::BitmapAsset key = { 1, 2, 16, 8, key_Bitmap, sprite_Palette };
const gba::BitmapAsset lock = { 1, 1, 8, 8, lock_Bitmap, sprite_Palette };
const gba::BitmapAsset lockface = { 1, 1, 16, 16, lockface_Bitmap, sprite_Palette };
const gba::BitmapAsset musicbox = { 1, 1, 32, 32, musicbox_Bitmap, sprite_Palette };
const gba::BitmapAsset other = { 1, 1, 32, 32, other_Bitmap, sprite_Palette };
const gba::BitmapAsset suck = { 1, 1, 32, 32, suck_Bitmap, sprite_Palette };
const gba::BitmapAsset tele = { 1, 1, 16, 16, tele_Bitmap, sprite_Palette };
const gba::BitmapAsset trap = { 1, 1, 16, 16, trap_Bitmap, sprite_Palette };
const gba::BitmapAsset treasure = { 1, 1, 32, 32, treasure_Bitmap, sprite_Palette };
const gba::BitmapAsset vertdoor = { 1, 1, 32, 32, vertdoor_Bitmap, sprite_Palette };
const gba::BitmapAsset locklisten = { 1, 2, 16, 8, locklisten_Bitmap, sprite_Palette };
const gba::BitmapAsset bullet = { 1, 1, 8, 8, bullet_Bitmap, sprite_Palette };
const gba::BitmapAsset touchvert = { 1, 1, 16, 16, touchvert_Bitmap, sprite_Palette };
const gba::BitmapAsset touchhorz = { 1, 1, 16, 16, touchhorz_Bitmap, sprite_Palette };
const gba::BitmapAsset wallsBitmap = { 32, 32, 8, 8, walls_Bitmap, walls_Palette };
const gba::BitmapAsset hudBitmap = { 32, 32, 8, 8, hud_Bitmap, walls_Palette };
const gba::BitmapAsset quake3 = { 1, 1, 256, 256, quake3_Bitmap, sprite_Palette };
const gba::BitmapAsset nobitmap = { 1, 1, 8, 8, nobitmap_Bitmap, sprite_Palette };
const gba::SpriteAsset badguySprite = { gba::Vec<s32>( 6, 3 ), badguy };
const gba::SpriteAsset benignSprite = { gba::Vec<s32>( 15, 7 ), benign };
const gba::SpriteAsset bonusSprite = { gba::Vec<s32>( 13, 7 ), bonus };
const gba::SpriteAsset boxSprite = { gba::Vec<s32>( 9, 7 ), box };
const gba::SpriteAsset chaliceSprite = { gba::Vec<s32>( 11, 5 ), chalice };
const gba::SpriteAsset crownSprite = { gba::Vec<s32>( 19, 4 ), crown };
const gba::SpriteAsset flitSprite = { gba::Vec<s32>( 13, 5 ), flit };
const gba::SpriteAsset horzdoorSprite = { gba::Vec<s32>( 30, 4 ), horzdoor };
const gba::SpriteAsset illuminaSprite = { gba::Vec<s32>( 11, 6 ), illumina };
const gba::SpriteAsset itemSprite = { gba::Vec<s32>( 6, 3 ), item };
const gba::SpriteAsset keySprite = { gba::Vec<s32>( 16, 8 ), key };
const gba::SpriteAsset lockSprite = { gba::Vec<s32>( 8, 5 ), lock };
const gba::SpriteAsset lockfaceSprite = { gba::Vec<s32>( 14, 6 ), lockface };
const gba::SpriteAsset musicboxSprite = { gba::Vec<s32>( 20, 7 ), musicbox };
const gba::SpriteAsset otherSprite = { gba::Vec<s32>( 21, 13 ), other };
const gba::SpriteAsset suckSprite = { gba::Vec<s32>( 23, 5 ), suck };
const gba::SpriteAsset teleSprite = { gba::Vec<s32>( 9, 5 ), tele };
const gba::SpriteAsset trapSprite = { gba::Vec<s32>( 11, 5 ), trap };
const gba::SpriteAsset treasureSprite = { gba::Vec<s32>( 17, 6 ), treasure };
const gba::SpriteAsset vertdoorSprite = { gba::Vec<s32>( 6, 32 ), vertdoor };
const gba::SpriteAsset locklistenSprite = { gba::Vec<s32>( 16, 8 ), locklisten };
const gba::SpriteAsset bulletSprite = { gba::Vec<s32>( 2, 1 ), bullet };
const gba::SpriteAsset touchvertSprite = { gba::Vec<s32>( 2, 16 ), touchvert };
const gba::SpriteAsset touchhorzSprite = { gba::Vec<s32>( 16, 2 ), touchhorz };
const ActorInit playerInit = { "player", ActorInit::player, &otherSprite, 86, 88, 0, 0, -4, -4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.375 * 16 + 0 * 192 - 16), (6.5 * 16 + 0 * 192 - 16)
PlayerMethod playerBehaviourMethod;
AnkhActor player( playerInit, &playerBehaviourMethod );
const ActorInit bullet000Init = { "bullet000", ActorInit::none, &bulletSprite, 80, 89, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (6 * 16 + 0 * 192 - 16), (6.6 * 16 + 0 * 192 - 16)
NoneMethod bullet000BehaviourMethod;
AnkhActor bullet000( bullet000Init, &bullet000BehaviourMethod );
const ActorInit bullet001Init = { "bullet001", ActorInit::none, &bulletSprite, 81, 91, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (6.1 * 16 + 0 * 192 - 16), (6.7 * 16 + 0 * 192 - 16)
NoneMethod bullet001BehaviourMethod;
AnkhActor bullet001( bullet001Init, &bullet001BehaviourMethod );
const ActorInit bullet002Init = { "bullet002", ActorInit::none, &bulletSprite, 80, 92, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (6 * 16 + 0 * 192 - 16), (6.8 * 16 + 0 * 192 - 16)
NoneMethod bullet002BehaviourMethod;
AnkhActor bullet002( bullet002Init, &bullet002BehaviourMethod );
const ActorInit bullet003Init = { "bullet003", ActorInit::none, &bulletSprite, 81, 94, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (6.1 * 16 + 0 * 192 - 16), (6.9 * 16 + 0 * 192 - 16)
NoneMethod bullet003BehaviourMethod;
AnkhActor bullet003( bullet003Init, &bullet003BehaviourMethod );
const ActorInit bullet004Init = { "bullet004", ActorInit::none, &bulletSprite, 80, 96, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (6 * 16 + 0 * 192 - 16), (7 * 16 + 0 * 192 - 16)
NoneMethod bullet004BehaviourMethod;
AnkhActor bullet004( bullet004Init, &bullet004BehaviourMethod );
const ActorInit bullet005Init = { "bullet005", ActorInit::none, &bulletSprite, 81, 97, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (6.1 * 16 + 0 * 192 - 16), (7.1 * 16 + 0 * 192 - 16)
NoneMethod bullet005BehaviourMethod;
AnkhActor bullet005( bullet005Init, &bullet005BehaviourMethod );
const ActorInit bullet006Init = { "bullet006", ActorInit::none, &bulletSprite, 80, 99, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (6 * 16 + 0 * 192 - 16), (7.2 * 16 + 0 * 192 - 16)
NoneMethod bullet006BehaviourMethod;
AnkhActor bullet006( bullet006Init, &bullet006BehaviourMethod );
const ActorInit bullet007Init = { "bullet007", ActorInit::none, &bulletSprite, 81, 100, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (6.1 * 16 + 0 * 192 - 16), (7.3 * 16 + 0 * 192 - 16)
NoneMethod bullet007BehaviourMethod;
AnkhActor bullet007( bullet007Init, &bullet007BehaviourMethod );
const ActorInit bullet008Init = { "bullet008", ActorInit::none, &bulletSprite, 80, 96, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (6 * 16 + 0 * 192 - 16), (7 * 16 + 0 * 192 - 16)
NoneMethod bullet008BehaviourMethod;
AnkhActor bullet008( bullet008Init, &bullet008BehaviourMethod );
const ActorInit bullet009Init = { "bullet009", ActorInit::none, &bulletSprite, 81, 97, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (6.1 * 16 + 0 * 192 - 16), (7.1 * 16 + 0 * 192 - 16)
NoneMethod bullet009BehaviourMethod;
AnkhActor bullet009( bullet009Init, &bullet009BehaviourMethod );
const ActorInit bullet010Init = { "bullet010", ActorInit::none, &bulletSprite, 80, 99, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (6 * 16 + 0 * 192 - 16), (7.2 * 16 + 0 * 192 - 16)
NoneMethod bullet010BehaviourMethod;
AnkhActor bullet010( bullet010Init, &bullet010BehaviourMethod );
const ActorInit bullet011Init = { "bullet011", ActorInit::none, &bulletSprite, 81, 100, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (6.1 * 16 + 0 * 192 - 16), (7.3 * 16 + 0 * 192 - 16)
NoneMethod bullet011BehaviourMethod;
AnkhActor bullet011( bullet011Init, &bullet011BehaviourMethod );
const ActorInit touchvert000Init = { "touchvert000", ActorInit::none, &touchvertSprite, 64, 90, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (5 * 16 + 0 * 192 - 16), (6.625 * 16 + 0 * 192 - 16)
NoneMethod touchvert000BehaviourMethod;
AnkhActor touchvert000( touchvert000Init, &touchvert000BehaviourMethod );
const ActorInit touchhorz000Init = { "touchhorz000", ActorInit::none, &touchhorzSprite, 72, 90, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (5.5 * 16 + 0 * 192 - 16), (6.625 * 16 + 0 * 192 - 16)
NoneMethod touchhorz000BehaviourMethod;
AnkhActor touchhorz000( touchhorz000Init, &touchhorz000BehaviourMethod );
const ActorInit touchvert001Init = { "touchvert001", ActorInit::none, &touchvertSprite, 64, 90, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (5 * 16 + 0 * 192 - 16), (6.625 * 16 + 0 * 192 - 16)
NoneMethod touchvert001BehaviourMethod;
AnkhActor touchvert001( touchvert001Init, &touchvert001BehaviourMethod );
const ActorInit touchhorz001Init = { "touchhorz001", ActorInit::none, &touchhorzSprite, 72, 90, 0, 0, 4, 4, ACTOR_COLLISION | ACTOR_VISIBLE  }; // (5.5 * 16 + 0 * 192 - 16), (6.625 * 16 + 0 * 192 - 16)
NoneMethod touchhorz001BehaviourMethod;
AnkhActor touchhorz001( touchhorz001Init, &touchhorz001BehaviourMethod );
const ActorInit candy000Init = { "candy000", ActorInit::candy, &itemSprite, 64, 38, 0, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5 * 16 + 0 * 192 - 16), (3.375 * 16 + 0 * 192 - 16)
CandyMethod candy000BehaviourMethod;
AnkhActor candy000( candy000Init, &candy000BehaviourMethod );
const ActorInit candy001Init = { "candy001", ActorInit::candy, &itemSprite, 106, 58, 0, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (7.625 * 16 + 0 * 192 - 16), (4.625 * 16 + 0 * 192 - 16)
CandyMethod candy001BehaviourMethod;
AnkhActor candy001( candy001Init, &candy001BehaviourMethod );
const ActorInit candy002Init = { "candy002", ActorInit::osc, &itemSprite, 108, 80, 0, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (7.75 * 16 + 0 * 192 - 16), (6 * 16 + 0 * 192 - 16)
OscMethod candy002BehaviourMethod( -1, -1 );
AnkhActor candy002( candy002Init, &candy002BehaviourMethod );
const ActorInit door000Init = { "door000", ActorInit::horzdoor, &horzdoorSprite, 80, 160, 0, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 0 * 192 - 16), (11 * 16 + 0 * 192 - 16)
HorzdoorMethod door000BehaviourMethod;
AnkhActor door000( door000Init, &door000BehaviourMethod );
const ActorInit musicbox000Init = { "musicbox000", ActorInit::musicbox, &musicboxSprite, 32, 90, 0, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (3 * 16 + 0 * 192 - 16), (6.625 * 16 + 0 * 192 - 16)
MusicboxMethod musicbox000BehaviourMethod( piano461, BehaviourMethod::UP, BehaviourMethod::SHOOT );
AnkhActor musicbox000( musicbox000Init, &musicbox000BehaviourMethod );
const ActorInit doorlock000Init = { "doorlock000", ActorInit::doorlock, &lockfaceSprite, 128, 132, 0, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9 * 16 + 0 * 192 - 16), (9.25 * 16 + 0 * 192 - 16)
DoorlockMethod doorlock000BehaviourMethod( door000 );
AnkhActor doorlock000( doorlock000Init, &doorlock000BehaviourMethod );
const ActorInit treasure000Init = { "treasure000", ActorInit::treasure, &treasureSprite, 88, 38, 0, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.5 * 16 + 0 * 192 - 16), (3.375 * 16 + 0 * 192 - 16)
TreasureMethod treasure000BehaviourMethod( 10 );
AnkhActor treasure000( treasure000Init, &treasure000BehaviourMethod );
const ActorInit enemy010Init = { "enemy010", ActorInit::badguy, &badguySprite, 96, 288, 0, 1, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (7 * 16 + 0 * 192 - 16), (7 * 16 + 1 * 192 - 16)
BadguyMethod enemy010BehaviourMethod;
AnkhActor enemy010( enemy010Init, &enemy010BehaviourMethod );
const ActorInit enemy020Init = { "enemy020", ActorInit::badguy, &badguySprite, 64, 422, 0, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5 * 16 + 0 * 192 - 16), (3.375 * 16 + 2 * 192 - 16)
BadguyMethod enemy020BehaviourMethod;
AnkhActor enemy020( enemy020Init, &enemy020BehaviourMethod );
const ActorInit enemy021Init = { "enemy021", ActorInit::badguy, &badguySprite, 160, 464, 0, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11 * 16 + 0 * 192 - 16), (6 * 16 + 2 * 192 - 16)
BadguyMethod enemy021BehaviourMethod;
AnkhActor enemy021( enemy021Init, &enemy021BehaviourMethod );
const ActorInit treasure030Init = { "treasure030", ActorInit::treasure, &treasureSprite, 48, 712, 0, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4 * 16 + 0 * 192 - 16), (9.5 * 16 + 3 * 192 - 16)
TreasureMethod treasure030BehaviourMethod( 10 );
AnkhActor treasure030( treasure030Init, &treasure030BehaviourMethod );
const ActorInit treasure031Init = { "treasure031", ActorInit::treasure, &treasureSprite, 126, 712, 0, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8.875 * 16 + 0 * 192 - 16), (9.5 * 16 + 3 * 192 - 16)
TreasureMethod treasure031BehaviourMethod( 10 );
AnkhActor treasure031( treasure031Init, &treasure031BehaviourMethod );
const ActorInit doorlock040Init = { "doorlock040", ActorInit::doorlock, &lockSprite, 132, 838, 0, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.25 * 16 + 0 * 192 - 16), (5.375 * 16 + 4 * 192 - 16)
DoorlockMethod doorlock040BehaviourMethod( door040 );
AnkhActor doorlock040( doorlock040Init, &doorlock040BehaviourMethod );
const ActorInit doorlock041Init = { "doorlock041", ActorInit::doorlock, &lockSprite, 68, 902, 0, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.25 * 16 + 0 * 192 - 16), (9.375 * 16 + 4 * 192 - 16)
DoorlockMethod doorlock041BehaviourMethod( door041 );
AnkhActor doorlock041( doorlock041Init, &doorlock041BehaviourMethod );
const ActorInit door040Init = { "door040", ActorInit::vertdoor, &vertdoorSprite, 128, 848, 0, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9 * 16 + 0 * 192 - 16), (6 * 16 + 4 * 192 - 16)
VertdoorMethod door040BehaviourMethod;
AnkhActor door040( door040Init, &door040BehaviourMethod );
const ActorInit door041Init = { "door041", ActorInit::horzdoor, &horzdoorSprite, 80, 896, 0, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 0 * 192 - 16), (9 * 16 + 4 * 192 - 16)
HorzdoorMethod door041BehaviourMethod;
AnkhActor door041( door041Init, &door041BehaviourMethod );
const ActorInit doorlock060Init = { "doorlock060", ActorInit::doorlock, &lockfaceSprite, 112, 1284, 0, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8 * 16 + 0 * 192 - 16), (9.25 * 16 + 6 * 192 - 16)
DoorlockMethod doorlock060BehaviourMethod;
AnkhActor doorlock060( doorlock060Init, &doorlock060BehaviourMethod );
const ActorInit door060Init = { "door060", ActorInit::vertdoor, &vertdoorSprite, 74, 1216, 0, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.625 * 16 + 0 * 192 - 16), (5 * 16 + 6 * 192 - 16)
VertdoorMethod door060BehaviourMethod;
AnkhActor door060( door060Init, &door060BehaviourMethod );
const ActorInit doorlock061Init = { "doorlock061", ActorInit::doorlock, &itemSprite, 58, 1230, 0, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.625 * 16 + 0 * 192 - 16), (5.875 * 16 + 6 * 192 - 16)
DoorlockMethod doorlock061BehaviourMethod( door160 );
AnkhActor doorlock061( doorlock061Init, &doorlock061BehaviourMethod );
const ActorInit benign100Init = { "benign100", ActorInit::benign, &benignSprite, 280, 92, 1, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.5 * 16 + 1 * 192 - 16), (6.75 * 16 + 0 * 192 - 16)
BenignMethod benign100BehaviourMethod;
AnkhActor benign100( benign100Init, &benign100BehaviourMethod );
const ActorInit candy120Init = { "candy120", ActorInit::candy, &itemSprite, 210, 552, 1, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.125 * 16 + 1 * 192 - 16), (11.5 * 16 + 2 * 192 - 16)
CandyMethod candy120BehaviourMethod;
AnkhActor candy120( candy120Init, &candy120BehaviourMethod );
const ActorInit doorlock130Init = { "doorlock130", ActorInit::doorlock, &lockSprite, 228, 614, 1, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (3.25 * 16 + 1 * 192 - 16), (3.375 * 16 + 3 * 192 - 16)
DoorlockMethod doorlock130BehaviourMethod( door130 );
AnkhActor doorlock130( doorlock130Init, &doorlock130BehaviourMethod );
const ActorInit door130Init = { "door130", ActorInit::horzdoor, &horzdoorSprite, 272, 604, 1, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 1 * 192 - 16), (2.75 * 16 + 3 * 192 - 16)
HorzdoorMethod door130BehaviourMethod;
AnkhActor door130( door130Init, &door130BehaviourMethod );
const ActorInit enemy130Init = { "enemy130", ActorInit::badguy, &badguySprite, 322, 644, 1, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.125 * 16 + 1 * 192 - 16), (5.25 * 16 + 3 * 192 - 16)
BadguyMethod enemy130BehaviourMethod;
AnkhActor enemy130( enemy130Init, &enemy130BehaviourMethod );
const ActorInit enemy131Init = { "enemy131", ActorInit::badguy, &badguySprite, 288, 688, 1, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (7 * 16 + 1 * 192 - 16), (8 * 16 + 3 * 192 - 16)
BadguyMethod enemy131BehaviourMethod;
AnkhActor enemy131( enemy131Init, &enemy131BehaviourMethod );
const ActorInit door140Init = { "door140", ActorInit::horzdoor, &horzdoorSprite, 272, 880, 1, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 1 * 192 - 16), (8 * 16 + 4 * 192 - 16)
HorzdoorMethod door140BehaviourMethod;
AnkhActor door140( door140Init, &door140BehaviourMethod );
const ActorInit musicbox140Init = { "musicbox140", ActorInit::musicbox, &musicboxSprite, 280, 918, 1, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.5 * 16 + 1 * 192 - 16), (10.375 * 16 + 4 * 192 - 16)
MusicboxMethod musicbox140BehaviourMethod( piano461, BehaviourMethod::LEFT, BehaviourMethod::SHOOT );
AnkhActor musicbox140( musicbox140Init, &musicbox140BehaviourMethod );
const ActorInit doorlock150Init = { "doorlock150", ActorInit::doorlock, &lockfaceSprite, 288, 1028, 1, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (7 * 16 + 1 * 192 - 16), (5.25 * 16 + 5 * 192 - 16)
DoorlockMethod doorlock150BehaviourMethod;
AnkhActor doorlock150( doorlock150Init, &doorlock150BehaviourMethod );
const ActorInit door150Init = { "door150", ActorInit::vertdoor, &vertdoorSprite, 266, 1072, 1, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.625 * 16 + 1 * 192 - 16), (8 * 16 + 5 * 192 - 16)
VertdoorMethod door150BehaviourMethod;
AnkhActor door150( door150Init, &door150BehaviourMethod );
const ActorInit doorlock151Init = { "doorlock151", ActorInit::doorlock, &itemSprite, 250, 1086, 1, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.625 * 16 + 1 * 192 - 16), (8.875 * 16 + 5 * 192 - 16)
DoorlockMethod doorlock151BehaviourMethod(door060);
AnkhActor doorlock151( doorlock151Init, &doorlock151BehaviourMethod );
const ActorInit enemy150Init = { "enemy150", ActorInit::badguy, &badguySprite, 342, 1002, 1, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (10.375 * 16 + 1 * 192 - 16), (3.625 * 16 + 5 * 192 - 16)
BadguyMethod enemy150BehaviourMethod;
AnkhActor enemy150( enemy150Init, &enemy150BehaviourMethod );
const ActorInit doorlock160Init = { "doorlock160", ActorInit::doorlock, &lockfaceSprite, 258, 1172, 1, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.125 * 16 + 1 * 192 - 16), (2.25 * 16 + 6 * 192 - 16)
DoorlockMethod doorlock160BehaviourMethod;
AnkhActor doorlock160( doorlock160Init, &doorlock160BehaviourMethod );
const ActorInit door160Init = { "door160", ActorInit::horzdoor, &horzdoorSprite, 240, 1280, 1, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4 * 16 + 1 * 192 - 16), (9 * 16 + 6 * 192 - 16)
HorzdoorMethod door160BehaviourMethod;
AnkhActor door160( door160Init, &door160BehaviourMethod );
const ActorInit doorlock161Init = { "doorlock161", ActorInit::doorlock, &itemSprite, 254, 1296, 1, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.875 * 16 + 1 * 192 - 16), (10 * 16 + 6 * 192 - 16)
DoorlockMethod doorlock161BehaviourMethod(door260);
AnkhActor doorlock161( doorlock161Init, &doorlock161BehaviourMethod );
const ActorInit crown200Init = { "crown200", ActorInit::trigger, &crownSprite, 470, 36, 2, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.375 * 16 + 2 * 192 - 16), (3.25 * 16 + 0 * 192 - 16)
TriggerMethod crown200BehaviourMethod( trap200, BehaviourMethod::TRIGGER, BehaviourMethod::TOUCH );
AnkhActor crown200( crown200Init, &crown200BehaviourMethod );
const ActorInit trap200Init = { "trap200", ActorInit::trap, &trapSprite, 474, 140, 2, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.625 * 16 + 2 * 192 - 16), (9.75 * 16 + 0 * 192 - 16)
TrapMethod trap200BehaviourMethod( 0,-2,-1 );
AnkhActor trap200( trap200Init, &trap200BehaviourMethod );
const ActorInit teleswitch210Init = { "teleswitch210", ActorInit::doorlock, &teleSprite, 404, 340, 2, 1, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.25 * 16 + 2 * 192 - 16), (10.25 * 16 + 1 * 192 - 16)
DoorlockMethod teleswitch210BehaviourMethod;
AnkhActor teleswitch210( teleswitch210Init, &teleswitch210BehaviourMethod );
const ActorInit teleport210Init = { "teleport210", ActorInit::tele, &badguySprite, 532, 342, 2, 1, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (10.25 * 16 + 2 * 192 - 16), (10.375 * 16 + 1 * 192 - 16)
TeleMethod teleport210BehaviourMethod;
AnkhActor teleport210( teleport210Init, &teleport210BehaviourMethod );
const ActorInit lock220Init = { "lock220", ActorInit::trigger, &itemSprite, 454, 500, 2, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.375 * 16 + 2 * 192 - 16), (8.25 * 16 + 2 * 192 - 16)
TriggerMethod lock220BehaviourMethod( trap200, BehaviourMethod::UP,    BehaviourMethod::TOUCH );
AnkhActor lock220( lock220Init, &lock220BehaviourMethod );
const ActorInit lock221Init = { "lock221", ActorInit::trigger, &itemSprite, 502, 456, 2, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8.375 * 16 + 2 * 192 - 16), (5.5 * 16 + 2 * 192 - 16)
TriggerMethod lock221BehaviourMethod( trap200, BehaviourMethod::DOWN,  BehaviourMethod::TOUCH );
AnkhActor lock221( lock221Init, &lock221BehaviourMethod );
const ActorInit lock222Init = { "lock222", ActorInit::trigger, &itemSprite, 516, 498, 2, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.25 * 16 + 2 * 192 - 16), (8.125 * 16 + 2 * 192 - 16)
TriggerMethod lock222BehaviourMethod( trap200, BehaviourMethod::LEFT,  BehaviourMethod::TOUCH );
AnkhActor lock222( lock222Init, &lock222BehaviourMethod );
const ActorInit lock223Init = { "lock223", ActorInit::trigger, &itemSprite, 422, 456, 2, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (3.375 * 16 + 2 * 192 - 16), (5.5 * 16 + 2 * 192 - 16)
TriggerMethod lock223BehaviourMethod( trap200, BehaviourMethod::RIGHT, BehaviourMethod::TOUCH );
AnkhActor lock223( lock223Init, &lock223BehaviourMethod );
const ActorInit face220Init = { "face220", ActorInit::doorlock, &lockfaceSprite, 546, 514, 2, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.125 * 16 + 2 * 192 - 16), (9.125 * 16 + 2 * 192 - 16)
DoorlockMethod face220BehaviourMethod( door320 );
AnkhActor face220( face220Init, &face220BehaviourMethod );
const ActorInit enemy230Init = { "enemy230", ActorInit::badguy, &badguySprite, 500, 624, 2, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8.25 * 16 + 2 * 192 - 16), (4 * 16 + 3 * 192 - 16)
BadguyMethod enemy230BehaviourMethod;
AnkhActor enemy230( enemy230Init, &enemy230BehaviourMethod );
const ActorInit enemy231Init = { "enemy231", ActorInit::badguy, &badguySprite, 434, 636, 2, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.125 * 16 + 2 * 192 - 16), (4.75 * 16 + 3 * 192 - 16)
BadguyMethod enemy231BehaviourMethod;
AnkhActor enemy231( enemy231Init, &enemy231BehaviourMethod );
const ActorInit doorlock240Init = { "doorlock240", ActorInit::doorlock, &lockfaceSprite, 400, 884, 2, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2 * 16 + 2 * 192 - 16), (8.25 * 16 + 4 * 192 - 16)
DoorlockMethod doorlock240BehaviourMethod;
AnkhActor doorlock240( doorlock240Init, &doorlock240BehaviourMethod );
const ActorInit door240Init = { "door240", ActorInit::horzdoor, &horzdoorSprite, 464, 844, 2, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 2 * 192 - 16), (5.75 * 16 + 4 * 192 - 16)
HorzdoorMethod door240BehaviourMethod;
AnkhActor door240( door240Init, &door240BehaviourMethod );
const ActorInit doorlock241Init = { "doorlock241", ActorInit::doorlock, &itemSprite, 476, 830, 2, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.75 * 16 + 2 * 192 - 16), (4.875 * 16 + 4 * 192 - 16)
DoorlockMethod doorlock241BehaviourMethod(door360);
AnkhActor doorlock241( doorlock241Init, &doorlock241BehaviourMethod );
const ActorInit doorlock250Init = { "doorlock250", ActorInit::doorlock, &lockfaceSprite, 496, 1076, 2, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8 * 16 + 2 * 192 - 16), (8.25 * 16 + 5 * 192 - 16)
DoorlockMethod doorlock250BehaviourMethod(door350);
AnkhActor doorlock250( doorlock250Init, &doorlock250BehaviourMethod );
const ActorInit door250Init = { "door250", ActorInit::horzdoor, &horzdoorSprite, 432, 1036, 2, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4 * 16 + 2 * 192 - 16), (5.75 * 16 + 5 * 192 - 16)
HorzdoorMethod door250BehaviourMethod;
AnkhActor door250( door250Init, &door250BehaviourMethod );
const ActorInit key251Init = { "key251", ActorInit::key, &keySprite, 442, 1018, 2, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.625 * 16 + 2 * 192 - 16), (4.625 * 16 + 5 * 192 - 16)
KeyMethod key251BehaviourMethod(door140);
AnkhActor key251( key251Init, &key251BehaviourMethod );
const ActorInit doorlock260Init = { "doorlock260", ActorInit::doorlock, &lockfaceSprite, 544, 1268, 2, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11 * 16 + 2 * 192 - 16), (8.25 * 16 + 6 * 192 - 16)
DoorlockMethod doorlock260BehaviourMethod;
AnkhActor doorlock260( doorlock260Init, &doorlock260BehaviourMethod );
const ActorInit door260Init = { "door260", ActorInit::vertdoor, &vertdoorSprite, 458, 1198, 2, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.625 * 16 + 2 * 192 - 16), (3.875 * 16 + 6 * 192 - 16)
VertdoorMethod door260BehaviourMethod;
AnkhActor door260( door260Init, &door260BehaviourMethod );
const ActorInit doorlock261Init = { "doorlock261", ActorInit::doorlock, &itemSprite, 442, 1214, 2, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.625 * 16 + 2 * 192 - 16), (4.875 * 16 + 6 * 192 - 16)
DoorlockMethod doorlock261BehaviourMethod(door240);
AnkhActor doorlock261( doorlock261Init, &doorlock261BehaviourMethod );
const ActorInit shooter300Init = { "shooter300", ActorInit::shooter, &boxSprite, 600, 36, 3, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.5 * 16 + 3 * 192 - 16), (3.25 * 16 + 0 * 192 - 16)
ShooterMethod shooter300BehaviourMethod( 1,0, 60 );
AnkhActor shooter300( shooter300Init, &shooter300BehaviourMethod );
const ActorInit leftdoor300Init = { "leftdoor300", ActorInit::vertdoor, &vertdoorSprite, 628, 24, 3, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.25 * 16 + 3 * 192 - 16), (2.5 * 16 + 0 * 192 - 16)
VertdoorMethod leftdoor300BehaviourMethod;
AnkhActor leftdoor300( leftdoor300Init, &leftdoor300BehaviourMethod );
const ActorInit rightdoor300Init = { "rightdoor300", ActorInit::vertdoor, &vertdoorSprite, 708, 24, 3, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.25 * 16 + 3 * 192 - 16), (2.5 * 16 + 0 * 192 - 16)
VertdoorMethod rightdoor300BehaviourMethod;
AnkhActor rightdoor300( rightdoor300Init, &rightdoor300BehaviourMethod );
const ActorInit face300Init = { "face300", ActorInit::doorlock, &lockfaceSprite, 732, 36, 3, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (10.75 * 16 + 3 * 192 - 16), (3.25 * 16 + 0 * 192 - 16)
DoorlockMethod face300BehaviourMethod( entrydoor300 );
AnkhActor face300( face300Init, &face300BehaviourMethod );
const ActorInit musicbox301Init = { "musicbox301", ActorInit::musicbox, &musicboxSprite, 668, 96, 3, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.75 * 16 + 3 * 192 - 16), (7 * 16 + 0 * 192 - 16)
MusicboxMethod musicbox301BehaviourMethod( piano461, BehaviourMethod::DOWN, BehaviourMethod::SHOOT );
AnkhActor musicbox301( musicbox301Init, &musicbox301BehaviourMethod );
const ActorInit entrydoor300Init = { "entrydoor300", ActorInit::horzdoor, &horzdoorSprite, 656, 160, 3, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 3 * 192 - 16), (11 * 16 + 0 * 192 - 16)
HorzdoorMethod entrydoor300BehaviourMethod;
AnkhActor entrydoor300( entrydoor300Init, &entrydoor300BehaviourMethod );
const ActorInit lock300Init = { "lock300", ActorInit::doorlock, &itemSprite, 602, 80, 3, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.625 * 16 + 3 * 192 - 16), (6 * 16 + 0 * 192 - 16)
DoorlockMethod lock300BehaviourMethod( leftdoor300 );
AnkhActor lock300( lock300Init, &lock300BehaviourMethod );
const ActorInit lock301Init = { "lock301", ActorInit::doorlock, &itemSprite, 742, 80, 3, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.375 * 16 + 3 * 192 - 16), (6 * 16 + 0 * 192 - 16)
DoorlockMethod lock301BehaviourMethod( rightdoor300 );
AnkhActor lock301( lock301Init, &lock301BehaviourMethod );
const ActorInit treasure310Init = { "treasure310", ActorInit::treasure, &treasureSprite, 664, 284, 3, 1, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.5 * 16 + 3 * 192 - 16), (6.75 * 16 + 1 * 192 - 16)
TreasureMethod treasure310BehaviourMethod( 10 );
AnkhActor treasure310( treasure310Init, &treasure310BehaviourMethod );
const ActorInit treasure320Init = { "treasure320", ActorInit::treasure, &treasureSprite, 732, 514, 3, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (10.75 * 16 + 3 * 192 - 16), (9.125 * 16 + 2 * 192 - 16)
TreasureMethod treasure320BehaviourMethod( 10 );
AnkhActor treasure320( treasure320Init, &treasure320BehaviourMethod );
const ActorInit benign320Init = { "benign320", ActorInit::benign, &benignSprite, 690, 528, 3, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8.125 * 16 + 3 * 192 - 16), (10 * 16 + 2 * 192 - 16)
BenignMethod benign320BehaviourMethod( 2, 0 );
AnkhActor benign320( benign320Init, &benign320BehaviourMethod );
const ActorInit benign321Init = { "benign321", ActorInit::benign, &benignSprite, 628, 518, 3, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.25 * 16 + 3 * 192 - 16), (9.375 * 16 + 2 * 192 - 16)
BenignMethod benign321BehaviourMethod( 2, 0 );
AnkhActor benign321( benign321Init, &benign321BehaviourMethod );
const ActorInit door320Init = { "door320", ActorInit::horzdoor, &horzdoorSprite, 656, 508, 3, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 3 * 192 - 16), (8.75 * 16 + 2 * 192 - 16)
HorzdoorMethod door320BehaviourMethod;
AnkhActor door320( door320Init, &door320BehaviourMethod );
const ActorInit illumina320Init = { "illumina320", ActorInit::illumina, &illuminaSprite, 592, 552, 3, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2 * 16 + 3 * 192 - 16), (11.5 * 16 + 2 * 192 - 16)
IlluminaMethod illumina320BehaviourMethod( ActorInit::treasure );
AnkhActor illumina320( illumina320Init, &illumina320BehaviourMethod );
const ActorInit enemy330Init = { "enemy330", ActorInit::badguy, &badguySprite, 646, 620, 3, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.375 * 16 + 3 * 192 - 16), (3.75 * 16 + 3 * 192 - 16)
BadguyMethod enemy330BehaviourMethod;
AnkhActor enemy330( enemy330Init, &enemy330BehaviourMethod );
const ActorInit enemy331Init = { "enemy331", ActorInit::badguy, &badguySprite, 710, 610, 3, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.375 * 16 + 3 * 192 - 16), (3.125 * 16 + 3 * 192 - 16)
BadguyMethod enemy331BehaviourMethod;
AnkhActor enemy331( enemy331Init, &enemy331BehaviourMethod );
const ActorInit enemy332Init = { "enemy332", ActorInit::badguy, &badguySprite, 692, 724, 3, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8.25 * 16 + 3 * 192 - 16), (10.25 * 16 + 3 * 192 - 16)
BadguyMethod enemy332BehaviourMethod;
AnkhActor enemy332( enemy332Init, &enemy332BehaviourMethod );
const ActorInit doorlock340Init = { "doorlock340", ActorInit::doorlock, &lockfaceSprite, 688, 884, 3, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8 * 16 + 3 * 192 - 16), (8.25 * 16 + 4 * 192 - 16)
DoorlockMethod doorlock340BehaviourMethod;
AnkhActor doorlock340( doorlock340Init, &doorlock340BehaviourMethod );
const ActorInit door340Init = { "door340", ActorInit::horzdoor, &horzdoorSprite, 624, 844, 3, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4 * 16 + 3 * 192 - 16), (5.75 * 16 + 4 * 192 - 16)
HorzdoorMethod door340BehaviourMethod;
AnkhActor door340( door340Init, &door340BehaviourMethod );
const ActorInit doorlock341Init = { "doorlock341", ActorInit::doorlock, &itemSprite, 638, 830, 3, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.875 * 16 + 3 * 192 - 16), (4.875 * 16 + 4 * 192 - 16)
DoorlockMethod doorlock341BehaviourMethod( door250 );
AnkhActor doorlock341( doorlock341Init, &doorlock341BehaviourMethod );
const ActorInit doorlock350Init = { "doorlock350", ActorInit::doorlock, &lockfaceSprite, 640, 1012, 3, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5 * 16 + 3 * 192 - 16), (4.25 * 16 + 5 * 192 - 16)
DoorlockMethod doorlock350BehaviourMethod;
AnkhActor doorlock350( doorlock350Init, &doorlock350BehaviourMethod );
const ActorInit door350Init = { "door350", ActorInit::vertdoor, &vertdoorSprite, 650, 1038, 3, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.625 * 16 + 3 * 192 - 16), (5.875 * 16 + 5 * 192 - 16)
VertdoorMethod door350BehaviourMethod;
AnkhActor door350( door350Init, &door350BehaviourMethod );
const ActorInit doorlock351Init = { "doorlock351", ActorInit::doorlock, &itemSprite, 636, 1054, 3, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.75 * 16 + 3 * 192 - 16), (6.875 * 16 + 5 * 192 - 16)
DoorlockMethod doorlock351BehaviourMethod(door150);
AnkhActor doorlock351( doorlock351Init, &doorlock351BehaviourMethod );
const ActorInit doorlock360Init = { "doorlock360", ActorInit::doorlock, &lockfaceSprite, 608, 1220, 3, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (3 * 16 + 3 * 192 - 16), (5.25 * 16 + 6 * 192 - 16)
DoorlockMethod doorlock360BehaviourMethod;
AnkhActor doorlock360( doorlock360Init, &doorlock360BehaviourMethod );
const ActorInit door360Init = { "door360", ActorInit::vertdoor, &vertdoorSprite, 686, 1198, 3, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (7.875 * 16 + 3 * 192 - 16), (3.875 * 16 + 6 * 192 - 16)
VertdoorMethod door360BehaviourMethod;
AnkhActor door360( door360Init, &door360BehaviourMethod );
const ActorInit doorlock361Init = { "doorlock361", ActorInit::doorlock, &itemSprite, 700, 1214, 3, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8.75 * 16 + 3 * 192 - 16), (4.875 * 16 + 6 * 192 - 16)
DoorlockMethod doorlock361BehaviourMethod( door340 );
AnkhActor doorlock361( doorlock361Init, &doorlock361BehaviourMethod );
const ActorInit suck400Init = { "suck400", ActorInit::suck, &suckSprite, 852, 168, 4, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.25 * 16 + 4 * 192 - 16), (11.5 * 16 + 0 * 192 - 16)
SuckMethod suck400BehaviourMethod(1, -4 /*damage*/);
AnkhActor suck400( suck400Init, &suck400BehaviourMethod );
const ActorInit suck401Init = { "suck401", ActorInit::suck, &suckSprite, 852, 18, 4, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.25 * 16 + 4 * 192 - 16), (2.125 * 16 + 0 * 192 - 16)
SuckMethod suck401BehaviourMethod(-1, -4 /*damage*/);
AnkhActor suck401( suck401Init, &suck401BehaviourMethod );
const ActorInit enemy430Init = { "enemy430", ActorInit::badguy, &badguySprite, 890, 714, 4, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8.625 * 16 + 4 * 192 - 16), (9.625 * 16 + 3 * 192 - 16)
BadguyMethod enemy430BehaviourMethod;
AnkhActor enemy430( enemy430Init, &enemy430BehaviourMethod );
const ActorInit enemy431Init = { "enemy431", ActorInit::badguy, &badguySprite, 842, 712, 4, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.625 * 16 + 4 * 192 - 16), (9.5 * 16 + 3 * 192 - 16)
BadguyMethod enemy431BehaviourMethod;
AnkhActor enemy431( enemy431Init, &enemy431BehaviourMethod );
const ActorInit enemy432Init = { "enemy432", ActorInit::badguy, &badguySprite, 864, 688, 4, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (7 * 16 + 4 * 192 - 16), (8 * 16 + 3 * 192 - 16)
BadguyMethod enemy432BehaviourMethod;
AnkhActor enemy432( enemy432Init, &enemy432BehaviourMethod );
const ActorInit enemy440Init = { "enemy440", ActorInit::badguy, &badguySprite, 914, 918, 4, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (10.125 * 16 + 4 * 192 - 16), (10.375 * 16 + 4 * 192 - 16)
BadguyMethod enemy440BehaviourMethod;
AnkhActor enemy440( enemy440Init, &enemy440BehaviourMethod );
const ActorInit enemy441Init = { "enemy441", ActorInit::badguy, &badguySprite, 822, 790, 4, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.375 * 16 + 4 * 192 - 16), (2.375 * 16 + 4 * 192 - 16)
BadguyMethod enemy441BehaviourMethod;
AnkhActor enemy441( enemy441Init, &enemy441BehaviourMethod );
const ActorInit enemy442Init = { "enemy442", ActorInit::badguy, &badguySprite, 930, 834, 4, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.125 * 16 + 4 * 192 - 16), (5.125 * 16 + 4 * 192 - 16)
BadguyMethod enemy442BehaviourMethod;
AnkhActor enemy442( enemy442Init, &enemy442BehaviourMethod );
const ActorInit enemy443Init = { "enemy443", ActorInit::badguy, &badguySprite, 786, 820, 4, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.125 * 16 + 4 * 192 - 16), (4.25 * 16 + 4 * 192 - 16)
BadguyMethod enemy443BehaviourMethod;
AnkhActor enemy443( enemy443Init, &enemy443BehaviourMethod );
const ActorInit enemy450Init = { "enemy450", ActorInit::badguy, &badguySprite, 900, 1122, 4, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.25 * 16 + 4 * 192 - 16), (11.125 * 16 + 5 * 192 - 16)
BadguyMethod enemy450BehaviourMethod;
AnkhActor enemy450( enemy450Init, &enemy450BehaviourMethod );
const ActorInit enemy451Init = { "enemy451", ActorInit::badguy, &badguySprite, 900, 980, 4, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.25 * 16 + 4 * 192 - 16), (2.25 * 16 + 5 * 192 - 16)
BadguyMethod enemy451BehaviourMethod;
AnkhActor enemy451( enemy451Init, &enemy451BehaviourMethod );
const ActorInit enemy452Init = { "enemy452", ActorInit::badguy, &badguySprite, 804, 996, 4, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (3.25 * 16 + 4 * 192 - 16), (3.25 * 16 + 5 * 192 - 16)
BadguyMethod enemy452BehaviourMethod;
AnkhActor enemy452( enemy452Init, &enemy452BehaviourMethod );
const ActorInit enemy453Init = { "enemy453", ActorInit::badguy, &badguySprite, 924, 1010, 4, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (10.75 * 16 + 4 * 192 - 16), (4.125 * 16 + 5 * 192 - 16)
BadguyMethod enemy453BehaviourMethod;
AnkhActor enemy453( enemy453Init, &enemy453BehaviourMethod );
const ActorInit door460Init = { "door460", ActorInit::horzdoor, &horzdoorSprite, 848, 1296, 4, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 4 * 192 - 16), (10 * 16 + 6 * 192 - 16)
HorzdoorMethod door460BehaviourMethod;
AnkhActor door460( door460Init, &door460BehaviourMethod );
const ActorInit door461Init = { "door461", ActorInit::horzdoor, &horzdoorSprite, 848, 1264, 4, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 4 * 192 - 16), (8 * 16 + 6 * 192 - 16)
HorzdoorMethod door461BehaviourMethod(false,-1);
AnkhActor door461( door461Init, &door461BehaviourMethod );
const ActorInit doorlock460Init = { "doorlock460", ActorInit::tonegen, &locklistenSprite, 832, 1252, 4, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5 * 16 + 4 * 192 - 16), (7.25 * 16 + 6 * 192 - 16)
TonegenMethod doorlock460BehaviourMethod;
AnkhActor doorlock460( doorlock460Init, &doorlock460BehaviourMethod );
const gba::SpriteAsset piano461Sprite = { gba::Vec<s32>( 1, 1 ), nobitmap };
const ActorInit piano461Init = { "piano461", ActorInit::statetrigger, &piano461Sprite, 832, 1252, 4, 6, 4, 4, ACTOR_COLLISION_TRIGGER  }; // (5 * 16 + 4 * 192 - 16), (7.25 * 16 + 6 * 192 - 16)
StatetriggerMethod piano461BehaviourMethod( door460, door461 );
AnkhActor piano461( piano461Init, &piano461BehaviourMethod );
const ActorInit treasure470Init = { "treasure470", ActorInit::treasure, &treasureSprite, 910, 1432, 4, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.875 * 16 + 4 * 192 - 16), (6.5 * 16 + 7 * 192 - 16)
TreasureMethod treasure470BehaviourMethod( 10 );
AnkhActor treasure470( treasure470Init, &treasure470BehaviourMethod );
const ActorInit treasure471Init = { "treasure471", ActorInit::treasure, &treasureSprite, 910, 1424, 4, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.875 * 16 + 4 * 192 - 16), (6 * 16 + 7 * 192 - 16)
TreasureMethod treasure471BehaviourMethod( 10 );
AnkhActor treasure471( treasure471Init, &treasure471BehaviourMethod );
const ActorInit treasure472Init = { "treasure472", ActorInit::treasure, &treasureSprite, 910, 1416, 4, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.875 * 16 + 4 * 192 - 16), (5.5 * 16 + 7 * 192 - 16)
TreasureMethod treasure472BehaviourMethod( 10 );
AnkhActor treasure472( treasure472Init, &treasure472BehaviourMethod );
const ActorInit enemy500Init = { "enemy500", ActorInit::badguy, &badguySprite, 1060, 150, 5, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (7.25 * 16 + 5 * 192 - 16), (10.375 * 16 + 0 * 192 - 16)
BadguyMethod enemy500BehaviourMethod;
AnkhActor enemy500( enemy500Init, &enemy500BehaviourMethod );
const ActorInit enemy501Init = { "enemy501", ActorInit::badguy, &badguySprite, 1060, 20, 5, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (7.25 * 16 + 5 * 192 - 16), (2.25 * 16 + 0 * 192 - 16)
BadguyMethod enemy501BehaviourMethod;
AnkhActor enemy501( enemy501Init, &enemy501BehaviourMethod );
const ActorInit up540Init = { "up540", ActorInit::trigger, &itemSprite, 1126, 924, 5, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.375 * 16 + 5 * 192 - 16), (10.75 * 16 + 4 * 192 - 16)
TriggerMethod up540BehaviourMethod( player540, BehaviourMethod::UP, BehaviourMethod::TOUCH );
AnkhActor up540( up540Init, &up540BehaviourMethod );
const ActorInit down541Init = { "down541", ActorInit::trigger, &itemSprite, 1090, 924, 5, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.125 * 16 + 5 * 192 - 16), (10.75 * 16 + 4 * 192 - 16)
TriggerMethod down541BehaviourMethod( player540, BehaviourMethod::DOWN, BehaviourMethod::TOUCH );
AnkhActor down541( down541Init, &down541BehaviourMethod );
const ActorInit stop542Init = { "stop542", ActorInit::trigger, &itemSprite, 1108, 938, 5, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (10.25 * 16 + 5 * 192 - 16), (11.625 * 16 + 4 * 192 - 16)
TriggerMethod stop542BehaviourMethod( player540, BehaviourMethod::STOP, BehaviourMethod::TOUCH );
AnkhActor stop542( stop542Init, &stop542BehaviourMethod );
const ActorInit shoot543Init = { "shoot543", ActorInit::trigger, &itemSprite, 1126, 918, 5, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.375 * 16 + 5 * 192 - 16), (10.375 * 16 + 4 * 192 - 16)
TriggerMethod shoot543BehaviourMethod( player540, BehaviourMethod::SHOOT, BehaviourMethod::TOUCH );
AnkhActor shoot543( shoot543Init, &shoot543BehaviourMethod );
const ActorInit face544Init = { "face544", ActorInit::doorlock, &lockfaceSprite, 1054, 860, 5, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.875 * 16 + 5 * 192 - 16), (6.75 * 16 + 4 * 192 - 16)
DoorlockMethod face544BehaviourMethod( door650 );
AnkhActor face544( face544Init, &face544BehaviourMethod );
const ActorInit player540Init = { "player540", ActorInit::playerdrone, &otherSprite, 976, 784, 5, 4, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2 * 16 + 5 * 192 - 16), (2 * 16 + 4 * 192 - 16)
PlayerdroneMethod player540BehaviourMethod;
AnkhActor player540( player540Init, &player540BehaviourMethod );
const gba::SpriteAsset triggerpad540Sprite = { gba::Vec<s32>( 1230 - 1152, 880 - 847 ), nobitmap };
const ActorInit triggerpad540Init = { "triggerpad540", ActorInit::triggerpad, &triggerpad540Sprite, 1152, 847, 5, 4, 0, 0, ACTOR_COLLISION_TRIGGER  }; // (1152px), (847px)
TriggerpadMethod triggerpad540BehaviourMethod( player540, BehaviourMethod::EXIT );
AnkhActor triggerpad540( triggerpad540Init, &triggerpad540BehaviourMethod );
const ActorInit bonus550Init = { "bonus550", ActorInit::candy, &bonusSprite, 1056, 1068, 5, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (7 * 16 + 5 * 192 - 16), (7.75 * 16 + 5 * 192 - 16)
CandyMethod bonus550BehaviourMethod;
AnkhActor bonus550( bonus550Init, &bonus550BehaviourMethod );
const ActorInit down550Init = { "down550", ActorInit::trigger, &itemSprite, 1088, 1116, 5, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9 * 16 + 5 * 192 - 16), (10.75 * 16 + 5 * 192 - 16)
TriggerMethod down550BehaviourMethod( player550, BehaviourMethod::DOWN, BehaviourMethod::TOUCH );
AnkhActor down550( down550Init, &down550BehaviourMethod );
const ActorInit up551Init = { "up551", ActorInit::trigger, &itemSprite, 1128, 1116, 5, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.5 * 16 + 5 * 192 - 16), (10.75 * 16 + 5 * 192 - 16)
TriggerMethod up551BehaviourMethod( player550, BehaviourMethod::UP, BehaviourMethod::TOUCH );
AnkhActor up551( up551Init, &up551BehaviourMethod );
const ActorInit shoot552Init = { "shoot552", ActorInit::trigger, &itemSprite, 1128, 1110, 5, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.5 * 16 + 5 * 192 - 16), (10.375 * 16 + 5 * 192 - 16)
TriggerMethod shoot552BehaviourMethod( player550, BehaviourMethod::SHOOT, BehaviourMethod::TOUCH );
AnkhActor shoot552( shoot552Init, &shoot552BehaviourMethod );
const ActorInit face550Init = { "face550", ActorInit::doorlock, &lockfaceSprite, 1054, 1052, 5, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.875 * 16 + 5 * 192 - 16), (6.75 * 16 + 5 * 192 - 16)
DoorlockMethod face550BehaviourMethod( door660 );
AnkhActor face550( face550Init, &face550BehaviourMethod );
const ActorInit player550Init = { "player550", ActorInit::playerdrone, &otherSprite, 976, 978, 5, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2 * 16 + 5 * 192 - 16), (2.125 * 16 + 5 * 192 - 16)
PlayerdroneMethod player550BehaviourMethod;
AnkhActor player550( player550Init, &player550BehaviourMethod );
const gba::SpriteAsset triggerpad550Sprite = { gba::Vec<s32>( 1230 - 1152, 1072 - 1039 ), nobitmap };
const ActorInit triggerpad550Init = { "triggerpad550", ActorInit::triggerpad, &triggerpad550Sprite, 1152, 1039, 5, 5, 0, 0, ACTOR_COLLISION_TRIGGER  }; // (1152px), (1039px)
TriggerpadMethod triggerpad550BehaviourMethod( player550, BehaviourMethod::EXIT );
AnkhActor triggerpad550( triggerpad550Init, &triggerpad550BehaviourMethod );
const ActorInit down560Init = { "down560", ActorInit::trigger, &itemSprite, 1088, 1308, 5, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9 * 16 + 5 * 192 - 16), (10.75 * 16 + 6 * 192 - 16)
TriggerMethod down560BehaviourMethod( player560, BehaviourMethod::DOWN, BehaviourMethod::TOUCH );
AnkhActor down560( down560Init, &down560BehaviourMethod );
const ActorInit shoot561Init = { "shoot561", ActorInit::trigger, &itemSprite, 1128, 1300, 5, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.5 * 16 + 5 * 192 - 16), (10.25 * 16 + 6 * 192 - 16)
TriggerMethod shoot561BehaviourMethod( player560, BehaviourMethod::SHOOT, BehaviourMethod::TOUCH );
AnkhActor shoot561( shoot561Init, &shoot561BehaviourMethod );
const ActorInit face560Init = { "face560", ActorInit::doorlock, &lockfaceSprite, 1054, 1244, 5, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.875 * 16 + 5 * 192 - 16), (6.75 * 16 + 6 * 192 - 16)
DoorlockMethod face560BehaviourMethod( door670 );
AnkhActor face560( face560Init, &face560BehaviourMethod );
const ActorInit player560Init = { "player560", ActorInit::playerdrone, &otherSprite, 976, 1168, 5, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2 * 16 + 5 * 192 - 16), (2 * 16 + 6 * 192 - 16)
PlayerdroneMethod player560BehaviourMethod;
AnkhActor player560( player560Init, &player560BehaviourMethod );
const gba::SpriteAsset triggerpad560Sprite = { gba::Vec<s32>( 1230 - 1152, 1264 - 1231 ), nobitmap };
const ActorInit triggerpad560Init = { "triggerpad560", ActorInit::triggerpad, &triggerpad560Sprite, 1152, 1231, 5, 6, 0, 0, ACTOR_COLLISION_TRIGGER  }; // (1152px), (1231px)
TriggerpadMethod triggerpad560BehaviourMethod( player560, BehaviourMethod::EXIT );
AnkhActor triggerpad560( triggerpad560Init, &triggerpad560BehaviourMethod );
const ActorInit trigger570Init = { "trigger570", ActorInit::doorlock, &itemSprite, 1128, 1478, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.5 * 16 + 5 * 192 - 16), (9.375 * 16 + 7 * 192 - 16)
DoorlockMethod trigger570BehaviourMethod( door570, BehaviourMethod::TRIGGER, BehaviourMethod::TOUCH );
AnkhActor trigger570( trigger570Init, &trigger570BehaviourMethod );
const ActorInit trigger571Init = { "trigger571", ActorInit::doorlock, &itemSprite, 1126, 1488, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.375 * 16 + 5 * 192 - 16), (10 * 16 + 7 * 192 - 16)
DoorlockMethod trigger571BehaviourMethod( door571, BehaviourMethod::TRIGGER, BehaviourMethod::TOUCH );
AnkhActor trigger571( trigger571Init, &trigger571BehaviourMethod );
const ActorInit trigger572Init = { "trigger572", ActorInit::doorlock, &itemSprite, 1128, 1492, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.5 * 16 + 5 * 192 - 16), (10.25 * 16 + 7 * 192 - 16)
DoorlockMethod trigger572BehaviourMethod( door572, BehaviourMethod::TRIGGER, BehaviourMethod::TOUCH );
AnkhActor trigger572( trigger572Init, &trigger572BehaviourMethod );
const ActorInit trigger573Init = { "trigger573", ActorInit::doorlock, &itemSprite, 1126, 1498, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.375 * 16 + 5 * 192 - 16), (10.625 * 16 + 7 * 192 - 16)
DoorlockMethod trigger573BehaviourMethod( door573, BehaviourMethod::TRIGGER, BehaviourMethod::TOUCH );
AnkhActor trigger573( trigger573Init, &trigger573BehaviourMethod );
const ActorInit trigger574Init = { "trigger574", ActorInit::doorlock, &itemSprite, 1108, 1514, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (10.25 * 16 + 5 * 192 - 16), (11.625 * 16 + 7 * 192 - 16)
DoorlockMethod trigger574BehaviourMethod( door574, BehaviourMethod::TRIGGER, BehaviourMethod::TOUCH );
AnkhActor trigger574( trigger574Init, &trigger574BehaviourMethod );
const ActorInit trigger575Init = { "trigger575", ActorInit::doorlock, &itemSprite, 1090, 1496, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.125 * 16 + 5 * 192 - 16), (10.5 * 16 + 7 * 192 - 16)
DoorlockMethod trigger575BehaviourMethod( door575, BehaviourMethod::TRIGGER, BehaviourMethod::TOUCH );
AnkhActor trigger575( trigger575Init, &trigger575BehaviourMethod );
const ActorInit trigger576Init = { "trigger576", ActorInit::doorlock, &itemSprite, 1092, 1480, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.25 * 16 + 5 * 192 - 16), (9.5 * 16 + 7 * 192 - 16)
DoorlockMethod trigger576BehaviourMethod( door576, BehaviourMethod::TRIGGER, BehaviourMethod::TOUCH );
AnkhActor trigger576( trigger576Init, &trigger576BehaviourMethod );
const ActorInit trigger577Init = { "trigger577", ActorInit::doorlock, &itemSprite, 1088, 1476, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9 * 16 + 5 * 192 - 16), (9.25 * 16 + 7 * 192 - 16)
DoorlockMethod trigger577BehaviourMethod;
AnkhActor trigger577( trigger577Init, &trigger577BehaviourMethod );
const ActorInit face570Init = { "face570", ActorInit::doorlock, &lockfaceSprite, 1052, 1436, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.75 * 16 + 5 * 192 - 16), (6.75 * 16 + 7 * 192 - 16)
DoorlockMethod face570BehaviourMethod( door671 );
AnkhActor face570( face570Init, &face570BehaviourMethod );
const ActorInit enemy570Init = { "enemy570", ActorInit::badguy, &badguySprite, 978, 1362, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.125 * 16 + 5 * 192 - 16), (2.125 * 16 + 7 * 192 - 16)
BadguyMethod enemy570BehaviourMethod;
AnkhActor enemy570( enemy570Init, &enemy570BehaviourMethod );
const ActorInit door570Init = { "door570", ActorInit::vertdoor, &vertdoorSprite, 1034, 1424, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.625 * 16 + 5 * 192 - 16), (6 * 16 + 7 * 192 - 16)
VertdoorMethod door570BehaviourMethod;
AnkhActor door570( door570Init, &door570BehaviourMethod );
const ActorInit door571Init = { "door571", ActorInit::vertdoor, &vertdoorSprite, 1002, 1360, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (3.625 * 16 + 5 * 192 - 16), (2 * 16 + 7 * 192 - 16)
VertdoorMethod door571BehaviourMethod;
AnkhActor door571( door571Init, &door571BehaviourMethod );
const ActorInit door572Init = { "door572", ActorInit::vertdoor, &vertdoorSprite, 1002, 1424, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (3.625 * 16 + 5 * 192 - 16), (6 * 16 + 7 * 192 - 16)
VertdoorMethod door572BehaviourMethod;
AnkhActor door572( door572Init, &door572BehaviourMethod );
const ActorInit door573Init = { "door573", ActorInit::vertdoor, &vertdoorSprite, 1036, 1360, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.75 * 16 + 5 * 192 - 16), (2 * 16 + 7 * 192 - 16)
VertdoorMethod door573BehaviourMethod;
AnkhActor door573( door573Init, &door573BehaviourMethod );
const ActorInit door574Init = { "door574", ActorInit::horzdoor, &horzdoorSprite, 1008, 1392, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4 * 16 + 5 * 192 - 16), (4 * 16 + 7 * 192 - 16)
HorzdoorMethod door574BehaviourMethod;
AnkhActor door574( door574Init, &door574BehaviourMethod );
const ActorInit door575Init = { "door575", ActorInit::horzdoor, &horzdoorSprite, 1040, 1422, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 5 * 192 - 16), (5.875 * 16 + 7 * 192 - 16)
HorzdoorMethod door575BehaviourMethod;
AnkhActor door575( door575Init, &door575BehaviourMethod );
const ActorInit door576Init = { "door576", ActorInit::horzdoor, &horzdoorSprite, 1040, 1454, 5, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 5 * 192 - 16), (7.875 * 16 + 7 * 192 - 16)
HorzdoorMethod door576BehaviourMethod;
AnkhActor door576( door576Init, &door576BehaviourMethod );
const ActorInit door600Init = { "door600", ActorInit::vertdoor, &vertdoorSprite, 1226, 78, 6, 0, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.625 * 16 + 6 * 192 - 16), (5.875 * 16 + 0 * 192 - 16)
VertdoorMethod door600BehaviourMethod;
AnkhActor door600( door600Init, &door600BehaviourMethod );
const ActorInit door610Init = { "door610", ActorInit::horzdoor, &horzdoorSprite, 1232, 256, 6, 1, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 6 * 192 - 16), (5 * 16 + 1 * 192 - 16)
HorzdoorMethod door610BehaviourMethod;
AnkhActor door610( door610Init, &door610BehaviourMethod );
const ActorInit doorlock610Init = { "doorlock610", ActorInit::doorlock, &lockfaceSprite, 1186, 334, 6, 1, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (3.125 * 16 + 6 * 192 - 16), (9.875 * 16 + 1 * 192 - 16)
DoorlockMethod doorlock610BehaviourMethod( door600 );
AnkhActor doorlock610( doorlock610Init, &doorlock610BehaviourMethod );
const ActorInit musicbox610Init = { "musicbox610", ActorInit::musicbox, &musicboxSprite, 1184, 282, 6, 1, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (3 * 16 + 6 * 192 - 16), (6.625 * 16 + 1 * 192 - 16)
MusicboxMethod musicbox610BehaviourMethod( piano461, BehaviourMethod::RIGHT, BehaviourMethod::SHOOT );
AnkhActor musicbox610( musicbox610Init, &musicbox610BehaviourMethod );
const gba::SpriteAsset touchpad610Sprite = { gba::Vec<s32>( 93, 111 ), nobitmap };
const ActorInit touchpad610Init = { "touchpad610", ActorInit::touchpad, &touchpad610Sprite, 1200, 193, 6, 1, 0, 0, ACTOR_COLLISION_TRIGGER  }; // (1200px), (193px)
TouchpadMethod touchpad610BehaviourMethod( 12 );
AnkhActor touchpad610( touchpad610Init, &touchpad610BehaviourMethod );
const ActorInit door620Init = { "door620", ActorInit::vertdoor, &vertdoorSprite, 1274, 398, 6, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8.625 * 16 + 6 * 192 - 16), (1.875 * 16 + 2 * 192 - 16)
VertdoorMethod door620BehaviourMethod(false,-1,64);
AnkhActor door620( door620Init, &door620BehaviourMethod );
const ActorInit doorlock620Init = { "doorlock620", ActorInit::doorlock, &lockfaceSprite, 1182, 554, 6, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.875 * 16 + 6 * 192 - 16), (11.625 * 16 + 2 * 192 - 16)
DoorlockMethod doorlock620BehaviourMethod( door620 );
AnkhActor doorlock620( doorlock620Init, &doorlock620BehaviourMethod );
const ActorInit key620Init = { "key620", ActorInit::key, &keySprite, 1182, 482, 6, 2, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.875 * 16 + 6 * 192 - 16), (7.125 * 16 + 2 * 192 - 16)
KeyMethod key620BehaviourMethod( door610 );
AnkhActor key620( key620Init, &key620BehaviourMethod );
const ActorInit candy630Init = { "candy630", ActorInit::candy, &itemSprite, 1182, 644, 6, 3, 4, 8 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.875 * 16 + 6 * 192 - 16), (5.25 * 16 + 3 * 192 - 16)
CandyMethod candy630BehaviourMethod;
AnkhActor candy630( candy630Init, &candy630BehaviourMethod );
const ActorInit candy631Init = { "candy631", ActorInit::candy, &itemSprite, 1168, 648, 6, 3, 4, 8 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2 * 16 + 6 * 192 - 16), (5.5 * 16 + 3 * 192 - 16)
CandyMethod candy631BehaviourMethod;
AnkhActor candy631( candy631Init, &candy631BehaviourMethod );
const ActorInit candy632Init = { "candy632", ActorInit::candy, &itemSprite, 1168, 640, 6, 3, 4, 8 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2 * 16 + 6 * 192 - 16), (5 * 16 + 3 * 192 - 16)
CandyMethod candy632BehaviourMethod;
AnkhActor candy632( candy632Init, &candy632BehaviourMethod );
const ActorInit candy633Init = { "candy633", ActorInit::candy, &itemSprite, 1182, 640, 6, 3, 4, 8 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.875 * 16 + 6 * 192 - 16), (5 * 16 + 3 * 192 - 16)
CandyMethod candy633BehaviourMethod;
AnkhActor candy633( candy633Init, &candy633BehaviourMethod );
const ActorInit candy634Init = { "candy634", ActorInit::candy, &itemSprite, 1174, 648, 6, 3, 4, 8 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.375 * 16 + 6 * 192 - 16), (5.5 * 16 + 3 * 192 - 16)
CandyMethod candy634BehaviourMethod;
AnkhActor candy634( candy634Init, &candy634BehaviourMethod );
const ActorInit candy635Init = { "candy635", ActorInit::candy, &itemSprite, 1168, 644, 6, 3, 4, 8 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2 * 16 + 6 * 192 - 16), (5.25 * 16 + 3 * 192 - 16)
CandyMethod candy635BehaviourMethod;
AnkhActor candy635( candy635Init, &candy635BehaviourMethod );
const ActorInit candy636Init = { "candy636", ActorInit::candy, &itemSprite, 1174, 640, 6, 3, 4, 8 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.375 * 16 + 6 * 192 - 16), (5 * 16 + 3 * 192 - 16)
CandyMethod candy636BehaviourMethod;
AnkhActor candy636( candy636Init, &candy636BehaviourMethod );
const ActorInit candy637Init = { "candy637", ActorInit::candy, &itemSprite, 1182, 648, 6, 3, 4, 8 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.875 * 16 + 6 * 192 - 16), (5.5 * 16 + 3 * 192 - 16)
CandyMethod candy637BehaviourMethod;
AnkhActor candy637( candy637Init, &candy637BehaviourMethod );
const ActorInit doorlock630Init = { "doorlock630", ActorInit::doorlock, &lockSprite, 1156, 694, 6, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (1.25 * 16 + 6 * 192 - 16), (8.375 * 16 + 3 * 192 - 16)
DoorlockMethod doorlock630BehaviourMethod( door631 );
AnkhActor doorlock630( doorlock630Init, &doorlock630BehaviourMethod );
const ActorInit door630Init = { "door630", ActorInit::vertdoor, &vertdoorSprite, 1280, 656, 6, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9 * 16 + 6 * 192 - 16), (6 * 16 + 3 * 192 - 16)
VertdoorMethod door630BehaviourMethod;
AnkhActor door630( door630Init, &door630BehaviourMethod );
const ActorInit doorlock631Init = { "doorlock631", ActorInit::doorlock, &lockSprite, 1284, 692, 6, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.25 * 16 + 6 * 192 - 16), (8.25 * 16 + 3 * 192 - 16)
DoorlockMethod doorlock631BehaviourMethod( door630 );
AnkhActor doorlock631( doorlock631Init, &doorlock631BehaviourMethod );
const ActorInit door631Init = { "door631", ActorInit::horzdoor, &horzdoorSprite, 1232, 704, 6, 3, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6 * 16 + 6 * 192 - 16), (9 * 16 + 3 * 192 - 16)
HorzdoorMethod door631BehaviourMethod;
AnkhActor door631( door631Init, &door631BehaviourMethod );
const gba::SpriteAsset panpad630Sprite = { gba::Vec<s32>( 1134 - 1086, 1519 - 1423 ), nobitmap };
const ActorInit panpad630Init = { "panpad630", ActorInit::interppad, &panpad630Sprite, 1086, 1423, 6, 3, 0, 0, ACTOR_COLLISION_TRIGGER  }; // (1086px), (1423px)
InterppadMethod panpad630BehaviourMethod( -28,/*-*/ 0,   0,/*-*/ 0);
AnkhActor panpad630( panpad630Init, &panpad630BehaviourMethod );
const ActorInit enemy650Init = { "enemy650", ActorInit::badguy, &badguySprite, 1196, 1052, 6, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (3.75 * 16 + 6 * 192 - 16), (6.75 * 16 + 5 * 192 - 16)
BadguyMethod enemy650BehaviourMethod;
AnkhActor enemy650( enemy650Init, &enemy650BehaviourMethod );
const ActorInit enemy651Init = { "enemy651", ActorInit::badguy, &badguySprite, 1270, 1060, 6, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8.375 * 16 + 6 * 192 - 16), (7.25 * 16 + 5 * 192 - 16)
BadguyMethod enemy651BehaviourMethod;
AnkhActor enemy651( enemy651Init, &enemy651BehaviourMethod );
const ActorInit door650Init = { "door650", ActorInit::vertdoor, &vertdoorSprite, 1226, 1040, 6, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.625 * 16 + 6 * 192 - 16), (6 * 16 + 5 * 192 - 16)
VertdoorMethod door650BehaviourMethod;
AnkhActor door650( door650Init, &door650BehaviourMethod );
const ActorInit door651Init = { "door651", ActorInit::vertdoor, &vertdoorSprite, 1264, 1040, 6, 5, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8 * 16 + 6 * 192 - 16), (6 * 16 + 5 * 192 - 16)
VertdoorMethod door651BehaviourMethod;
AnkhActor door651( door651Init, &door651BehaviourMethod );
const ActorInit enemy660Init = { "enemy660", ActorInit::badguy, &badguySprite, 1196, 1240, 6, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (3.75 * 16 + 6 * 192 - 16), (6.5 * 16 + 6 * 192 - 16)
BadguyMethod enemy660BehaviourMethod;
AnkhActor enemy660( enemy660Init, &enemy660BehaviourMethod );
const ActorInit enemy661Init = { "enemy661", ActorInit::badguy, &badguySprite, 1292, 1244, 6, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.75 * 16 + 6 * 192 - 16), (6.75 * 16 + 6 * 192 - 16)
BadguyMethod enemy661BehaviourMethod;
AnkhActor enemy661( enemy661Init, &enemy661BehaviourMethod );
const ActorInit door660Init = { "door660", ActorInit::vertdoor, &vertdoorSprite, 1226, 1232, 6, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.625 * 16 + 6 * 192 - 16), (6 * 16 + 6 * 192 - 16)
VertdoorMethod door660BehaviourMethod;
AnkhActor door660( door660Init, &door660BehaviourMethod );
const ActorInit door661Init = { "door661", ActorInit::vertdoor, &vertdoorSprite, 1264, 1232, 6, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8 * 16 + 6 * 192 - 16), (6 * 16 + 6 * 192 - 16)
VertdoorMethod door661BehaviourMethod;
AnkhActor door661( door661Init, &door661BehaviourMethod );
const ActorInit enemy670Init = { "enemy670", ActorInit::badguy, &badguySprite, 1180, 1432, 6, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.75 * 16 + 6 * 192 - 16), (6.5 * 16 + 7 * 192 - 16)
BadguyMethod enemy670BehaviourMethod;
AnkhActor enemy670( enemy670Init, &enemy670BehaviourMethod );
const ActorInit enemy671Init = { "enemy671", ActorInit::badguy, &badguySprite, 1284, 1438, 6, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.25 * 16 + 6 * 192 - 16), (6.875 * 16 + 7 * 192 - 16)
BadguyMethod enemy671BehaviourMethod;
AnkhActor enemy671( enemy671Init, &enemy671BehaviourMethod );
const ActorInit door670Init = { "door670", ActorInit::vertdoor, &vertdoorSprite, 1226, 1424, 6, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.625 * 16 + 6 * 192 - 16), (6 * 16 + 7 * 192 - 16)
VertdoorMethod door670BehaviourMethod;
AnkhActor door670( door670Init, &door670BehaviourMethod );
const ActorInit door671Init = { "door671", ActorInit::vertdoor, &vertdoorSprite, 1264, 1424, 6, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8 * 16 + 6 * 192 - 16), (6 * 16 + 7 * 192 - 16)
VertdoorMethod door671BehaviourMethod;
AnkhActor door671( door671Init, &door671BehaviourMethod );
const ActorInit redirect710Init = { "redirect710", ActorInit::redirect, &boxSprite, 1434, 316, 7, 1, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.625 * 16 + 7 * 192 - 16), (8.75 * 16 + 1 * 192 - 16)
RedirectMethod redirect710BehaviourMethod( 0, -1 );
AnkhActor redirect710( redirect710Init, &redirect710BehaviourMethod );
const ActorInit redirect711Init = { "redirect711", ActorInit::redirect, &boxSprite, 1434, 252, 7, 1, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.625 * 16 + 7 * 192 - 16), (4.75 * 16 + 1 * 192 - 16)
RedirectMethod redirect711BehaviourMethod( -1, 0 );
AnkhActor redirect711( redirect711Init, &redirect711BehaviourMethod );
const ActorInit shooter710Init = { "shooter710", ActorInit::shooter, &badguySprite, 1486, 318, 7, 1, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.875 * 16 + 7 * 192 - 16), (8.875 * 16 + 1 * 192 - 16)
ShooterMethod shooter710BehaviourMethod( -1, 0, 15 );
AnkhActor shooter710( shooter710Init, &shooter710BehaviourMethod );
const ActorInit doorlock710Init = { "doorlock710", ActorInit::doorlock, &badguySprite, 1388, 254, 7, 1, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (3.75 * 16 + 7 * 192 - 16), (4.875 * 16 + 1 * 192 - 16)
DoorlockMethod doorlock710BehaviourMethod;
AnkhActor doorlock710( doorlock710Init, &doorlock710BehaviourMethod );
const ActorInit trigger760Init = { "trigger760", ActorInit::trigger, &itemSprite, 1402, 1324, 7, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.625 * 16 + 7 * 192 - 16), (11.75 * 16 + 6 * 192 - 16)
TriggerMethod trigger760BehaviourMethod( door760, BehaviourMethod::TRIGGER, BehaviourMethod::TOUCH );
AnkhActor trigger760( trigger760Init, &trigger760BehaviourMethod );
const ActorInit trigger761Init = { "trigger761", ActorInit::trigger, &itemSprite, 1372, 1202, 7, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (2.75 * 16 + 7 * 192 - 16), (4.125 * 16 + 6 * 192 - 16)
TriggerMethod trigger761BehaviourMethod( randshooter760, BehaviourMethod::TRIGGER, BehaviourMethod::TOUCH );
AnkhActor trigger761( trigger761Init, &trigger761BehaviourMethod );
const ActorInit door760Init = { "door760", ActorInit::horzdoor, &horzdoorSprite, 1408, 1294, 7, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5 * 16 + 7 * 192 - 16), (9.875 * 16 + 6 * 192 - 16)
HorzdoorMethod door760BehaviourMethod(false,-1);
AnkhActor door760( door760Init, &door760BehaviourMethod );
const ActorInit doorlock760Init = { "doorlock760", ActorInit::doorlock, &badguySprite, 1444, 1170, 7, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (7.25 * 16 + 7 * 192 - 16), (2.125 * 16 + 6 * 192 - 16)
DoorlockMethod doorlock760BehaviourMethod( mover762 );
AnkhActor doorlock760( doorlock760Init, &doorlock760BehaviourMethod );
const ActorInit randshooter760Init = { "randshooter760", ActorInit::shooter, &boxSprite, 1444, 1264, 7, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (7.25 * 16 + 7 * 192 - 16), (8 * 16 + 6 * 192 - 16)
ShooterMethod randshooter760BehaviourMethod(0,-2,30,60,3,false);
AnkhActor randshooter760( randshooter760Init, &randshooter760BehaviourMethod );
const ActorInit redirect760Init = { "redirect760", ActorInit::redirect, &boxSprite, 1410, 1254, 7, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.125 * 16 + 7 * 192 - 16), (7.375 * 16 + 6 * 192 - 16)
RedirectMethod redirect760BehaviourMethod(-1,0);
AnkhActor redirect760( redirect760Init, &redirect760BehaviourMethod );
const ActorInit redirect761Init = { "redirect761", ActorInit::redirect, &boxSprite, 1414, 1202, 7, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.375 * 16 + 7 * 192 - 16), (4.125 * 16 + 6 * 192 - 16)
RedirectMethod redirect761BehaviourMethod(1,0);
AnkhActor redirect761( redirect761Init, &redirect761BehaviourMethod );
const ActorInit redirect762Init = { "redirect762", ActorInit::redirect, &boxSprite, 1470, 1282, 7, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8.875 * 16 + 7 * 192 - 16), (9.125 * 16 + 6 * 192 - 16)
RedirectMethod redirect762BehaviourMethod;
AnkhActor redirect762( redirect762Init, &redirect762BehaviourMethod );
const ActorInit mover762Init = { "mover762", ActorInit::shooter, &trapSprite, 1506, 1170, 7, 6, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (11.125 * 16 + 7 * 192 - 16), (2.125 * 16 + 6 * 192 - 16)
ShooterMethod mover762BehaviourMethod( 0, 1, -1 );
AnkhActor mover762( mover762Init, &mover762BehaviourMethod );
const ActorInit door770Init = { "door770", ActorInit::horzdoor, &horzdoorSprite, 1488, 1488, 7, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (10 * 16 + 7 * 192 - 16), (10 * 16 + 7 * 192 - 16)
HorzdoorMethod door770BehaviourMethod;
AnkhActor door770( door770Init, &door770BehaviourMethod );
const ActorInit door771Init = { "door771", ActorInit::horzdoor, &horzdoorSprite, 1488, 1388, 7, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (10 * 16 + 7 * 192 - 16), (3.75 * 16 + 7 * 192 - 16)
HorzdoorMethod door771BehaviourMethod;
AnkhActor door771( door771Init, &door771BehaviourMethod );
const ActorInit door772Init = { "door772", ActorInit::horzdoor, &horzdoorSprite, 1408, 1388, 7, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5 * 16 + 7 * 192 - 16), (3.75 * 16 + 7 * 192 - 16)
HorzdoorMethod door772BehaviourMethod;
AnkhActor door772( door772Init, &door772BehaviourMethod );
const ActorInit enemy770Init = { "enemy770", ActorInit::badguy, &badguySprite, 1438, 1408, 7, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (6.875 * 16 + 7 * 192 - 16), (5 * 16 + 7 * 192 - 16)
BadguyMethod enemy770BehaviourMethod;
AnkhActor enemy770( enemy770Init, &enemy770BehaviourMethod );
const ActorInit enemy771Init = { "enemy771", ActorInit::badguy, &badguySprite, 1460, 1452, 7, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (8.25 * 16 + 7 * 192 - 16), (7.75 * 16 + 7 * 192 - 16)
BadguyMethod enemy771BehaviourMethod;
AnkhActor enemy771( enemy771Init, &enemy771BehaviourMethod );
const ActorInit trigger770Init = { "trigger770", ActorInit::doorlock, &itemSprite, 1482, 1484, 7, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.625 * 16 + 7 * 192 - 16), (9.75 * 16 + 7 * 192 - 16)
DoorlockMethod trigger770BehaviourMethod( door770 );
AnkhActor trigger770( trigger770Init, &trigger770BehaviourMethod );
const ActorInit trigger771Init = { "trigger771", ActorInit::doorlock, &itemSprite, 1482, 1392, 7, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (9.625 * 16 + 7 * 192 - 16), (4 * 16 + 7 * 192 - 16)
DoorlockMethod trigger771BehaviourMethod( door771 );
AnkhActor trigger771( trigger771Init, &trigger771BehaviourMethod );
const ActorInit trigger772Init = { "trigger772", ActorInit::doorlock, &itemSprite, 1400, 1360, 7, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (4.5 * 16 + 7 * 192 - 16), (2 * 16 + 7 * 192 - 16)
DoorlockMethod trigger772BehaviourMethod( door772 );
AnkhActor trigger772( trigger772Init, &trigger772BehaviourMethod );
const ActorInit chalice772Init = { "chalice772", ActorInit::chalice, &chaliceSprite, 1500, 1502, 7, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (10.75 * 16 + 7 * 192 - 16), (10.875 * 16 + 7 * 192 - 16)
ChaliceMethod chalice772BehaviourMethod;
AnkhActor chalice772( chalice772Init, &chalice772BehaviourMethod );
const ActorInit face770Init = { "face770", ActorInit::doorlock, &lockfaceSprite, 1496, 1372, 7, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (10.5 * 16 + 7 * 192 - 16), (2.75 * 16 + 7 * 192 - 16)
DoorlockMethod face770BehaviourMethod( door661 );
AnkhActor face770( face770Init, &face770BehaviourMethod );
const ActorInit flit770Init = { "flit770", ActorInit::flit, &flitSprite, 1410, 1468, 7, 7, 4, 4 , ACTOR_COLLISION_PHYSICS | ACTOR_VISIBLE | ACTOR_PICKUP }; // (5.125 * 16 + 7 * 192 - 16), (8.75 * 16 + 7 * 192 - 16)
FlitMethod flit770BehaviourMethod;
AnkhActor flit770( flit770Init, &flit770BehaviourMethod );


AnkhActor* gAllActors[212] = { &player, &bullet000, &bullet001, &bullet002, &bullet003, &bullet004, &bullet005, &bullet006, &bullet007, &bullet008, &bullet009, &bullet010, &bullet011, &touchvert000, &touchhorz000, &touchvert001, &touchhorz001, &candy000, &candy001, &candy002, &door000, &musicbox000, &doorlock000, &treasure000, &enemy010, &enemy020, &enemy021, &treasure030, &treasure031, &doorlock040, &doorlock041, &door040, &door041, &doorlock060, &door060, &doorlock061, &benign100, &candy120, &doorlock130, &door130, &enemy130, &enemy131, &door140, &musicbox140, &doorlock150, &door150, &doorlock151, &enemy150, &doorlock160, &door160, &doorlock161, &crown200, &trap200, &teleswitch210, &teleport210, &lock220, &lock221, &lock222, &lock223, &face220, &enemy230, &enemy231, &doorlock240, &door240, &doorlock241, &doorlock250, &door250, &key251, &doorlock260, &door260, &doorlock261, &shooter300, &leftdoor300, &rightdoor300, &face300, &musicbox301, &entrydoor300, &lock300, &lock301, &treasure310, &treasure320, &benign320, &benign321, &door320, &illumina320, &enemy330, &enemy331, &enemy332, &doorlock340, &door340, &doorlock341, &doorlock350, &door350, &doorlock351, &doorlock360, &door360, &doorlock361, &suck400, &suck401, &enemy430, &enemy431, &enemy432, &enemy440, &enemy441, &enemy442, &enemy443, &enemy450, &enemy451, &enemy452, &enemy453, &door460, &door461, &doorlock460, &piano461, &treasure470, &treasure471, &treasure472, &enemy500, &enemy501, &up540, &down541, &stop542, &shoot543, &face544, &player540, &triggerpad540, &bonus550, &down550, &up551, &shoot552, &face550, &player550, &triggerpad550, &down560, &shoot561, &face560, &player560, &triggerpad560, &trigger570, &trigger571, &trigger572, &trigger573, &trigger574, &trigger575, &trigger576, &trigger577, &face570, &enemy570, &door570, &door571, &door572, &door573, &door574, &door575, &door576, &door600, &door610, &doorlock610, &musicbox610, &touchpad610, &door620, &doorlock620, &key620, &candy630, &candy631, &candy632, &candy633, &candy634, &candy635, &candy636, &candy637, &doorlock630, &door630, &doorlock631, &door631, &panpad630, &enemy650, &enemy651, &door650, &door651, &enemy660, &enemy661, &door660, &door661, &enemy670, &enemy671, &door670, &door671, &redirect710, &redirect711, &shooter710, &doorlock710, &trigger760, &trigger761, &door760, &doorlock760, &randshooter760, &redirect760, &redirect761, &redirect762, &mover762, &door770, &door771, &door772, &enemy770, &enemy771, &trigger770, &trigger771, &trigger772, &chalice772, &face770, &flit770,  };
