const MEDIA_SESSION_ACTIONS=['play','pause','stop','previoustrack','nexttrack','seekto','seekbackward','seekforward'];
const NEXT_MODES=new Set(['fwd','back','rnd']);
const LOOP_MODES=new Set(['once_fwd','once_bck','repeat_fwd','repeat_bck']);
const clamp=(value,min,max)=>Math.max(min,Math.min(max,value));

export class SoundManager {
  constructor({onBackground=()=>{},windowTarget=globalThis,documentTarget=globalThis.document}={}) {
    this.window=windowTarget;this.document=documentTarget;this.onBackground=onBackground;
    this.context=null;this.master=null;this.slots=new Map();this.buffers=new Map();this.loading=new Map();this.playing=new Set();this.pending=[];this.listener=()=>null;this.active=false;this.paused=false;this.destroyed=false;this.unlockElement=null;this.unlockClick=null;
    this.onBlur=()=>this.background();this.onVisibility=()=>{if(this.document?.hidden)this.background();};this.onPageHide=()=>this.background();
    this.window?.addEventListener?.('blur',this.onBlur);this.window?.addEventListener?.('pagehide',this.onPageHide);this.document?.addEventListener?.('visibilitychange',this.onVisibility);
  }

  configure(definitions=[]) {
    this.stopAll();this.slots.clear();
    for(const definition of definitions){
      const sources=Array.isArray(definition.src)?definition.src:[definition.src];
      if(!definition.id||!sources.length||sources.some(source=>typeof source!=='string'||!source))throw new Error('Each sound slot needs an id and one or more src values');
      const next=definition.next_src??'fwd',loop=definition.loop??'once_fwd';
      if(!NEXT_MODES.has(next))throw new Error(`Unknown next_src for ${definition.id}: ${next}`);
      if(!LOOP_MODES.has(loop))throw new Error(`Unknown loop for ${definition.id}: ${loop}`);
      if(this.slots.has(definition.id))throw new Error(`Duplicate sound slot: ${definition.id}`);
      const volume=Number(definition.vol??1);
      this.slots.set(definition.id,{id:definition.id,sources,next,loop,polyphony:Math.max(1,Math.trunc(definition.polyphony??1)),positional:Boolean(definition.positional),volume:Number.isFinite(volume)?Math.max(0,volume):1,cursor:next==='back'?sources.length-1:0,generation:0,voices:[]});
    }
    return this;
  }

  setListener(provider){this.listener=typeof provider==='function'?provider:()=>provider;return this;}
  bindUnlock(element,onUnlock=()=>{}){if(this.unlockElement&&this.unlockClick)this.unlockElement.removeEventListener('click',this.unlockClick);this.unlockElement=element;this.unlockClick=async event=>{event.preventDefault();try{if(await this.wake()){element.hidden=true;onUnlock();}}catch(error){console.warn('Audio initialization failed',error);}};element?.addEventListener?.('click',this.unlockClick);if(element)element.hidden=this.active;return this;}
  setPolyphony(id,count){const slot=this.slots.get(id);if(!slot)return false;slot.polyphony=Math.max(1,Math.trunc(count));while(slot.voices.length>slot.polyphony)this.stopVoice(slot.voices[0]);return true;}

  primeContext(){const source=this.context.createBufferSource();source.buffer=this.context.createBuffer(1,1,this.context.sampleRate);source.connect(this.master);source.start(0);}

