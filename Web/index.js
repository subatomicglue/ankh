


// a tile map
// map is defined with an image containing JxK sprite tiles, and an NxM grid of indexes into those sprite tiles. 
//   filename = the image containing a grid of sprite tiles
//   width = width of the image in pixels  (one sprite will be width/tilesx)
//   height = height of the image in pixels (one sprite will be height/tilesy)
//   tilesx/y = how many sprite tiles are in the image
//   map      = grid of indexes into those sprite tiles (what sprite to show for each tile)
//   mapx     = how wide is the map grid
//   collidable = the indexes that result in a collision, when calling Map.collide() 
//
// example usage:
//   let map = new Map( "walls.jpg", 8, 16, 32, [2,2,2,2, 2,3,4,2, 2,5,6,2, 2,2,2,2], [2, 6] )
//   map.draw( canvas.getContext("2d") );
function Map( filename, tilesx = 8, tilesy = 16, mapx = 4, map = [2,2,2,2, 2,3,4,2, 2,5,6,2, 2,2,2,2], collidable = [2, 7, 12, 17] ) {
  this.img = new Image();    // a new empty image
  this.img.addEventListener( "load", () => {
    this.width = this.img.width / this.tilesx;
    this.height = this.img.height / this.tilesy;
    console.log( `Map ${filename} loaded ${this.img.width} x ${this.img.height} total, ${this.width} x ${this.height} per tile, ${tilesx} x ${tilesy} tiles`)
  });
  this.img.src = filename;// loads the image
  this.tilesx = tilesx;   // number of x sprites in the image
  this.tilesy = tilesy;   // number of y sprites in the image
  this.mapx = mapx;       // number of x sprites in the map
  this.map = map;         // the sprite layout to draw.  e.g. [ 2,2,2,2 ], where each ID is an index into the image counting from left to right, starting at top left.
  this.collidable = collidable; // which sprite indexes are collidable
  this.x = 0;             // offset to draw the map into the canvas
  this.y = 0;             // offset to draw the map into the canvas

  this.draw = function( ctx ) {
    // todo: detect parent viewport extents, dont bother drawing out of bounds

    for (let i = 0; i < map.length; ++i) {
      let x = i % mapx;
      let y = Math.floor( i / mapx );
      let tx = map[i] % tilesx;
      let ty = Math.floor( map[i] / tilesx );

      // draws the image into the canvas
      ctx.drawImage( this.img,
        tx * this.width, ty * this.height,
        this.width, this.height, // original image pixels
        this.x + x * this.width, this.y + y * this.height,
        this.width, this.height  // new pos/size you want to draw
      );
    }
  }

  function mapXYtoIndex( x, y ) {
    return x + y * this.mapx;
  }

  // does x,y intersect a collidable tile?
  this.collide = function ( x, y ) {
    let mapx = Math.floor( x / this.width - this.x + 1);
    let mapy = Math.floor( y / this.height - this.y + 1 );
    let mapindex = mapx + mapy * this.mapx; // get the map grid location of x,y
    let id = this.map[mapindex];            // get the tile ID at the grid location 
    return this.collidable.find( r => r == id ); // is the tile ID collidable?
  }

  // does the box intersect a collidable tile?
  this.collideBox = function ( x, y, width, height ) {
    let topleft = vec( x, y );
    let botright = vec( x+width, y+height );
    // what map tiles does the given box pass through?
    let min_map = floor( sub( div( topleft, vec( this.width, this.height ) ), vec( this.x, this.y ) ) );
    let max_map = floor( sub( div( botright, vec( this.width, this.height ) ), vec( this.x, this.y ) ) );
    // if any of those tiles are collidable, signal a collision
    for (let j = min_map.y; j <= max_map.y; ++j) {
      for (let i = min_map.x; i <= max_map.x; ++i) {
        let index = i + j * this.mapx;
        let id = this.map[index];
        if (this.collidable.find( r => r == id )) {
          return true;
        }
      }
    }
    return false; // no collision
  }
}

// a sprite
// sprite is defined with an image containing JxK sprite tiles, and animation sequences of those. 
//   filename   = the image containing a grid of sprite tiles
//   x/y        = the position of the sprite    (use this for initial position)
//   dx/dy      = the velocity of the sprite    (use this to move)
//   width      = width of the sprite in pixels (image width == width * tilesx)
//   height     = height of the sprite in pixels (image height == height * tilesy)
//   tilesx/y   = how many sprite tiles are in the image
//   sequences  = several named animations, each one containing a sequence of tiles and the speed to play
//   updateFunc = called for each draw()
//
// example usage:
//   let actor = new Sprite( "sprites.png", 40,40, 9,4, { default: {interval: 0.05, frames: [[0,3], ] } }, { x: 22, y: 15, w: 20, h: 48 }, actor => {} )
//   actor.draw( canvas.getContext("2d") );
function Sprite( filename, x, y, tilesx = 9, tilesy = 4, sequences = {default: {interval: 0.4, frames: [[0,0], [1,0]]} }, bbox = { x: 22, y: 15, w: 20, h: 48 }, updateFunc = (s) => {} ) {
  this.img = new Image();    // a new empty image
  this.img.addEventListener( "load", () => {
    this.width = this.img.width / this.tilesx
    this.height = this.img.height / this.tilesy
    console.log( `Sprite ${filename} loaded with ${this.width} x ${this.height}` )
    if (this.bbox == undefined) {
      this.bbox = { x: 0, y: 0, w: this.width, h: this.height }
      console.log( ` - autocompute bbox = ${this.bbox}` )
    }
  });
  this.img.src = filename;  // loads the image
  this.x = x;               // sprite x position
  this.y = y;               // sprite y position
  this.tilesx = tilesx;     // number of x sprites in the image
  this.tilesy = tilesy;     // number of y sprites in the image
  this.width = this.img.width / this.tilesx; // width of the sprite in pixels
  this.height = this.img.height / this.tilesy; // height of the sprite in pixels
  this.anim = 0;            // animation clock, controls which frame to display, loops
  this.sequences = sequences; // collection of named animation sequences, e.g. { default: {interval: 0.4, frames: [[0,0], [1,0]]}, ... }
  this.animation = this.sequences.default; // current sequence, e.g. {interval: 0.4, frames: [[0,0], [1,0]]}
  this.dx = 0;              // velocity in x
  this.dy = 0;              // velocity in y
  this.bbox = bbox;         // collision bounding box relative to x/y
  this.showbbox = false;    // debug: display the bounding box

  // draw the sprite using the given canvas ctx
  this.draw = function( ctx ) {   
    let frame = this.animation.frames[Math.floor( this.anim )]

    // draws the image into the canvas
    if (this.showbbox)
      ctx.fillRect( this.x + this.bbox.x,
                    this.y + this.bbox.y,
                    this.bbox.w,
                    this.bbox.h );
    //ctx.globalCompositeOperation = "screen";
    ctx.drawImage( this.img,
      frame[0] * this.width, frame[1] * this.height,
      this.width, this.height, // original image pixels
      this.x, this.y,
      this.width, this.height  // new pos/size you want to draw
    );

    this.anim = (this.anim + this.animation.interval) % this.animation.frames.length;
    //console.log( `${Math.floor( this.anim ) * this.width} ${Math.floor( this.bank ) * this.height}`)

    updateFunc( this );
  };

  // change animation
  this.changeSequence = ( name ) => {
    this.animation = this.sequences[name] ? this.sequences[name] : this.sequences.default;
    this.anim = 0;
  }
}

function collideAgainstMap( actor ) {
  // if (!actors[0].collide( actor.x + actor.dx * 1/fps, actor.y + actor.dy * 1/fps )) {
  //   actor.x += actor.dx * 1/fps; // move by dx every second
  //   actor.y += actor.dy * 1/fps; // move by dy every second
  // }

  if (!actors[0].collideBox( actor.x + actor.bbox.x + actor.dx * 1/fps,
                            actor.y + actor.bbox.y + actor.dy * 1/fps,
                            actor.bbox.w,
                            actor.bbox.h )) {
    actor.x += actor.dx * 1/fps; // move by dx every second
    actor.y += actor.dy * 1/fps; // move by dy every second
  }
}

