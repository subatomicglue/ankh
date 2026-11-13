const debugPort=process.env.ANKH_DEBUG_PORT??'9222',requestedUrl=process.env.ANKH_TEST_URL;
const pages=await (await fetch(`http://127.0.0.1:${debugPort}/json/list`)).json();
const page=pages.find(page=>page.type==='page'&&requestedUrl&&page.url===requestedUrl)??pages.find(page=>page.type==='page'&&page.title==='ANKH — 64 Rooms');
if(!page)throw new Error('live ANKH tab not found');
const socket=new WebSocket(page.webSocketDebuggerUrl);
await new Promise((resolve,reject)=>{socket.onopen=resolve;socket.onerror=reject;});
let id=0;const pending=new Map();
socket.onmessage=event=>{const message=JSON.parse(event.data);if(!message.id)return;const request=pending.get(message.id);pending.delete(message.id);message.error?request.reject(message.error):request.resolve(message.result);};
const send=(method,params={})=>new Promise((resolve,reject)=>{const requestId=++id;pending.set(requestId,{resolve,reject});socket.send(JSON.stringify({id:requestId,method,params}));});
const result=await send('Runtime.evaluate',{returnByValue:true,expression:`(()=>{
  const g=ANKH.game,p=g.player,w=g.world,a=g.assets;
  const inspect=(dx,dy)=>{
    const left=Math.floor(p.x+dx),top=Math.floor(p.y+dy),contacts=[];
    for(let y=0;y<p.height;y++)for(let x=0;x<p.width;x++){
      const actor=a.actorPixel(p,x,y),wx=left+x,wy=top+y,tile=w.tileAt(wx,wy),wall=w.pixelAt(wx,wy);
      if(actor&&w.isSolidPixel(wx,wy))contacts.push({x,y,wx,wy,actor,wall,tile});
    }
    return {blocked:contacts.length>0,contacts:contacts.slice(0,20)};
  };
  const [cx]=p.center,[,edgeY]=p.touchOrigin([0,1]);let downBeam=null;
  for(let animation=1;animation<=40&&!downBeam;animation++){
    const probe={x:cx-1,y:edgeY,width:2,height:animation};
    const contact=w.firstCollision(probe);
    if(contact)downBeam={animation,probe,contact,wall:w.pixelAt(contact.x,contact.y),tile:w.tileAt(contact.x,contact.y)};
  }
  if(downBeam){downBeam.neighborhood=[];for(let y=downBeam.contact.y-2;y<=downBeam.contact.y+3;y++){let row='';for(let x=downBeam.contact.x-3;x<=downBeam.contact.x+5;x++)row+=w.isSolidPixel(x,y)?'#':'.';downBeam.neighborhood.push({y,row});}}
  const old=[p.x,p.y];let downVoidExample=null;
  for(let y=768;y<960-p.height&&!downVoidExample;y++)for(let x=0;x<192-p.width&&!downVoidExample;x++){
    p.x=x;p.y=y;if(g.collision.actorWorld(p))continue;
    const [testX]=p.center,[,testY]=p.touchOrigin([0,1]);
    for(let animation=1;animation<=24&&!downVoidExample;animation++){
      const probe={x:testX-1,y:testY,width:2,height:animation};if(w.collidesBox(probe))break;
      let voidPixels=0;for(let wy=probe.y;wy<=probe.y+probe.height;wy++)for(let wx=probe.x;wx<=probe.x+probe.width;wx++){const tile=w.tileAt(wx,wy);if(![346,347,348,349,378,379,380,381].includes(tile)&&!w.isSolidPixel(wx,wy))voidPixels++;}
      if(voidPixels)downVoidExample={player:{x,y},animation,probe,voidPixels};
    }
  }
  [p.x,p.y]=old;
  return {phase:g.phase,player:{x:p.x,y:p.y,width:p.width,height:p.height,sprite:p.spriteFile},camera:{x:g.camera.x,y:g.camera.y},left:inspect(-1,0),down:[1,2,3].map(step=>({step,...inspect(0,step)})),here:inspect(0,0),downBeam,downVoidExample};
})()`});
console.log(JSON.stringify(result.result.value,null,2));
socket.close();
