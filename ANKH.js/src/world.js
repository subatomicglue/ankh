import { WALL_TILES, WORLD } from './data.js';

const PASSABLE_TILES = new Set([346,347,348,349,378,379,380,381]);
// grit remaps the GBA's transparent wall color (raw index 200) to BMP index 255.
const WALL_VOID_PIXEL = 255;

export class World {
  constructor() { this.map = WALL_TILES; this.width = WORLD.width; this.height = WORLD.height; this.atlas=null; }
  tileAt(x, y) {
    const tx = Math.floor(x / 8), ty = Math.floor(y / 8);
    if (tx < 0 || ty < 0 || tx >= 192 || ty >= 192) return -1;
    return this.map[ty * 192 + tx];
  }
  pixelAt(x,y) {
    const tile=this.tileAt(x,y); if(tile<0||!this.atlas)return 0;
    return this.atlas.pixel((tile%32)*8+(Math.floor(x)%8+8)%8,Math.floor(tile/32)*8+(Math.floor(y)%8+8)%8);
  }
  isSolidPixel(x,y) {
    return !PASSABLE_TILES.has(this.tileAt(x,y))&&this.pixelAt(x,y)!==WALL_VOID_PIXEL;
  }
  collisionPixels(box,maskPixel=null) {
    const hits=[],left=Math.floor(box.x),top=Math.floor(box.y),right=Math.floor(box.x+box.width),bottom=Math.floor(box.y+box.height);
    for(let y=top;y<=bottom;y++)for(let x=left;x<=right;x++)if(this.isSolidPixel(x,y)&&(!maskPixel||maskPixel(x-left,y-top)!==0))hits.push({x,y,tile:this.tileAt(x,y)});
    return hits;
  }
  firstCollision(box,maskPixel=null) {
    return this.collisionPixels(box,maskPixel)[0]??null;
  }
  collidesBox(box,maskPixel=null) {return this.firstCollision(box,maskPixel)!==null;}
  collidesPastThinSurface(box,maskPixel,[dx,dy],maxHits=Infinity) {
    const hits=this.collisionPixels(box,maskPixel);
    if(!hits.length)return false;
    return hits.length>maxHits||hits.some(hit=>this.isSolidPixel(hit.x+dx,hit.y+dy));
  }

  // GBA Background::update() displays the visited-room bitfield in room 1,2.
  // Each visited room contributes its upper-left 2x2 world tiles to this 16x16 map.
  *visitedRoomTiles(rooms) {
    for (const room of rooms) {
      const [roomX,roomY]=room.split(',').map(Number);
      if(roomX<0||roomX>=8||roomY<0||roomY>=8)continue;
      for(let y=0;y<2;y++)for(let x=0;x<2;x++){
        const sourceX=roomX*24+x,sourceY=roomY*24+y;
        yield {x:27+roomX*2+x,y:51+roomY*2+y,tile:this.map[sourceX+sourceY*192]};
      }
    }
  }
}

export { WORLD };