// clear the canvas using the given canvas ctx
function clear( ctx ) {
  ctx.fillStyle = "#000000";                    // use black
  ctx.fillRect(0,0,canvas.width,canvas.height); // clear the canvas
}

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

let fps = 30;  // frame rate  (film is 24hz, TV at 60hz)
let actors = [
  // actors[0] is the map
  new Map( "assets/walls.png", 16, 8, 12*8, [

16, 32, 16, 32, 16, 32, 16, 32, 16, 32, 16, 32, 57, 58, 57, 58, 
57, 58, 57, 58, 57, 58, 57, 58, 32, 32, 32, 32, 32, 32, 32, 32, 
32, 32, 32, 32, 53, 12, 53, 12, 53, 12, 53, 12, 53, 12, 53, 12, 
55, 13, 55, 13, 55, 13, 55, 13, 55, 13, 55, 13, 50, 93, 50, 93, 
50, 93, 50, 93, 50, 93, 50, 93, 77, 61, 77, 61, 77, 61, 77, 61, 
77, 61, 77, 61, 89, 90, 89, 90, 89, 90, 89, 90, 89, 90, 89, 90
,
32, 16, 32, 16, 32, 16, 32, 16, 32, 16, 32, 16, 57, 58, 57, 58, 
57, 58, 57, 58, 57, 58, 57, 58, 32, 32, 32, 32, 32, 32, 32, 32, 
32, 32, 32, 32, 53, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 12, 
55, 13, 55, 13, 55, 93, 93, 13, 55, 13, 55, 13, 93, 50, 93, 50, 
93, 50, 93, 50, 93, 50, 93, 50, 61, 77, 61, 77, 61, 77, 61, 77, 
61, 77, 61, 77, 90, 89, 90, 89, 90, 89, 90, 89, 90, 89, 90, 89
,
16, 32, 16, 32, 93, 93, 93, 93, 16, 32, 16, 32, 57, 58, 93, 93, 
93, 93, 93, 93, 93, 93, 57, 58, 32, 32, 93, 32, 32, 93, 93, 32, 
32, 93, 32, 32, 53, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 12, 
55, 13, 93, 93, 93, 93, 93, 93, 93, 93, 55, 13, 50, 93, 50, 93, 
50, 93, 50, 93, 50, 93, 50, 93, 77, 61, 77, 61, 77, 61, 77, 61, 
77, 61, 77, 61, 89, 90, 89, 90, 89, 90, 89, 90, 89, 90, 89, 90
,
32, 16, 32, 93, 93, 93, 93, 93, 93, 16, 32, 16, 57, 58, 93, 93, 
93, 93, 93, 93, 93, 93, 57, 58, 32, 32, 32, 32, 32, 93, 93, 32, 
32, 32, 32, 32, 53, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 12, 
55, 13, 93, 93, 93, 93, 93, 93, 93, 93, 55, 13, 93, 50, 93, 50, 
93, 50, 93, 50, 93, 50, 93, 50, 61, 77, 61, 77, 61, 77, 61, 77, 
61, 77, 61, 77, 90, 89, 90, 89, 90, 89, 90, 89, 90, 89, 90, 89
,
16, 32, 93, 93, 93, 93, 93, 93, 93, 93, 16, 32, 57, 58, 93, 93, 
58, 57, 58, 57, 93, 93, 57, 58, 32, 32, 93, 93, 93, 93, 93, 93, 
93, 93, 32, 32, 53, 12, 53, 12, 53, 12, 53, 12, 53, 12, 53, 12, 
55, 13, 93, 93, 93, 93, 93, 93, 93, 93, 55, 13, 50, 93, 50, 93, 
50, 93, 50, 93, 50, 93, 50, 93, 77, 61, 77, 61, 77, 61, 77, 61, 
77, 61, 77, 61, 89, 90, 89, 90, 89, 90, 89, 90, 89, 90, 89, 90
,
32, 16, 93, 93, 93, 93, 93, 93, 93, 93, 32, 16, 57, 58, 93, 93, 
58, 93, 93, 57, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 12, 93, 93, 93, 93, 53, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 89, 90, 89, 90, 89
,
16, 32, 93, 93, 93, 93, 93, 93, 93, 93, 16, 32, 57, 58, 93, 93, 
58, 93, 93, 57, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 12, 93, 93, 93, 93, 53, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 90, 89, 90, 89, 90
,
32, 16, 93, 93, 93, 93, 93, 93, 93, 93, 32, 16, 57, 58, 93, 93, 
58, 57, 58, 57, 93, 93, 57, 58, 32, 32, 93, 93, 93, 93, 93, 93, 
93, 93, 32, 32, 53, 12, 53, 12, 93, 93, 93, 93, 53, 12, 53, 12, 
55, 13, 93, 93, 93, 93, 93, 93, 93, 93, 55, 13, 93, 50, 93, 50, 
93, 50, 93, 50, 93, 50, 93, 50, 61, 77, 61, 77, 61, 93, 93, 77, 
61, 77, 61, 77, 90, 89, 90, 89, 90, 93, 93, 89, 90, 89, 90, 89
,
16, 32, 16, 93, 93, 93, 93, 93, 93, 32, 16, 32, 57, 58, 93, 93, 
93, 93, 93, 93, 93, 93, 57, 58, 32, 32, 32, 32, 32, 93, 93, 32, 
32, 32, 32, 32, 53, 12, 93, 12, 93, 93, 93, 93, 53, 93, 53, 12, 
55, 13, 93, 93, 93, 93, 93, 93, 93, 93, 55, 13, 50, 93, 50, 93, 
50, 93, 50, 93, 50, 93, 50, 93, 77, 61, 77, 61, 77, 93, 93, 61, 
77, 61, 77, 61, 89, 90, 89, 90, 89, 93, 93, 90, 89, 90, 89, 90
,
32, 16, 32, 16, 93, 93, 93, 93, 32, 16, 32, 16, 57, 58, 93, 93, 
93, 93, 93, 93, 93, 93, 57, 58, 32, 32, 93, 32, 32, 93, 93, 32, 
32, 93, 32, 32, 53, 93, 53, 12, 93, 93, 93, 93, 53, 12, 93, 12, 
55, 13, 93, 93, 93, 93, 93, 93, 93, 93, 55, 13, 93, 50, 93, 50, 
93, 50, 93, 50, 93, 50, 93, 50, 61, 77, 61, 77, 61, 93, 93, 77, 
61, 77, 61, 77, 90, 89, 90, 89, 90, 93, 93, 89, 90, 89, 90, 89
,
16, 32, 16, 32, 16, 93, 93, 32, 16, 32, 16, 32, 57, 58, 57, 58, 
57, 93, 93, 58, 57, 58, 57, 58, 32, 32, 32, 32, 32, 32, 32, 32, 
32, 32, 32, 32, 53, 93, 93, 12, 53, 93, 93, 12, 53, 93, 93, 12, 
55, 13, 55, 13, 55, 93, 93, 13, 55, 13, 55, 13, 50, 93, 50, 93, 
50, 93, 50, 93, 50, 93, 50, 93, 77, 61, 77, 61, 77, 93, 93, 61, 
77, 61, 77, 61, 89, 90, 89, 90, 89, 93, 93, 90, 89, 90, 89, 90
,
32, 16, 32, 16, 32, 93, 93, 16, 32, 16, 32, 16, 57, 58, 57, 58, 
57, 93, 93, 58, 57, 58, 57, 58, 32, 32, 32, 32, 32, 32, 32, 32, 
32, 32, 32, 32, 53, 12, 53, 12, 53, 93, 93, 12, 53, 12, 53, 12, 
55, 13, 55, 13, 55, 13, 55, 13, 55, 13, 55, 13, 93, 50, 93, 50, 
93, 50, 93, 50, 93, 50, 93, 50, 61, 77, 61, 77, 61, 93, 93, 77, 
61, 77, 61, 77, 90, 89, 90, 89, 90, 93, 93, 89, 90, 89, 90, 89
,
33, 17, 33, 17, 33, 93, 93, 17, 33, 17, 33, 17, 56, 56, 56, 56, 
56, 93, 93, 56, 56, 56, 56, 56, 87, 87, 87, 87, 87, 87, 87, 87, 
87, 87, 87, 87, 82, 83, 82, 83, 82, 93, 93, 93, 82, 83, 82, 83, 
45, 29, 45, 29, 45, 29, 45, 29, 45, 29, 45, 29, 81, 50, 81, 50, 
81, 50, 81, 50, 81, 50, 81, 50, 60, 59, 60, 59, 60, 93, 93, 59, 
60, 59, 60, 59, 91, 92, 91, 92, 91, 93, 93, 92, 91, 92, 91, 92
,
17, 33, 17, 33, 17, 93, 93, 33, 17, 33, 17, 33, 56, 56, 56, 56, 
56, 93, 93, 56, 56, 56, 56, 56, 87, 87, 87, 87, 87, 93, 93, 93, 
93, 87, 87, 87, 83, 93, 93, 93, 83, 93, 93, 93, 83, 82, 83, 82, 
29, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 45, 50, 81, 50, 81, 
50, 81, 50, 81, 50, 81, 50, 81, 59, 60, 59, 60, 93, 93, 93, 93, 
59, 60, 59, 60, 92, 91, 92, 91, 92, 93, 93, 91, 92, 91, 92, 91
,
33, 17, 33, 17, 93, 93, 93, 93, 33, 17, 33, 17, 56, 56, 56, 56, 
56, 93, 93, 56, 56, 56, 56, 56, 87, 87, 87, 87, 93, 93, 93, 93, 
93, 93, 87, 87, 82, 93, 93, 93, 82, 93, 93, 93, 93, 93, 82, 83, 
45, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 29, 81, 50, 81, 50, 
81, 50, 81, 50, 81, 50, 81, 50, 60, 59, 60, 93, 93, 93, 93, 93, 
93, 59, 60, 59, 91, 92, 91, 92, 91, 93, 93, 93, 91, 92, 91, 92
,
17, 33, 17, 93, 93, 93, 93, 93, 93, 33, 17, 33, 56, 56, 56, 56, 
56, 93, 93, 56, 56, 56, 56, 56, 87, 87, 87, 87, 87, 93, 93, 93, 
93, 87, 87, 87, 83, 93, 93, 93, 83, 93, 93, 93, 93, 93, 83, 82, 
29, 93, 93, 45, 29, 93, 93, 93, 45, 93, 93, 45, 50, 81, 50, 81, 
50, 81, 50, 81, 50, 81, 50, 81, 59, 60, 59, 60, 93, 93, 93, 93, 
59, 60, 59, 60, 92, 91, 93, 93, 93, 93, 93, 93, 92, 91, 92, 91
,
33, 17, 93, 93, 93, 93, 93, 93, 93, 93, 33, 17, 56, 56, 56, 56, 
93, 93, 93, 93, 56, 56, 56, 56, 87, 87, 87, 87, 87, 87, 93, 93, 
87, 87, 87, 87, 82, 83, 82, 83, 82, 83, 82, 83, 93, 93, 82, 83, 
45, 93, 93, 29, 45, 93, 93, 93, 29, 93, 93, 29, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 50, 81, 50, 60, 59, 60, 59, 60, 93, 93, 59, 
60, 59, 60, 59, 91, 92, 93, 93, 93, 93, 93, 93, 91, 92, 91, 92
,
17, 33, 93, 93, 93, 93, 93, 93, 93, 93, 17, 33, 56, 56, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 87, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 83, 93, 93, 82, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 45, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 81, 50, 81, 59, 93, 93, 60, 93, 93, 93, 93, 
59, 93, 93, 60, 92, 91, 92, 91, 93, 93, 93, 93, 92, 91, 92, 91
,
33, 17, 93, 93, 93, 93, 93, 93, 93, 93, 33, 17, 56, 56, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 87, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 82, 93, 93, 83, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 29, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 50, 81, 50, 60, 59, 93, 59, 93, 93, 93, 93, 
60, 93, 60, 59, 91, 92, 91, 92, 93, 93, 93, 93, 91, 92, 91, 92
,
17, 33, 93, 93, 93, 93, 93, 93, 93, 93, 17, 33, 56, 56, 93, 93, 
93, 93, 93, 93, 56, 56, 56, 56, 87, 87, 87, 93, 93, 87, 87, 87, 
87, 87, 87, 87, 83, 82, 93, 93, 83, 82, 83, 82, 83, 82, 83, 82, 
29, 93, 93, 45, 29, 93, 93, 93, 45, 93, 93, 45, 50, 81, 50, 81, 
50, 93, 93, 81, 50, 81, 50, 81, 59, 60, 59, 60, 59, 60, 59, 60, 
59, 93, 59, 60, 92, 91, 92, 91, 93, 93, 93, 93, 93, 93, 92, 91
,
33, 17, 33, 93, 93, 93, 93, 93, 93, 17, 33, 17, 56, 56, 93, 93, 
93, 93, 93, 93, 56, 56, 56, 56, 87, 87, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 87, 82, 83, 93, 93, 93, 93, 93, 83, 93, 93, 93, 83, 
45, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 29, 81, 50, 81, 50, 
81, 93, 93, 50, 81, 50, 81, 50, 60, 59, 93, 93, 93, 93, 93, 93, 
60, 93, 60, 59, 91, 92, 91, 92, 93, 93, 93, 93, 93, 93, 93, 92
,
17, 33, 17, 33, 93, 93, 93, 93, 17, 33, 17, 33, 56, 56, 93, 93, 
93, 93, 93, 56, 56, 56, 56, 56, 87, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 87, 83, 82, 93, 93, 93, 93, 93, 82, 93, 93, 93, 82, 
29, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 45, 50, 93, 93, 81, 
50, 93, 93, 81, 50, 81, 50, 81, 59, 60, 93, 93, 93, 93, 93, 93, 
59, 93, 59, 60, 92, 91, 92, 91, 93, 93, 93, 91, 92, 91, 92, 91
,
33, 17, 33, 17, 33, 93, 93, 17, 33, 17, 33, 17, 56, 56, 56, 56, 
56, 93, 93, 56, 56, 56, 56, 56, 87, 87, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 87, 82, 83, 82, 83, 82, 93, 93, 83, 93, 93, 93, 83, 
45, 29, 45, 29, 45, 29, 45, 29, 45, 29, 45, 29, 81, 93, 93, 93, 
93, 93, 93, 50, 81, 50, 81, 50, 60, 59, 60, 59, 60, 59, 60, 59, 
60, 59, 60, 59, 91, 92, 91, 92, 91, 93, 93, 92, 91, 92, 91, 92
,
17, 33, 17, 33, 17, 93, 93, 33, 17, 33, 17, 33, 56, 56, 56, 56, 
56, 93, 93, 56, 56, 56, 56, 56, 87, 87, 87, 87, 87, 87, 87, 87, 
87, 87, 87, 87, 83, 82, 83, 82, 83, 93, 93, 82, 83, 82, 83, 82, 
29, 45, 29, 45, 29, 45, 29, 45, 29, 45, 29, 45, 50, 93, 93, 93, 
93, 93, 93, 81, 50, 81, 50, 81, 59, 60, 59, 60, 59, 60, 59, 60, 
59, 60, 59, 60, 92, 91, 92, 91, 92, 93, 93, 91, 92, 91, 92, 91
,
18, 34, 18, 34, 18, 93, 93, 34, 18, 34, 18, 34, 54, 54, 54, 54, 
54, 93, 93, 54, 54, 54, 54, 54, 51, 51, 51, 51, 51, 51, 51, 51, 
51, 51, 51, 51, 85, 84, 85, 84, 85, 93, 93, 84, 85, 84, 85, 84, 
52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 80, 52, 80, 
52, 93, 93, 80, 52, 80, 52, 80, 75, 76, 75, 76, 75, 76, 75, 76, 
75, 76, 75, 76, 73, 74, 73, 74, 73, 93, 93, 74, 73, 74, 73, 74
,
34, 18, 34, 18, 34, 93, 93, 18, 34, 18, 34, 18, 54, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 54, 51, 51, 93, 93, 93, 93, 93, 93, 
51, 93, 93, 51, 84, 85, 84, 85, 84, 93, 93, 85, 84, 85, 84, 85, 
52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 80, 52, 80, 52, 
80, 93, 93, 52, 80, 52, 80, 52, 76, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 75, 74, 73, 74, 73, 74, 93, 93, 73, 74, 73, 74, 73
,
18, 34, 18, 93, 93, 93, 93, 93, 93, 34, 18, 34, 54, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 54, 51, 51, 93, 51, 51, 51, 93, 51, 
51, 93, 51, 51, 85, 84, 85, 84, 85, 93, 93, 84, 85, 84, 85, 84, 
52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 80, 75, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 76, 73, 74, 73, 74, 73, 93, 93, 74, 73, 74, 73, 74
,
34, 18, 93, 93, 93, 93, 93, 93, 93, 93, 34, 18, 54, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 54, 51, 51, 93, 51, 93, 93, 93, 93, 
93, 93, 93, 51, 84, 85, 93, 93, 93, 93, 93, 93, 93, 93, 84, 85, 
52, 52, 52, 93, 93, 93, 93, 93, 93, 52, 52, 52, 80, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 52, 76, 93, 93, 93, 93, 93, 93, 75, 
93, 93, 93, 75, 74, 73, 74, 73, 74, 93, 93, 73, 74, 73, 74, 73
,
18, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 34, 54, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 54, 51, 51, 93, 51, 51, 51, 51, 93, 
51, 51, 51, 51, 85, 84, 93, 93, 93, 93, 93, 93, 93, 93, 85, 84, 
52, 52, 52, 93, 93, 93, 93, 93, 93, 52, 52, 52, 52, 93, 93, 80, 
52, 93, 93, 80, 52, 93, 93, 80, 75, 93, 93, 93, 75, 76, 75, 76, 
93, 93, 93, 76, 73, 74, 73, 74, 73, 93, 93, 74, 73, 74, 73, 74
,
34, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 18, 54, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 54, 51, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 84, 93, 93, 85, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 52, 52, 93, 93, 52, 52, 52, 80, 93, 93, 52, 
80, 93, 93, 52, 80, 93, 93, 52, 76, 93, 93, 93, 76, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 73, 74, 73, 74, 73
,
18, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 34, 54, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 54, 51, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 85, 93, 93, 84, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 52, 52, 93, 93, 52, 52, 52, 52, 93, 93, 80, 
52, 93, 93, 80, 52, 93, 93, 80, 75, 93, 93, 93, 75, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 74, 73, 74, 73, 74
,
34, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 18, 54, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 54, 51, 51, 51, 51, 93, 51, 51, 51, 
93, 51, 51, 51, 84, 85, 84, 85, 84, 93, 93, 85, 84, 85, 84, 85, 
52, 52, 52, 93, 93, 93, 93, 93, 93, 52, 52, 52, 80, 93, 93, 52, 
80, 93, 93, 52, 80, 93, 93, 52, 76, 93, 93, 93, 76, 93, 93, 75, 
76, 75, 76, 75, 74, 73, 74, 73, 74, 93, 93, 73, 74, 73, 74, 73
,
18, 34, 93, 93, 93, 93, 93, 93, 93, 93, 18, 34, 54, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 54, 51, 93, 93, 93, 93, 51, 93, 93, 
93, 51, 93, 51, 85, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 84, 
52, 52, 52, 93, 93, 93, 93, 93, 93, 52, 52, 52, 52, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 80, 75, 76, 75, 76, 75, 93, 93, 76, 
75, 76, 75, 76, 73, 74, 73, 74, 73, 93, 93, 74, 73, 74, 73, 74
,
34, 18, 34, 93, 93, 93, 93, 93, 93, 18, 34, 18, 54, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 54, 51, 51, 93, 51, 51, 51, 93, 51, 
51, 51, 93, 51, 84, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 85, 
52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 80, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 52, 76, 93, 93, 93, 93, 93, 93, 75, 
76, 75, 76, 75, 74, 73, 74, 73, 74, 93, 93, 73, 74, 73, 74, 73
,
18, 34, 18, 34, 18, 93, 93, 34, 18, 34, 18, 34, 54, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 54, 51, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 51, 85, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 84, 
52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 80, 52, 80, 
52, 93, 93, 80, 52, 80, 52, 80, 75, 93, 93, 93, 93, 93, 93, 76, 
75, 76, 75, 76, 73, 74, 73, 74, 73, 93, 93, 74, 73, 74, 73, 74
,
34, 18, 34, 18, 34, 93, 93, 18, 34, 18, 34, 18, 54, 54, 54, 54, 
54, 93, 93, 54, 54, 54, 54, 54, 51, 51, 51, 51, 51, 51, 51, 51, 
51, 51, 51, 51, 84, 85, 84, 85, 84, 85, 84, 85, 84, 85, 84, 85, 
52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 80, 52, 80, 52, 
80, 93, 93, 52, 80, 52, 80, 52, 76, 75, 76, 75, 76, 75, 76, 75, 
76, 75, 76, 75, 74, 73, 74, 73, 74, 93, 93, 73, 74, 73, 74, 73
,
35, 19, 35, 19, 35, 93, 93, 19, 35, 19, 35, 19, 20, 36, 20, 36, 
20, 93, 93, 36, 20, 36, 20, 36, 28, 44, 28, 44, 28, 44, 28, 44, 
28, 44, 28, 44, 26, 42, 26, 42, 26, 42, 26, 42, 26, 42, 26, 42, 
43, 27, 43, 27, 43, 27, 43, 27, 43, 27, 43, 27, 23, 39, 23, 39, 
23, 93, 93, 39, 23, 39, 23, 39, 38, 22, 38, 22, 38, 22, 38, 22, 
38, 22, 38, 22, 61, 77, 61, 77, 61, 93, 93, 77, 61, 77, 61, 77
,
19, 35, 19, 35, 19, 93, 93, 35, 19, 35, 19, 35, 36, 20, 36, 20, 
36, 93, 93, 20, 36, 20, 36, 20, 44, 28, 44, 28, 44, 28, 44, 28, 
44, 28, 44, 28, 42, 26, 42, 26, 42, 26, 42, 26, 42, 26, 42, 26, 
27, 43, 27, 43, 27, 43, 27, 43, 27, 43, 27, 43, 39, 23, 39, 23, 
39, 93, 93, 23, 39, 23, 39, 23, 22, 38, 22, 38, 22, 38, 22, 38, 
22, 38, 22, 38, 77, 61, 77, 61, 77, 93, 93, 61, 77, 61, 77, 61
,
35, 19, 35, 19, 93, 93, 93, 93, 35, 19, 35, 19, 20, 36, 20, 93, 
93, 93, 93, 93, 93, 36, 20, 36, 28, 44, 28, 93, 93, 93, 93, 93, 
93, 44, 28, 44, 26, 42, 26, 93, 93, 93, 93, 93, 93, 42, 26, 42, 
43, 27, 43, 93, 93, 93, 93, 93, 93, 27, 43, 27, 23, 39, 23, 39, 
23, 93, 93, 39, 23, 39, 23, 39, 38, 22, 38, 22, 38, 22, 38, 22, 
38, 22, 38, 22, 61, 77, 61, 77, 61, 93, 93, 77, 61, 77, 61, 77
,
19, 35, 19, 93, 93, 93, 93, 93, 93, 35, 19, 35, 36, 20, 93, 93, 
93, 93, 93, 93, 93, 93, 36, 20, 44, 28, 93, 93, 93, 93, 93, 93, 
93, 93, 44, 28, 42, 26, 93, 93, 93, 93, 93, 93, 93, 93, 42, 26, 
27, 43, 93, 93, 93, 93, 93, 93, 93, 93, 27, 43, 39, 23, 39, 23, 
93, 93, 93, 93, 39, 23, 39, 23, 22, 38, 22, 38, 22, 38, 22, 38, 
22, 38, 22, 38, 77, 61, 77, 61, 77, 93, 93, 61, 77, 61, 77, 61
,
35, 19, 35, 93, 93, 93, 93, 93, 93, 19, 35, 19, 20, 36, 93, 93, 
93, 93, 93, 93, 93, 93, 20, 36, 28, 44, 93, 93, 93, 93, 93, 93, 
93, 93, 28, 44, 26, 42, 93, 93, 93, 93, 93, 93, 93, 93, 26, 42, 
43, 27, 93, 93, 93, 93, 93, 93, 93, 93, 43, 27, 23, 39, 23, 39, 
93, 93, 93, 93, 23, 39, 23, 39, 38, 93, 93, 93, 93, 93, 93, 93, 
38, 22, 38, 22, 61, 77, 61, 77, 61, 93, 93, 77, 61, 77, 61, 77
,
19, 35, 19, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 61, 77, 61, 77, 61
,
35, 19, 35, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 77, 61, 77, 61, 77
,
19, 35, 19, 93, 93, 93, 93, 93, 93, 35, 19, 35, 36, 20, 93, 93, 
93, 93, 93, 93, 93, 93, 36, 20, 44, 28, 93, 93, 93, 93, 93, 93, 
93, 93, 44, 28, 42, 26, 93, 93, 93, 93, 93, 93, 93, 93, 42, 26, 
27, 43, 93, 93, 93, 93, 93, 93, 93, 93, 27, 43, 39, 23, 39, 23, 
93, 93, 93, 93, 39, 23, 39, 23, 22, 93, 93, 93, 93, 93, 93, 93, 
22, 38, 22, 38, 77, 61, 77, 61, 77, 61, 77, 61, 77, 61, 77, 61
,
35, 19, 35, 93, 93, 93, 93, 93, 93, 19, 35, 19, 20, 36, 93, 93, 
93, 93, 93, 93, 93, 93, 20, 36, 28, 44, 93, 93, 93, 93, 93, 93, 
93, 93, 28, 44, 26, 42, 93, 93, 93, 93, 93, 93, 93, 93, 26, 42, 
43, 27, 93, 93, 93, 93, 93, 93, 93, 93, 43, 27, 23, 39, 23, 39, 
23, 39, 23, 39, 23, 39, 23, 39, 38, 22, 38, 22, 38, 93, 93, 22, 
38, 22, 38, 22, 61, 77, 61, 77, 61, 77, 61, 77, 61, 77, 61, 77
,
19, 35, 19, 35, 93, 93, 93, 93, 19, 35, 19, 35, 36, 20, 36, 93, 
93, 93, 93, 93, 93, 20, 36, 20, 44, 28, 44, 93, 93, 93, 93, 93, 
93, 28, 44, 28, 42, 26, 42, 93, 93, 93, 93, 93, 93, 26, 42, 26, 
27, 43, 27, 93, 93, 93, 93, 93, 93, 43, 27, 43, 39, 23, 39, 23, 
39, 23, 39, 23, 39, 23, 39, 23, 22, 38, 22, 38, 22, 93, 93, 38, 
22, 38, 22, 38, 77, 61, 77, 61, 77, 61, 77, 61, 77, 61, 77, 61
,
35, 19, 35, 19, 35, 93, 93, 19, 35, 19, 35, 19, 20, 36, 20, 36, 
20, 36, 20, 36, 20, 36, 20, 36, 28, 44, 28, 44, 28, 44, 28, 44, 
28, 44, 28, 44, 26, 42, 26, 42, 26, 42, 26, 42, 26, 42, 26, 42, 
43, 27, 43, 27, 43, 93, 93, 27, 43, 27, 43, 27, 23, 39, 23, 39, 
23, 39, 23, 39, 23, 39, 23, 39, 38, 22, 38, 22, 38, 93, 93, 22, 
38, 22, 38, 22, 61, 77, 61, 77, 61, 77, 61, 77, 61, 77, 61, 77
,
19, 35, 19, 35, 19, 93, 93, 35, 19, 35, 19, 35, 36, 20, 36, 20, 
36, 20, 36, 20, 36, 20, 36, 20, 44, 28, 44, 28, 44, 28, 44, 28, 
44, 28, 44, 28, 42, 26, 42, 26, 42, 26, 42, 26, 42, 26, 42, 26, 
27, 43, 27, 43, 27, 93, 93, 43, 27, 43, 27, 43, 39, 23, 39, 23, 
39, 23, 39, 23, 39, 23, 39, 23, 22, 38, 22, 38, 22, 93, 93, 38, 
22, 38, 22, 38, 77, 61, 77, 61, 77, 61, 77, 61, 77, 61, 77, 61
,
21, 37, 21, 37, 21, 93, 93, 37, 21, 37, 21, 37, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 
24, 40, 24, 40, 24, 93, 93, 40, 24, 40, 24, 40, 86, 86, 86, 86, 
86, 86, 86, 86, 86, 86, 86, 86, 87, 87, 87, 87, 87, 93, 93, 87, 
87, 87, 87, 87, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88
,
37, 21, 37, 21, 93, 93, 93, 93, 37, 21, 37, 21, 0, 93, 93, 93, 
0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 9, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 9, 
40, 24, 40, 93, 93, 93, 93, 24, 93, 24, 40, 24, 86, 93, 93, 93, 
93, 93, 93, 86, 86, 86, 86, 86, 87, 93, 93, 87, 87, 93, 93, 87, 
87, 93, 93, 87, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88
,
21, 37, 21, 37, 93, 93, 93, 93, 21, 37, 21, 37, 0, 93, 93, 93, 
0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 9, 93, 9, 9, 9, 9, 93, 93, 93, 93, 93, 9, 
24, 40, 93, 93, 93, 93, 93, 40, 93, 93, 24, 40, 86, 93, 93, 93, 
93, 93, 93, 86, 86, 93, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88
,
37, 21, 37, 21, 93, 93, 93, 93, 37, 21, 37, 21, 0, 93, 93, 93, 
0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 93, 93, 7, 
7, 7, 7, 7, 9, 93, 9, 93, 93, 9, 93, 93, 93, 93, 93, 9, 
40, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 24, 86, 93, 93, 93, 
93, 93, 93, 86, 86, 86, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88
,
21, 37, 21, 37, 93, 93, 93, 93, 21, 37, 21, 37, 0, 0, 0, 0, 
0, 93, 93, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 93, 93, 7, 
7, 7, 7, 7, 9, 9, 9, 93, 93, 9, 9, 9, 9, 9, 9, 9, 
24, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 40, 86, 93, 93, 93, 
93, 93, 93, 86, 86, 86, 86, 86, 87, 87, 87, 87, 87, 93, 93, 87, 
87, 87, 87, 87, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88
,
37, 21, 37, 21, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 9, 9, 9, 9, 9, 
40, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 24, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 88, 88, 88, 88
,
21, 37, 21, 37, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 9, 9, 9, 9, 9, 
24, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 40, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 88, 88, 88, 88
,
37, 21, 37, 21, 93, 93, 93, 93, 37, 21, 37, 21, 0, 0, 0, 0, 
0, 93, 93, 0, 0, 0, 0, 0, 7, 93, 7, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 9, 9, 9, 9, 9, 93, 93, 93, 9, 9, 9, 9, 
40, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 24, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 86, 86, 87, 87, 87, 87, 87, 93, 93, 87, 
87, 87, 87, 87, 88, 88, 88, 88, 88, 93, 93, 93, 88, 88, 88, 88
,
21, 37, 21, 37, 21, 93, 93, 37, 21, 37, 21, 37, 0, 0, 0, 0, 
93, 93, 93, 93, 0, 0, 0, 0, 7, 7, 7, 93, 93, 93, 7, 7, 
7, 7, 7, 7, 9, 9, 9, 9, 9, 93, 93, 9, 9, 9, 9, 9, 
24, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 40, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 88, 88, 88, 88, 93, 93, 93, 88, 88, 88, 88
,
37, 21, 37, 21, 37, 93, 93, 21, 37, 21, 37, 21, 0, 0, 0, 93, 
93, 93, 93, 93, 93, 0, 0, 0, 7, 7, 7, 93, 93, 93, 7, 7, 
7, 7, 7, 7, 9, 9, 9, 9, 9, 93, 93, 9, 9, 9, 9, 9, 
40, 24, 93, 93, 40, 93, 93, 93, 93, 93, 40, 24, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 88, 88, 88, 88, 93, 93, 93, 88, 88, 88, 88
,
21, 37, 21, 37, 21, 93, 93, 37, 21, 37, 21, 37, 0, 0, 0, 0, 
93, 93, 93, 93, 0, 0, 0, 0, 7, 7, 7, 93, 93, 93, 7, 7, 
7, 7, 7, 7, 9, 9, 9, 9, 9, 93, 93, 9, 9, 9, 9, 9, 
24, 40, 24, 93, 24, 93, 93, 93, 93, 40, 24, 40, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 88, 88, 88, 88, 93, 93, 93, 88, 88, 88, 88
,
37, 21, 37, 21, 37, 93, 93, 21, 37, 21, 37, 21, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 
7, 7, 7, 7, 9, 9, 9, 9, 9, 93, 93, 9, 9, 9, 9, 9, 
40, 24, 40, 24, 40, 93, 93, 24, 40, 24, 40, 24, 86, 86, 86, 86, 
86, 86, 86, 86, 86, 86, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 88, 88, 88, 88, 93, 93, 93, 88, 88, 88, 88
,
0, 0, 0, 0, 0, 93, 93, 0, 0, 0, 0, 0, 3, 3, 3, 3, 
3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 1, 1, 1, 2, 2, 2, 2, 2, 93, 93, 2, 2, 2, 2, 2, 
41, 25, 41, 25, 41, 93, 93, 25, 41, 25, 41, 25, 86, 86, 86, 86, 
86, 86, 86, 86, 86, 86, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 88, 88, 88, 88, 93, 93, 93, 88, 88, 88, 88
,
0, 0, 0, 0, 0, 93, 93, 0, 93, 93, 93, 0, 3, 3, 3, 3, 
3, 3, 3, 3, 93, 93, 93, 3, 1, 1, 1, 1, 1, 1, 93, 93, 
93, 93, 93, 1, 2, 2, 2, 2, 2, 93, 93, 2, 2, 2, 2, 2, 
25, 41, 25, 93, 25, 93, 93, 93, 93, 41, 25, 41, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 86, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 88
,
0, 0, 0, 0, 0, 93, 93, 0, 93, 93, 93, 0, 3, 3, 3, 3, 
3, 3, 3, 3, 93, 93, 93, 3, 1, 1, 1, 1, 1, 1, 93, 93, 
93, 93, 93, 1, 2, 2, 2, 2, 2, 93, 93, 2, 2, 2, 2, 2, 
41, 25, 93, 93, 41, 93, 93, 93, 93, 93, 41, 25, 86, 93, 93, 93, 
93, 93, 93, 86, 86, 93, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 88
,
0, 0, 0, 0, 0, 93, 93, 0, 93, 93, 93, 0, 3, 3, 3, 3, 
3, 3, 3, 3, 93, 93, 93, 3, 1, 1, 1, 93, 93, 1, 93, 93, 
93, 93, 93, 1, 2, 2, 2, 2, 93, 93, 93, 2, 2, 2, 2, 2, 
25, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 41, 86, 93, 93, 93, 
93, 93, 93, 86, 86, 86, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 88
,
0, 0, 0, 0, 0, 93, 93, 0, 0, 0, 0, 0, 3, 3, 3, 3, 
3, 3, 93, 3, 3, 3, 3, 3, 1, 1, 1, 93, 93, 1, 1, 1, 
93, 93, 93, 1, 2, 2, 2, 2, 2, 93, 93, 2, 2, 2, 2, 2, 
41, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 25, 86, 93, 93, 93, 
93, 93, 93, 86, 86, 86, 86, 86, 87, 87, 87, 87, 87, 93, 93, 87, 
87, 87, 87, 87, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88
,
0, 0, 0, 0, 0, 93, 93, 0, 0, 0, 0, 0, 3, 3, 3, 3, 
3, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 1, 
93, 93, 93, 1, 2, 2, 2, 94, 94, 93, 93, 2, 2, 2, 2, 2, 
25, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 41, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 88, 93, 93, 93, 88
,
0, 0, 0, 0, 0, 93, 93, 0, 0, 0, 0, 0, 3, 3, 3, 3, 
3, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 1, 
1, 1, 1, 1, 2, 2, 2, 94, 94, 93, 93, 2, 2, 2, 2, 2, 
41, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 25, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 88, 93, 93, 93, 88
,
0, 0, 0, 0, 0, 93, 93, 0, 0, 0, 0, 0, 3, 3, 3, 93, 
93, 93, 93, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 93, 93, 93, 
1, 1, 1, 1, 2, 2, 2, 2, 2, 93, 93, 2, 2, 2, 2, 2, 
25, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 41, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 86, 86, 87, 87, 87, 87, 87, 93, 93, 87, 
87, 87, 87, 87, 88, 88, 88, 88, 88, 93, 93, 88, 88, 88, 88, 88
,
0, 93, 93, 93, 0, 93, 93, 0, 0, 0, 0, 0, 3, 3, 3, 93, 
93, 93, 93, 3, 93, 93, 93, 3, 1, 93, 93, 93, 1, 93, 93, 1, 
1, 1, 1, 1, 2, 2, 2, 2, 2, 93, 93, 2, 93, 93, 93, 2, 
41, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 25, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 88
,
0, 93, 93, 93, 0, 93, 93, 0, 0, 0, 0, 0, 3, 3, 3, 3, 
3, 93, 93, 3, 93, 93, 93, 3, 1, 93, 93, 93, 1, 93, 93, 1, 
1, 1, 1, 1, 2, 2, 2, 2, 2, 93, 93, 2, 93, 93, 93, 2, 
25, 41, 93, 93, 93, 93, 93, 41, 93, 93, 25, 41, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 88
,
0, 93, 93, 93, 0, 93, 93, 0, 0, 0, 0, 0, 3, 3, 3, 3, 
3, 93, 93, 3, 93, 93, 93, 3, 1, 93, 93, 93, 1, 93, 93, 1, 
1, 1, 1, 1, 2, 2, 2, 2, 2, 93, 93, 2, 93, 93, 93, 2, 
41, 25, 41, 93, 93, 93, 93, 25, 93, 25, 41, 25, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 88
,
0, 0, 0, 0, 0, 93, 93, 0, 0, 0, 0, 0, 3, 3, 3, 3, 
3, 93, 93, 3, 3, 3, 3, 3, 1, 1, 1, 1, 1, 93, 93, 1, 
1, 1, 1, 1, 2, 2, 2, 2, 2, 93, 93, 2, 2, 2, 2, 2, 
25, 41, 25, 41, 25, 93, 93, 41, 25, 41, 25, 41, 86, 86, 86, 86, 
86, 86, 86, 86, 86, 86, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88
,
4, 4, 4, 4, 4, 93, 93, 4, 4, 4, 4, 4, 5, 5, 5, 5, 
5, 93, 93, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 93, 93, 6, 
6, 6, 6, 6, 8, 8, 8, 8, 8, 93, 93, 8, 8, 8, 8, 8, 
10, 11, 10, 11, 10, 93, 93, 11, 10, 11, 10, 11, 86, 86, 86, 86, 
86, 86, 86, 86, 86, 86, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88
,
4, 4, 4, 4, 4, 93, 93, 4, 93, 93, 93, 4, 5, 5, 5, 5, 
93, 93, 93, 5, 93, 93, 93, 5, 6, 6, 6, 6, 6, 93, 93, 6, 
93, 93, 93, 6, 8, 8, 8, 8, 8, 93, 93, 8, 8, 8, 8, 8, 
11, 10, 11, 10, 93, 93, 93, 93, 11, 10, 11, 10, 86, 93, 93, 93, 
93, 93, 93, 86, 86, 86, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 93, 93, 88, 93, 88, 93, 88, 93, 88
,
4, 4, 4, 4, 4, 93, 93, 4, 93, 93, 93, 4, 5, 5, 5, 5, 
5, 93, 93, 5, 93, 93, 93, 5, 6, 6, 6, 6, 6, 93, 93, 6, 
93, 93, 93, 6, 8, 8, 8, 8, 8, 93, 93, 8, 8, 8, 8, 8, 
10, 11, 10, 93, 93, 93, 93, 93, 93, 11, 10, 11, 86, 93, 93, 93, 
93, 93, 93, 86, 86, 93, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 88, 88, 88, 88, 88, 93, 88, 88, 88, 93, 88
,
4, 4, 4, 4, 4, 93, 93, 4, 93, 93, 93, 4, 5, 5, 5, 5, 
5, 93, 93, 5, 93, 93, 93, 5, 6, 6, 6, 93, 93, 93, 93, 6, 
93, 93, 93, 6, 8, 8, 8, 8, 8, 93, 93, 93, 93, 8, 8, 8, 
11, 10, 93, 93, 93, 10, 11, 93, 93, 93, 11, 10, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 86, 93, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 88, 93, 93, 93, 93, 93, 88, 93, 88
,
4, 4, 4, 93, 93, 93, 93, 4, 93, 93, 93, 4, 5, 5, 5, 5, 
5, 93, 93, 5, 5, 5, 5, 5, 6, 6, 6, 93, 93, 93, 93, 6, 
6, 6, 6, 6, 8, 8, 93, 8, 8, 93, 93, 93, 93, 8, 8, 8, 
10, 11, 93, 93, 93, 11, 10, 93, 93, 93, 10, 11, 86, 93, 93, 93, 
93, 93, 93, 86, 86, 86, 86, 86, 87, 87, 87, 87, 87, 93, 93, 87, 
87, 87, 87, 87, 88, 93, 93, 88, 93, 93, 93, 93, 93, 88, 93, 88
,
4, 4, 4, 93, 93, 93, 93, 4, 4, 4, 4, 4, 5, 5, 5, 5, 
5, 93, 93, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 8, 8, 8, 8, 8, 
11, 10, 11, 93, 93, 93, 93, 93, 93, 10, 11, 10, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 88, 93, 93, 93, 93, 93, 88, 93, 88
,
4, 4, 4, 4, 4, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 5, 93, 93, 93, 5, 93, 93, 93, 93, 6, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 8, 8, 8, 8, 8, 
10, 11, 10, 11, 93, 93, 93, 93, 10, 11, 10, 11, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 88, 93, 93, 93, 93, 93, 88, 93, 88
,
4, 4, 4, 4, 4, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 5, 93, 93, 93, 5, 93, 93, 93, 93, 6, 6, 6, 6, 
6, 6, 93, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
11, 10, 11, 10, 11, 93, 93, 10, 11, 10, 11, 10, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 86, 86, 87, 87, 87, 87, 87, 93, 93, 87, 
87, 87, 87, 87, 88, 93, 93, 88, 93, 93, 93, 93, 93, 88, 93, 88
,
4, 93, 93, 93, 4, 4, 4, 93, 4, 4, 4, 4, 5, 5, 5, 93, 
93, 5, 5, 5, 93, 93, 93, 5, 93, 93, 93, 93, 6, 6, 6, 6, 
6, 6, 6, 6, 8, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 8, 
10, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 11, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 88, 93, 93, 93, 93, 93, 88, 93, 88
,
4, 93, 93, 93, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 93, 
93, 5, 93, 93, 93, 93, 93, 5, 93, 93, 93, 93, 6, 6, 6, 6, 
6, 6, 6, 6, 8, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 8, 
11, 10, 11, 10, 11, 93, 93, 10, 11, 10, 11, 10, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 93, 93, 93, 88, 88, 88, 88, 88, 88
,
4, 93, 93, 93, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 
5, 5, 93, 93, 93, 93, 93, 5, 93, 93, 93, 93, 6, 6, 6, 6, 
6, 6, 6, 6, 8, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 8, 
10, 11, 10, 11, 10, 93, 93, 11, 10, 11, 10, 11, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 93, 93, 93, 88, 93, 93, 93, 93, 88
,
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 
5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 
6, 6, 6, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
11, 10, 11, 10, 11, 93, 93, 10, 11, 10, 11, 10, 86, 86, 86, 86, 
86, 86, 86, 86, 86, 86, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88
,
66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 68, 67, 68, 67, 
68, 67, 68, 67, 68, 67, 68, 67, 70, 69, 70, 69, 70, 69, 70, 69, 
70, 69, 70, 69, 71, 72, 71, 72, 71, 72, 71, 72, 71, 72, 71, 72, 
64, 65, 64, 65, 64, 93, 93, 65, 64, 65, 64, 65, 86, 86, 86, 86, 
86, 86, 86, 86, 86, 86, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88
,
66, 66, 66, 93, 93, 66, 66, 66, 66, 66, 66, 66, 67, 68, 67, 68, 
67, 68, 67, 68, 67, 68, 67, 68, 69, 70, 69, 70, 69, 70, 69, 70, 
69, 70, 69, 70, 72, 93, 93, 93, 93, 71, 72, 71, 72, 71, 72, 71, 
65, 64, 65, 64, 65, 93, 93, 93, 93, 93, 65, 64, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 86, 93, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 93, 93, 93, 88, 93, 88, 93, 93, 88
,
66, 66, 93, 93, 93, 93, 66, 66, 66, 66, 66, 66, 68, 67, 68, 93, 
93, 93, 93, 93, 93, 67, 68, 67, 70, 69, 70, 69, 70, 69, 70, 69, 
70, 69, 70, 69, 71, 93, 93, 93, 93, 93, 93, 72, 71, 72, 71, 72, 
64, 65, 64, 65, 64, 93, 93, 93, 93, 93, 64, 65, 86, 93, 93, 93, 
93, 93, 93, 86, 86, 93, 86, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 93, 93, 93, 88, 88, 88, 93, 93, 88
,
66, 66, 93, 93, 93, 93, 66, 66, 66, 66, 66, 66, 67, 68, 93, 93, 
93, 93, 93, 93, 93, 93, 67, 68, 69, 70, 69, 70, 69, 70, 69, 70, 
69, 70, 69, 70, 72, 93, 93, 93, 93, 93, 93, 71, 72, 71, 72, 71, 
65, 64, 65, 64, 65, 64, 65, 93, 93, 93, 65, 64, 86, 86, 86, 93, 
93, 93, 93, 86, 86, 86, 93, 86, 87, 93, 93, 93, 87, 93, 93, 87, 
93, 93, 93, 87, 88, 93, 93, 88, 93, 93, 93, 93, 93, 93, 93, 88
,
66, 66, 66, 93, 93, 66, 66, 66, 66, 66, 66, 66, 68, 93, 93, 93, 
93, 67, 68, 93, 93, 93, 93, 67, 70, 69, 70, 69, 70, 69, 70, 69, 
70, 69, 70, 69, 71, 72, 93, 93, 71, 93, 93, 72, 71, 72, 71, 72, 
64, 65, 93, 93, 93, 65, 64, 93, 93, 93, 64, 65, 86, 93, 93, 93, 
93, 93, 93, 86, 86, 86, 86, 86, 87, 87, 87, 87, 87, 93, 93, 87, 
87, 87, 87, 87, 88, 93, 93, 88, 93, 93, 93, 93, 93, 93, 93, 88
,
66, 66, 66, 93, 93, 66, 66, 66, 66, 93, 93, 93, 93, 93, 93, 93, 
67, 93, 93, 68, 93, 93, 93, 93, 93, 93, 69, 70, 69, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 93, 72, 93, 93, 71, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 64, 65, 93, 93, 93, 65, 64, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 88, 93, 93, 93, 93, 93, 93, 93, 88
,
66, 66, 66, 93, 93, 66, 66, 66, 66, 93, 93, 93, 93, 93, 93, 93, 
68, 93, 68, 67, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 69, 
70, 93, 93, 93, 93, 93, 93, 93, 71, 93, 93, 72, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 65, 64, 93, 93, 65, 64, 65, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 93, 93, 93, 93, 88, 93, 93, 93, 93, 93, 93, 93, 88
,
66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 67, 93, 93, 93, 
93, 68, 67, 68, 93, 93, 67, 68, 69, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 69, 70, 72, 71, 72, 71, 72, 93, 93, 71, 93, 93, 72, 71, 
65, 64, 93, 93, 93, 64, 65, 93, 93, 64, 65, 64, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 86, 86, 87, 87, 87, 87, 87, 87, 87, 87, 
87, 87, 87, 87, 88, 88, 88, 88, 93, 93, 93, 93, 93, 93, 93, 88
,
66, 66, 66, 93, 93, 66, 66, 66, 66, 66, 66, 66, 68, 67, 93, 93, 
93, 93, 93, 93, 68, 93, 93, 67, 70, 69, 70, 69, 70, 93, 93, 93, 
93, 93, 70, 69, 71, 72, 71, 72, 93, 93, 93, 93, 93, 93, 71, 72, 
64, 65, 64, 93, 93, 65, 64, 93, 93, 65, 64, 65, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 86, 87, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 87, 88, 93, 93, 88, 93, 93, 93, 93, 93, 93, 93, 88
,
66, 66, 93, 93, 93, 93, 66, 66, 66, 66, 66, 66, 67, 68, 67, 93, 
93, 93, 93, 93, 93, 68, 93, 68, 69, 70, 69, 70, 69, 70, 69, 93, 
93, 70, 69, 70, 72, 71, 72, 71, 93, 93, 93, 93, 93, 93, 72, 71, 
65, 64, 65, 93, 93, 93, 93, 93, 93, 64, 65, 64, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 86, 87, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 87, 88, 93, 93, 88, 93, 93, 88, 88, 88, 93, 93, 88
,
66, 66, 66, 93, 93, 66, 66, 66, 66, 66, 66, 66, 68, 67, 68, 67, 
68, 67, 68, 67, 93, 93, 93, 67, 70, 69, 70, 69, 70, 69, 70, 69, 
70, 69, 70, 69, 71, 72, 71, 72, 71, 93, 93, 72, 71, 72, 71, 72, 
64, 65, 64, 93, 93, 93, 93, 93, 93, 65, 64, 65, 86, 93, 93, 93, 
93, 93, 93, 86, 93, 93, 93, 86, 87, 93, 93, 93, 93, 93, 93, 93, 
93, 93, 93, 87, 88, 93, 93, 88, 93, 93, 88, 93, 88, 93, 93, 88
,
66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 67, 68, 67, 68, 
67, 68, 67, 68, 67, 68, 67, 68, 69, 70, 69, 70, 69, 70, 69, 70, 
69, 70, 69, 70, 72, 71, 72, 71, 72, 71, 72, 71, 72, 71, 72, 71, 
65, 64, 65, 64, 65, 64, 65, 64, 65, 64, 65, 64, 86, 86, 86, 86, 
86, 86, 86, 86, 86, 86, 86, 86, 87, 87, 87, 87, 87, 87, 87, 87, 
87, 87, 87, 87, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88
  ], [2, 7, 12, 17] ),

  // actors[1] is the player character
  new Sprite( "assets/other.png", 360,40, 1,1, {
    default: {interval: 0.0, frames: [[0,0], ] },
  },
  { x: 0, y: 0, w: 22, h: 14 }, // bounding box
  actor => {
    // if (!actors[0].collide( actor.x + actor.dx * 1/fps, actor.y + actor.dy * 1/fps )) {
    //   actor.x += actor.dx * 1/fps; // move by dx every second
    //   actor.y += actor.dy * 1/fps; // move by dy every second
    // }

    if (!actors[0].collideBox( actor.x + actor.bbox.x + actor.dx * 1/fps,
                              actor.y + actor.bbox.y + actor.dy * 1/fps,
                              actor.bbox.w,
                              actor.bbox.h )) {
      actor.x += actor.dx * 1/fps; // move by dx every second
      actor.y += actor.dy * 1/fps; // move by dy every second
    }
  }),

  new Sprite( "sprites.png", 400,40, 9,4, {
      default: {interval: 0.0, frames: [[0,0], ] },
    },
    { x: 0, y: 0, w: 32, h: 32 }, // bounding box
    collideAgainstMap ),

  new Sprite( "sprites.png", 400,40, 9,4, {
      default: {interval: 0.0, frames: [[0,0], ] },
    },
    { x: 0, y: 0, w: 32, h: 32 }, // bounding box
    collideAgainstMap ),
]; // end of actor array...

