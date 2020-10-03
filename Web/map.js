import { sub, vec, floor, div } from "./math.js";

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
export function Map( filename, tilesx = 8, tilesy = 16, mapx = 4, map = [2,2,2,2, 2,3,4,2, 2,5,6,2, 2,2,2,2], collidable = [2, 7, 12, 17] ) {
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

