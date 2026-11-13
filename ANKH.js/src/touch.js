const SLOT_DIRECTIONS=[[-1,0],[0,-1],[1,0],[0,1]];
const directionSlot=([x,y])=>x<0?0:y<0?1:x>0?2:3;

export class TouchManager {
  constructor(game){this.game=game;this.reset();}
  reset(){this.active=false;this.mode='touch';this.animation=0;this.travel=1;this.touched=false;this.shapes=[];}
  begin(mode,slot,direction,length){if(this.active)return false;this.active=true;this.mode=mode;this.slot=slot;this.direction=[...direction];this.length=Math.max(1,length);this.animation=0;this.travel=1;this.touched=false;this.shapes=[];return true;}
  start(direction){return this.begin('touch',directionSlot(direction),direction,24+this.game.extraTouch);}
  startDrop(slot){
    const item=this.game.inventory.slots[slot];if(!item)return false;
    const [ox,oy]=item.offset,actor=item.actor,player=this.game.player;
    const lengths=[Math.abs(ox)-actor.width-2,Math.abs(oy)-actor.height-2,Math.abs(ox)-player.width-3,Math.abs(oy)-player.height-3];
    return this.begin('drop',slot,SLOT_DIRECTIONS[slot],lengths[slot]);
  }
  geometry(){
    const player=this.game.player,[dx,dy]=this.direction,[cx,cy]=player.center,[edgeX,edgeY]=player.touchOrigin(this.direction),endX=edgeX+dx*this.animation,endY=edgeY+dy*this.animation;
    const horizontal=dx!==0,kind=horizontal?'touchhorz':'touchvert';
    const first={kind,x:dx<0?endX:endX-(dx>0?16:0),y:dy<0?endY:endY-(dy>0?16:0)};
    const shapes=[first];
    if(this.animation>16)shapes.push({kind,x:edgeX+dx*16-(dx>0?16:0),y:edgeY+dy*16-(dy>0?16:0)});
    let probe;
    if(dx<0)probe={x:endX,y:cy-1,width:player.x-endX,height:2};
    else if(dx>0)probe={x:player.x+player.width,y:cy-1,width:endX-(player.x+player.width),height:2};
    else if(dy<0)probe={x:cx-1,y:endY,width:2,height:player.y-endY};
    else probe={x:cx-1,y:player.y+player.height,width:2,height:endY-(player.y+player.height)};
    return {shapes,probe};
  }
  touchActor(probe){
    if(this.mode!=='touch'||this.touched)return;
    const hit=this.game.collision.firstActor(probe,this.game.player);if(!hit)return;
    this.game.touchSlot=this.slot;hit.event('touch',this.game,this.game.player);
    this.game.countObject(hit);
    if(!hit.touched){hit.touched=true;this.game.addKarma(hit.touchKarma);}
    this.touched=true;this.travel=-1;
  }
  retract(){
    if(this.travel<0)return;
    this.travel=-1;
    if(this.mode==='drop')this.game.inventory.drop(this.slot);
  }
  update(){
    if(!this.active){this.shapes=[];return;}
    this.animation+=this.travel;const {shapes,probe}=this.geometry();this.shapes=shapes;this.touchActor(probe);
    if(this.game.collision.beamWorld(probe,this.direction)||this.animation>this.length)this.retract();
    if(this.travel<0&&this.animation<=1)this.reset();
  }
}
