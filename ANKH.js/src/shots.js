export class ShotManager {
  constructor(game) { this.game = game; this.shots = [];this.capacity=12;game.sounds?.setPolyphony?.('shot',this.capacity); }
  canShoot() { return this.shots.length < this.capacity; }
  shoot(position, velocity, owner) {
    if (!velocity[0] && !velocity[1] || !this.canShoot()) return;
    this.shots.push({ x: position[0]-1, y: position[1], width: 2, height: 1, spriteFile:'bullet', dx: velocity[0], dy: velocity[1], owner, sound:this.game.sound('shot') });
  }
  remove(shot) { shot.sound?.stop?.();return false; }
  update() {
    this.shots = this.shots.filter(shot => {
      if (this.game.collision.actorWorld(shot) || !this.game.onScreen(shot)) return this.remove(shot);
      const hit = this.game.actors.find(a => a.enabled && a.visible && a.worldSpace && this.game.onScreen(a) && a !== shot.owner && a.has('ACTOR_COLLISION_PHYSICS') && this.game.actorOverlaps(shot, a));
      if(hit){if(!shot.owner||hit.type!==shot.owner.type){hit.event('shoot',this.game,shot);this.game.countObject(hit);if(!hit.shot){hit.shot=true;this.game.addKarma(hit.shotKarma);}}return this.remove(shot);}
      shot.x += shot.dx; shot.y += shot.dy;return this.game.onScreen(shot)||this.remove(shot);
    });
  }
}
