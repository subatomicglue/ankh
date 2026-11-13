import assert from 'node:assert/strict';
import fs from 'node:fs';

const debugPort=process.env.ANKH_DEBUG_PORT??'9222',requestedUrl=process.env.ANKH_TEST_URL;
const pages=await (await fetch(`http://127.0.0.1:${debugPort}/json/list`)).json();
const page=pages.find(page=>page.type==='page'&&requestedUrl&&page.url===requestedUrl)??pages.find(page=>page.type==='page'&&page.title==='ANKH — 64 Rooms');
assert(page,'live ANKH tab not found');
const socket=new WebSocket(page.webSocketDebuggerUrl);
await new Promise((resolve,reject)=>{socket.onopen=resolve;socket.onerror=reject;});
let id=0;const pending=new Map();
socket.onmessage=event=>{const message=JSON.parse(event.data);if(!message.id)return;const request=pending.get(message.id);pending.delete(message.id);message.error?request.reject(message.error):request.resolve(message.result);};
const send=(method,params={})=>new Promise((resolve,reject)=>{const requestId=++id;pending.set(requestId,{resolve,reject});socket.send(JSON.stringify({id:requestId,method,params}));});
const evaluate=async expression=>{const reply=await send('Runtime.evaluate',{expression,returnByValue:true,awaitPromise:true});if(reply.exceptionDetails)throw new Error(reply.exceptionDetails.exception?.description??reply.exceptionDetails.text);return reply.result.value;};
await send('Page.reload',{ignoreCache:true});
for(let tries=0;tries<100&&!await evaluate('Boolean(window.ANKH?.game)');tries++)await new Promise(resolve=>setTimeout(resolve,50));
const result=await evaluate(`(()=>{
  const g=ANKH.game,p=g.player,w=g.world,passable=new Set([346,347,348,349,378,379,380,381]);
  if(g.phase==='intro')g.phase='game';
  const old={x:p.x,y:p.y,enabled:g.actors.map(actor=>actor.enabled)};
  g.actors.forEach(actor=>{if(actor!==p)actor.enabled=false;});
  const detail=(x,y)=>{
    p.x=x;p.y=y;let actorInWallVoid=0,solidWallInActorVoid=0,opaqueContacts=0;
    for(let ay=0;ay<=p.height;ay++)for(let ax=0;ax<=p.width;ax++){
      const actor=g.assets.actorPixel(p,ax,ay),wx=x+ax,wy=y+ay,tile=w.tileAt(wx,wy);
      if(passable.has(tile))continue;
      const wall=w.pixelAt(wx,wy);
      const solid=w.isSolidPixel(wx,wy);
      if(actor&&!solid)actorInWallVoid++;
      if(!actor&&solid)solidWallInActorVoid++;
      if(actor&&solid)opaqueContacts++;
    }
    return {legal:opaqueContacts===0,actorInWallVoid,solidWallInActorVoid};
  };
  let found=null,legalVoidPositions=0,firstVoid=null;
  for(let y=0;y<192-p.height&&!found;y++)for(let x=0;x<192-p.width&&!found;x++)for(const [dx,dy] of [[-1,0],[1,0],[0,-1],[0,1]]){
    const start=detail(x,y),one=detail(x+dx,y+dy),two=detail(x+2*dx,y+2*dy),target=detail(x+3*dx,y+3*dy);
    if(start.legal&&start.actorInWallVoid){legalVoidPositions++;firstVoid??={x,y,detail:start};}
    if(start.legal&&one.legal&&two.legal&&target.legal&&target.actorInWallVoid>0){
      p.x=x;p.y=y;const moved=g.collision.move(p,dx*3,dy*3);
      if(moved&&p.x===x+dx*3&&p.y===y+dy*3){found={start:{x,y},target:{x:p.x,y:p.y},direction:{dx,dy},detail:target};break;}
    }
  }
  if(found){g.camera.useRoomMode();g.camera.follow(p);ANKH.renderer.render(g);}
  else {p.x=old.x;p.y=old.y;g.actors.forEach((actor,index)=>actor.enabled=old.enabled[index]);}
  return {found,legalVoidPositions,firstVoid};
})()`);
assert(result.found,`could not step the player into a transparent wall void; legal void positions: ${result.legalVoidPositions}, first: ${JSON.stringify(result.firstVoid)}`);
console.log(JSON.stringify(result,null,2));
const screenshot=await send('Page.captureScreenshot',{format:'png'});
fs.writeFileSync('/tmp/ankh-wall-nesting.png',Buffer.from(screenshot.data,'base64'));
socket.close();
