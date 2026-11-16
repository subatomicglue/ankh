import { WORLD } from './world.js';
import { HudTileText } from './hud-text.js';

export class Renderer {
  async load() {}
  resize() {}
  render() { throw new Error('Renderer.render must be implemented'); }
  destroy() {}
}

const loadImage=src=>new Promise((resolve,reject)=>{const image=new Image();image.onload=()=>resolve(image);image.onerror=reject;image.src=src;});
const frameHeight=(actor,image)=>image.height/actor.frameCount;
export const karmaMeter=karma=>({fill:(Math.floor((karma%100)/5)+1)/20,levels:Math.min(20,Math.floor(karma/100))});
export const mobileKarmaInset=(karma,portrait)=>portrait?(karmaMeter(karma).levels?4:2):0;

export class CanvasRenderer extends Renderer {
  constructor(canvas,base='.') {
    super();this.canvas=canvas;this.ctx=canvas.getContext('2d');this.base=base;this.images=new Map();
    this.mobileQuery=matchMedia('(max-width: 700px), (pointer: coarse)');
    this.onLayoutChange=()=>requestAnimationFrame(()=>this.resize());this.mobileQuery.addEventListener('change',this.onLayoutChange);window.addEventListener('resize',this.onLayoutChange);window.visualViewport?.addEventListener('resize',this.onLayoutChange);
    this.resizeObserver=new ResizeObserver(this.onLayoutChange);this.resizeObserver.observe(canvas.parentElement);
  }
  async load(game) {
    this.game=game;[this.walls,this.hud]=await Promise.all([loadImage(`${this.base}/assets/walls.bmp`),loadImage(`${this.base}/assets/hud.png`)]);
    this.hudText=new HudTileText(this.ctx,this.hud);
    this.titles=await Promise.all(Array.from({length:9},(_,i)=>loadImage(`${this.base}/assets/titles/title${i+1}.bmp`)));
    const names=new Set(game.actors.map(actor=>actor.spriteFile));names.add('bullet');
    await Promise.all([...names].map(name=>this.loadSprite(name)));
    this.worldCanvas=document.createElement('canvas');this.worldCanvas.width=WORLD.width;this.worldCanvas.height=WORLD.height;
    const context=this.worldCanvas.getContext('2d');context.imageSmoothingEnabled=false;
    const {tileSize,mapWidth,atlasColumns}=game.world;
    game.world.map.forEach((tile,index)=>context.drawImage(this.walls,(tile%atlasColumns)*tileSize,Math.floor(tile/atlasColumns)*tileSize,tileSize,tileSize,(index%mapWidth)*tileSize,Math.floor(index/mapWidth)*tileSize,tileSize,tileSize));
    this.resize();
  }
  async loadSprite(name) {
    const image=await loadImage(`${this.base}/assets/sprites/${name}.bmp`),indexed=this.game.assets.bitmaps.get(name);
    const surface=document.createElement('canvas');surface.width=image.width;surface.height=image.height;
    const context=surface.getContext('2d');context.drawImage(image,0,0);const rgba=context.getImageData(0,0,surface.width,surface.height);
    for(let pixel=0;pixel<indexed.pixels.length;pixel++)if(indexed.pixels[pixel]===0)rgba.data[pixel*4+3]=0;
    context.putImageData(rgba,0,0);this.images.set(name,surface);
  }
  layoutMobileViewport(karma=0) {
    this.outputWidth=this.logicalWidth;this.portrait=this.logicalHeight-40>=this.logicalWidth;this.karmaInset=mobileKarmaInset(karma,this.portrait);
    this.viewportY=40+this.karmaInset;this.outputHeight=this.logicalHeight-this.viewportY;
    const aspect=this.outputWidth/this.outputHeight;
    if(aspect<1){this.viewportWidth=192*aspect;this.viewportHeight=192;}else{this.viewportWidth=192;this.viewportHeight=192/aspect;}
  }
  syncMobileKarma(karma) {
    if(!this.mobile||mobileKarmaInset(karma,this.portrait)===this.karmaInset)return;
    this.layoutMobileViewport(karma);this.worldScale=this.outputWidth/this.viewportWidth;
    if(this.game){this.game.camera.useGbaMode(this.viewportWidth,this.viewportHeight);this.game.camera.follow(this.game.player,this.game.cameraOffset);}
  }
  resize() {
    this.mobile=this.mobileQuery.matches;
    if(this.mobile){
      const rect=this.canvas.parentElement.getBoundingClientRect(),ratio=rect.width&&rect.height?rect.width/rect.height:5/6;
      if(ratio<=1){this.logicalWidth=160;this.logicalHeight=Math.round(160/ratio);}else{this.logicalHeight=120;this.logicalWidth=Math.round(120*ratio);}
      this.layoutMobileViewport(this.game?.karma??0);
    }else{this.logicalWidth=272;this.logicalHeight=192;this.viewportY=0;this.outputWidth=192;this.outputHeight=192;this.viewportWidth=192;this.viewportHeight=192;}
    const display=this.canvas.getBoundingClientRect(),pixelRatio=window.devicePixelRatio||1;
    this.canvas.width=Math.max(1,Math.round((display.width||this.logicalWidth)*pixelRatio));
    this.canvas.height=Math.max(1,Math.round((display.height||this.logicalHeight)*pixelRatio));
    this.ctx.setTransform(this.canvas.width/this.logicalWidth,0,0,this.canvas.height/this.logicalHeight,0,0);
    this.worldScale=this.outputWidth/this.viewportWidth;this.ctx.imageSmoothingEnabled=true;this.ctx.imageSmoothingQuality='high';
    if(this.game){this.mobile?this.game.camera.useGbaMode(this.viewportWidth,this.viewportHeight):this.game.camera.useRoomMode();this.game.camera.follow(this.game.player,this.game.cameraOffset);}
  }
  clientToCanvas(clientX,clientY) {const rect=this.canvas.getBoundingClientRect();return [(clientX-rect.left)*this.logicalWidth/rect.width,(clientY-rect.top)*this.logicalHeight/rect.height];}
  mobileHudTarget(x,y){if(y>=0&&y<29)return {type:'inventory',slot:Math.max(0,Math.min(3,Math.floor(x/(this.logicalWidth/4))))};if(y>=29&&y<40)return {type:'score'};return null;}
  drawSprite(actor,x,y) {
    const image=this.images.get(actor.spriteFile);if(!image)return;
    const height=frameHeight(actor,image);this.ctx.drawImage(image,0,actor.frame*height,image.width,height,Math.round(x),Math.round(y),image.width,height);
  }
  worldToCanvas(x,y,camera=this.game.camera){return [(x-camera.x)*this.worldScale,(y-camera.y)*this.worldScale+this.viewportY];}
  drawActor(actor,camera) {const image=this.images.get(actor.spriteFile);if(!image)return;const height=frameHeight(actor,image),[x,y]=this.worldToCanvas(actor.x,actor.y,camera),scale=this.worldScale;this.ctx.drawImage(image,0,actor.frame*height,image.width,height,Math.round(x),Math.round(y),image.width*scale,height*scale);}
  drawTouchShape(shape,camera,player) {
    const image=this.images.get(shape.kind);if(!image)return;
    let [x,y]=this.worldToCanvas(shape.x,shape.y,camera);const [playerX,playerY]=this.worldToCanvas(player.x,player.y,camera),scale=this.worldScale;
    if(shape.kind==='touchvert')x=Math.round(playerX)+(shape.x-player.x)*scale;
    else y=Math.round(playerY)+(shape.y-player.y)*scale;
    this.ctx.drawImage(image,shape.kind==='touchvert'?x:Math.round(x),shape.kind==='touchhorz'?y:Math.round(y),image.width*scale,image.height*scale);
  }
  drawWorld(game) {
    const camera=game.camera,context=this.ctx;
    context.save();context.beginPath();context.rect(0,this.viewportY,this.outputWidth,this.outputHeight);context.clip();
    context.drawImage(this.worldCanvas,camera.x,camera.y,this.viewportWidth,this.viewportHeight,0,this.viewportY,this.outputWidth,this.outputHeight);
    for(const entry of game.world.visitedRoomTiles(game.rooms)){
      const size=game.world.tileSize,[x,y]=this.worldToCanvas(entry.x,entry.y,camera),scale=size*this.worldScale,columns=game.world.atlasColumns;
      context.drawImage(this.walls,(entry.tile%columns)*size,Math.floor(entry.tile/columns)*size,size,size,Math.round(x),Math.round(y),scale,scale);
    }
    for(const shape of game.touches.shapes)this.drawTouchShape(shape,camera,game.player);
    const bullet=this.images.get('bullet');game.shots.shots.forEach(shot=>{const [x,y]=this.worldToCanvas(shot.x,shot.y,camera);context.drawImage(bullet,Math.round(x),Math.round(y),bullet.width*this.worldScale,bullet.height*this.worldScale);});
    game.actors.filter(actor=>actor.enabled&&actor.visible&&actor.has('ACTOR_VISIBLE')&&actor.worldSpace&&game.onScreen(actor)).sort((a,b)=>b.depth-a.depth).forEach(actor=>this.drawActor(actor,camera));
    context.restore();
  }
  drawInventory(game,positions,boxes) {
    const context=this.ctx;context.strokeStyle='#f0f';
    boxes.forEach(box=>context.strokeRect(...box));
    game.inventory.slots.forEach((item,index)=>{if(item)this.drawSprite(item.actor,...positions[index]);});
  }
  drawDebugLabel(game) {
    if(!game.debugFlyover)return;
    const context=this.ctx,label='DEBUG';context.save();context.font='bold 6px ui-monospace, monospace';context.textBaseline='top';const width=Math.ceil(context.measureText(label).width);context.fillStyle='#050508cc';context.fillRect(2,this.viewportY+3,width+4,9);context.fillStyle='#fe02fe';context.fillText(label,4,this.viewportY+4);context.restore();
  }
  drawMobileHud(game) {
    const context=this.ctx,cell=this.logicalWidth/4,positions=[],boxes=[];context.fillStyle='#050508';context.fillRect(0,0,this.logicalWidth,40);
    for(let index=0;index<4;index++){const left=index*cell;positions.push([left+(cell-32)/2,1]);boxes.push([left+3,1,cell-6,28]);}
    this.drawInventory(game,positions,boxes);
    if(game.paused)this.hudText.printPixels(Math.round((this.logicalWidth-128)/2),31,'UNIVERSE ON HOLD');
    else{this.hudText.printPixels(3,31,'ROOMS');this.hudText.printnumPixels(47,31,String(game.rooms.size).padStart(2,'0'));this.hudText.printPixels(67,31,'OBJECTS');this.hudText.printnumPixels(127,31,String(game.objects).padStart(4,'0'));}
    context.fillStyle='#fe02fe';const meter=karmaMeter(game.karma);
    if(this.portrait){context.fillRect(0,40,this.outputWidth*meter.fill,2);for(let level=0;level<meter.levels;level++)context.fillRect(level*3,42,2,2);}
    else{context.fillRect(this.outputWidth-2,40+this.outputHeight*(1-meter.fill),2,this.outputHeight*meter.fill);for(let level=0;level<meter.levels;level++)context.fillRect(this.outputWidth-4,40+level*3,2,2);}
  }
  drawDesktopHud(game) {
    const context=this.ctx;context.fillStyle='#050508';context.fillRect(192,0,80,192);
    this.drawInventory(game,[[216,8],[245,8],[216,37],[245,37]],[[212,4,25,25],[241,4,25,25],[212,33,25,25],[241,33,25,25]]);
    this.hudText.print(26,9,'ROOMS\n\nOBJECTS');this.hudText.printnum(26,10,`${String(game.rooms.size).padStart(2,'0')}\n\n${String(game.objects).padStart(4,'0')}`);
    const meter=karmaMeter(game.karma);context.fillStyle='#fe02fe';context.fillRect(192,192-192*meter.fill,2,192*meter.fill);for(let level=0;level<meter.levels;level++)context.fillRect(194,level*8,2,6);
    if(game.paused){this.hudText.print(26,22,'UNIVERSE');this.hudText.print(26,23,' ON HOLD');}
  }
  render(game) {
    if(this.mobile!==this.mobileQuery.matches)this.resize();
    this.syncMobileKarma(game.karma);
    const context=this.ctx;context.clearRect(0,0,this.logicalWidth,this.logicalHeight);
    if(game.phase==='intro'){const title=this.titles[game.titleIndex];if(this.mobile){const scale=Math.min(this.logicalWidth/240,this.logicalHeight/160),width=240*scale,height=160*scale;context.drawImage(title,0,0,240,160,(this.logicalWidth-width)/2,(this.logicalHeight-height)/2,width,height);}else context.drawImage(title,0,0,240,160,16,16,240,160);return;}
    this.drawWorld(game);this.mobile?this.drawMobileHud(game):this.drawDesktopHud(game);this.drawDebugLabel(game);
  }
  destroy(){this.resizeObserver.disconnect();this.mobileQuery.removeEventListener('change',this.onLayoutChange);window.removeEventListener('resize',this.onLayoutChange);window.visualViewport?.removeEventListener('resize',this.onLayoutChange);}
}
