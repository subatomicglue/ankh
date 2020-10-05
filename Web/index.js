import { sub, vec, floor, div } from "./math.js";
import { Map } from "./map.js";
import { Sprite } from "./sprite.js";
import { game_data } from "./game_data.js";

//console.log( game_data );

function collideAgainstMap( actor ) {
  // if (!actors[0].collide( actor.x + actor.dx * 1/fps, actor.y + actor.dy * 1/fps )) {
  //   actor.x += actor.dx * 1/fps; // move by dx every second
  //   actor.y += actor.dy * 1/fps; // move by dy every second
  // }

  if (!map.collideBox( actor.x + actor.bbox.x + actor.dx * 1/fps,
                            actor.y + actor.bbox.y + actor.dy * 1/fps,
                            actor.bbox.w,
                            actor.bbox.h )) {
    actor.x += actor.dx * 1/fps; // move by dx every second
    actor.y += actor.dy * 1/fps; // move by dy every second
  }
}

// clear the canvas using the given canvas ctx
function clear( ctx ) {
  ctx.fillStyle = "#000000";                    // use black
  ctx.fillRect(0,0,canvas.width,canvas.height); // clear the canvas
}

////////////////////////////////////////////////////////////////////////////////
// game data
////////////////////////////////////////////////////////////////////////////////

let fps = 30;  // frame rate  (film is 24hz, TV at 60hz)
let actors = [];
let map;

////////////////////////////////////////////////////////////////////////////////
// INIT
////////////////////////////////////////////////////////////////////////////////

async function init() {
  let mapdata = await (await fetch("map.json")).json();
  map = new Map( mapdata.spritemap, mapdata.tilesx, mapdata.tilesy, mapdata.mapx, mapdata.tiles, mapdata.not_collidable );
  await map.img_done;

  // map always first
  actors.push( map );

  for (let actor_def of game_data.actor_layout) {
    let sprite = game_data.sprites[actor_def.sprite];
    function parseNumber( n, m ) { return typeof(n)==="number" ? (n-1)*m : parseInt(n) ? (parseInt(n)-1)*m : parseInt( n.match(/([0-9]+)px/)[1] ) }
    actor_def.x = parseNumber( actor_def.x, map.width );
    actor_def.y = parseNumber( actor_def.y, map.height );
    if (actor_def.room) {
      let ax = actor_def.x + actor_def.room[0] * mapdata.roomx*map.width;
      let ay = actor_def.y + actor_def.room[1] * mapdata.roomy*map.height;
      console.log( "room translating", actor_def.x, actor_def.y, "to", ax, ay )
      actor_def.x = ax;
      actor_def.y = ay;
    }
    console.log( `Actor: type:${actor_def.type} sprite:${actor_def.sprite} img:${sprite.src} ${actor_def.x},${actor_def.y}` );
    let actor = new Sprite( sprite.src, actor_def.x, actor_def.y, sprite.tilesx ? sprite.tilesx : 1, sprite.tilesy ? sprite.tilesy : 1, {
        default: {interval: 0.0, frames: [[0,0], ] },
      },
      { x: sprite.bbox.x, y: sprite.bbox.y, w: sprite.bbox.w, h: sprite.bbox.h }, // bounding box
      collideAgainstMap
    );
    actors.push( actor );
  }
}

////////////////////////////////////////////////////////////////////////////////
// UPDATE
////////////////////////////////////////////////////////////////////////////////
function update( t ) {
  for (let x=0; x < actors.length; ++x) {
    actors[x].update( actors[x], t );
  }
}

////////////////////////////////////////////////////////////////////////////////
// DRAW
////////////////////////////////////////////////////////////////////////////////

function draw() {
  let ctx = canvas.getContext("2d");
  clear( ctx );
  for (let x=0; x < actors.length; ++x) {
    actors[x].draw( ctx );
  }
}

////////////////////////////////////////////////////////////////////////////////
// RESIZE
////////////////////////////////////////////////////////////////////////////////

function resize() {
  // Check if the canvas's image buffer is not the same size as the browser's area.
  if (canvas.width  != canvas.clientWidth ||
      canvas.height != canvas.clientHeight) {
    // Make the canvas the same size
    canvas.width  = canvas.clientWidth;
    canvas.height = canvas.clientHeight;
    console.log( `resizing canvas to: ${canvas.width} x ${canvas.height}` )
  }
}

////////////////////////////////////////////////////////////////////////////////
// MAIN LOOP
////////////////////////////////////////////////////////////////////////////////

let frameid = 0;
let running = false;
// t is a DOMHighResTimeStamp provided by requestAnimationFrame.
function start() {
  running = true;
  console.log( "starting main loop" );
  function _start( t ) {
    //let now = window.performance.now();
    frameid = window.requestAnimationFrame( _start );
    resize();
    update( t );
    draw();
  }
  _start( 0.0 );
}
function stop() {
  running = false;
  console.log( "pausing main loop" );
  window.cancelAnimationFrame( frameid );
}

////////////////////////////////////////////////////////////////////////////////
// start the game loop
(async ()=>{
  await init();
  start();
})();

////////////////////////////////////////////////////////////////////////////////
// respond to Input
document.onkeydown = (event) => {
  if (event.repeat) return;
  let speed = 3;
  switch (event.key) {
    case "ArrowLeft":
      actors[1].changeSequence("left");
      actors[1].dx = -32*speed;
      actors[1].dy = 0;
      break;
    case "ArrowRight":
      actors[1].changeSequence("right");
      actors[1].dx = 32*speed;
      actors[1].dy = 0;
      break;
    case "ArrowUp":
      actors[1].changeSequence("up");
      actors[1].dx = 0;
      actors[1].dy = -32*speed;
      break;
    case "ArrowDown":
      actors[1].changeSequence("down");
      actors[1].dx = 0;
      actors[1].dy = 32*speed;
      break;
    case " ":
      actors[1].dx = 0;
      actors[1].dy = 0;
      break;
  }
  console.log('keydown event\n\n' + 'key: ' + event.key, " repeat: ", event.repeat);
};

document.onkeyup = (event) => {
  if (event.repeat) return;
  //console.log( event.key )
  // cancel velocity
  switch (event.key) {
    case "ArrowLeft":
      //actors[1].dx = 0;
      actors[1].changeSequence("left_idle");
      break;
    case "ArrowRight":
      //actors[1].dx = 0;
      actors[1].changeSequence("right_idle");
      break;
    case "ArrowUp":
      //actors[1].dy = 0;
      actors[1].changeSequence("up_idle");
      break;
    case "ArrowDown":
      //actors[1].dy = 0;
      actors[1].changeSequence("down_idle");
      break;
  }
  console.log('keyup event\n\n' + 'key: ' + event.key, " repeat: ", event.repeat);
};

function onvischange(e) {
  if (running)  if (e.type == "blur"  || (e.type == "visibilitychange" && document.visibilityState === 'hidden')) { stop(); }
  if (!running) if (e.type == "focus" || (e.type == "visibilitychange" && document.visibilityState === 'visible')) { start(); }
}
document.body.onblur=onvischange; 
document.body.onfocus=onvischange; 
document.addEventListener("visibilitychange", onvischange);
