import { WORLD } from './data.js';

export class Camera {
  constructor(world=WORLD){this.world=world;this.reset();}
  reset(){this.mode='room';this.viewportWidth=this.world.roomWidth;this.viewportHeight=this.world.roomHeight;this.resetPosition();}
  resetPosition(){this.x=0;this.y=0;this.freeX=0;this.freeY=0;}
  follow(player,offset=[0,0]){if(this.mode==='free'){this.x=this.freeX;this.y=this.freeY;return;}const {width,height,roomWidth,roomHeight}=this.world,[px,py]=player.center,roomX=Math.floor(px/roomWidth)*roomWidth,roomY=Math.floor(py/roomHeight)*roomHeight;if(this.mode==='gba'){const rangeX=roomWidth-this.viewportWidth,rangeY=roomHeight-this.viewportHeight,localX=px-roomX,localY=py-roomY;this.x=Math.max(0,Math.min(width-this.viewportWidth,roomX+Math.trunc(localX*rangeX/roomWidth)+offset[0]));this.y=Math.max(0,Math.min(height-this.viewportHeight,roomY+Math.trunc(localY*rangeY/roomHeight)+offset[1]));return;}this.x=Math.max(0,Math.min(width-roomWidth,roomX+offset[0]));this.y=Math.max(0,Math.min(height-roomHeight,roomY+offset[1]));}
  useRoomMode(){this.mode='room';this.viewportWidth=this.world.roomWidth;this.viewportHeight=this.world.roomHeight;}
  useGbaMode(width=160,height=160){this.mode='gba';this.viewportWidth=width;this.viewportHeight=height;}
  useFreeMode(x=this.x,y=this.y){this.mode='free';this.setFreePosition(x,y);}
  setFreePosition(x,y){this.freeX=Math.max(0,Math.min(this.world.width-this.world.roomWidth,x));this.freeY=Math.max(0,Math.min(this.world.height-this.world.roomHeight,y));if(this.mode==='free'){this.x=this.freeX;this.y=this.freeY;}}
}