////////////////////////////////////////////////////////////////////////////////
// game loop methods
function init() {
}

function draw() {
  let ctx = canvas.getContext("2d");
  clear( ctx );
  for (let x=0; x < actors.length; ++x) {
    actors[x].draw( ctx );
  }
}

function resize() {
  // Check if the canvas's image buffer is not the same size as the browser's area.
  if (canvas.width  != canvas.clientWidth ||
      canvas.height != canvas.clientHeight) {
    // Make the canvas the same size
    canvas.width  = canvas.clientWidth;
    canvas.height = canvas.clientHeight;
    console.log( `resizing canvas to: ${canvas.width} x ${canvas.height}` )
  }
}

////////////////////////////////////////////////////////////////////////////////
// start the game loop
init();
setInterval( () => {
  resize();
  draw();
}, 1000 / fps );

////////////////////////////////////////////////////////////////////////////////
// respond to Input
document.onkeydown = (event) => {
  if (event.repeat) return;
  let speed = 3;
  switch (event.key) {
    case "ArrowLeft":
      actors[1].changeSequence("left");
      actors[1].dx = -32*speed;
      actors[1].dy = 0;
      break;
    case "ArrowRight":
      actors[1].changeSequence("right");
      actors[1].dx = 32*speed;
      actors[1].dy = 0;
      break;
    case "ArrowUp":
      actors[1].changeSequence("up");
      actors[1].dx = 0;
      actors[1].dy = -32*speed;
      break;
    case "ArrowDown":
      actors[1].changeSequence("down");
      actors[1].dx = 0;
      actors[1].dy = 32*speed;
      break;
  }
  console.log('keydown event\n\n' + 'key: ' + event.key, " repeat: ", event.repeat);
};

document.onkeyup = (event) => {
  if (event.repeat) return;
  // cancel velocity
  actors[1].dx = 0;
  actors[1].dy = 0;
  switch (event.key) {
    case "ArrowLeft":
      actors[1].changeSequence("left_idle");
      break;
    case "ArrowRight":
      actors[1].changeSequence("right_idle");
      break;
    case "ArrowUp":
      actors[1].changeSequence("up_idle");
      break;
    case "ArrowDown":
      actors[1].changeSequence("down_idle");
      break;
  }
  console.log('keyup event\n\n' + 'key: ' + event.key, " repeat: ", event.repeat);
};

