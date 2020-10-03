
////////////////////////////////////////////////////////////////////////////////
// game data

// sprites, 2d geometry, simply a textured quad.  width, height, bitmap
let badguy = { bbox: { w: 6, h: 3 }, src: "assets/badguy.png" };
let benign = { bbox: { w: 15, h: 7 }, src: "assets/benign.png" };
let bonus = { bbox: { w: 13, h: 7 }, src: "assets/bonus.png" };
let box = { bbox: { w: 9, h: 7 }, src: "assets/box.png" };
let chalice = { bbox: { w: 11, h: 5 }, src: "assets/chalice.png" };
let crown = { bbox: { w: 19, h: 4 }, src: "assets/crown.png" };
let flit = { bbox: { w: 13, h: 5 }, src: "assets/flit.png" };
let horzdoor = { bbox: { w: 30, h: 4 }, src: "assets/horzdoor.png" };
let illumina = { bbox: { w: 11, h: 6 }, src: "assets/illumina.png" };
let item = { bbox: { w: 6, h: 3 }, src: "assets/item.png" };
let key = { bbox: { w: 16, h: 8 }, src: "assets/key.png" };
let lock = { bbox: { w: 8, h: 5 }, src: "assets/lock.png" };
let lockface = { bbox: { w: 14, h: 6 }, src: "assets/lockface.png" };
let musicbox = { bbox: { w: 20, h: 7 }, src: "assets/musicbox.png" };
let other = { bbox: { w: 21, h: 13 }, src: "assets/other.png" };
let suck = { bbox: { w: 23, h: 5 }, src: "assets/suck.png" };
let tele = { bbox: { w: 9, h: 5 }, src: "assets/tele.png" };
let trap = { bbox: { w: 11, h: 5 }, src: "assets/trap.png" };
let treasure = { bbox: { w: 17, h: 6 }, src: "assets/treasure.png" };
let vertdoor = { bbox: { w: 6, h: 32 }, src: "assets/vertdoor.png" };
let locklisten = { bbox: { w: 16, h: 8 }, src: "assets/locklisten.png" };
let bullet = { bbox: { w: 2, h: 1 }, src: "assets/bullet.png" };
let touchvert = { bbox: { w: 2, h: 16 }, src: "assets/touchvert.png" };
let touchhorz = { bbox: { w: 16, h: 2 }, src: "assets/touchhorz.png" };
let touchpad610 = { bbox: { w: 93, h: 111 }, src: undefined };
let piano461 = { bbox: { w: 1, h: 1 }, src: undefined };
let triggerpad540 = { bbox: { w: 1230 - 1152, h: 880 - 847 }, src: undefined };
let triggerpad550 = { bbox: { w: 1230 - 1152, h: 1072 - 1039 }, src: undefined };
let triggerpad560 = { bbox: { w: 1230 - 1152, h: 1264 - 1231 }, src: undefined };
let panpad630 = { bbox: { w: 1134 - 1086, h: 1519 - 1423 }, src: undefined };


// "name": [ "type", "sprite", posx, posy, karmapoints,shootpoints, {'type' params} }
// NOTE: actor name is name<roomx><roomy><uniquenumber>

// DATATYPES:
// actor { "type", "sprite", posx, posy }
// ankhactor { karmapoints, shootpoints, canpickup }
// badguy {} // creature that shoots at you.
// benign {} // creature with normal object attrs, can't be killed.
// bonus {value} // gives points
// redirect {directionx, directiony} // shoots when shot
// chalice {} // ?
// triggeritem {triggername, when} // triggers another item when touched or shot, pickedup, dropped, collided.
// flit {} // makes time slow
// door {velocity} // moves when triggered
// illumina {} // draw enables all treasure
// item {} // does nothing
// key {triggername}  // triggers something when dropped near it.
// musicbox {song}
// player {power}
// suck {amount} // flocks to player, sucks some amount from when touching.
// tele {where} // when triggered, moves player to a location
// trap {shotdir} // shoots when triggered
// mover {item, direction} // causes item to move in some direction.
// treasure {value}
// listentrigger {{items}, distance, triggername} // listens for triggers from items in order, triggers another item.
// osc {dir} // moves something along dir, when collide, reverse dir.
// shooter {dir} // shoots when triggered
// shooter2 {} // shoots randomly when turned on (trigger toggles).

