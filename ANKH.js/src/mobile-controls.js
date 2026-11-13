const STOP_TOLERANCE=14;
const REACTION_TICKS=4;
export const coneDirection=(dx,dy)=>Math.abs(dx)>=Math.abs(dy)?[Math.sign(dx)||1,0]:[0,Math.sign(dy)||1];

export class MobileControls {
  constructor(canvas,buttons,input,game,renderer,settings=null,keyboard=null) {
    this.canvas=canvas;
    this.input=input;
    this.game=game;
    this.renderer=renderer;
    this.settings=settings;
    this.keyboard=keyboard;
    this.mode=null;
    this.press=null;
    this.lastRunPoint=null;
    this.stayAnchor=null;
    this.coneMode=new URLSearchParams(globalThis.location?.search??'').get('cones')==='stay'?'stay':'follow';
    this.reactionTicks=new URLSearchParams(globalThis.location?.search??'').get('lag')==='off'?0:REACTION_TICKS;
    this.hudPointers=new Set();this.settingsOpen=false;this.pausedBeforeSettings=false;
    this.buttons=new Map([...buttons].map(button=>[button.dataset.mode,button]));this.keyboardPads=new Map([['z','step'],['x','shoot'],['c','touch'],['v','stop'],[' ','stop']]);this.heldPadKeys=new Set();
    this.installButtons(buttons);
    this.installKeyboardPads();
    this.installCanvas();
    this.installSettings();
  }

  installButtons(buttons) {
    for(const button of buttons) {
      const mode=button.dataset.mode;
      button.addEventListener('pointerdown',event=>this.holdMode(event,button,mode));
      button.addEventListener('pointerup',event=>this.releaseMode(event,button,mode));
      button.addEventListener('pointercancel',event=>this.releaseMode(event,button,mode));
    }
  }

  installKeyboardPads(){
    addEventListener('keydown',event=>{const key=event.key.toLowerCase(),mode=this.keyboardPads.get(key);if(!mode||!this.renderer.mobile)return;event.preventDefault();this.heldPadKeys.delete(key);this.heldPadKeys.add(key);this.syncKeyboardPad();this.buttons.get(mode)?.classList.add('held');if(mode==='stop'&&key==='v')this.stop();});
    addEventListener('keyup',event=>{const key=event.key.toLowerCase(),mode=this.keyboardPads.get(key);if(!mode)return;this.heldPadKeys.delete(key);this.syncKeyboardPad();if(![...this.heldPadKeys].some(held=>this.keyboardPads.get(held)===mode))this.buttons.get(mode)?.classList.remove('held');});
    addEventListener('blur',()=>{this.heldPadKeys.clear();this.syncKeyboardPad();this.buttons.forEach(button=>button.classList.remove('held'));});
  }

  keyboardMode(){for(const key of [...this.heldPadKeys].reverse()){const mode=this.keyboardPads.get(key);if(mode!=='stop')return mode;}return null;}
  syncKeyboardPad(){this.keyboard?.setPadMode(this.renderer.mobile?this.keyboardMode():null);}

  holdMode(event,button,mode) {
    event.preventDefault();
    button.setPointerCapture(event.pointerId);
    button.classList.add('held');
    if(mode==='stop'){this.stop();return;}
    this.mode=mode;
  }

  releaseMode(event,button,mode) {
    event.preventDefault();
    button.classList.remove('held');
    if(this.mode===mode)this.mode=null;
  }

  installCanvas() {
    this.canvas.addEventListener('contextmenu',event=>event.preventDefault());
    this.canvas.addEventListener('pointerdown',event=>this.pointerDown(event));
    this.canvas.addEventListener('pointermove',event=>this.pointerMove(event));
    this.canvas.addEventListener('pointerup',event=>this.pointerEnd(event));
    this.canvas.addEventListener('pointercancel',event=>this.pointerEnd(event));
  }

  installSettings(){
    if(!this.settings)return;
    this.settings.querySelectorAll('[data-cone-mode]').forEach(button=>button.addEventListener('click',()=>this.setConeMode(button.dataset.coneMode)));
    this.settings.querySelectorAll('[data-reaction-lag]').forEach(button=>button.addEventListener('click',()=>this.setReactionLag(button.dataset.reactionLag==='on')));
    this.settings.querySelector('[data-settings-close]')?.addEventListener('click',()=>this.closeSettings());this.syncSettings();
  }

  syncSettings(){
    if(!this.settings)return;
    this.settings.querySelectorAll('[data-cone-mode]').forEach(button=>button.setAttribute('aria-pressed',String(button.dataset.coneMode===this.coneMode)));
    this.settings.querySelectorAll('[data-reaction-lag]').forEach(button=>button.setAttribute('aria-pressed',String((button.dataset.reactionLag==='on')===Boolean(this.reactionTicks))));
  }

