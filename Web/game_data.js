let COLLISION = 1;
let VISIBLE = 2;
let COLLISION_TRIGGER = 4;

export let game_data = {
////////////////////////////////////////////////////////////////////////////////
// game data

// sprites, 2d geometry, simply a textured quad.  width, height, bitmap
sprites: {
  badguy: { bbox: { x: 0, y: 0, w: 6, h: 3 }, src: "assets/badguy.png" },
  benign: { bbox: { x: 0, y: 0, w: 15, h: 7 }, tilesx: 1, tilesy: 2, src: "assets/benign.png" },
  bonus: { bbox: { x: 0, y: 0, w: 13, h: 7 }, src: "assets/bonus.png" },
  box: { bbox: { x: 0, y: 0, w: 9, h: 7 }, src: "assets/box.png" },
  chalice: { bbox: { x: 0, y: 0, w: 11, h: 5 }, src: "assets/chalice.png" },
  crown: { bbox: { x: 0, y: 0, w: 19, h: 4 }, src: "assets/crown.png" },
  flit: { bbox: { x: 0, y: 0, w: 13, h: 5 }, src: "assets/flit.png" },
  horzdoor: { bbox: { x: 0, y: 0, w: 30, h: 4 }, src: "assets/horzdoor.png" },
  illumina: { bbox: { x: 0, y: 0, w: 11, h: 6 }, src: "assets/illumina.png" },
  item: { bbox: { x: 0, y: 0, w: 6, h: 3 }, src: "assets/item.png" },
  key: { bbox: { x: 0, y: 0, w: 16, h: 8 }, tilesx: 1, tilesy: 2, src: "assets/key.png" },
  lock: { bbox: { x: 0, y: 0, w: 8, h: 5 }, src: "assets/lock.png" },
  lockface: { bbox: { x: 0, y: 0, w: 14, h: 6 }, src: "assets/lockface.png" },
  musicbox: { bbox: { x: 0, y: 0, w: 20, h: 7 }, src: "assets/musicbox.png" },
  other: { bbox: { x: 0, y: 0, w: 21, h: 13 }, src: "assets/other.png" },
  suck: { bbox: { x: 0, y: 0, w: 23, h: 5 }, src: "assets/suck.png" },
  tele: { bbox: { x: 0, y: 0, w: 9, h: 5 }, src: "assets/tele.png" },
  trap: { bbox: { x: 0, y: 0, w: 11, h: 5 }, src: "assets/trap.png" },
  treasure: { bbox: { x: 0, y: 0, w: 17, h: 6 }, src: "assets/treasure.png" },
  vertdoor: { bbox: { x: 0, y: 0, w: 6, h: 32 }, src: "assets/vertdoor.png" },
  locklisten: { bbox: { x: 0, y: 0, w: 16, h: 8 }, src: "assets/locklisten.png" },
  bullet: { bbox: { x: 0, y: 0, w: 2, h: 1 }, src: "assets/bullet.png" },
  touchvert: { bbox: { x: 0, y: 0, w: 2, h: 16 }, src: "assets/touchvert.png" },
  touchhorz: { bbox: { x: 0, y: 0, w: 16, h: 2 }, src: "assets/touchhorz.png" },
  touchpad610: { bbox: { x: 0, y: 0, w: 93, h: 111 }, src: undefined },
  piano461: { bbox: { x: 0, y: 0, w: 1, h: 1 }, src: undefined },
  triggerpad540: { bbox: { x: 0, y: 0, w: 1230 - 1152, h: 880 - 847 }, src: undefined },
  triggerpad550: { bbox: { x: 0, y: 0, w: 1230 - 1152, h: 1072 - 1039 }, src: undefined },
  triggerpad560: { bbox: { x: 0, y: 0, w: 1230 - 1152, h: 1264 - 1231 }, src: undefined },
  panpad630: { bbox: { x: 0, y: 0, w: 1134 - 1086, h: 1519 - 1423 }, src: undefined },
},


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

actor_layout: [
  // player
  { room: [0,0], name: "player", type: "player", sprite: "other", x: 6.375, y: 6.5, collision1: -4, collision2: -4 },
  { name: "bullet000", type: "none", sprite: "bullet", x: 6, y: 6.6, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "bullet001", type: "none", sprite: "bullet", x: 6.1, y: 6.7, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "bullet002", type: "none", sprite: "bullet", x: 6, y: 6.8, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "bullet003", type: "none", sprite: "bullet", x: 6.1, y: 6.9, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "bullet004", type: "none", sprite: "bullet", x: 6, y: 7, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "bullet005", type: "none", sprite: "bullet", x: 6.1, y: 7.1, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "bullet006", type: "none", sprite: "bullet", x: 6, y: 7.2, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "bullet007", type: "none", sprite: "bullet", x: 6.1, y: 7.3, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "bullet008", type: "none", sprite: "bullet", x: 6, y: 7, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "bullet009", type: "none", sprite: "bullet", x: 6.1, y: 7.1, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "bullet010", type: "none", sprite: "bullet", x: 6, y: 7.2, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "bullet011", type: "none", sprite: "bullet", x: 6.1, y: 7.3, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "touchvert000", type: "none", sprite: "touchvert", x: 5, y: 6.625, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "touchhorz000", type: "none", sprite: "touchhorz", x: 5.5, y: 6.625, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "touchvert001", type: "none", sprite: "touchvert", x: 5, y: 6.625, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },
  { name: "touchhorz001", type: "none", sprite: "touchhorz", x: 5.5, y: 6.625, collision1: 4, collision2: 4, collision_type: COLLISION | VISIBLE },


  // room 0,0
  { room: [0,0], name: "candy000", type: "candy", sprite: "item", x: 5, y: 3.375, collision1: 4, collision2: 4 },
  { room: [0,0], name: "candy001", type: "candy", sprite: "item", x: 7.625, y: 4.625, collision1: 4, collision2: 4 },
  { room: [0,0], name: "candy002", type: "osc", type_params: "-1, -1", sprite: "item", x: 7.75, y: 6, collision1: 4, collision2: 4 },
  { room: [0,0], name: "door000", type: "horzdoor", sprite: "horzdoor", x: 6, y: 11, collision1: 4, collision2: 4 },
  { room: [0,0], name: "musicbox000", type: "musicbox", type_params: "piano461, UP, SHOOT", sprite: "musicbox", x: 3, y: 6.625, collision1: 4, collision2: 4 },
  { room: [0,0], name: "doorlock000", type: "doorlock", type_param: "door000", sprite: "lockface", x: 9, y: 9.25, collision1: 4, collision2: 4 },
  { room: [0,0], name: "treasure000", type: "treasure", type_param: "10", sprite: "treasure", x: 6.5, y: 3.375, collision1: 4, collision2: 4 },


  // room 0,1
  { room: [0,1], name: "enemy010", type: "badguy", sprite: "badguy", x: 7, y: 7, collision1: 4, collision2: 4 },

  // room 0,2
  { room: [0,2], name: "enemy020", type: "badguy", sprite: "badguy", x: 5, y: 3.375, collision1: 4, collision2: 4 },
  { room: [0,2], name: "enemy021", type: "badguy", sprite: "badguy", x: 11, y: 6, collision1: 4, collision2: 4 },

  // room 0,3
  { room: [0,3], name: "treasure030", type: "treasure", type_param: "10", sprite: "treasure", x: 4, y: 9.5, collision1: 4, collision2: 4 },
  { room: [0,3], name: "treasure031", type: "treasure", type_param: "10", sprite: "treasure", x: 8.875, y: 9.5, collision1: 4, collision2: 4 },

  // room 0,4
  { room: [0,4], name: "doorlock040", type: "doorlock", type_param: "door040", sprite: "lock", x: 9.25, y: 5.375, collision1: 4, collision2: 4 },
  { room: [0,4], name: "doorlock041", type: "doorlock", type_param: "door041", sprite: "lock", x: 5.25, y: 9.375, collision1: 4, collision2: 4 },
  { room: [0,4], name: "door040", type: "vertdoor", sprite: "vertdoor", x: 9, y: 6, collision1: 4, collision2: 4 },
  { room: [0,4], name: "door041", type: "horzdoor", sprite: "horzdoor", x: 6, y: 9, collision1: 4, collision2: 4 },

  // room 0,5 nothing...

  // room 0,6
  { room: [0,6], name: "doorlock060", type: "doorlock", sprite: "lockface", x: 8, y: 9.25, collision1: 4, collision2: 4 },
  { room: [0,6], name: "door060", type: "vertdoor", sprite: "vertdoor", x: 5.625, y: 5, collision1: 4, collision2: 4 },
  { room: [0,6], name: "doorlock061", type: "doorlock", type_param: "door160", sprite: "item", x: 4.625, y: 5.875, collision1: 4, collision2: 4 },

  // room 0,7 nothing.  end!




  // room 1,0
  { room: [1,0], name: "benign100", type: "benign", sprite: "benign", x: 6.5, y: 6.75, collision1: 4, collision2: 4 },

  // room 1,1

  // room 1,2
  { room: [1,2], name: "candy120", type: "candy", sprite: "item", x: 2.125, y: 11.5, collision1: 4, collision2: 4 },

  // room 1,3
  { room: [1,3], name: "doorlock130", type: "doorlock", type_param: "door130", sprite: "lock", x: 3.25, y: 3.375, collision1: 4, collision2: 4 },
  { room: [1,3], name: "door130", type: "horzdoor", sprite: "horzdoor", x: 6, y: 2.75, collision1: 4, collision2: 4 },
  { room: [1,3], name: "enemy130", type: "badguy", sprite: "badguy", x: 9.125, y: 5.25, collision1: 4, collision2: 4 },
  { room: [1,3], name: "enemy131", type: "badguy", sprite: "badguy", x: 7, y: 8, collision1: 4, collision2: 4 },

  // room 1,4
  { room: [1,4], name: "door140", type: "horzdoor", sprite: "horzdoor", x: 6, y: 8, collision1: 4, collision2: 4 },
  { room: [1,4], name: "musicbox140", type: "musicbox", type_params: "piano461, LEFT, SHOOT", sprite: "musicbox", x: 6.5, y: 10.375, collision1: 4, collision2: 4 },

  // room 1,5
  { room: [1,5], name: "doorlock150", type: "doorlock", sprite: "lockface", x: 7, y: 5.25, collision1: 4, collision2: 4 },
  { room: [1,5], name: "door150", type: "vertdoor", sprite: "vertdoor", x: 5.625, y: 8, collision1: 4, collision2: 4 },
  { room: [1,5], name: "doorlock151", type: "doorlock", type_param: "door060", sprite: "item", x: 4.625, y: 8.875, collision1: 4, collision2: 4 },
  { room: [1,5], name: "enemy150", type: "badguy", sprite: "badguy", x: 10.375, y: 3.625, collision1: 4, collision2: 4 },

  // room 1,6
  { room: [1,6], name: "doorlock160", type: "doorlock", sprite: "lockface", x: 5.125, y: 2.25, collision1: 4, collision2: 4 },
  { room: [1,6], name: "door160", type: "horzdoor", sprite: "horzdoor", x: 4, y: 9, collision1: 4, collision2: 4 },
  { room: [1,6], name: "doorlock161", type: "doorlock", type_param: "door260", sprite: "item", x: 4.875, y: 10, collision1: 4, collision2: 4 },

  // room 1,7




  // room 2,0
  { room: [2,0], name: "crown200", type: "trigger", type_params: "trap200, TRIGGER, TOUCH", sprite: "crown", x: 6.375, y: 3.25, collision1: 4, collision2: 4 },
  { room: [2,0], name: "trap200", type: "trap", type_params: "0,-2,-1", sprite: "trap", x: 6.625, y: 9.75, collision1: 4, collision2: 4 },

  // room 2,1
  { room: [2,1], name: "teleswitch210", type: "doorlock", sprite: "tele", x: 2.25, y: 10.25, collision1: 4, collision2: 4 },
  { room: [2,1], name: "teleport210", type: "tele", sprite: "badguy", x: 10.25, y: 10.375, collision1: 4, collision2: 4 },

  // room 2,2
  { room: [2,2], name: "lock220", type: "trigger", type_params: "trap200, UP,    TOUCH", sprite: "item", x: 5.375, y: 8.25, collision1: 4, collision2: 4 },
  { room: [2,2], name: "lock221", type: "trigger", type_params: "trap200, DOWN,  TOUCH", sprite: "item", x: 8.375, y: 5.5, collision1: 4, collision2: 4 },
  { room: [2,2], name: "lock222", type: "trigger", type_params: "trap200, LEFT,  TOUCH", sprite: "item", x: 9.25, y: 8.125, collision1: 4, collision2: 4 },
  { room: [2,2], name: "lock223", type: "trigger", type_params: "trap200, RIGHT, TOUCH", sprite: "item", x: 3.375, y: 5.5, collision1: 4, collision2: 4 },
  { room: [2,2], name: "face220", type: "doorlock", type_param: "door320", sprite: "lockface", x: 11.125, y: 9.125, collision1: 4, collision2: 4 },

  // room 2,3
  { room: [2,3], name: "enemy230", type: "badguy", sprite: "badguy", x: 8.25, y: 4, collision1: 4, collision2: 4 },
  { room: [2,3], name: "enemy231", type: "badguy", sprite: "badguy", x: 4.125, y: 4.75, collision1: 4, collision2: 4 },

  // room 2,4
  { room: [2,4], name: "doorlock240", type: "doorlock", sprite: "lockface", x: 2, y: 8.25, collision1: 4, collision2: 4 },
  { room: [2,4], name: "door240", type: "horzdoor", sprite: "horzdoor", x: 6, y: 5.75, collision1: 4, collision2: 4 },
  { room: [2,4], name: "doorlock241", type: "doorlock", type_param: "door360", sprite: "item", x: 6.75, y: 4.875, collision1: 4, collision2: 4 },

  // room 2,5
  { room: [2,5], name: "doorlock250", type: "doorlock", type_param: "door350", sprite: "lockface", x: 8, y: 8.25, collision1: 4, collision2: 4 },
  { room: [2,5], name: "door250", type: "horzdoor", sprite: "horzdoor", x: 4, y: 5.75, collision1: 4, collision2: 4 },
  { room: [2,5], name: "key251", type: "key", type_param: "door140", sprite: "key", x: 4.625, y: 4.625, collision1: 4, collision2: 4 },

  // room 2,6
  { room: [2,6], name: "doorlock260", type: "doorlock", sprite: "lockface", x: 11, y: 8.25, collision1: 4, collision2: 4 },
  { room: [2,6], name: "door260", type: "vertdoor", sprite: "vertdoor", x: 5.625, y: 3.875, collision1: 4, collision2: 4 },
  { room: [2,6], name: "doorlock261", type: "doorlock", type_param: "door240", sprite: "item", x: 4.625, y: 4.875, collision1: 4, collision2: 4 },

  // room 2,7



  // room 3,0
  { room: [3,0], name: "shooter300", type: "shooter", type_params: "1,0, 60", sprite: "box", x: 2.5, y: 3.25, collision1: 4, collision2: 4 },
  { room: [3,0], name: "leftdoor300", type: "vertdoor", sprite: "vertdoor", x: "4.25", y: 2.5, collision1: 4, collision2: 4 },
  { room: [3,0], name: "rightdoor300", type: "vertdoor", sprite: "vertdoor", x: "9.25", y: 2.5, collision1: 4, collision2: 4 },
  { room: [3,0], name: "face300", type: "doorlock", type_param: "entrydoor300", sprite: "lockface", x: 10.75, y: 3.25, collision1: 4, collision2: 4 },
  { room: [3,0], name: "musicbox301", type: "musicbox", type_params: "piano461, DOWN, SHOOT", sprite: "musicbox", x: 6.75, y: 7, collision1: 4, collision2: 4 },
  { room: [3,0], name: "entrydoor300", type: "horzdoor", sprite: "horzdoor", x: "6", y: 11, collision1: 4, collision2: 4 },
  { room: [3,0], name: "lock300", type: "doorlock", type_params: "leftdoor300", sprite: "item", x: 2.625, y: 6, collision1: 4, collision2: 4 },
  { room: [3,0], name: "lock301", type: "doorlock", type_params: "rightdoor300", sprite: "item", x: 11.375, y: 6, collision1: 4, collision2: 4 },
  //sprite panpad300 = { 48, 33, nobitmap };
  //sprite panpad301 = { 48, 33, nobitmap };
  //sprite panpad302 = { 702 - 639, 190 - 79, nobitmap };
  //{ room: [3,0], name: "panpad300", type: "panpad", type_params: "60,0", sprite: "panpad300", x: "575px", y: "79px",   collision1: 0, collision2: 0, collision_type:COLLISION_TRIGGER ],
  //{ room: [3,0], name: "panpad301", type: "panpad", type_params: "-60, 0", sprite: "panpad301", x: "719px", y: "79px", collision1: 0, collision2: 0, collision_type:COLLISION_TRIGGER ],
  //{ room: [3,0], name: "panpad302", type: "interppad", type_params: "0,0, 0,-70", sprite: "panpad302", x: "639px", y: "79px", collision1: 0, collision2: 0, collision_type:COLLISION_TRIGGER ],

  // room 3,1
  { room: [3,1], name: "treasure310", type: "treasure", type_param: "10", sprite: "treasure", x: 6.5, y: 6.75, collision1: 4, collision2: 4 },

  // room 3,2
  { room: [3,2], name: "treasure320", type: "treasure", type_param: "10", sprite: "treasure", x: 10.75, y: 9.125, collision1: 4, collision2: 4 },
  { room: [3,2], name: "benign320", type: "benign", type_params: "2, 0", sprite: "benign", x: 8.125, y: 10, collision1: 4, collision2: 4 },
  { room: [3,2], name: "benign321", type: "benign", type_params: "2, 0", sprite: "benign", x: 4.25, y: 9.375, collision1: 4, collision2: 4 },
  { room: [3,2], name: "door320", type: "horzdoor", sprite: "horzdoor", x: 6, y: 8.75, collision1: 4, collision2: 4 },
  { room: [3,2], name: "illumina320", type: "illumina", type_param: "treasure", sprite: "illumina", x: 2, y: 11.5, collision1: 4, collision2: 4 },

  // room 3,3
  { room: [3,3], name: "enemy330", type: "badguy", sprite: "badguy", x: 5.375, y: 3.75, collision1: 4, collision2: 4 },
  { room: [3,3], name: "enemy331", type: "badguy", sprite: "badguy", x: 9.375, y: 3.125, collision1: 4, collision2: 4 },
  { room: [3,3], name: "enemy332", type: "badguy", sprite: "badguy", x: 8.25, y: 10.25, collision1: 4, collision2: 4 },

  // room 3,4
  { room: [3,4], name: "doorlock340", type: "doorlock", sprite: "lockface", x: 8, y: 8.25, collision1: 4, collision2: 4 },
  { room: [3,4], name: "door340", type: "horzdoor", sprite: "horzdoor", x: 4, y: 5.75, collision1: 4, collision2: 4 },
  { room: [3,4], name: "doorlock341", type: "doorlock", type_param: "door250", sprite: "item", x: 4.875, y: 4.875, collision1: 4, collision2: 4 },

  // room 3,5
  { room: [3,5], name: "doorlock350", type: "doorlock", sprite: "lockface", x: 5, y: 4.25, collision1: 4, collision2: 4 },
  { room: [3,5], name: "door350", type: "vertdoor", sprite: "vertdoor", x: 5.625, y: 5.875, collision1: 4, collision2: 4 },
  { room: [3,5], name: "doorlock351", type: "doorlock", type_param: "door150", sprite: "item", x: 4.75, y: 6.875, collision1: 4, collision2: 4 },

  // room 3,6
  { room: [3,6], name: "doorlock360", type: "doorlock", sprite: "lockface", x: 3, y: 5.25, collision1: 4, collision2: 4 },
  { room: [3,6], name: "door360", type: "vertdoor", sprite: "vertdoor", x: 7.875, y: 3.875, collision1: 4, collision2: 4 },
  { room: [3,6], name: "doorlock361", type: "doorlock", type_param: "door340", sprite: "item", x: 8.75, y: 4.875, collision1: 4, collision2: 4 },

  // room 3,7



  // room 4,0
  { room: [4,0], name: "suck400", type: "suck", type_params: "1, -4 ", sprite: "suck", x: 6.25, y: 11.5, collision1: 4, collision2: 4 },
  { room: [4,0], name: "suck401", type: "suck", type_params: "-1, -4 ", sprite: "suck", x: 6.25, y: 2.125, collision1: 4, collision2: 4 },

  // room 4,1

  // room 4,2

  // room 4,3
  { room: [4,3], name: "enemy430", type: "badguy", sprite: "badguy", x: 8.625, y: 9.625, collision1: 4, collision2: 4 },
  { room: [4,3], name: "enemy431", type: "badguy", sprite: "badguy", x: 5.625, y: 9.5, collision1: 4, collision2: 4 },
  { room: [4,3], name: "enemy432", type: "badguy", sprite: "badguy", x: 7, y: 8, collision1: 4, collision2: 4 },

  // room 4,4
  { room: [4,4], name: "enemy440", type: "badguy", sprite: "badguy", x: 10.125, y: 10.375, collision1: 4, collision2: 4 },
  { room: [4,4], name: "enemy441", type: "badguy", sprite: "badguy", x: 4.375, y: 2.375, collision1: 4, collision2: 4 },
  { room: [4,4], name: "enemy442", type: "badguy", sprite: "badguy", x: 11.125, y: 5.125, collision1: 4, collision2: 4 },
  { room: [4,4], name: "enemy443", type: "badguy", sprite: "badguy", x: 2.125, y: 4.25, collision1: 4, collision2: 4 },

  // room 4,5
  { room: [4,5], name: "enemy450", type: "badguy", sprite: "badguy", x: 9.25, y: 11.125, collision1: 4, collision2: 4 },
  { room: [4,5], name: "enemy451", type: "badguy", sprite: "badguy", x: 9.25, y: 2.25, collision1: 4, collision2: 4 },
  { room: [4,5], name: "enemy452", type: "badguy", sprite: "badguy", x: 3.25, y: 3.25, collision1: 4, collision2: 4 },
  { room: [4,5], name: "enemy453", type: "badguy", sprite: "badguy", x: 10.75, y: 4.125, collision1: 4, collision2: 4 },

  // room 4,6
  { room: [4,6], name: "door460", type: "horzdoor", sprite: "horzdoor", x: 6, y: 10, collision1: 4, collision2: 4 },
  { room: [4,6], name: "door461", type: "horzdoor", type_params: "false,-1", sprite: "horzdoor", x: 6, y: 8, collision1: 4, collision2: 4 },
  { room: [4,6], name: "doorlock460", type: "tonegen", sprite: "locklisten", x: 5, y: 7.25, collision1: 4, collision2: 4 },
  { room: [4,6], name: "piano461", type: "statetrigger", type_params: "door460, door461", sprite: "piano461", x: 5, y: 7.25, collision1: 4, collision2: 4, collision_type: COLLISION_TRIGGER },


  // room 4,7
  { room: [4,7], name: "treasure470", type: "treasure", type_param: "10", sprite: "treasure", x: 9.875, y: 6.5, collision1: 4, collision2: 4 },
  { room: [4,7], name: "treasure471", type: "treasure", type_param: "10", sprite: "treasure", x: 9.875, y: 6, collision1: 4, collision2: 4 },
  { room: [4,7], name: "treasure472", type: "treasure", type_param: "10", sprite: "treasure", x: 9.875, y: 5.5, collision1: 4, collision2: 4 },




  // room 5,0
  { room: [5,0], name: "enemy500", type: "badguy", sprite: "badguy", x: 7.25, y: 10.375, collision1: 4, collision2: 4 },
  { room: [5,0], name: "enemy501", type: "badguy", sprite: "badguy", x: 7.25, y: 2.25, collision1: 4, collision2: 4 },

  // room 5,1

  // room 5,2

  // room 5,3

  // room 5,4
  { room: [5,4], name: "up540", type: "trigger", type_params: "player540, UP, TOUCH", sprite: "item", x: 11.375, y: 10.75, collision1: 4, collision2: 4 },
  { room: [5,4], name: "down541", type: "trigger", type_params: "player540, DOWN, TOUCH", sprite: "item", x: 9.125, y: 10.75, collision1: 4, collision2: 4 },
  { room: [5,4], name: "stop542", type: "trigger", type_params: "player540, STOP, TOUCH", sprite: "item", x: 10.25, y: 11.625, collision1: 4, collision2: 4 },
  { room: [5,4], name: "shoot543", type: "trigger", type_params: "player540, SHOOT, TOUCH", sprite: "item", x: 11.375, y: 10.375, collision1: 4, collision2: 4 },
  { room: [5,4], name: "face544", type: "doorlock", type_param: "door650", sprite: "lockface", x: 6.875, y: 6.75, collision1: 4, collision2: 4 },
  { room: [5,4], name: "player540", type: "playerdrone", sprite: "other", x: 2, y: 2, collision1: 4, collision2: 4 },
  { room: [5,4], name: "triggerpad540", type: "triggerpad", type_params: "player540, EXIT", sprite: "triggerpad540", x: "1152px", y: "847px", collision1: 0, collision2: 0, collision_type: COLLISION_TRIGGER },

  // room 5,5
  { room: [5,5], name: "bonus550", type: "candy", sprite: "bonus", x: 7, y: 7.75, collision1: 4, collision2: 4 },
  { room: [5,5], name: "down550", type: "trigger", type_params: "player550, DOWN, TOUCH", sprite: "item", x: 9, y: 10.75, collision1: 4, collision2: 4 },
  { room: [5,5], name: "up551", type: "trigger", type_params: "player550, UP, TOUCH", sprite: "item", x: 11.5, y: 10.75, collision1: 4, collision2: 4 },
  { room: [5,5], name: "shoot552", type: "trigger", type_params: "player550, SHOOT, TOUCH", sprite: "item", x: 11.5, y: 10.375, collision1: 4, collision2: 4 },
  { room: [5,5], name: "face550", type: "doorlock", type_param: "door660", sprite: "lockface", x: 6.875, y: 6.75, collision1: 4, collision2: 4 },
  { room: [5,5], name: "player550", type: "playerdrone", sprite: "other", x: 2, y: 2.125, collision1: 4, collision2: 4 },
  { room: [5,5], name: "triggerpad550", type: "triggerpad", type_params: "player550, EXIT", sprite: "triggerpad550", x: "1152px", y: "1039px", collision1: 0, collision2: 0, collision_type: COLLISION_TRIGGER },

  // room 5,6
  { room: [5,6], name: "down560", type: "trigger", type_params: "player560, DOWN, TOUCH", sprite: "item", x: 9, y: 10.75, collision1: 4, collision2: 4 },
  { room: [5,6], name: "shoot561", type: "trigger", type_params: "player560, SHOOT, TOUCH", sprite: "item", x: 11.5, y: 10.25, collision1: 4, collision2: 4 },
  { room: [5,6], name: "face560", type: "doorlock", type_param: "door670", sprite: "lockface", x: 6.875, y: 6.75, collision1: 4, collision2: 4 },
  { room: [5,6], name: "player560", type: "playerdrone", sprite: "other", x: 2, y: 2, collision1: 4, collision2: 4 },
  { room: [5,6], name: "triggerpad560", type: "triggerpad", type_params: "player560, EXIT", sprite: "triggerpad560", x: "1152px", y: "1231px", collision1: 0, collision2: 0, collision_type: COLLISION_TRIGGER },

  // room 5,7
  { room: [5,7], name: "trigger570", type: "doorlock", type_params: "door570, TRIGGER, TOUCH", sprite: "item", x: 11.5, y: 9.375, collision1: 4, collision2: 4 },
  { room: [5,7], name: "trigger571", type: "doorlock", type_params: "door571, TRIGGER, TOUCH", sprite: "item", x: 11.375, y: 10, collision1: 4, collision2: 4 },
  { room: [5,7], name: "trigger572", type: "doorlock", type_params: "door572, TRIGGER, TOUCH", sprite: "item", x: 11.5, y: 10.25, collision1: 4, collision2: 4 },
  { room: [5,7], name: "trigger573", type: "doorlock", type_params: "door573, TRIGGER, TOUCH", sprite: "item", x: 11.375, y: 10.625, collision1: 4, collision2: 4 },
  { room: [5,7], name: "trigger574", type: "doorlock", type_params: "door574, TRIGGER, TOUCH", sprite: "item", x: 10.25, y: 11.625, collision1: 4, collision2: 4 },
  { room: [5,7], name: "trigger575", type: "doorlock", type_params: "door575, TRIGGER, TOUCH", sprite: "item", x: 9.125, y: 10.5, collision1: 4, collision2: 4 },
  { room: [5,7], name: "trigger576", type: "doorlock", type_params: "door576, TRIGGER, TOUCH", sprite: "item", x: 9.25, y: 9.5, collision1: 4, collision2: 4 },
  { room: [5,7], name: "trigger577", type: "doorlock", sprite: "item", x: 9, y: 9.25, collision1: 4, collision2: 4 },
  { room: [5,7], name: "face570", type: "doorlock", type_param: "door671", sprite: "lockface", x: 6.75, y: 6.75, collision1: 4, collision2: 4 },
  { room: [5,7], name: "enemy570", type: "badguy", sprite: "badguy", x: 2.125, y: 2.125, collision1: 4, collision2: 4 },
  { room: [5,7], name: "door570", type: "vertdoor", sprite: "vertdoor", x: 5.625, y: 6, collision1: 4, collision2: 4 },
  { room: [5,7], name: "door571", type: "vertdoor", sprite: "vertdoor", x: 3.625, y: 2, collision1: 4, collision2: 4 },
  { room: [5,7], name: "door572", type: "vertdoor", sprite: "vertdoor", x: 3.625, y: 6, collision1: 4, collision2: 4 },
  { room: [5,7], name: "door573", type: "vertdoor", sprite: "vertdoor", x: 5.75, y: 2, collision1: 4, collision2: 4 },
  { room: [5,7], name: "door574", type: "horzdoor", sprite: "horzdoor", x: 4, y: 4, collision1: 4, collision2: 4 },
  { room: [5,7], name: "door575", type: "horzdoor", sprite: "horzdoor", x: 6, y: 5.875, collision1: 4, collision2: 4 },
  { room: [5,7], name: "door576", type: "horzdoor", sprite: "horzdoor", x: 6, y: 7.875, collision1: 4, collision2: 4 },



  // room 6,0
  { room: [6,0], name: "door600", type: "vertdoor", sprite: "vertdoor", x: 5.625, y: 5.875, collision1: 4, collision2: 4 },

  // room 6,1
  { room: [6,1], name: "door610", type: "horzdoor", sprite: "horzdoor", x: 6, y: 5, collision1: 4, collision2: 4 },
  { room: [6,1], name: "doorlock610", type: "doorlock", type_params: "door600", sprite: "lockface", x: 3.125, y: 9.875, collision1: 4, collision2: 4 },
  { room: [6,1], name: "musicbox610", type: "musicbox", type_params: "piano461, RIGHT, SHOOT", sprite: "musicbox", x: 3, y: 6.625, collision1: 4, collision2: 4 },
  { room: [6,1], name: "touchpad610", type: "touchpad", type_params: "12", sprite: "touchpad610", x: 1200, y: 193, collision1: 0, collision2: 0, collision_type: COLLISION_TRIGGER },
  //1200,193   1293, 304

  // room 6,2
  { room: [6,2], name: "door620", type: "vertdoor", type_params: "false,-1,64", sprite: "vertdoor", x: 8.625, y: 1.875, collision1: 4, collision2: 4 },
  { room: [6,2], name: "doorlock620", type: "doorlock", type_params: "door620", sprite: "lockface", x: 2.875, y: 11.625, collision1: 4, collision2: 4 },
  { room: [6,2], name: "key620", type: "key", type_params: "door610", sprite: "key", x: 2.875, y: 7.125, collision1: 4, collision2: 4 },

  // room 6,3
  { room: [6,3], name: "candy630", type: "candy", sprite: "item", x: 2.875, y: 5.25, collision1: 4, collision2: 8 },
  { room: [6,3], name: "candy631", type: "candy", sprite: "item", x: 2, y: 5.5, collision1: 4, collision2: 8 },
  { room: [6,3], name: "candy632", type: "candy", sprite: "item", x: 2, y: 5, collision1: 4, collision2: 8 },
  { room: [6,3], name: "candy633", type: "candy", sprite: "item", x: 2.875, y: 5, collision1: 4, collision2: 8 },
  { room: [6,3], name: "candy634", type: "candy", sprite: "item", x: 2.375, y: 5.5, collision1: 4, collision2: 8 },
  { room: [6,3], name: "candy635", type: "candy", sprite: "item", x: 2, y: 5.25, collision1: 4, collision2: 8 },
  { room: [6,3], name: "candy636", type: "candy", sprite: "item", x: 2.375, y: 5, collision1: 4, collision2: 8 },
  { room: [6,3], name: "candy637", type: "candy", sprite: "item", x: 2.875, y: 5.5, collision1: 4, collision2: 8 },
  { room: [6,3], name: "doorlock630", type: "doorlock", type_param: "door631", sprite: "lock", x: 1.25, y: 8.375, collision1: 4, collision2: 4 },
  { room: [6,3], name: "door630", type: "vertdoor", sprite: "vertdoor", x: 9, y: 6, collision1: 4, collision2: 4 },
  { room: [6,3], name: "doorlock631", type: "doorlock", type_param: "door630", sprite: "lock", x: 9.25, y: 8.25, collision1: 4, collision2: 4 },
  { room: [6,3], name: "door631", type: "horzdoor", sprite: "horzdoor", x: 6, y: 9, collision1: 4, collision2: 4 },
  { room: [6,3], name: "panpad630", type: "interppad", type_params: " -28,0,   0,0", sprite: "panpad630", x: "1086px", y: "1423px", collision1: 0, collision2: 0, collision_type: COLLISION_TRIGGER },


  // room 6,4

  // room 6,5
  { room: [6,5], name: "enemy650", type: "badguy", sprite: "badguy", x: 3.75, y: 6.75, collision1: 4, collision2: 4 },
  { room: [6,5], name: "enemy651", type: "badguy", sprite: "badguy", x: 8.375, y: 7.25, collision1: 4, collision2: 4 },
  { room: [6,5], name: "door650", type: "vertdoor", sprite: "vertdoor", x: 5.625, y: 6, collision1: 4, collision2: 4 },
  { room: [6,5], name: "door651", type: "vertdoor", sprite: "vertdoor", x: 8, y: 6, collision1: 4, collision2: 4 },

  // room 6,6
  { room: [6,6], name: "enemy660", type: "badguy", sprite: "badguy", x: 3.75, y: 6.5, collision1: 4, collision2: 4 },
  { room: [6,6], name: "enemy661", type: "badguy", sprite: "badguy", x: 9.75, y: 6.75, collision1: 4, collision2: 4 },
  { room: [6,6], name: "door660", type: "vertdoor", sprite: "vertdoor", x: 5.625, y: 6, collision1: 4, collision2: 4 },
  { room: [6,6], name: "door661", type: "vertdoor", sprite: "vertdoor", x: 8, y: 6, collision1: 4, collision2: 4 },


  // room 6,7
  { room: [6,7], name: "enemy670", type: "badguy", sprite: "badguy", x: 2.75, y: 6.5, collision1: 4, collision2: 4 },
  { room: [6,7], name: "enemy671", type: "badguy", sprite: "badguy", x: 9.25, y: 6.875, collision1: 4, collision2: 4 },
  { room: [6,7], name: "door670", type: "vertdoor", sprite: "vertdoor", x: 5.625, y: 6, collision1: 4, collision2: 4 },
  { room: [6,7], name: "door671", type: "vertdoor", sprite: "vertdoor", x: 8, y: 6, collision1: 4, collision2: 4 },





  // room 7,0

  // room 7,1
  { room: [7,1], name: "redirect710", type: "redirect", type_params: "0, -1", sprite: "box", x: 6.625, y: 8.75, collision1: 4, collision2: 4 },
  { room: [7,1], name: "redirect711", type: "redirect", type_params: "-1, 0", sprite: "box", x: 6.625, y: 4.75, collision1: 4, collision2: 4 },
  { room: [7,1], name: "shooter710", type: "shooter", type_params: "-1, 0, 15", sprite: "badguy", x: 9.875, y: 8.875, collision1: 4, collision2: 4 },
  { room: [7,1], name: "doorlock710", type: "doorlock", sprite: "badguy", x: 3.75, y: 4.875, collision1: 4, collision2: 4 },

  // room 7,2

  // room 7,3

  // room 7,4

  // room 7,5

  // room 7,6
  { room: [7,6], name: "trigger760", type: "trigger", type_params: "door760, TRIGGER, TOUCH", sprite: "item", x: 4.625, y: 11.75, collision1: 4, collision2: 4 },
  { room: [7,6], name: "trigger761", type: "trigger", type_params: "randshooter760, TRIGGER, TOUCH", sprite: "item", x: 2.75, y: 4.125, collision1: 4, collision2: 4 },
  { room: [7,6], name: "door760", type: "horzdoor", type_params: "false,-1", sprite: "horzdoor", x: 5, y: 9.875, collision1: 4, collision2: 4 },
  { room: [7,6], name: "doorlock760", type: "doorlock", type_param: "mover762", sprite: "badguy", x: 7.25, y: 2.125, collision1: 4, collision2: 4 },
  { room: [7,6], name: "randshooter760", type: "shooter", type_params: "0,-2,30,60,3,false", sprite: "box", x: 7.25, y: 8, collision1: 4, collision2: 4 },
  { room: [7,6], name: "redirect760", type: "redirect", type_params: "-1,0", sprite: "box", x: 5.125, y: 7.375, collision1: 4, collision2: 4 },
  { room: [7,6], name: "redirect761", type: "redirect", type_params: "1,0", sprite: "box", x: 5.375, y: 4.125, collision1: 4, collision2: 4 },
  { room: [7,6], name: "redirect762", type: "redirect", sprite: "box", x: 8.875, y: 9.125, collision1: 4, collision2: 4 },
  { room: [7,6], name: "mover762", type: "shooter", type_params: "0, 1, -1", sprite: "trap", x: 11.125, y: 2.125, collision1: 4, collision2: 4 },

  // room 7,7
  { room: [7,7], name: "door770", type: "horzdoor", sprite: "horzdoor", x: 10, y: 10, collision1: 4, collision2: 4 },
  { room: [7,7], name: "door771", type: "horzdoor", sprite: "horzdoor", x: 10, y: 3.75, collision1: 4, collision2: 4 },
  { room: [7,7], name: "door772", type: "horzdoor", sprite: "horzdoor", x: 5, y: 3.75, collision1: 4, collision2: 4 },
  { room: [7,7], name: "enemy770", type: "badguy", sprite: "badguy", x: 6.875, y: 5, collision1: 4, collision2: 4 },
  { room: [7,7], name: "enemy771", type: "badguy", sprite: "badguy", x: 8.25, y: 7.75, collision1: 4, collision2: 4 },
  { room: [7,7], name: "trigger770", type: "doorlock", type_param: "door770", sprite: "item", x: 9.625, y: 9.75, collision1: 4, collision2: 4 },
  { room: [7,7], name: "trigger771", type: "doorlock", type_param: "door771", sprite: "item", x: 9.625, y: 4, collision1: 4, collision2: 4 },
  { room: [7,7], name: "trigger772", type: "doorlock", type_param: "door772", sprite: "item", x: 4.5, y: 2, collision1: 4, collision2: 4 },
  { room: [7,7], name: "chalice772", type: "chalice", sprite: "chalice", x: 10.75, y: 10.875, collision1: 4, collision2: 4 },
  { room: [7,7], name: "face770", type: "doorlock", type_param: "door661", sprite: "lockface", x: 10.5, y: 2.75, collision1: 4, collision2: 4 },
  { room: [7,7], name: "flit770", type: "flit", sprite: "flit", x: 5.125, y: 8.75, collision1: 4, collision2: 4 },
], // actor_layout
};
