import { sub, vec, floor, div, Vec } from "./math.js";
import { map } from "./index.js";

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
export class Map {
  // the sprites
  img = new Image();    // the sprites: a bitmap with every map sprite possible
  img_done = new Promise((rs,rj) => rs()); // you can await on this promise to know when map is loaded
  tilesx = 0;           // number of x sprites in the image
  tilesy = 0;           // number of y sprites in the image
  tile_width = 0;            // width of one map sprite
  tile_height = 0;           // height of one map sprite

  // the map
  map = [];             // the map: the sprite layout to draw.  e.g. [ 2,2,2,2 ],
                        // where each ID is an index into the map sprite image counting
                        // from left to right, starting at top left.
  mapx = 0;             // number of map sprites in the sprite layout
  not_collidable = [];  // which sprite indexes are not collidable
  x = 0;                // offset to draw the map into the canvas
  y = 0;                // offset to draw the map into the canvas

  // rooms
  room_tilesx = 0;      // number of tiles in a room
  room_tilesy = 0;      // number of tiles in a room

  constructor( filename, tilesx = 8, tilesy = 16, roomx=12, roomy=12, mapx = 4, map = [2,2,2,2, 2,3,4,2, 2,5,6,2, 2,2,2,2], not_collidable = [93] ) {
    if (filename == undefined) { console.log( "ERROR: map sprites filename isn't set" ); }
    this.img_done = new Promise( (rs, rj) => {
      this.img.addEventListener( "load", () => {
        this.tile_width = this.img.width / this.tilesx;
        this.tile_height = this.img.height / this.tilesy;
        console.log( `Map ${filename} loaded ${this.img.width} x ${this.img.height} total, ${this.tile_width} x ${this.tile_height} per tile, ${tilesx} x ${tilesy} tiles`)
        rs();
      });
    })
    this.img.src = filename;
    this.tilesx = tilesx;
    this.tilesy = tilesy;
    this.mapx = mapx;
    this.map = map;
    this.room_tilesx = roomx;
    this.room_tilesy = roomy;
    this.not_collidable = not_collidable;
    this.x = 0;
    this.y = 0;
  }

  update( t ) {
  }

  position2tile( pos ) {
    let mapx = Math.floor( pos.x / this.tile_width - this.x + 1);
    let mapy = Math.floor( pos.y / this.tile_height - this.y + 1 );
    return new Vec( mapx, mapy );
  }

  tile2room( tile ) {
    let roomx = Math.floor( tile.x / this.room_tilesx );
    let roomy = Math.floor( tile.y / this.room_tilesy );
    return new Vec( roomx, roomy );
  }

  position2room( pos ) {
    return this.tile2room( this.position2tile( pos ) );
  }

  // get the 1st (upper/left) tile for the room.  give room 1,0, returns 12,0
  room2tile( room ) {
    return new Vec( room.x * this.room_tilesx, room.y * this.room_tilesy );
  }

  tile2id( tile ) {
    return this.map[this.tile2mapindex(tile)];            // get the tile ID at the grid location
  }

  tile2mapindex( tile ) {
    let mapindex = tile.x + tile.y * this.mapx; // get the map grid location of x,y
    return mapindex;
  }

  room2id( room ) {
    return this.tile2id( this.room2tile( room ) );
  }

  position2id( pos ) {
    return this.tile2id( this.position2tile( pos ) );
  }

  room2mapindex( room ) {
    return this.tile2mapindex( this.room2tile( room ) );
  }

  position2mapindex( pos ) {
    return this.tile2mapindex( this.position2tile( pos ) );
  }

  setTileId( tile, id ) {
    this.map[this.tile2mapindex( tile )] = id;            // get the tile ID at the grid location 
  }

  // write the tile id of the room you're currently in, to the "maproom"
  updateMapRoom( pos, maproom_room = new Vec(1, 2), maproom_tileoffset = new Vec( 2,2 ) ) {
    let room = this.position2room( pos );
    let id = this.room2id( room );
    let maproom_tile = this.room2tile( maproom_room );
    maproom_tile.x += maproom_tileoffset.x + room.x;
    maproom_tile.y += maproom_tileoffset.y + room.y;
    //console.log( "maproom_tile:", maproom_tile, "new_id:", id, "player_room:", room );
    this.setTileId( maproom_tile, id );
  }

  draw( ctx ) {
    // todo: detect parent viewport extents, dont bother drawing out of bounds

    for (let i = 0; i < this.map.length; ++i) {
      let x = i % this.mapx;
      let y = Math.floor( i / this.mapx );
      let tx = this.map[i] % this.tilesx;
      let ty = Math.floor( this.map[i] / this.tilesx );

      // draws the image into the canvas
      ctx.drawImage( this.img,
        tx * this.tile_width, ty * this.tile_height,
        this.tile_width, this.tile_height, // original image pixels
        this.x + x * this.tile_width, this.y + y * this.tile_height,
        this.tile_width, this.tile_height  // new pos/size you want to draw
      );
    }
  }

  mapXYtoIndex( x, y ) {
    return x + y * this.mapx;
  }

  // does x,y intersect a collidable tile?
  collide( x, y ) {
    let mapx = Math.floor( x / this.tile_width - this.x + 1);
    let mapy = Math.floor( y / this.tile_height - this.y + 1 );
    let mapindex = mapx + mapy * this.mapx; // get the map grid location of x,y
    let id = this.map[mapindex];            // get the tile ID at the grid location 
    return this.collidable.find( r => r == id ); // is the tile ID collidable?
  }

  // does the box intersect a collidable tile?
  collideBox( x, y, width, height ) {
    let topleft = vec( x, y );
    let botright = vec( x+width, y+height );
    // what map tiles does the given box pass through?
    let min_map = floor( sub( div( topleft, vec( this.tile_width, this.tile_height ) ), vec( this.x, this.y ) ) );
    let max_map = floor( sub( div( botright, vec( this.tile_width, this.tile_height ) ), vec( this.x, this.y ) ) );
    // if any of those tiles are collidable, signal a collision
    for (let j = min_map.y; j <= max_map.y; ++j) {
      for (let i = min_map.x; i <= max_map.x; ++i) {
        let index = i + j * this.mapx;
        let id = this.map[index];
        if (this.not_collidable.find( r => r == id )) {
          return false; // no collision
        }
      }
    }
    return true; 
  }
}

export function intersectMap( actor ) {
  console.log( "who is calling this?" );
  let sprite = actor.sprite;
  return map.collideBox( sprite.x + sprite.bbox.x,
                        sprite.y + sprite.bbox.y,
                        sprite.bbox.w,
                        sprite.bbox.h );
}
