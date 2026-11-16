console.log(`
ANKH.js

Run the game:
  npm start
  Open http://localhost:8080/

Debug server and Chrome:
  npm run start-debug  Find a free port, serve HTTPS, and launch inspectable Chrome

Desktop controls:
  I J K L          Run
  Ctrl/Shift + I J K L  Step
  W A S D          Shoot
  Ctrl/Shift + W A S D  Touch / take
  Space            Stop (or start from the title sequence)
  1 2 3 4          Drop an inventory object
  Escape / P       Pause
  R                Restart
  F2 or \`          Toggle GBA-style debug flyover

Debug flyover:
  Toggle it on, then use I J K L normally. Other and the room-following
  camera pass through walls and actors. Toggle it off to resume physics.
  R after moving resets the world but keeps the current flyover position.
  Press R again without moving to perform the normal full reset.

Mobile controls:
  Press a 90° cone       Run in that direction; drag to steer
  Stop                   Same spot, opposite cone, or STOP pad
  Cone origin            follows Other
  Inventory cells        Tap to drop that item
  Settings               Two-finger touch on score text
  Hold STEP + tap        Take one careful step
  Hold STEP/SHOOT/TOUCH  Modify the directional press
  Desktop mobile debug  Hold Z/X/C + click or IJKL; V/Space=STOP

Development:
  npm test           Run engine tests
  npm run audit      Verify generated ANKH.js data and engine tests
  npm run generate  Build src/data.js from data/*.json
  npm run help       Show this help
  npm run start-debug  Start HTTPS plus the isolated debug Chrome profile
`);
