import { ACTOR_DEFINITIONS } from './data.js';
import { Actor } from './actor.js';
import { createBehavior } from './behaviors.js';
import { Inventory } from './inventory.js';
import { ShotManager } from './shots.js';
import { World } from './world.js';
import { CollisionSystem } from './collision.js';
import { Camera } from './camera.js';
import { TouchManager } from './touch.js';
import { ActorManager } from './actor-manager.js';

export class Game {
  constructor(input, assets, sound=null) {
    this.input=input;this.assets=assets;this.sounds=sound;this.world=new World();this.world.atlas=assets.walls;
    this.actors=ACTOR_DEFINITIONS.map(def=>new Actor(def,createBehavior(def.behavior)));
    this.actorById=new Map(this.actors.map(a=>[a.id,a]));this.player=this.actorById.get('player');
    this.camera=new Camera(this.world);this.collision=new CollisionSystem(this.world,this.assets,()=>this.actors,a=>this.onScreen(a));
    this.actorManager=new ActorManager(this.actors,this.player,actor=>this.onScreen(actor));
    this.inventory=new Inventory(this);this.shots=new ShotManager(this);this.touches=new TouchManager(this);this.phase='intro';this.titleIndex=0;this.titleTicks=0;this.reset(false);
  }
  sound(id,options){return this.sounds?.play(id,options);}
  reset(playSpawnSound=true) {
    this.sounds?.stopAll?.();this.sounds?.unpause?.();
    this.actors.forEach(a=>a.reset());this.actors.filter(a=>a.type==='treasure'||a.type==='none').forEach(a=>a.visible=false);
    this.player.visible=true;this.player.enabled=true;this.velocity=[0,0];this.camera.resetPosition();this.karma=15;this.objects=1;this.rooms=new Set();this.paused=false;this.debugFlyover=false;this.debugResetPosition=null;this.touchSlot=0;
    this.inventory.slots.fill(null);this.shots.shots=[];this.touches.reset();this.cameraOffset=[0,0];this.rumbleCounter=0;this.extraTouch=0;this.slow=false;this.slowTick=0;this.lastRoom=null;
    if(playSpawnSound)this.sound('resurrection');
  }
  actorOverlaps(a,b) { return this.collision.actorsOverlap(a,b); }
  pixelOverlap(a,b) { return this.collision.actorsOverlap(a,b); }
  collides(actor,exclude=null) { return this.collision.actor(actor,exclude); }
  bump(){const offsets=[[1,0],[0,1],[-1,0],[0,-1]],[dx,dy]=offsets[this.rumbleCounter];this.player.x+=dx;this.player.y+=dy;this.rumbleCounter=(this.rumbleCounter+1)%offsets.length;}
  restart(){
    const position=[this.player.x,this.player.y],moved=this.debugFlyover&&this.debugResetPosition?.some((value,index)=>value!==position[index]);
    if(!moved){this.reset();this.camera.follow(this.player,this.cameraOffset);return;}
    this.reset();this.player.x=position[0];this.player.y=position[1];this.debugFlyover=true;this.debugResetPosition=position;this.camera.follow(this.player,this.cameraOffset);
  }
  moveActor(actor,dx,dy,physics=true,exclude=null){if(!physics||(this.debugFlyover&&actor===this.player)){actor.x=Math.max(0,Math.min(this.world.width-actor.width,actor.x+dx));actor.y=Math.max(0,Math.min(this.world.height-actor.height,actor.y+dy));return true;}const oldX=actor.x,oldY=actor.y,moved=this.collision.move(actor,dx,dy,exclude,actor===this.player);if(actor===this.player&&!moved){actor.event('collide',this);if(actor.x===oldX&&actor.y===oldY)this.bump();}return moved;}
  moveActorExact(actor,dx,dy,exclude=null){const [x,y]=[actor.x,actor.y];actor.x+=dx;actor.y+=dy;if(this.collides(actor,exclude)){actor.x=x;actor.y=y;return false;}return true;}
  onScreen(a) { return a.x+a.width>=this.camera.x&&a.x<=this.camera.x+this.camera.viewportWidth&&a.y+a.height>=this.camera.y&&a.y<=this.camera.y+this.camera.viewportHeight; }
  touch(direction) { this.touches.start(direction); }
  setKarma(value){const previous=this.karma,next=Math.max(0,Math.min(99999,Math.trunc(value)));this.karma=next;if(previous>0&&next===0)this.sound('death');}
  addKarma(value){this.setKarma(this.karma+value);}
  setObjects(value){this.objects=Math.max(0,Math.min(99999,Math.trunc(value)));}
  countObject(actor){if(!actor.counted){actor.counted=true;this.setObjects(this.objects+1);}}
  setDebugFlyover(enabled){this.debugFlyover=Boolean(enabled);this.debugResetPosition=this.debugFlyover?[this.player.x,this.player.y]:null;this.velocity=[0,0];}
  applyCommand(command) {
    if(this.paused&&!['pause','reset','debug-flyover'].includes(command.type))return false;
    const [dx,dy]=command.direction??[0,0];
    switch(command.type){
      case 'stop':this.velocity=[0,0];this.player.event('stop',this);break;
      case 'run':{const wasRunning=this.velocity.some(Boolean);if(command.replace)this.velocity=[dx*3,dy*3];else this.velocity=dx?[this.velocity[0]?0:dx*3,0]:[0,this.velocity[1]?0:dy*3];const running=this.velocity.some(Boolean);if(!wasRunning&&running)this.player.event('run',this);else if(wasRunning&&!running)this.player.event('stop',this);break;}
      case 'step':this.velocity=[0,0];this.player.event('step',this);this.moveActor(this.player,dx*3,dy*3);break;
      case 'shoot':this.shots.shoot(this.player.muzzle([dx,dy]),[dx*2,dy*2],this.player);break;
      case 'touch':this.touch([dx,dy]);break;
      case 'drop':this.touches.startDrop(command.slot);break;
      case 'pause':this.paused=!this.paused;if(this.paused)this.sounds?.pause?.();else this.sounds?.unpause?.();this.player.event(!this.paused&&this.velocity.some(Boolean)?'run':'stop',this);break;
      case 'debug-flyover':this.setDebugFlyover(!this.debugFlyover);break;
      case 'reset':this.restart();break;
    }
    return true;
  }
  applyCommands(commands){let gameplayBlocked=this.paused;for(const command of commands){const allowed=['pause','reset','debug-flyover'].includes(command.type);if(gameplayBlocked&&!allowed)continue;this.applyCommand(command);if(this.paused)gameplayBlocked=true;}}
  update() {
    if(this.phase==='intro'){const commands=this.input.drain();if(commands.some(command=>command.type==='start'||command.type==='stop')){this.phase='game';this.sounds?.unpause?.();this.sound('resurrection');}else if(++this.titleTicks%180===0)this.titleIndex=(this.titleIndex+1)%9;return;}
    if(this.paused){this.applyCommands(this.input.drain());if(this.paused)return;}
    if(this.slow){if(++this.slowTick<3)return;this.slowTick=0;}else this.slowTick=0;
    this.applyCommands(this.input.drain());if(this.paused)return;
    this.moveActor(this.player,this.velocity[0],this.velocity[1]);
    let [px,py]=this.player.center,rx=Math.max(0,Math.min(Math.ceil(this.world.width/this.world.roomWidth)-1,Math.floor(px/this.world.roomWidth))),ry=Math.max(0,Math.min(Math.ceil(this.world.height/this.world.roomHeight)-1,Math.floor(py/this.world.roomHeight)));const roomKey=`${rx},${ry}`;this.rooms.add(roomKey);if(!this.debugFlyover&&this.lastRoom!==null&&roomKey!==this.lastRoom){this.player.x+=this.velocity[0]*5;this.player.y+=this.velocity[1]*5;this.velocity=[0,0];this.player.event('stop',this);[px,py]=this.player.center;}this.lastRoom=roomKey;
    this.camera.follow(this.player,this.cameraOffset);
    this.actorManager.update(this);
    this.actors.filter(a=>a!==this.player&&a.enabled&&a.visible&&a.worldSpace&&this.onScreen(a)&&a.has('ACTOR_COLLISION_TRIGGER')&&this.actorOverlaps(a,this.player)).slice(0,1).forEach(a=>a.event('collide',this,this.player));this.touches.update();this.shots.update();
  }
}
