// Platform-neutral counterpart of the GBA ActorDrawManager.
// Rendering is delegated, but on-screen membership and behaviour timing stay here.
export class ActorManager {
  constructor(actors,player,isOnScreen){this.actors=actors;this.player=player;this.isOnScreen=isOnScreen;}
  update(game){
    for(const actor of this.actors){
      if(actor===this.player)continue;
      const was=actor.wasOnScreen;
      if(was)actor.event('update',game);
      const onScreen=actor.worldSpace?this.isOnScreen(actor):actor.x+actor.width>=0&&actor.x<=240&&actor.y+actor.height>=0&&actor.y<=160;
      const now=actor.enabled&&actor.visible&&onScreen;
      if(was&&!now)actor.event('offscreen',game);
      if(!was&&now)actor.frame=0;
      actor.wasOnScreen=now;
      if(now)actor.frame=(actor.frame+1)%actor.frameCount;
    }
  }
}
