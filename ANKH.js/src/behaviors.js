const numbers = text => [...text.replace(/\/\*[\s\S]*?\*\//g,'').matchAll(/-?\d+/g)].map(x=>Number(x[0]));
const refs = text => [...text.matchAll(/\b([a-zA-Z]+\d{3})\b/g)].map(x=>x[1]);
const eventNames = text => [...text.matchAll(/BehaviourMethod::(\w+)/g)].map(x=>x[1].toLowerCase());
const pointInside = (actor,[x,y]) => x>=actor.x&&x<=actor.x+actor.width&&y>=actor.y&&y<=actor.y+actor.height;
const sameRoom = (a,b) => a.room[0]===b.room[0]&&a.room[1]===b.room[1];
const sign = n => n<0?-1:n>0?1:0;
const randomInt=(from,to)=>{if(to<from)[from,to]=[to,from];return from+Math.floor(Math.random()*(to-from+1));};
const projectileOrigin=(actor,[x,y])=>{const [cx,cy]=actor.center;return [cx+sign(x)*Math.trunc(actor.width/2),cy+sign(y)*Math.trunc(actor.height/2)];};

export class Behavior { reset(){} handle(){} }
class ObjectBehavior extends Behavior { handle(k,a,c,g){if(k==='touch')g.inventory.pickUp(g.touchSlot,a,c);} }

class BadguyBehavior extends Behavior {
  constructor(args){super();const n=numbers(args);this.speed=[n[0]??2,n[1]??2];this.reset();}
  reset(){this.direction=[0,0];this.frequency=0;this.count=0;}
  handle(k,a,c,g){
    if(k==='shoot'){a.visible=false;a.enabled=false;return;} if(k!=='update'||++this.count<this.frequency)return;
    this.frequency=randomInt(1,2);this.count=0;const [px,py]=g.player.center,[ax,ay]=a.center;
    if(randomInt(1,30)===1){if(Math.hypot(px-ax,py-ay)<=50)this.direction=[sign(ax-px),sign(ay-py)];this.direction=this.direction.map(value=>Math.max(-1,Math.min(1,value)));}
    else {const axis=randomInt(0,1);if(!this.direction[axis])this.direction[axis]=randomInt(-1,1);else if(this.direction[1-axis])this.direction[1-axis]=0;}
    const sx=a.x-g.camera.x,sy=a.y-g.camera.y;if((this.direction[0]<0&&sx<20)||(this.direction[0]>0&&sx>140))this.direction[0]*=-1;if((this.direction[1]<0&&sy<20)||(this.direction[1]>0&&sy>148))this.direction[1]*=-1;
    const offset=[this.direction[0]*this.speed[0],this.direction[1]*this.speed[1]];if(!g.moveActorExact(a,...offset))this.direction=this.direction.map(value=>-value);
    if(++BadguyBehavior.timer>BadguyBehavior.timeout){let v=[...offset];if(BadguyBehavior.timeout>50){const d=Math.trunc(Math.hypot(px-ax,py-ay))||1;v=[Math.trunc((px-ax)*3/d),Math.trunc((py-ay)*3/d)];}while(!v[0]&&!v[1])v=[randomInt(-1,1),randomInt(-1,1)];g.shots.shoot([a.x,a.y],v,a);BadguyBehavior.timeout=randomInt(0,60);BadguyBehavior.timer=0;}
  }
}
BadguyBehavior.timer=0;
BadguyBehavior.timeout=30;
class SuckBehavior extends Behavior {
  constructor(args){super();const n=numbers(args);this.side=n[0]??1;this.damage=n[1]??-4;this.reset();}reset(){this.attached=false;this.playerPos=null;}
  handle(k,a,c,g){if(k==='touch'||k==='offscreen'){this.attached=false;a.x=a.startX;a.y=a.startY;return;}if(k!=='update')return;const old=[a.x,a.y];let dir=[0,0];if(!this.attached){const dx=g.player.x-a.x,dy=g.player.y-a.y,length=Math.trunc(Math.hypot(dx,dy)),divisor=Math.trunc(length/3)||1;dir=[Math.trunc(dx/divisor),Math.trunc(dy/divisor)];a.x+=dir[0];a.y+=dir[1];this.playerPos=[g.player.x,g.player.y];}if(this.attached||g.actorOverlaps(a,g.player)){g.player.x=this.playerPos[0];g.player.y=this.playerPos[1];g.velocity=[0,0];const [px,py]=g.player.center;a.x=Math.trunc(px-a.width/2);a.y=Math.trunc(py+this.side*(g.player.height-2)-a.height/2);g.addKarma(this.damage);this.attached=true;}if(g.collides(a,g.player)){a.x=old[0];a.y=old[1];this.attached=false;const length=Math.trunc(Math.hypot(...dir));a.y+=length*-this.side;if(g.collides(a,g.player)){a.x=old[0];a.y=old[1];}}}
}
class PadBehavior extends Behavior {constructor(args){super();this.args=numbers(args);this.inside=false;}reset(){this.inside=false;}transition(a,g,enter,inside,exit){const now=pointInside(a,g.player.center);if(now&&!this.inside)enter?.();if(now)inside?.();if(!now&&this.inside)exit?.();this.inside=now;}}
class PanpadBehavior extends PadBehavior {handle(k,a,c,g){if(k==='update')this.transition(a,g,()=>g.cameraOffset=[this.args[0]??0,this.args[1]??0],null,()=>g.cameraOffset=[0,0]);}}
class InterppadBehavior extends PadBehavior {handle(k,a,c,g){if(k!=='update')return;this.transition(a,g,null,()=>{const [sx=0,ex=0,sy=0,ey=0]=this.args,[px,py]=g.player.center;g.cameraOffset=[Math.trunc((px-a.x)*(ex-sx)/a.width)+sx,Math.trunc((py-a.y)*(ey-sy)/a.height)+sy];},()=>g.cameraOffset=[0,0]);}}
class TouchpadBehavior extends PadBehavior {handle(k,a,c,g){if(k==='update')this.transition(a,g,()=>g.extraTouch=this.args[0]??6,null,()=>g.extraTouch=0);}}

class ShooterBehavior extends Behavior {
  constructor(args){super();const n=numbers(args);this.initial={velocity:[n[0]??0,n[1]??-1],delay:n[2]??30,range:n[3]??-1,spread:n[4]??0,enabled:!args.includes('false')};this.reset();}
  reset(){this.velocity=[...this.initial.velocity];this.delay=this.initial.delay;this.range=this.initial.range;this.spread=this.initial.spread;this.enabled=this.initial.enabled;this.timer=0;this.doShot=false;}
  handle(k,a,c,g){if(k==='trigger'){if(this.delay>0)this.enabled=!this.enabled;else this.doShot=true;return;}if(k!=='update'||!g.shots.canShoot())return;const automatic=this.enabled&&this.delay>=0&&(this.delay===0||++this.timer%this.delay===0);if(!this.doShot&&!automatic)return;this.doShot=false;this.timer=0;const velocity=[this.velocity[0]+randomInt(-this.spread*this.velocity[1],this.spread*this.velocity[1]),this.velocity[1]+randomInt(-this.spread*this.velocity[0],this.spread*this.velocity[0])];g.shots.shoot(projectileOrigin(a,velocity),velocity,a);if(this.range>0)this.delay=randomInt(0,this.range);}
}
class TrapBehavior extends ShooterBehavior {
  reset(){super.reset();this.move=[0,0];}
  handle(k,a,c,g){if(k==='touch')g.inventory.pickUp(g.touchSlot,a,c);if(k==='left')this.move=[this.move[0]-3,0];if(k==='right')this.move=[this.move[0]+3,0];if(k==='up')this.move=[0,this.move[1]-3];if(k==='down')this.move=[0,this.move[1]+3];if(k==='update'&&(this.move[0]||this.move[1])){this.move=this.move.map(value=>Math.max(-3,Math.min(3,value)));const old=[a.x,a.y],moved=g.moveActor(a,...this.move,true);if(!moved&&a.x===old[0]&&a.y===old[1])this.move=[0,0];}super.handle(k,a,c,g);}
}
class FlitBehavior extends ObjectBehavior {handle(k,a,c,g){super.handle(k,a,c,g);if(k==='pickup')g.slow=true;if(k==='drop')g.slow=false;}}
class TreasureBehavior extends ObjectBehavior {constructor(args){super();this.initial=numbers(args)[0]??1;this.reset();}reset(){this.hits=this.initial;}handle(k,a,c,g){if(k==='shoot'&&--this.hits===0){a.visible=false;a.enabled=false;}super.handle(k,a,c,g);}}
class RedirectBehavior extends ObjectBehavior {constructor(args){super();const n=numbers(args);this.velocity=[n[0]??0,n[1]??-1];}handle(k,a,c,g){if(k==='shoot')g.shots.shoot(projectileOrigin(a,this.velocity),this.velocity,a);super.handle(k,a,c,g);}}
class KeyBehavior extends ObjectBehavior {constructor(args){super();this.target=refs(args)[0]??null;}handle(k,a,c,g){super.handle(k,a,c,g);if(k==='drop'&&this.target){const t=g.actorById.get(this.target);if(t&&sameRoom(a,t)){if(t.type==='horzdoor'||t.type==='vertdoor')g.sound('lock');t.event('trigger',g,a);}}}}
class IlluminaBehavior extends ObjectBehavior {handle(k,a,c,g){super.handle(k,a,c,g);if(k==='touch')g.actors.filter(x=>x.type==='treasure'&&x.enabled).forEach(x=>x.visible=true);if(k==='drop')g.actors.filter(x=>x.type==='treasure').forEach(x=>x.visible=false);}}

class TriggerBehavior extends Behavior {
  constructor(args,defaults={},unlocked=true){super();const e=eventNames(args);this.target=refs(args)[0]??null;this.send=e[0]??defaults.send??'trigger';this.receive=e[1]??defaults.receive??'shoot';this.sameRoom=defaults.sameRoom??false;this.initialUnlocked=unlocked;this.unlocked=unlocked;}
  reset(){this.unlocked=this.initialUnlocked;}
  handle(k,a,c,g){if(k==='unlock'){this.unlocked=true;return;}if(!this.unlocked||k!==this.receive||!this.target)return;const t=g.actorById.get(this.target);if(t&&(!this.sameRoom||sameRoom(a,t))){if(t.type==='horzdoor'||t.type==='vertdoor')g.sound('lock');t.event(this.send,g,a);}}
}
class MusicboxBehavior extends TriggerBehavior {constructor(args){super(args,{sameRoom:true});}handle(k,a,c,g){if(k==='touch'){g.inventory.pickUp(g.touchSlot,a,c);return;}const sounds=a.behaviorSpec;if(k==='shoot'&&sounds.shotSound)g.sound(sounds.shotSound);if(k==='pickup'&&sounds.songSound)g.sound(sounds.songSound);if(k==='drop'&&sounds.songSound)g.sounds?.stop?.(sounds.songSound);super.handle(k,a,c,g);}}
class TriggerpadBehavior extends TriggerBehavior {constructor(args){super(args,{receive:'enter'});this.inside=false;}reset(){super.reset();this.inside=false;}handle(k,a,c,g){if(k!=='update')return;const now=pointInside(a,g.player.center);if(now&&!this.inside)super.handle(this.receive,a,c,g);this.inside=now;}}
class StatetriggerBehavior extends Behavior {constructor(args){super();this.targets=refs(args);this.send=eventNames(args)[0]??'trigger';this.reset();}reset(){this.state=0;}handle(k,a,c,g){if(k==='up')this.state=1;else if(k==='left')this.state=this.state===1?2:0;else if(k==='down')this.state=this.state===2?3:0;else if(k==='right')this.state=this.state===3?4:0;if(this.state===4){const targets=this.targets.map(id=>g.actorById.get(id)).filter(Boolean);if(targets.some(t=>t.type==='horzdoor'||t.type==='vertdoor'))g.sound('lock');targets.forEach(t=>t.event(this.send,g,a));this.state=0;}}}
class DoorBehavior extends Behavior {constructor(args,axis){super();const p=args.split(',').map(x=>x.trim());this.axis=axis;this.initial={open:p[0]==='true',direction:p[1]===undefined?1:Number(p[1]),distance:p[2]===undefined?-1:Number(p[2])};this.reset();}reset(){this.open=this.initial.open;this.direction=this.initial.direction;this.distance=this.initial.distance;this.count=0;this.soundPending=false;}handle(k,a,c,g){if(k==='trigger'){this.open=!this.open;this.soundPending=true;return;}if(k!=='update')return;const dist=this.distance<0?(this.axis===0?a.width:a.height):this.distance;if(this.open&&this.count>-dist){if(this.soundPending){g.sound('doormove');this.soundPending=false;}this.count--;if(this.axis===0)a.x-=this.direction;else a.y-=this.direction;}else if(!this.open&&this.count<=0){if(this.soundPending){g.sound('doormove');this.soundPending=false;}this.count++;if(this.axis===0)a.x+=this.direction;else a.y+=this.direction;}}}
class OscBehavior extends ObjectBehavior {constructor(args){super();const n=numbers(args);this.initial=[n[0]??1,n[1]??0];this.reset();}reset(){this.direction=[...this.initial];}handle(k,a,c,g){if(k==='update'&&!g.moveActorExact(a,...this.direction))this.direction=this.direction.map(x=>-x);super.handle(k,a,c,g);}}
class PlayerdroneBehavior extends Behavior {constructor(){super();this.reset();}reset(){this.direction=[0,0];}handle(k,a,c,g){const map={up:[0,-1],down:[0,1],right:[-1,0],left:[1,0],stop:[0,0]};if(map[k])this.direction=map[k];else if(k==='shoot')g.shots.shoot(a.muzzle([1,0]),[1,0],a);else if(k==='exit'){a.x=a.startX;a.y=a.startY;}else if(k==='update'&&!g.moveActorExact(a,...this.direction))this.direction=[0,0];}}
class PlayerBehavior extends Behavior {handle(k,a,c,g){if(k==='shoot')g.addKarma(a.touchKarma);else if(k==='collide')g.addKarma(a.collideKarma??0);else if(k==='run')g.sound('movement');else if(k==='stop')g.sounds?.stop?.('movement');else if(k==='step'){g.sounds?.stop?.('movement');g.sound('step');}}}

export function createBehavior({type:t,args:a,unlocked=true}){
  if(['none','tele','tonegen'].includes(t))return new Behavior();if(t==='player')return new PlayerBehavior();if(t==='badguy'||t==='benign')return new BadguyBehavior(a);
  if(['object','candy','bonus','chalice'].includes(t))return new ObjectBehavior();if(t==='flit')return new FlitBehavior();if(t==='treasure')return new TreasureBehavior(a);if(t==='redirect')return new RedirectBehavior(a);if(t==='key')return new KeyBehavior(a);if(t==='illumina')return new IlluminaBehavior();
  if(t==='suck')return new SuckBehavior(a);if(t==='panpad')return new PanpadBehavior(a);if(t==='interppad')return new InterppadBehavior(a);if(t==='touchpad')return new TouchpadBehavior(a);if(t==='shooter')return new ShooterBehavior(a);if(t==='trap')return new TrapBehavior(a);
  if(t==='doorlock'||t==='trigger')return new TriggerBehavior(a,{},unlocked);if(t==='musicbox')return new MusicboxBehavior(a);if(t==='triggerpad')return new TriggerpadBehavior(a);if(t==='statetrigger')return new StatetriggerBehavior(a);
  if(t==='horzdoor')return new DoorBehavior(a,0);if(t==='vertdoor')return new DoorBehavior(a,1);if(t==='osc')return new OscBehavior(a);if(t==='playerdrone')return new PlayerdroneBehavior();throw new Error(`Unported GBA behavior: ${t}`);
}
