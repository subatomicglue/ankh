import { WALL_TILES, WORLD } from './data.js';

const PASSABLE_TILES = new Set([93,94]);
// grit remaps the GBA's transparent wall color (raw index 200) to BMP index 255.
const WALL_VOID_PIXEL = 255;

export class World {
  constructor() { this.map=WALL_TILES;Object.assign(this,WORLD);this.atlas=null; }
  tileAt(x, y) {
    const tx=Math.floor(x/this.tileSize),ty=Math.floor(y/this.tileSize);
    if(tx<0||ty<0||tx>=this.mapWidth||ty>=this.mapHeight)return -1;
    return this.map[ty*this.mapWidth+tx];
  }
  pixelAt(x,y) {
    const tile=this.tileAt(x,y); if(tile<0||!this.atlas)return 0;
    const localX=(Math.floor(x)%this.tileSize+this.tileSize)%this.tileSize,localY=(Math.floor(y)%this.tileSize+this.tileSize)%this.tileSize;
    return this.atlas.pixel((tile%this.atlasColumns)*this.tileSize+localX,Math.floor(tile/this.atlasColumns)*this.tileSize+localY);
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
  // Each visited room contributes its upper-left 16x16 world tile to the map display.
  *visitedRoomTiles(rooms) {
    for (const room of rooms) {
      const [roomX,roomY]=room.split(',').map(Number);
      const roomsWide=Math.floor(this.width/this.roomWidth),roomsHigh=Math.floor(this.height/this.roomHeight);
      if(roomX<0||roomX>=roomsWide||roomY<0||roomY>=roomsHigh)continue;
      const sourceX=roomX*(this.roomWidth/this.tileSize),sourceY=roomY*(this.roomHeight/this.tileSize);
      yield {x:216+roomX*this.tileSize,y:408+roomY*this.tileSize,tile:this.map[sourceX+sourceY*this.mapWidth]};
    }
  }
}

export { WORLD };
