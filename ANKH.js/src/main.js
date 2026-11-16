import { AssetStore } from './bitmap.js';
import { InputBuffer } from './input.js';
import { CanvasRenderer } from './renderer.js';
import { Game } from './game.js';
import { BrowserControls } from './browser-controls.js';
import { DebugInspector } from './debug-inspector.js';
import { SoundManager } from './sound-manager.js';

const canvas=document.querySelector('#game'),status=document.querySelector('#status');
const input=new InputBuffer(),assets=new AssetStore('.');
status.textContent='Loading the MetaReal World…';
const soundDefinitions=await fetch('./data/sounds.json').then(response=>{if(!response.ok)throw new Error(`Unable to load sounds.json: ${response.status}`);return response.json();});
const spriteNames=['badguy','benign','bonus','box','bullet','chalice','crown','flit','horzdoor','illumina','item','key','lock','lockface','locklisten','musicbox','nobitmap','other','suck','tele','touchhorz','touchvert','trap','treasure','vertdoor'];
await assets.load(spriteNames);
let game;const soundManager=new SoundManager({onBackground:()=>{if(game?.phase==='game'&&!game.paused)game.applyCommand({type:'pause'});}}).configure(soundDefinitions);game=new Game(input,assets,soundManager);soundManager.setListener(()=>game.player.center);
soundManager.bindUnlock(document.querySelector('#audio-unlock'));
const renderer=new CanvasRenderer(canvas,'.');await renderer.load(game);status.textContent='';
const actionButtons=document.querySelectorAll('.mobile-actions button'),padKeys={step:'Z',shoot:'X',touch:'C',stop:'V / SPACE'};actionButtons.forEach(button=>{renderer.hudText.label(button,button.textContent.trim());const tip=document.createElement('span');tip.className='key-tip';tip.textContent=padKeys[button.dataset.mode];tip.setAttribute('aria-hidden','true');button.append(tip);});
const browserControls=new BrowserControls(canvas,actionButtons,input,game,renderer,document.querySelector('.mobile-settings'),soundManager);
const debugInspector=new DebugInspector(canvas,game,renderer);
let previous=performance.now(),accumulator=0;const STEP=1000/60;
function frame(now){accumulator+=Math.min(100,now-previous);previous=now;while(accumulator>=STEP){game.update();accumulator-=STEP;}soundManager.update();debugInspector.update();renderer.render(game);requestAnimationFrame(frame);}
requestAnimationFrame(frame);
window.ANKH={game,renderer,assets,browserControls,soundManager,debugInspector};
