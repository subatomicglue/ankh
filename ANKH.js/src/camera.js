export class Camera {
  constructor(){this.reset();}
  reset(){this.mode='room';this.viewportWidth=192;this.viewportHeight=192;this.resetPosition();}
  resetPosition(){this.x=0;this.y=0;this.freeX=0;this.freeY=0;}
  follow(player,offset=[0,0]){if(this.mode==='free'){this.x=this.freeX;this.y=this.freeY;return;}const [px,py]=player.center,roomX=Math.floor(px/192)*192,roomY=Math.floor(py/192)*192;if(this.mode==='gba'){const rangeX=192-this.viewportWidth,rangeY=192-this.viewportHeight,localX=px-roomX,localY=py-roomY;this.x=Math.max(0,Math.min(1536-this.viewportWidth,roomX+Math.trunc(localX*rangeX/192)+offset[0]));this.y=Math.max(0,Math.min(1536-this.viewportHeight,roomY+Math.trunc(localY*rangeY/192)+offset[1]));return;}this.x=Math.max(0,Math.min(1344,roomX+offset[0]));this.y=Math.max(0,Math.min(1344,roomY+offset[1]));}
  useRoomMode(){this.mode='room';this.viewportWidth=192;this.viewportHeight=192;}
  useGbaMode(width=160,height=160){this.mode='gba';this.viewportWidth=width;this.viewportHeight=height;}
  useFreeMode(x=this.x,y=this.y){this.mode='free';this.setFreePosition(x,y);}
  setFreePosition(x,y){this.freeX=Math.max(0,Math.min(1344,x));this.freeY=Math.max(0,Math.min(1344,y));if(this.mode==='free'){this.x=this.freeX;this.y=this.freeY;}}
}
