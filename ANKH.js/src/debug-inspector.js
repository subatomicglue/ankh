const escapeHtml=value=>String(value).replace(/[&<>"']/g,char=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[char]));

export class DebugInspector {
  constructor(canvas,game,renderer) {
    this.canvas=canvas;this.game=game;this.renderer=renderer;
    this.popup=document.createElement('aside');this.popup.className='debug-inspector';this.popup.hidden=true;this.popup.setAttribute('role','status');
    canvas.parentElement.append(this.popup);
    this.onPointerDown=event=>this.inspect(event);
    this.onDocumentPointerDown=event=>this.dismissFromPointer(event);
    this.onKeyDown=event=>this.dismissFromKey(event);
    this.onToggle=()=>requestAnimationFrame(()=>this.constrainPopup());
    this.onResize=()=>requestAnimationFrame(()=>this.constrainPopup());
    canvas.addEventListener('pointerdown',this.onPointerDown,true);
    document.addEventListener('pointerdown',this.onDocumentPointerDown,true);
    addEventListener('keydown',this.onKeyDown,true);
    this.popup.addEventListener('toggle',this.onToggle,true);
    addEventListener('resize',this.onResize);visualViewport?.addEventListener('resize',this.onResize);
  }

  worldPoint(event) {
    const [x,y]=this.renderer.clientToCanvas(event.clientX,event.clientY);
    if(x<0||x>=this.renderer.outputWidth||y<this.renderer.viewportY||y>=this.renderer.viewportY+this.renderer.outputHeight)return null;
    return {canvasX:x,canvasY:y,x:this.game.camera.x+x/this.renderer.worldScale,y:this.game.camera.y+(y-this.renderer.viewportY)/this.renderer.worldScale};
  }

  actorAt(x,y) {
    const actors=this.game.actors.filter(actor=>actor.enabled&&actor.visible&&actor.worldSpace&&actor.has('ACTOR_VISIBLE')&&this.game.onScreen(actor));
    actors.sort((a,b)=>b.depth-a.depth);
    for(let index=actors.length-1;index>=0;index--){
      const actor=actors[index],localX=Math.floor(x-actor.x),localY=Math.floor(y-actor.y),bitmap=this.game.assets.bitmaps.get(actor.spriteFile),frameHeight=bitmap?bitmap.height/actor.frameCount:actor.height;
      if(localX<0||localY<0||localX>=(bitmap?.width??actor.width)||localY>=frameHeight)continue;
      if(!bitmap||bitmap.pixel(localX,Math.floor(actor.frame*frameHeight)+localY)!==0)return actor;
    }
    return null;
  }

  actorHtml(actor) {
    const definition=actor.behavior?`${actor.behavior.constructor.name}`:'none',args=actor.behaviorSpec?.args||'—';
    const initialization=JSON.stringify(actor.behaviorSpec,null,2),runtime=JSON.stringify(actor.behavior,(key,value)=>typeof value==='function'?undefined:value,2);
    return `<strong>${escapeHtml(actor.id)}</strong><dl><dt>actor</dt><dd>${escapeHtml(actor.type)}</dd><dt>world</dt><dd>${actor.x}, ${actor.y}</dd><dt>room</dt><dd>${actor.roomX}, ${actor.roomY}</dd><dt>local</dt><dd>${actor.x-actor.roomX*192}, ${actor.y-actor.roomY*192}</dd><dt>sprite</dt><dd>${escapeHtml(actor.spriteFile)} · frame ${actor.frame}/${actor.frameCount}</dd><dt>behavior</dt><dd>${escapeHtml(definition)}</dd><dt>arguments</dt><dd>${escapeHtml(args)}</dd><dt>flags</dt><dd>${escapeHtml(actor.flags.join(', '))}</dd></dl><details><summary>initialization data</summary><pre>${escapeHtml(initialization)}</pre></details><details><summary>runtime behavior data</summary><pre>${escapeHtml(runtime)}</pre></details>`;
  }

  tileHtml(x,y) {
    const world=this.game.world,size=world.tileSize,tx=Math.floor(x/size),ty=Math.floor(y/size),tile=world.tileAt(x,y),roomX=Math.floor(x/world.roomWidth),roomY=Math.floor(y/world.roomHeight);
    return `<strong>wall tile ${tile}</strong><dl><dt>tile cell</dt><dd>${tx}, ${ty}</dd><dt>world pixel</dt><dd>${Math.floor(x)}, ${Math.floor(y)}</dd><dt>room</dt><dd>${roomX}, ${roomY}</dd><dt>room tile</dt><dd>${tx-roomX*(world.roomWidth/size)}, ${ty-roomY*(world.roomHeight/size)}</dd><dt>tile pixel</dt><dd>${Math.floor(x)%size}, ${Math.floor(y)%size}</dd><dt>palette</dt><dd>${world.pixelAt(x,y)}</dd><dt>collision</dt><dd>${world.isSolidPixel(x,y)?'solid':'void'}</dd></dl>`;
  }

  show(event,html) {
    this.popup.innerHTML=html;this.popup.hidden=false;
    const parent=this.canvas.parentElement.getBoundingClientRect(),gap=10;
    this.popup.style.maxHeight='';
    this.popup.style.left=`${event.clientX-parent.left+gap}px`;this.popup.style.top=`${event.clientY-parent.top+gap}px`;
    const box=this.popup.getBoundingClientRect();
    this.popup.style.left=`${Math.max(4,Math.min(parent.width-box.width-4,event.clientX-parent.left+gap))}px`;
    this.popup.style.top=`${Math.max(4,Math.min(parent.height-box.height-4,event.clientY-parent.top+gap))}px`;
    this.constrainPopup();
  }

  constrainPopup(){
    if(this.popup.hidden)return;
    const parent=this.canvas.parentElement.getBoundingClientRect(),margin=4,width=this.popup.offsetWidth;
    const left=Math.max(margin,Math.min(parent.width-width-margin,parseFloat(this.popup.style.left)||margin));
    const top=Math.max(margin,Math.min(parent.height-margin-1,parseFloat(this.popup.style.top)||margin));
    this.popup.style.left=`${left}px`;this.popup.style.top=`${top}px`;
    this.popup.style.maxHeight=`${Math.max(1,parent.height-top-margin)}px`;
  }

  hide(){this.popup.hidden=true;this.popup.replaceChildren();}

  update(){if(!this.game.debugFlyover&&!this.popup.hidden)this.hide();}

  dismissFromPointer(event){
    if(this.popup.hidden||this.popup.contains(event.target))return;
    if(event.target===this.canvas)return;
    event.preventDefault();event.stopImmediatePropagation();this.hide();
  }

  dismissFromKey(event){
    if(this.popup.hidden||event.key.toLowerCase()!=='escape')return;
    this.hide();
    event.preventDefault();event.stopImmediatePropagation();
  }

  inspect(event) {
    if(!this.game.debugFlyover||this.game.phase!=='game')return;
    const canvasPoint=this.renderer.clientToCanvas(event.clientX,event.clientY);
    if(this.renderer.mobile&&this.renderer.mobileHudTarget(...canvasPoint))return;
    event.preventDefault();event.stopImmediatePropagation();
    const point=this.worldPoint(event);if(!point)return;
    const actor=this.actorAt(point.x,point.y);
    this.show(event,actor?this.actorHtml(actor):this.tileHtml(point.x,point.y));
  }

  destroy(){this.canvas.removeEventListener('pointerdown',this.onPointerDown,true);document.removeEventListener('pointerdown',this.onDocumentPointerDown,true);removeEventListener('keydown',this.onKeyDown,true);this.popup.removeEventListener('toggle',this.onToggle,true);removeEventListener('resize',this.onResize);visualViewport?.removeEventListener('resize',this.onResize);this.popup.remove();}
}