  openSettings(){
    if(!this.settings||this.settingsOpen)return;
    this.settingsOpen=true;this.pausedBeforeSettings=this.game.paused;this.stop();if(!this.pausedBeforeSettings)this.input.push({type:'pause'});this.settings.hidden=false;this.hudPointers.clear();this.syncSettings();
  }

  closeSettings(){
    if(!this.settingsOpen)return;
    this.settingsOpen=false;this.settings.hidden=true;if(!this.pausedBeforeSettings)this.input.push({type:'pause'});
  }

  point(event){return this.renderer.clientToCanvas(event.clientX,event.clientY);}

  motionVelocity(){
    const pending=this.input.peek?.()??[];
    for(let index=pending.length-1;index>=0;index--){const command=pending[index];if(command.type==='run')return command.direction.map(value=>value*3);if(['stop','step','reset','debug-flyover'].includes(command.type))return [0,0];}
    return this.game.velocity;
  }

  followAnchor() {
    const [screenX,screenY]=this.playerAnchor();
    const velocity=this.motionVelocity();
    return [screenX-velocity[0]*this.renderer.worldScale*this.reactionTicks,screenY-velocity[1]*this.renderer.worldScale*this.reactionTicks];
  }

  playerAnchor(){return this.renderer.worldToCanvas(...this.game.player.center);}

  coneAnchor(point) {return this.coneMode==='follow'?this.followAnchor():this.stayAnchor??point;}

  setConeMode(mode){
    if(mode!=='follow'&&mode!=='stay')throw new Error(`Unknown cone mode: ${mode}`);
    if(this.coneMode===mode)return mode;
    this.coneMode=mode;this.press=null;this.stop();this.syncSettings();return mode;
  }

  toggleConeMode(){return this.setConeMode(this.coneMode==='follow'?'stay':'follow');}

  setReactionLag(enabled){this.reactionTicks=enabled?REACTION_TICKS:0;this.syncSettings();return Boolean(this.reactionTicks);}

  stop(){this.lastRunPoint=null;this.stayAnchor=null;this.input.push({type:'stop'});}

  direction(point,anchor) {
    const dx=point[0]-anchor[0],dy=point[1]-anchor[1];
    return coneDirection(dx,dy);
  }

  pointerDown(event) {
    if(!this.renderer.mobile)return;
    event.preventDefault();
    this.canvas.setPointerCapture(event.pointerId);
    if(this.settingsOpen)return;
    if(this.game.phase!=='game') { this.input.push({type:'start'}); return; }
    const point=this.point(event);
    const hudTarget=this.renderer.mobileHudTarget(...point);
    if(hudTarget?.type==='inventory'){this.input.push({type:'drop',slot:hudTarget.slot});return;}
    if(hudTarget?.type==='score'){this.hudPointers.add(event.pointerId);if(this.hudPointers.size>=2)this.openSettings();return;}
    const mode=this.mode??this.keyboardMode();
    if(mode) { this.input.push({type:mode,direction:this.direction(point,this.followAnchor())}); return; }
    const velocity=this.motionVelocity(),running=velocity.some(Boolean);
    if(!running)this.lastRunPoint=null;
    if(this.coneMode==='stay'&&!this.stayAnchor&&!running){this.stayAnchor=point;this.press={id:event.pointerId,direction:[0,0]};return;}
    const anchor=this.coneAnchor(point),direction=this.direction(point,anchor);
    const sameStart=running&&this.lastRunPoint&&Math.hypot(point[0]-this.lastRunPoint[0],point[1]-this.lastRunPoint[1])<=STOP_TOLERANCE;
    const playerAnchor=this.playerAnchor(),stopDirection=this.direction(point,playerAnchor);
    const opposite=(point[0]-playerAnchor[0])*velocity[0]+(point[1]-playerAnchor[1])*velocity[1]<0;
    const stop=sameStart||opposite;
    this.lastDecision={point:[...point],anchor:[...anchor],direction:[...direction],stopDirection:[...stopDirection],velocity:[...velocity],sameStart:Boolean(sameStart),opposite,stop};
    if(stop){this.press=null;this.stop();}
    else{this.press={id:event.pointerId,direction,anchor};this.lastRunPoint=point;this.input.push({type:'run',direction,replace:true});}
  }

  pointerMove(event) {
    if(!this.press||this.press.id!==event.pointerId)return;
    event.preventDefault();
    const point=this.point(event),anchor=this.coneMode==='follow'?this.followAnchor():this.stayAnchor,direction=this.direction(point,anchor);
    if(direction[0]===this.press.direction[0]&&direction[1]===this.press.direction[1])return;
    this.press.direction=direction;
    if(!this.lastRunPoint)this.lastRunPoint=point;
    this.input.push(direction[0]||direction[1]?{type:'run',direction,replace:true}:{type:'stop'});
  }

  pointerEnd(event) {
    if(this.hudPointers.delete(event.pointerId)){event.preventDefault();return;}
    if(!this.press||this.press.id!==event.pointerId)return;
    event.preventDefault();
    this.press=null;
  }
}