let COLLISION = 1;
let VISIBLE = 2;
let COLLISION_TRIGGER = 4;
/*
let actor_layout = {
  player: [ "player", other, 6.375, 6.5,			-4, -4 ],
  bullet000: [ "none", bullet, 6, 6.6, 4, 4, COLLISION | VISIBLE ],
  bullet001: [ "none", bullet, 6.1, 6.7, 4, 4, COLLISION | VISIBLE ],
  bullet002: [ "none", bullet, 6, 6.8, 4, 4, COLLISION | VISIBLE ],
  bullet003: [ "none", bullet, 6.1, 6.9, 4, 4, COLLISION | VISIBLE ],
  bullet004: [ "none", bullet, 6, 7, 4, 4, COLLISION | VISIBLE ],
  bullet005: [ "none", bullet, 6.1, 7.1, 4, 4, COLLISION | VISIBLE ],
  bullet006: [ "none", bullet, 6, 7.2, 4, 4, COLLISION | VISIBLE ],
  bullet007: [ "none", bullet, 6.1, 7.3, 4, 4, COLLISION | VISIBLE ],
  bullet008: [ "none", bullet, 6, 7, 4, 4, COLLISION | VISIBLE ],
  bullet009: [ "none", bullet, 6.1, 7.1, 4, 4, COLLISION | VISIBLE ],
  bullet010: [ "none", bullet, 6, 7.2, 4, 4, COLLISION | VISIBLE ],
  bullet011: [ "none", bullet, 6.1, 7.3, 4, 4, COLLISION | VISIBLE ],
  touchvert000: [ "none", touchvert, 5, 6.625, 4, 4, COLLISION | VISIBLE ],
  touchhorz000: [ "none", touchhorz, 5.5, 6.625, 4, 4, COLLISION | VISIBLE ],
  touchvert001: [ "none", touchvert, 5, 6.625, 4, 4, COLLISION | VISIBLE ],
  touchhorz001: [ "none", touchhorz, 5.5, 6.625, 4, 4, COLLISION | VISIBLE ],


  // room 0,0
  candy000: [ candy, item, 5, 3.375,			4, 4 ],
  candy001: [ candy, item, 7.625, 4.625,		4, 4 ],
  candy002: [ osc( -1, -1 ), item, 7.75, 6,	4, 4 ],
  door000: [  horzdoor, horzdoor, 6, 11,		4, 4 ],
  musicbox000: [  musicbox( piano461, UP, SHOOT ), musicbox, 3, 6.625, 4, 4 ],
  doorlock000: [  doorlock( door000 ), lockface, 9, 9.25, 4, 4 ],
  treasure000: [  treasure( 10 ), treasure, 6.5, 3.375, 4, 4 ],


  // room 0,1
  enemy010: [ badguy, badguy, 7, 7, 4, 4 ],

  // room 0,2
  enemy020: [ badguy, badguy, 5, 3.375, 4, 4 ],
  enemy021: [ badguy, badguy, 11, 6, 4, 4 ],

  // room 0,3
  treasure030: [ treasure( 10 ), treasure, 4, 9.5, 4, 4 ],
  treasure031: [ treasure( 10 ), treasure, 8.875, 9.5, 4, 4 ],

  // room 0,4
  doorlock040: [ doorlock( door040 ), lock, 9.25, 5.375, 4, 4 ],
  doorlock041: [ doorlock( door041 ), lock, 5.25, 9.375, 4, 4 ],
  door040: [ vertdoor, vertdoor, 9, 6, 4, 4 ],
  door041: [ horzdoor, horzdoor, 6, 9, 4, 4 ],

  // room 0,5 nothing...

  // room 0,6
  doorlock060: [ doorlock, lockface, 8, 9.25, 4, 4 ],
  door060: [ vertdoor, vertdoor, 5.625, 5, 4, 4 ],
  doorlock061: [ doorlock( door160 ), item, 4.625, 5.875, 4, 4 ],

  // room 0,7 nothing.  end!




  // room 1,0
  benign100: [ benign, benign, 6.5, 6.75, 4, 4 ],

  // room 1,1

  // room 1,2
  candy120: [ candy, item, 2.125, 11.5, 4, 4 ],

  // room 1,3
  doorlock130: [ doorlock( door130 ), lock, 3.25, 3.375, 4, 4 ],
  door130: [ horzdoor, horzdoor, 6, 2.75, 4, 4 ],
  enemy130: [ badguy, badguy, 9.125, 5.25, 4, 4 ],
  enemy131: [ badguy, badguy, 7, 8, 4, 4 ],

  // room 1,4
  door140: [ horzdoor, horzdoor, 6, 8, 4, 4 ],
  musicbox140: [ musicbox( piano461, LEFT, SHOOT ), musicbox, 6.5, 10.375, 4, 4 ],

  // room 1,5
  doorlock150: [ doorlock, lockface, 7, 5.25, 4, 4 ],
  door150: [ vertdoor, vertdoor, 5.625, 8, 4, 4 ],
  doorlock151: [ doorlock(door060), item, 4.625, 8.875, 4, 4 ],
  enemy150: [ badguy, badguy, 10.375, 3.625, 4, 4 ],

  // room 1,6
  doorlock160: [ doorlock, lockface, 5.125, 2.25, 4, 4 ],
  door160: [ horzdoor, horzdoor, 4, 9, 4, 4 ],
  doorlock161: [ doorlock(door260), item, 4.875, 10, 4, 4 ],

  // room 1,7




  // room 2,0
  crown200: [ trigger( trap200, TRIGGER, TOUCH ), crown, 6.375, 3.25, 4, 4 ],
  trap200: [ trap( 0,-2,-1 ), trap, 6.625, 9.75, 4, 4 ],

  // room 2,1
  teleswitch210: [ doorlock, tele, 2.25, 10.25, 4, 4 ],
  teleport210: [ tele, badguy, 10.25, 10.375, 4, 4 ],

  // room 2,2
  lock220: [ trigger( trap200, UP,    TOUCH ), item, 5.375, 8.25, 4, 4 ],
  lock221: [ trigger( trap200, DOWN,  TOUCH ), item, 8.375, 5.5, 4, 4 ],
  lock222: [ trigger( trap200, LEFT,  TOUCH ), item, 9.25, 8.125, 4, 4 ],
  lock223: [ trigger( trap200, RIGHT, TOUCH ), item, 3.375, 5.5, 4, 4 ],
  face220: [ doorlock( door320 ), lockface, 11.125, 9.125, 4, 4 ],

  // room 2,3
  enemy230: [ badguy, badguy, 8.25, 4, 4, 4 ],
  enemy231: [ badguy, badguy, 4.125, 4.75, 4, 4 ],

  // room 2,4
  doorlock240: [ doorlock, lockface, 2, 8.25,				4, 4 ],
  door240: [ horzdoor, horzdoor, 6, 5.75,					4, 4 ],
  doorlock241: [ doorlock(door360), item, 6.75, 4.875,		4, 4 ],

  // room 2,5
  doorlock250: [ doorlock(door350), lockface, 8, 8.25,		4, 4 ],
  door250: [ horzdoor, horzdoor, 4, 5.75,					4, 4 ],
  key251: [ key(door140), key, 4.625, 4.625,				4, 4 ],

  // room 2,6
  doorlock260: [ doorlock, lockface, 11, 8.25,				4, 4 ],
  door260: [ vertdoor, vertdoor, 5.625, 3.875,				4, 4 ],
  doorlock261: [ doorlock(door240), item, 4.625, 4.875,	4, 4 ],

  // room 2,7



  // room 3,0
  shooter300: [ shooter( 1,0, 60 ), box, 2.5, 3.25,        4,4 ],
  leftdoor300: [ vertdoor, vertdoor, 4.25, 2.5,            4,4 ],
  rightdoor300: [ vertdoor, vertdoor, 9.25, 2.5,           4,4 ],
  face300: [ doorlock( entrydoor300 ), lockface, 10.75, 3.25, 4, 4 ],
  musicbox301: [ musicbox( piano461, DOWN, SHOOT ), musicbox, 6.75, 7,              4,4 ],
  entrydoor300: [ horzdoor, horzdoor, 6, 11,               4,4 ],
  lock300: [ doorlock( leftdoor300 ), item, 2.625, 6,      4,4 ],
  lock301: [ doorlock( rightdoor300 ), item, 11.375, 6,    4,4 ],
  //sprite panpad300 = { 48, 33, nobitmap };
  //sprite panpad301 = { 48, 33, nobitmap };
  //sprite panpad302 = { 702 - 639, 190 - 79, nobitmap };
  //panpad300: [ panpad( 60,0 ), panpad300, 575px, 79px,   0, 0, COLLISION_TRIGGER ],
  //panpad301: [ panpad( -60, 0 ), panpad301, 719px, 79px, 0, 0, COLLISION_TRIGGER ],
  //panpad302: [ interppad( 0,0, 0,-70), panpad302, 639px, 79px, 0, 0, COLLISION_TRIGGER ],

  // room 3,1
  treasure310: [ treasure( 10 ), treasure, 6.5, 6.75, 4, 4 ],

  // room 3,2
  treasure320: [ treasure( 10 ), treasure, 10.75, 9.125, 4, 4 ],
  benign320: [ benign( 2, 0 ), benign, 8.125, 10, 4, 4 ],
  benign321: [ benign( 2, 0 ), benign, 4.25, 9.375, 4, 4 ],
  door320: [ horzdoor, horzdoor, 6, 8.75, 4, 4 ],
  illumina320: [ illumina( treasure ), illumina, 2, 11.5, 4, 4 ],

  // room 3,3
  enemy330: [ badguy, badguy, 5.375, 3.75, 4, 4 ],
  enemy331: [ badguy, badguy, 9.375, 3.125, 4, 4 ],
  enemy332: [ badguy, badguy, 8.25, 10.25, 4, 4 ],

  // room 3,4
  doorlock340: [ doorlock, lockface, 8, 8.25, 4, 4 ],
  door340: [ horzdoor, horzdoor, 4, 5.75, 4, 4 ],
  doorlock341: [ doorlock( door250 ), item, 4.875, 4.875, 4, 4 ],

  // room 3,5
  doorlock350: [ doorlock, lockface, 5, 4.25, 4, 4 ],
  door350: [ vertdoor, vertdoor, 5.625, 5.875, 4, 4 ],
  doorlock351: [ doorlock(door150), item, 4.75, 6.875, 4, 4 ],

  // room 3,6
  doorlock360: [ doorlock, lockface, 3, 5.25, 4, 4 ],
  door360: [ vertdoor, vertdoor, 7.875, 3.875, 4, 4 ],
  doorlock361: [ doorlock( door340 ), item, 8.75, 4.875, 4, 4 ],

  // room 3,7



  // room 4,0
  suck400: [ suck(1, -4 ), suck, 6.25, 11.5, 4, 4 ],
  suck401: [ suck(-1, -4 ), suck, 6.25, 2.125, 4, 4 ],

  // room 4,1

  // room 4,2

  // room 4,3
  enemy430: [ badguy, badguy, 8.625, 9.625, 4, 4 ],
  enemy431: [ badguy, badguy, 5.625, 9.5, 4, 4 ],
  enemy432: [ badguy, badguy, 7, 8, 4, 4 ],

  // room 4,4
  enemy440: [ badguy, badguy, 10.125, 10.375, 4, 4 ],
  enemy441: [ badguy, badguy, 4.375, 2.375, 4, 4 ],
  enemy442: [ badguy, badguy, 11.125, 5.125, 4, 4 ],
  enemy443: [ badguy, badguy, 2.125, 4.25, 4, 4 ],

  // room 4,5
  enemy450: [ badguy, badguy, 9.25, 11.125, 4, 4 ],
  enemy451: [ badguy, badguy, 9.25, 2.25, 4, 4 ],
  enemy452: [ badguy, badguy, 3.25, 3.25, 4, 4 ],
  enemy453: [ badguy, badguy, 10.75, 4.125, 4, 4 ],

  // room 4,6
  door460: [ horzdoor, horzdoor, 6, 10, 4, 4 ],
  door461: [ horzdoor(false,-1), horzdoor, 6, 8, 4, 4 ],
  doorlock460: [ tonegen, locklisten, 5, 7.25, 4, 4 ],
  piano461: [ statetrigger( door460, door461 ), piano461, 5, 7.25, 4, 4, COLLISION_TRIGGER ],


  // room 4,7
  treasure470: [ treasure( 10 ), treasure, 9.875, 6.5, 4, 4 ],
  treasure471: [ treasure( 10 ), treasure, 9.875, 6, 4, 4 ],
  treasure472: [ treasure( 10 ), treasure, 9.875, 5.5, 4, 4 ],




  // room 5,0
  enemy500: [ badguy, badguy, 7.25, 10.375, 4, 4 ],
  enemy501: [ badguy, badguy, 7.25, 2.25, 4, 4 ],

  // room 5,1

  // room 5,2

  // room 5,3

  // room 5,4
  up540: [ trigger( player540, UP, TOUCH ), item, 11.375, 10.75, 4, 4 ],
  down541: [ trigger( player540, DOWN, TOUCH ), item, 9.125, 10.75, 4, 4 ],
  stop542: [ trigger( player540, STOP, TOUCH ), item, 10.25, 11.625, 4, 4 ],
  shoot543: [ trigger( player540, SHOOT, TOUCH ), item, 11.375, 10.375, 4, 4 ],
  face544: [ doorlock( door650 ), lockface, 6.875, 6.75, 4, 4 ],
  player540: [ playerdrone, other, 2, 2, 4, 4 ],
  triggerpad540: [ triggerpad( player540, EXIT ), triggerpad540, 1152px, 847px, 0, 0, COLLISION_TRIGGER ],

  // room 5,5
  bonus550: [ candy, bonus, 7, 7.75, 4, 4 ],
  down550: [ trigger( player550, DOWN, TOUCH ), item, 9, 10.75, 4, 4 ],
  up551: [ trigger( player550, UP, TOUCH ), item, 11.5, 10.75, 4, 4 ],
  shoot552: [ trigger( player550, SHOOT, TOUCH ), item, 11.5, 10.375, 4, 4 ],
  face550: [ doorlock( door660 ), lockface, 6.875, 6.75, 4, 4 ],
  player550: [ playerdrone, other, 2, 2.125, 4, 4 ],
  triggerpad550: [ triggerpad( player550, EXIT ), triggerpad550, 1152px, 1039px, 0, 0, COLLISION_TRIGGER ],

  // room 5,6
  down560: [ trigger( player560, DOWN, TOUCH ), item, 9, 10.75, 4, 4 ],
  shoot561: [ trigger( player560, SHOOT, TOUCH ), item, 11.5, 10.25, 4, 4 ],
  face560: [ doorlock( door670 ), lockface, 6.875, 6.75, 4, 4 ],
  player560: [ playerdrone, other, 2, 2, 4, 4 ],
  triggerpad560: [ triggerpad( player560, EXIT ), triggerpad560, 1152px, 1231px, 0, 0, COLLISION_TRIGGER ],

  // room 5,7
  trigger570: [ doorlock( door570, TRIGGER, TOUCH ), item, 11.5, 9.375, 4, 4 ],
  trigger571: [ doorlock( door571, TRIGGER, TOUCH ), item, 11.375, 10, 4, 4 ],
  trigger572: [ doorlock( door572, TRIGGER, TOUCH ), item, 11.5, 10.25, 4, 4 ],
  trigger573: [ doorlock( door573, TRIGGER, TOUCH ), item, 11.375, 10.625, 4, 4 ],
  trigger574: [ doorlock( door574, TRIGGER, TOUCH ), item, 10.25, 11.625, 4, 4 ],
  trigger575: [ doorlock( door575, TRIGGER, TOUCH ), item, 9.125, 10.5, 4, 4 ],
  trigger576: [ doorlock( door576, TRIGGER, TOUCH ), item, 9.25, 9.5, 4, 4 ],
  trigger577: [ doorlock, item, 9, 9.25, 4, 4 ],
  face570: [ doorlock( door671 ), lockface, 6.75, 6.75, 4, 4 ],
  enemy570: [ badguy, badguy, 2.125, 2.125, 4, 4 ],
  door570: [ vertdoor, vertdoor, 5.625, 6, 4, 4 ],
  door571: [ vertdoor, vertdoor, 3.625, 2, 4, 4 ],
  door572: [ vertdoor, vertdoor, 3.625, 6, 4, 4 ],
  door573: [ vertdoor, vertdoor, 5.75, 2, 4, 4 ],
  door574: [ horzdoor, horzdoor, 4, 4, 4, 4 ],
  door575: [ horzdoor, horzdoor, 6, 5.875, 4, 4 ],
  door576: [ horzdoor, horzdoor, 6, 7.875, 4, 4 ],



  // room 6,0
  door600: [ vertdoor, vertdoor, 5.625, 5.875, 4, 4 ],

  // room 6,1
  door610: [ horzdoor, horzdoor, 6, 5, 4, 4 ],
  doorlock610: [ doorlock( door600 ), lockface, 3.125, 9.875, 4, 4 ],
  musicbox610: [ musicbox( piano461, RIGHT, SHOOT ), musicbox, 3, 6.625, 4, 4 ],
  touchpad610: [ touchpad( 12 ), touchpad610, 1200, 193, 0, 0, COLLISION_TRIGGER ],
  //1200,193   1293, 304

  // room 6,2
  door620: [ vertdoor(false,-1,64), vertdoor, 8.625, 1.875, 4, 4 ],
  doorlock620: [ doorlock( door620 ), lockface, 2.875, 11.625, 4, 4 ],
  key620: [ key( door610 ), key, 2.875, 7.125, 4, 4 ],

  // room 6,3
  candy630: [ candy, item, 2.875, 5.25,	4, 8 ],
  candy631: [ candy, item, 2, 5.5,			4, 8 ],
  candy632: [ candy, item, 2, 5,			4, 8 ],
  candy633: [ candy, item, 2.875, 5,		4, 8 ],
  candy634: [ candy, item, 2.375, 5.5,		4, 8 ],
  candy635: [ candy, item, 2, 5.25,		4, 8 ],
  candy636: [ candy, item, 2.375, 5,		4, 8 ],
  candy637: [ candy, item, 2.875, 5.5,		4, 8 ],
  doorlock630: [ doorlock( door631 ), lock, 1.25, 8.375, 4, 4 ],
  door630: [ vertdoor, vertdoor, 9, 6,		4, 4 ],
  doorlock631: [ doorlock( door630 ), lock, 9.25, 8.25, 4, 4 ],
  door631: [ horzdoor, horzdoor, 6, 9,		4, 4 ],
  panpad630: [ interppad( -28,0,   0,0), panpad630, 1086px, 1423px, 0, 0, COLLISION_TRIGGER ],


  // room 6,4

  // room 6,5
  enemy650: [ badguy, badguy, 3.75, 6.75, 4, 4 ],
  enemy651: [ badguy, badguy, 8.375, 7.25, 4, 4 ],
  door650: [ vertdoor, vertdoor, 5.625, 6, 4, 4 ],
  door651: [ vertdoor, vertdoor, 8, 6, 4, 4 ],

  // room 6,6
  enemy660: [ badguy, badguy, 3.75, 6.5, 4, 4 ],
  enemy661: [ badguy, badguy, 9.75, 6.75, 4, 4 ],
  door660: [ vertdoor, vertdoor, 5.625, 6, 4, 4 ],
  door661: [ vertdoor, vertdoor, 8, 6, 4, 4 ],


  // room 6,7
  enemy670: [ badguy, badguy, 2.75, 6.5, 4, 4 ],
  enemy671: [ badguy, badguy, 9.25, 6.875, 4, 4 ],
  door670: [ vertdoor, vertdoor, 5.625, 6, 4, 4 ],
  door671: [ vertdoor, vertdoor, 8, 6, 4, 4 ],





  // room 7,0

  // room 7,1
  redirect710: [ redirect( 0, -1 ), box, 6.625, 8.75, 4, 4 ],
  redirect711: [ redirect( -1, 0 ), box, 6.625, 4.75, 4, 4 ],
  shooter710: [ shooter( -1, 0, 15 ), badguy, 9.875, 8.875, 4, 4 ],
  doorlock710: [ doorlock, badguy, 3.75, 4.875, 4, 4 ],

  // room 7,2

  // room 7,3

  // room 7,4

  // room 7,5

  // room 7,6
  trigger760: [ trigger( door760, TRIGGER, TOUCH ), item, 4.625, 11.75, 4, 4 ],
  trigger761: [ trigger( randshooter760, TRIGGER, TOUCH ), item, 2.75, 4.125, 4, 4 ],
  door760: [ horzdoor(false,-1), horzdoor, 5, 9.875, 4, 4 ],
  doorlock760: [ doorlock( mover762 ), badguy, 7.25, 2.125, 4, 4 ],
  randshooter760: [ shooter(0,-2,30,60,3,false), box, 7.25, 8, 4, 4 ],
  redirect760: [ redirect(-1,0), box, 5.125, 7.375, 4, 4 ],
  redirect761: [ redirect(1,0), box, 5.375, 4.125, 4, 4 ],
  redirect762: [ redirect, box, 8.875, 9.125, 4, 4 ],
  mover762: [ shooter( 0, 1, -1 ), trap, 11.125, 2.125, 4, 4 ],

  // room 7,7
  door770: [ horzdoor, horzdoor, 10, 10, 4, 4 ],
  door771: [ horzdoor, horzdoor, 10, 3.75, 4, 4 ],
  door772: [ horzdoor, horzdoor, 5, 3.75, 4, 4 ],
  enemy770: [ badguy, badguy, 6.875, 5, 4, 4 ],
  enemy771: [ badguy, badguy, 8.25, 7.75, 4, 4 ],
  trigger770: [ doorlock( door770 ), item, 9.625, 9.75, 4, 4 ],
  trigger771: [ doorlock( door771 ), item, 9.625, 4, 4, 4 ],
  trigger772: [ doorlock( door772 ), item, 4.5, 2, 4, 4 ],
  chalice772: [ chalice, chalice, 10.75, 10.875, 4, 4 ],
  face770: [ doorlock( door661 ), lockface, 10.5, 2.75, 4, 4 ],
  flit770: [ flit, flit, 5.125, 8.75, 4, 4 ],
};
*/
