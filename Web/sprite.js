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
//   let actor = new Sprite( "sprites.png", 40,40, 9,4, { default: {rate: 0.05, frames: [[0,3], ] } }, { x: 22, y: 15, w: 20, h: 48 }, actor => {} )

import { Vec } from "./math.js";
import { BEHAVIOR_THINGS } from "./behaviors.js";
import { actors } from "./index.js";

//   actor.draw( canvas.getContext("2d") );
export class Sprite {
  x;               // sprite x position
  y;               // sprite y position
  tilesx;     // number of x sprites in the image
  tilesy;     // number of y sprites in the image
  width;               // width  of the sprite in pixels (set once img loads)
  height;              // height of the sprite in pixels (set once img loads)
  anim;            // animation clock, controls which frame to display, loops
  sequences; // collection of named animation sequences, e.g. { default: {rate: 0.4, frames: [[0,0], [1,0]]}, ... }
  animation; // current sequence, e.g. {rate: 0.4, frames: [[0,0], [1,0]]}
  dx;              // velocity in x
  dy;              // velocity in y
  bbox;         // collision bounding box relative to x/y
  showbbox;    // debug: display the bounding box
  updateFunc = () => {};   //
  frame = [0,0];
  missing_image = 'assets/missing.png';
  collided = false;

  constructor( filename, x, y, tilesx = 9, tilesy = 4, sequences = {default: {rate: 0.4, frames: [[0,0], [1,0]]} }, bbox = { x: 22, y: 15, w: 20, h: 48 }, updateFunc = (s) => {} ) {
    if (filename == undefined) {
      console.info( `ERROR: Sprite has bad filename ${filename}` );
      filename = this.missing_image;
    }

    this.x = x;               // sprite x position
    this.y = y;               // sprite y position
    this.tilesx = tilesx;     // number of x sprites in the image
    this.tilesy = tilesy;     // number of y sprites in the image
    this.width;               // width  of the sprite in pixels (set once img loads)
    this.height;              // height of the sprite in pixels (set once img loads)
    this.anim = 0;            // animation clock, controls which frame to display, loops
    this.sequences = sequences; // collection of named animation sequences, e.g. { default: {rate: 0.4, frames: [[0,0], [1,0]]}, ... }
    this.animation = this.sequences.default; // current sequence, e.g. {rate: 0.4, frames: [[0,0], [1,0]]}
    this.dx = 0;              // velocity in x
    this.dy = 0;              // velocity in y
    this.bbox = bbox;         // collision bounding box relative to x/y
    this.showbbox = false;    // debug: display the bounding box
    this.updateFunc = updateFunc; //
    this.frame = [0,0];

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
    this.img.addEventListener( "error", () => {
      console.log(`ERROR loading ${filename}`);
      this.img.src = this.missing_image;
    });
    this.img.src = filename ? filename : this.missing_image;  // loads the image
  }

  // draw the sprite using the given canvas ctx
  draw( ctx ) {
    // draws the image into the canvas
    if (this.showbbox)
      ctx.fillRect( this.x + this.bbox.x,
                    this.y + this.bbox.y,
                    this.bbox.w,
                    this.bbox.h );
    //ctx.globalCompositeOperation = "screen";
    if (this.img.src == undefined) { console.error( `ERROR: Sprite has bad filename ${filename}` ); return; }
    if (this.frame == undefined) {
      console.log( "oops", this.anim, this.animation.frames, this.animation.frames[Math.floor( this.anim )] );
    }
    ctx.drawImage( this.img,
      this.frame[0] * this.width,
      this.frame[1] * this.height,
      this.width, this.height, // original image pixels
      this.x, this.y,
      this.width, this.height  // new pos/size you want to draw
    );
  }

  update( t ) {
    // sprite flipbook animation
    this.frame = this.animation.frames[Math.floor( this.anim )]
    this.anim = (this.anim + this.animation.rate * t) % this.animation.frames.length;
    if (this.anim < 0 || 1 < t) {
      console.log( "oops", this.anim, this.animation.frames, this.animation.rate, t, this.animation.frames.length, this.animation.frames[Math.floor( this.anim )] );
    }
    //console.log( `${Math.floor( this.anim ) * this.width} ${Math.floor( this.bank ) * this.height}`)

    // custom (most likely a collision func)
    this.updateFunc( this, t );
  }

  // change animation
  changeSequence( name ) {
    this.animation = this.sequences[name] ? this.sequences[name] : this.sequences.default;
    this.anim = 0;
  }
}

export class Actor {
  constructor( name, sprite, behavior ) {
    if (sprite == undefined) { console.info( "Actor(): ERROR: sprite isn't set!", sprite)}
    this.sprite = sprite;
    this.behavior = behavior;
    this.name = name;
  }

  draw( ctx ) {
    this.sprite.draw( ctx );
  }

  update( t ) {
    if (this.sprite.update == undefined) { console.error( "ERROR: sprite isn't set!", this.sprite)}
    this.sprite.update( t );
    this.behavior.doThing( BEHAVIOR_THINGS.UPDATE, this, this );
  }

  changeSequence( name ) {
    this.sprite.changeSequence( name );
  }

  getPosition() {
    return new Vec( this.sprite.x, this.sprite.y );
  }
  setPosition( x, y ) {
    this.sprite.x = x;
    this.sprite.y = y;
  }

  getVelocity() {
    return new Vec( this.sprite.dx, this.sprite.dy );
  }
  setVelocity( x, y ) {
    this.sprite.dx = x;
    this.sprite.dy = y;
  }
  setVelocityX( val ) {
    this.sprite.dx = val;
  }
  setVelocityY( val ) {
    this.sprite.dy = val;
  }

  sprite;
  name;
}

export function intersectAllActors( parent_actor, collision ) {
  return false;
}
