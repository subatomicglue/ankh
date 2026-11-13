export class Inventory {
  constructor(game) { this.game = game; this.slots = Array(4).fill(null); }
  pickUp(preferred, actor, caller) {
    if (!actor || !caller || !actor.has('ACTOR_PICKUP')) return false;
    let slot = this.slots[preferred] ? this.slots.indexOf(null) : preferred;
    if (slot < 0) return false;
    this.slots[slot] = { actor, offset: [actor.x - caller.x, actor.y - caller.y] };
    const centers=[[188,20],[220,20],[188,52],[220,52]],center=centers[slot];
    actor.worldSpace = false;actor.x=center[0]-Math.trunc(actor.width/2);actor.y=center[1]-Math.trunc(actor.height/2);actor.depth = 0; actor.visible = true;
    actor.event('pickup', this.game, caller);
    return true;
  }
  drop(slot) {
    const item = this.slots[slot]; if (!item) return false;
    const actor = item.actor, [ox, oy] = item.offset,old=[actor.x,actor.y];
    actor.worldSpace = true; actor.x = this.game.player.x + ox; actor.y = this.game.player.y + oy;
    if (this.game.collides(actor)) { actor.worldSpace = false;actor.x=old[0];actor.y=old[1];return false; }
    actor.depth = 1; actor.event('drop', this.game, this.game.player); this.slots[slot] = null; return true;
  }
}
