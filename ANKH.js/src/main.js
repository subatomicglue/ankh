import { AssetStore } from './bitmap.js';
import { InputBuffer, KeyboardControls } from './input.js';
import { CanvasRenderer } from './renderer.js';
import { Game } from './game.js';
import { MobileControls } from './mobile-controls.js';

const canvas=document.querySelector('#game'),status=document.querySelector('#status');
const input=new InputBuffer(),keyboardControls=new KeyboardControls(window,input),assets=new AssetStore('.');
status.textContent='Loading the MetaReal World…';
const spriteNames=['badguy','benign','bonus','box','bullet','chalice','crown','flit','horzdoor','illumina','item','key','lock','lockface','locklisten','musicbox','nobitmap','other','suck','tele','touchhorz','touchvert','trap','treasure','vertdoor'];
await assets.load(spriteNames);
const game=new Game(input,assets),renderer=new CanvasRenderer(canvas,'.');await renderer.load(game);status.textContent='';
const actionButtons=document.querySelectorAll('.mobile-actions button'),padKeys={step:'Z',shoot:'X',touch:'C',stop:'V / SPACE'};actionButtons.forEach(button=>{renderer.hudText.label(button,button.textContent.trim());const tip=document.createElement('span');tip.className='key-tip';tip.textContent=padKeys[button.dataset.mode];tip.setAttribute('aria-hidden','true');button.append(tip);});
const mobileControls=new MobileControls(canvas,actionButtons,input,game,renderer,document.querySelector('.mobile-settings'),keyboardControls);
let previous=performance.now(),accumulator=0;const STEP=1000/60;
function frame(now){accumulator+=Math.min(100,now-previous);previous=now;while(accumulator>=STEP){game.update();accumulator-=STEP;}renderer.render(game);requestAnimationFrame(frame);}
requestAnimationFrame(frame);
window.ANKH={game,renderer,assets,keyboardControls,mobileControls};
