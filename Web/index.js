import { sub, vec, floor, div } from "./math.js";
import { Map } from "./map.js";
import { Sprite } from "./sprite.js";

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
let actors;
let map;

////////////////////////////////////////////////////////////////////////////////
// INIT
////////////////////////////////////////////////////////////////////////////////

async function init() {
  let mapdata = await (await fetch("map.json")).json();
  map = new Map( "assets/walls.png", 16, 8, 12*8, mapdata, [2, 7, 12, 17] );

  actors = [
    map,

    // actors[1] is the player character
    new Sprite( "assets/other.png", 360,40, 1,1, {
        default: {interval: 0.0, frames: [[0,0], ] },
      },
      { x: 0, y: 0, w: 22, h: 14 }, // bounding box
      collideAgainstMap
    ),
  ]; // end of actor array...
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
// start the game loop
init();
setInterval( () => {
  resize();
  draw();
}, 1000 / fps );

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

