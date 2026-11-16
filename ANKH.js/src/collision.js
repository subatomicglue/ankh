export class CollisionSystem {
  constructor(world,assets,getActors,isOnScreen){this.world=world;this.assets=assets;this.getActors=getActors;this.isOnScreen=isOnScreen;}
  actorPixel(actor,x,y){return this.assets.actorPixel(actor,x,y);}
  firstWorldContact(actor){return this.world.firstCollision(actor,(x,y)=>this.actorPixel(actor,x,y));}
  actorWorld(actor,direction=null){const mask=(x,y)=>this.actorPixel(actor,x,y);return direction?this.world.collidesPastThinSurface(actor,mask,direction,2):this.world.collidesBox(actor,mask);}
  worldBox(box){return this.world.collidesBox(box);}
  beamWorld(box,direction){return this.world.collidesPastThinSurface(box,null,direction);}
  actorsOverlap(a,b){
    const l=Math.max(Math.floor(a.x),Math.floor(b.x)),t=Math.max(Math.floor(a.y),Math.floor(b.y)),r=Math.min(Math.floor(a.x+a.width),Math.floor(b.x+b.width)),bt=Math.min(Math.floor(a.y+a.height),Math.floor(b.y+b.height));if(l>r||t>bt)return false;
    for(let y=t;y<=bt;y++)for(let x=l;x<=r;x++){const ap=this.actorPixel(a,x-Math.floor(a.x),y-Math.floor(a.y)),bp=this.actorPixel(b,x-Math.floor(b.x),y-Math.floor(b.y));if(ap&&bp)return true;}return false;
  }
  actorActors(actor,exclude=null){return this.getActors().some(other=>other!==actor&&other!==exclude&&other.enabled&&other.visible&&other.worldSpace&&this.isOnScreen(other)&&other.has('ACTOR_COLLISION_PHYSICS')&&this.actorsOverlap(actor,other));}
  actor(actor,exclude=null,direction=null){return this.actorWorld(actor,direction)||this.actorActors(actor,exclude);}
  move(actor,dx,dy,exclude=null,tolerateThin=false) {
    const steps=Math.max(Math.abs(dx),Math.abs(dy));
    if(!steps)return true;
    let previousX=0,previousY=0;
    for(let step=1;step<=steps;step++) {
      const nextX=Math.round(dx*step/steps),nextY=Math.round(dy*step/steps);
      const moveX=nextX-previousX,moveY=nextY-previousY;
      actor.x+=moveX;actor.y+=moveY;
      if(this.actor(actor,exclude,tolerateThin?[Math.sign(moveX),Math.sign(moveY)]:null)) {
        actor.x-=moveX;actor.y-=moveY;
        return false;
      }
      previousX=nextX;previousY=nextY;
    }
    return true;
  }
  probeActor(probe,actor){const l=Math.max(Math.floor(probe.x),Math.floor(actor.x)),t=Math.max(Math.floor(probe.y),Math.floor(actor.y)),r=Math.min(Math.floor(probe.x+probe.width),Math.floor(actor.x+actor.width)),b=Math.min(Math.floor(probe.y+probe.height),Math.floor(actor.y+actor.height));for(let y=t;y<=b;y++)for(let x=l;x<=r;x++)if(this.actorPixel(actor,x-Math.floor(actor.x),y-Math.floor(actor.y)))return true;return false;}
  firstActor(probe,exclude=null){return this.getActors().find(actor=>actor!==exclude&&actor.enabled&&actor.visible&&actor.worldSpace&&actor.wasOnScreen&&actor.has('ACTOR_COLLISION_PHYSICS')&&this.probeActor(probe,actor));}
}