  async activate(whilePaused=false) {
    if(this.destroyed)return false;
    const AudioContext=this.window.AudioContext??this.window.webkitAudioContext;if(!AudioContext)return false;
    if(!this.context){this.context=new AudioContext();this.master=this.context.createGain();this.master.connect(this.context.destination);}
    if(this.paused&&!whilePaused)return false;
    this.master.gain.value=this.paused?0:1;
    if(this.active&&this.context.state==='running'){if(!this.paused)this.flushPending();return true;}
    this.primeContext();
    const resumed=this.context.state==='running'?null:this.context.resume();
    if(resumed)await resumed;
    if(this.context.state!=='running'){this.active=false;return false;}
    this.active=true;if(this.unlockElement)this.unlockElement.hidden=true;this.claimMediaSession();if(this.paused){const session=this.window.navigator?.mediaSession;if(session)session.playbackState='paused';}else this.flushPending();this.preload();return true;
  }
  init(){return this.activate(false);}
  wake(){return this.activate(true);}

  pause() {
    if(this.destroyed||this.paused)return false;
    this.paused=true;
    if(this.master)this.master.gain.value=0;
    const session=this.window.navigator?.mediaSession;if(session)session.playbackState='paused';
    this.context?.suspend?.().catch?.(()=>{});return true;
  }

  unpause() {
    if(this.destroyed||!this.paused)return false;
    this.paused=false;
    if(this.master)this.master.gain.value=1;
    const resumed=this.context?.state==='running'?Promise.resolve():this.context?.resume?.();resumed?.then?.(()=>{if(this.active)this.claimMediaSession();this.flushPending();}).catch?.(()=>{});return true;
  }

  preload(){return Promise.allSettled([...this.slots.values()].flatMap(slot=>slot.sources.map(source=>this.loadBuffer(source))));}

  loadBuffer(url) {
    if(this.buffers.has(url))return Promise.resolve(this.buffers.get(url));
    if(this.loading.has(url))return this.loading.get(url);
    if(!this.context)return Promise.reject(new Error('SoundManager.init() must run from a user gesture before loading audio'));
    const promise=fetch(url).then(response=>{if(!response.ok)throw new Error(`Unable to load sound ${url}: ${response.status}`);return response.arrayBuffer();}).then(data=>this.context.decodeAudioData(data)).then(buffer=>{this.buffers.set(url,buffer);this.loading.delete(url);return buffer;},error=>{this.loading.delete(url);throw error;});
    this.loading.set(url,promise);return promise;
  }

  sourceFor(slot) {
    const index=slot.next==='rnd'?Math.floor(Math.random()*slot.sources.length):slot.cursor,source=slot.sources[index];
    if(slot.next==='fwd')slot.cursor=(index+1)%slot.sources.length;else if(slot.next==='back')slot.cursor=(index-1+slot.sources.length)%slot.sources.length;
    return source;
  }

  reversed(buffer) {
    if(this.buffers.has(buffer))return this.buffers.get(buffer);
    const copy=this.context.createBuffer(buffer.numberOfChannels,buffer.length,buffer.sampleRate);
    for(let channel=0;channel<buffer.numberOfChannels;channel++)copy.copyToChannel(Float32Array.from(buffer.getChannelData(channel)).reverse(),channel);
    this.buffers.set(buffer,copy);return copy;
  }

  play(id,{position=null,volume=1,rate=1}={}) {
    const slot=this.slots.get(id);if(!slot)return null;
    let finish;const handle={slot,generation:slot.generation,options:{position,volume,rate},voice:null,queued:false,stopped:false,ready:new Promise(resolve=>finish=resolve),finish,stop:()=>{if(handle.stopped)return;handle.stopped=true;if(handle.queued)this.pending=this.pending.filter(candidate=>candidate!==handle);if(handle.voice){this.stopVoice(handle.voice);handle.voice=null;}handle.finish(null);}};
    if(this.active&&!this.paused&&this.context?.state==='running')this.startHandle(handle);else this.queueHandle(handle);
    return handle;
  }

