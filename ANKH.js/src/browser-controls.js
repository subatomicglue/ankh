const DIRECTIONS={w:[0,-1],a:[-1,0],s:[0,1],d:[1,0],i:[0,-1],j:[-1,0],k:[0,1],l:[1,0]};
const SAME_TAP_RADIUS_CSS=22;
const REACTION_TICKS=4;
const HUD_LONG_PRESS_MS=600;
const HUD_LONG_PRESS_TOLERANCE=8;
export const coneDirection=(dx,dy)=>Math.abs(dx)>=Math.abs(dy)?[Math.sign(dx)||1,0]:[0,Math.sign(dy)||1];
export const oppositeDirection=(direction,velocity)=>direction[0]*velocity[0]+direction[1]*velocity[1]<0;

export class KeyboardControls {
  constructor(target,input,onInput=()=>{},onUnknown=()=>{}){this.input=input;this.onInput=onInput;this.onUnknown=onUnknown;this.padMode=null;target.addEventListener('keydown',event=>this.keyDown(event));}
  setPadMode(mode){this.padMode=mode;}
  keyDown(event){
    this.onInput(event);if(event.repeat)return;const key=event.key.toLowerCase(),direction=DIRECTIONS[key];
    if(direction){event.preventDefault();const movement='ijkl'.includes(key),modified=event.shiftKey||event.ctrlKey;this.input.push({type:movement?(this.padMode??(modified?'step':'run')):(modified?'touch':'shoot'),direction});return;}
    const commands={' ':{type:'stop'},escape:{type:'pause'},p:{type:'pause'},r:{type:'reset'},f2:{type:'debug-flyover'},'`':{type:'debug-flyover'},'~':{type:'debug-flyover'},enter:{type:'start'},'1':{type:'drop',slot:0},'2':{type:'drop',slot:1},'3':{type:'drop',slot:2},'4':{type:'drop',slot:3}};
    if(commands[key]){event.preventDefault();this.input.push(commands[key]);return;}this.onUnknown(event);
  }
}

export class BrowserControls {
  constructor(canvas,buttons,input,game,renderer,settings=null,soundManager=null) {
    this.canvas=canvas;
    this.input=input;
    this.game=game;
    this.renderer=renderer;
    this.settings=settings;
    this.soundManager=soundManager;
    this.keyboard=new KeyboardControls(globalThis,input,()=>this.activateAudio(),event=>this.unknownInput(event));
    this.mode=null;
    this.press=null;
    this.lastRunClientPoint=null;
    this.reactionTicks=new URLSearchParams(globalThis.location?.search??'').get('lag')==='off'?0:REACTION_TICKS;
    this.hudGesture=null;this.settingsOpen=false;this.pausedBeforeSettings=false;
    this.buttons=new Map([...buttons].map(button=>[button.dataset.mode,button]));this.keyboardPads=new Map([['z','step'],['x','shoot'],['c','touch'],['v','stop'],[' ','stop']]);this.heldPadKeys=new Set();
    this.installButtons(buttons);
    this.installKeyboardPads();
    this.installCanvas();
    this.installSettings();
  }

