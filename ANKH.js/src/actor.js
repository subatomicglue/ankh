export const Flags = { VISIBLE: 1, COLLISION: 2, TRIGGER: 4, PHYSICS: 8, PICKUP: 16 };

export class Actor {
  constructor(definition, behavior) {
    Object.assign(this, definition);
    this.startX = this.x; this.startY = this.y;
    this.behavior = behavior;
    this.width = definition.box[0]; this.height = definition.box[1];
    // GBA Actor keeps drawing-list membership separate from ACTOR_VISIBLE.
    // Invisible trigger actors still enter ActorDrawManager and run behaviours.
    this.visible = true;
    this.wasOnScreen = false;
    this.enabled = true; this.worldSpace = true; this.depth = 1;
    this.touched = false; this.shot = false; this.counted = false;
    this.spriteFile = definition.image;
    this.frameCount=definition.frames??1;this.frame=0;
  }
  get center() { return [this.x+Math.trunc(this.width/2),this.y+Math.trunc(this.height/2)]; }
  muzzle([dx,dy]) {
    const [x,y]=this.center;
    if(dx<0)return [x-Math.trunc(this.width/2),y];
    if(dx>0)return [x+Math.trunc(this.width/2),y];
    if(dy<0)return [x+1,y-Math.trunc(this.height/2)];
    return [x+1,y+Math.trunc(this.height/2)];
  }
  touchOrigin([dx,dy]) {
    const [x,y]=this.center;
    if(dx<0)return [x-Math.trunc(this.width/2),y];
    if(dx>0)return [x+Math.trunc(this.width/2)+2,y];
    if(dy<0)return [x,y-Math.trunc(this.height/2)];
    return [x,y+Math.trunc(this.height/2)+2];
  }
  get room() { const [x, y] = this.center; return [Math.floor(x / 192), Math.floor(y / 192)]; }
  has(name) {
    if(name==='ACTOR_COLLISION')return this.flags.some(flag=>flag.startsWith('ACTOR_COLLISION'));
    return this.flags.includes(name);
  }
  reset() { this.x = this.startX; this.y = this.startY; this.enabled = true; this.visible = true; this.worldSpace=true;this.depth=1;this.frame=0;this.touched=false;this.shot=false;this.counted=false;this.wasOnScreen=false;this.behavior?.reset(); }
  event(kind, game, caller = null) { this.behavior?.handle(kind, this, caller, game); }
}

export function overlaps(a, b) {
  return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y;
}