  queueHandle(handle){if(!handle.queued&&!handle.stopped){handle.queued=true;this.pending.push(handle);}}
  flushPending(){if(!this.active||this.paused||this.context?.state!=='running')return;const pending=this.pending;this.pending=[];for(const handle of pending){handle.queued=false;this.startHandle(handle);}}
  async startHandle(handle){if(handle.stopped||handle.slot.generation!==handle.generation){handle.finish(null);return;}try{const url=this.sourceFor(handle.slot),backward=handle.slot.loop.endsWith('_bck'),buffer=await this.loadBuffer(url);if(handle.stopped||handle.slot.generation!==handle.generation){handle.finish(null);return;}handle.voice=this.startVoice(handle.slot,backward?this.reversed(buffer):buffer,handle.options);handle.finish(handle.voice);}catch(error){console.warn(`Unable to play sound ${handle.slot.id}`,error);handle.finish(null);}}

  startVoice(slot,buffer,{position,volume,rate}) {
    while(slot.voices.length>=slot.polyphony)this.stopVoice(slot.voices[0]);
    const source=this.context.createBufferSource(),gain=this.context.createGain(),requestedVolume=Number(volume),voice={slot,source,gain,position,volume:slot.volume*(Number.isFinite(requestedVolume)?Math.max(0,requestedVolume):1)};
    source.buffer=buffer;source.loop=slot.loop.startsWith('repeat_');source.playbackRate.value=rate;source.connect(gain).connect(this.master);slot.voices.push(voice);this.playing.add(voice);source.addEventListener('ended',()=>this.removeVoice(voice),{once:true});this.updateVoice(voice);source.start();return voice;
  }

  updateVoice(voice) {let attenuation=1;if(voice.slot.positional&&voice.position){const listener=this.listener();if(listener)attenuation=1/(1+Math.hypot(voice.position[0]-listener[0],voice.position[1]-listener[1])/192);}voice.gain.gain.value=voice.volume*attenuation;}
  update(){for(const voice of this.playing)this.updateVoice(voice);}
  removeVoice(voice){this.playing.delete(voice);const index=voice.slot.voices.indexOf(voice);if(index>=0)voice.slot.voices.splice(index,1);}
  stopVoice(voice){try{voice.source.stop();}catch{}this.removeVoice(voice);}
  stop(id){const slot=this.slots.get(id);if(slot){slot.generation++;for(const voice of [...slot.voices])this.stopVoice(voice);for(const handle of this.pending)if(handle.slot===slot){handle.stopped=true;handle.finish(null);}this.pending=this.pending.filter(handle=>handle.slot!==slot);}}
  stopAll(){for(const slot of this.slots.values())this.stop(slot.id);}

  claimMediaSession(){const session=this.window.navigator?.mediaSession;if(!session)return;const actions={play:()=>this.init(),pause:()=>this.background(),stop:()=>this.background()};for(const action of MEDIA_SESSION_ACTIONS){try{session.setActionHandler(action,actions[action]??null);}catch{}}if(this.window.MediaMetadata)session.metadata=new this.window.MediaMetadata({title:'ANKH',artist:'David Van Brink'});session.playbackState='playing';}
  releaseMediaSession(){const session=this.window.navigator?.mediaSession;if(!session)return;for(const action of MEDIA_SESSION_ACTIONS){try{session.setActionHandler(action,null);}catch{}}session.metadata=null;session.playbackState='none';try{session.setPositionState();}catch{}}
  background(){if(this.destroyed)return;this.onBackground();this.pause();}
  destroy(){if(this.destroyed)return;this.destroyed=true;this.stopAll();this.pending=[];this.active=false;this.paused=false;this.releaseMediaSession();this.window?.removeEventListener?.('blur',this.onBlur);this.window?.removeEventListener?.('pagehide',this.onPageHide);this.document?.removeEventListener?.('visibilitychange',this.onVisibility);if(this.unlockElement&&this.unlockClick)this.unlockElement.removeEventListener('click',this.unlockClick);this.unlockElement=null;this.unlockClick=null;this.context?.close?.().catch?.(()=>{});this.context=null;this.master=null;this.slots.clear();this.buffers.clear();this.loading.clear();}
}