  activateAudio(){this.soundManager?.wake().catch(error=>console.warn('Audio initialization failed',error));}
  unknownInput(event){const key=event.key?.toLowerCase();if(['shift','control','alt','meta'].includes(key)||(this.renderer.mobile&&this.keyboardPads.has(key)))return;this.soundManager?.play('unknown');}

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
    this.activateAudio();
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
    this.canvas.addEventListener('click',event=>{this.activateAudio();if(this.game.phase!=='game'){event.preventDefault();this.input.push({type:'start'});}});
    this.canvas.addEventListener('pointerdown',event=>this.pointerDown(event));
    this.canvas.addEventListener('pointermove',event=>this.pointerMove(event));
    this.canvas.addEventListener('pointerup',event=>this.pointerEnd(event));
    this.canvas.addEventListener('pointercancel',event=>this.pointerEnd(event));
  }

  installSettings(){
    if(!this.settings)return;
    this.settings.querySelector('[data-reaction-lag]')?.addEventListener('click',()=>this.setReactionLag(!this.reactionTicks));
    this.settings.querySelector('[data-debug-mode]')?.addEventListener('click',()=>{this.game.setDebugFlyover(!this.game.debugFlyover);this.syncSettings();});
    this.settings.addEventListener('click',event=>{if(event.target===this.settings)this.closeSettings();});this.syncSettings();
  }

  syncSettings(){
    if(!this.settings)return;
    this.settings.querySelector('[data-reaction-lag]')?.setAttribute('aria-pressed',String(Boolean(this.reactionTicks)));
    this.settings.querySelector('[data-debug-mode]')?.setAttribute('aria-pressed',String(this.game.debugFlyover));
  }

  openSettings(preserveGesture=false){
    if(!this.settings||this.settingsOpen)return;
    if(!preserveGesture)this.clearHudGesture();this.settingsOpen=true;this.pausedBeforeSettings=this.game.paused;this.stop();if(!this.pausedBeforeSettings)this.input.push({type:'pause'});this.settings.hidden=false;this.syncSettings();
  }

  closeSettings(){
    if(!this.settingsOpen)return;
    this.settingsOpen=false;this.settings.hidden=true;if(!this.pausedBeforeSettings)this.input.push({type:'pause'});
  }

  point(event){return this.renderer.clientToCanvas(event.clientX,event.clientY);}

  startHudPress(event,point){
    if(!this.hudGesture)this.hudGesture={pointers:new Map(),multi:false,long:false,timer:0};
    const gesture=this.hudGesture;gesture.pointers.set(event.pointerId,{point,time:performance.now()});
    if(gesture.pointers.size>1){gesture.multi=true;this.openSettings(true);}
    clearTimeout(gesture.timer);
    gesture.timer=setTimeout(()=>{
      if(this.hudGesture!==gesture||this.game.phase!=='game')return;
      gesture.long=true;
      if(gesture.multi){this.game.setDebugFlyover(!this.game.debugFlyover);this.syncSettings();}
      else this.openSettings(true);
    },HUD_LONG_PRESS_MS);
  }

  clearHudGesture(){if(!this.hudGesture)return;clearTimeout(this.hudGesture.timer);this.hudGesture=null;}

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

  setReactionLag(enabled){this.reactionTicks=enabled?REACTION_TICKS:0;this.syncSettings();return Boolean(this.reactionTicks);}

  stop(){this.lastRunClientPoint=null;this.input.push({type:'stop'});}

  direction(point,anchor) {
    const dx=point[0]-anchor[0],dy=point[1]-anchor[1];
    return coneDirection(dx,dy);
  }

  pointerDown(event) {
    this.activateAudio();
    if(this.game.phase!=='game') { event.preventDefault();this.input.push({type:'start'});return; }
    if(!this.renderer.mobile){this.soundManager?.play('unknown');return;}
    event.preventDefault();
    this.canvas.setPointerCapture(event.pointerId);
    if(this.settingsOpen)return;
    const point=this.point(event);
    const hudTarget=this.renderer.mobileHudTarget(...point);
    if(hudTarget?.type==='inventory'){this.input.push({type:'drop',slot:hudTarget.slot});return;}
    if(hudTarget?.type==='score'){this.startHudPress(event,point);return;}
    const mode=this.mode??this.keyboardMode();
    if(mode) { this.input.push({type:mode,direction:this.direction(point,this.followAnchor())}); return; }
    const velocity=this.motionVelocity(),running=velocity.some(Boolean);
    if(!running)this.lastRunClientPoint=null;
    const anchor=this.followAnchor(),direction=this.direction(point,anchor);
    const playerAnchor=this.playerAnchor(),stopDirection=this.direction(point,playerAnchor);
    const sameDirection=stopDirection[0]*velocity[0]+stopDirection[1]*velocity[1]>0;
    const opposite=oppositeDirection(stopDirection,velocity);
    const sameStart=running&&this.lastRunClientPoint&&Math.hypot(event.clientX-this.lastRunClientPoint[0],event.clientY-this.lastRunClientPoint[1])<=SAME_TAP_RADIUS_CSS;
    const stop=sameDirection||opposite||sameStart;
    this.lastDecision={point:[...point],anchor:[...anchor],direction:[...direction],stopDirection:[...stopDirection],velocity:[...velocity],sameDirection,sameStart:Boolean(sameStart),opposite,stop};
    if(stop){this.press=null;this.stop();}
    else{this.press={id:event.pointerId,direction,anchor};this.lastRunClientPoint=[event.clientX,event.clientY];this.input.push({type:'run',direction,replace:true});}
  }

  pointerMove(event) {
    const hudPointer=this.hudGesture?.pointers.get(event.pointerId);
    if(hudPointer){event.preventDefault();const point=this.point(event),target=this.renderer.mobileHudTarget(...point);if(target?.type!=='score'||Math.hypot(point[0]-hudPointer.point[0],point[1]-hudPointer.point[1])>HUD_LONG_PRESS_TOLERANCE)this.clearHudGesture();return;}
    if(!this.press||this.press.id!==event.pointerId)return;
    event.preventDefault();
    const point=this.point(event),anchor=this.followAnchor(),direction=this.direction(point,anchor);
    if(direction[0]===this.press.direction[0]&&direction[1]===this.press.direction[1])return;
    this.press.direction=direction;
    if(!this.lastRunClientPoint)this.lastRunClientPoint=[event.clientX,event.clientY];
    this.input.push(direction[0]||direction[1]?{type:'run',direction,replace:true}:{type:'stop'});
  }

  pointerEnd(event) {
    if(this.hudGesture?.pointers.has(event.pointerId)){
      event.preventDefault();const gesture=this.hudGesture;gesture.pointers.delete(event.pointerId);
      if(!gesture.pointers.size){if(!gesture.long&&!gesture.multi)this.input.push({type:'pause'});this.clearHudGesture();}
      return;
    }
    if(!this.press||this.press.id!==event.pointerId)return;
    event.preventDefault();
    this.press=null;
  }
}

