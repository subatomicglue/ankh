import { sub, vec, floor, div } from "./math.js";
import { Map } from "./map.js";
import { Sprite, Actor } from "./sprite.js";
import { game_data } from "./game_data.js";
import { createBehavior, OscMethod } from "./behaviors.js";

//console.log( game_data );

function simVelocity( sprite, t ) {
  // if (!actors[0].collide( sprite.x + sprite.dx * 1/fps, sprite.y + sprite.dy * 1/fps )) {
  //   sprite.x += sprite.dx * 1/fps; // move by dx every second
  //   sprite.y += sprite.dy * 1/fps; // move by dy every second
  // }

  if (!map.collideBox( sprite.x + sprite.bbox.x + sprite.dx * t,
                            sprite.y + sprite.bbox.y + sprite.dy * t,
                            sprite.bbox.w,
                            sprite.bbox.h )) {
    sprite.x += sprite.dx * t; // move by dx every second
    sprite.y += sprite.dy * t; // move by dy every second
    sprite.collided = false;
  } else {
    sprite.collided = true;
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

export let actors = [];
export let map;
export let player;

////////////////////////////////////////////////////////////////////////////////
// INIT
////////////////////////////////////////////////////////////////////////////////

async function init() {
  let mapdata = await (await fetch("map.json")).json();
  map = new Map( mapdata.spritemap, mapdata.tilesx, mapdata.tilesy, mapdata.roomx, mapdata.roomy, mapdata.mapx, mapdata.tiles, mapdata.not_collidable );
  await map.img_done;

  // map always first
  actors.push( map );

  for (let actor_def of game_data.actor_layout) {
    let sprite_def = game_data.sprites[actor_def.sprite];
    function parseNumber( n, m ) { return typeof(n)==="number" ? (n-1)*m : parseInt(n) ? (parseInt(n)-1)*m : parseInt( n.match(/([0-9]+)px/)[1] ) }
    actor_def.x = parseNumber( actor_def.x, map.tile_width );
    actor_def.y = parseNumber( actor_def.y, map.tile_height );
    if (actor_def.room) {
      let ax = actor_def.x + actor_def.room[0] * map.room_tilesx * map.tile_width;
      let ay = actor_def.y + actor_def.room[1] * map.room_tilesy * map.tile_height;
      //console.log( "room translating", actor_def.x, actor_def.y, "to", ax, ay )
      actor_def.x = ax;
      actor_def.y = ay;
    }
    console.log( `Actor: '${actor_def.name}' beh:${actor_def.behavior} spr:${actor_def.sprite} img:${sprite_def.src} [${actor_def.x},${actor_def.y}]` );
    let sprite = new Sprite( sprite_def.src, actor_def.x, actor_def.y, sprite_def.tilesx ? sprite_def.tilesx : 1, sprite_def.tilesy ? sprite_def.tilesy : 1, 
      sprite_def.anim_seq ? sprite_def.anim_seq : {default: {fps: 0, frames: [[0,0]] }},
      sprite_def.bbox,
      simVelocity
    );
    let beh = createBehavior( actor_def.behavior, actor_def.behavior_params );
    let actor = new Actor( actor_def.name, sprite, beh );
    if (actor_def.name == "player") player = actor; // set a convenient variable to the player
    actors.push( actor );
  }
}

////////////////////////////////////////////////////////////////////////////////
// UPDATE
////////////////////////////////////////////////////////////////////////////////
function update( t ) {
  map.updateMapRoom( player.getPosition() );
  for (let x=0; x < actors.length; ++x) {
    actors[x].update( t );
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
let last_time = 0;
// t is a DOMHighResTimeStamp provided by requestAnimationFrame.
function start() {
  const div_by_1000 = 1/1000;
  running = true;
  console.log( "starting main loop" );
  function _start( t ) {
    let td = last_time == 0 ? 1/60 : (t-last_time) * div_by_1000;
    frameid = window.requestAnimationFrame( _start );
    resize();
    update( td );
    draw();
    last_time = t;
  }
  _start( 0.0 );
}
function stop() {
  running = false;
  last_time = 0;
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
    case 'a':
    case "ArrowLeft":
      player.changeSequence("left");
      player.setVelocity( -32*speed, 0 );
      break;
    case 'd':
    case "ArrowRight":
      player.changeSequence("right");
      player.setVelocity( 32*speed, 0 );
      break;
    case 'w':
    case "ArrowUp":
      player.changeSequence("up");
      player.setVelocity( 0, -32*speed );
      break;
    case 's':
    case "ArrowDown":
      player.changeSequence("down");
      player.setVelocity( 0, 32*speed );
      break;
    case " ":
      player.setVelocity( 0, 0 );
      break;
  }
  console.log('keydown event\n\n' + 'key: ' + event.key, " repeat: ", event.repeat);
};

document.onkeyup = (event) => {
  if (event.repeat) return;
  //console.log( event.key )
  // cancel velocity
  switch (event.key) {
    case 'a':
    case "ArrowLeft":
      //player.setVelocityX( 0 );
      player.changeSequence("left_idle");
      break;
    case 'd':
    case "ArrowRight":
      //player.setVelocityX( 0 );
      player.changeSequence("right_idle");
      break;
    case 'w':
    case "ArrowUp":
      //player.setVelocityY( 0 );
      player.changeSequence("up_idle");
      break;
    case 's':
    case "ArrowDown":
      //player.setVelocityY( 0 );
      player.changeSequence("down_idle");
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
