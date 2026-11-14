# ANKH.js

A browser port of the Game Boy Advance recreation of David Van Brink's **ANKH**. It uses the original 64-room tile map, 212 actor definitions, indexed bitmap art, object flags, karma values, and behavior assignments from `GameBoyAdvance/`.

`Web/` is not used or modified.

## Run

From `ANKH.js/`:

```sh
npm start
```

Open <http://localhost:8080/>. Run `npm run help` for controls and development commands. `npm run start-debug` finds the Mac's LAN address and a free port, starts an HTTPS debug server, prints iPhone setup instructions, and launches an isolated Chrome instance that automation and Codex can inspect.

## Controls

- `I J K L`: run in four directions.
- `Ctrl` or `Shift` + `I J K L`: take one step in four directions.
- `W A S D`: fire in four directions.
- `Ctrl` or `Shift` + `W A S D`: touch/take in four directions while held.
- `Space`: stop.
- `1`–`4`: set down the corresponding inventory object.
- `Esc` or `P`: rest/pause.
- `R`: restart.
- `Space` or `Enter` starts from the original nine-screen title sequence.

### Mobile

- Press inside one of the four 90-degree cones around Other to run in that direction; drag through the cones to steer. The cone origin lags slightly behind a moving Other to compensate for human reaction time.
- Cone mode defaults to `follow`, which recomputes the cone origin from Other continuously. Use `?cones=stay` or `ANKH.mobileControls.toggleConeMode()` to test `stay`, where the first stopped press fixes the cone center until the next stop.
- Stop by tapping the same movement-start spot again, tapping the opposite cone, or pressing **Stop**.
- Tap an occupied inventory cell to drop that item; desktop number keys remain available for keyboard play.
- Touch the score-text strip with two fingers to open mobile settings for cone anchoring and reaction lag.
- Hold **Step** and tap a direction to take one careful step.
- Hold **Step**, **Shoot**, or **Touch** and press a direction to modify that directional action. **Stop** stops immediately.
- In desktop Chrome's mobile layout, hold `Z`, `X`, or `C` while clicking a direction or pressing IJKL to hold **Step**, **Shoot**, or **Touch** respectively; `V` and `Space` trigger **Stop**. Fine-pointer browsers show these key hints beneath the bitmap pad labels.
- The mobile layout fills the viewport, places all four inventory cells above the game, and uses the GBA room-panning camera.

## Architecture

The port retains the source engine's separations:

- `data.js`: generated 64-room map and actor initialization data.
- `actor.js` and `behaviors.js`: actors delegate events to pluggable behavior objects.
- `actor-manager.js`: GBA-style on-screen membership, behavior-update, offscreen-event, and animation timing.
- `world.js` and `collision.js`: renderer-independent indexed-pixel world and pixel-mask collision API.
- `inventory.js`, `shots.js`, and `touch.js`: independent managers corresponding to the C++ managers, including the extending/retracting touch animation.
- `camera.js`: room-sized normal framing plus an isolated free-camera debugging mode.
- `input.js`: the shared command buffer plus desktop keyboard adapter.
- `mobile-controls.js`: pointer/touch gestures translated into the same simple commands; it contains no gameplay rules.
- `game.js`: fixed-step state and camera coordinator.
- `renderer.js`: renderer interface plus the default `CanvasRenderer`.
- `bitmap.js`: 8-bit BMP decoder and authoritative collision-mask asset store.

A renderer implements `load(game)`, `resize()`, `render(game)`, and `destroy()`. It reads game state but does not own physics or collision, so a Three.js or SVG renderer can replace Canvas while using the original bitmap masks—or its own visuals—without changing simulation behavior.

Run `npm run generate` after changing `GameBoyAdvance/data.cpp` or `walls.map.c` to regenerate `src/data.js`. Run `npm run audit` to verify the complete generated map, all 212 ordered actor declarations, behavior targets, animation metadata, original bitmap assets, and engine tests against the GBA source.

## Verification

`npm test` runs the engine/data suite. For the real-browser suite, run `npm run start-debug` in one terminal and `npm run test:browser` in another. The launcher chooses a free HTTPS port and passes its URL to Chrome; the Chrome debugging port defaults to 9222. Set `DEBUG_SERVER_PORT` to pin the server port, or use `DEBUG_CHROME_PORT`, `DEBUG_CHROME_PROFILE`, and `DEBUG_CHROME_START_URL` for the other launch settings. Normal `npm start` remains on port 8080. Run `./start-debug-server.sh` or `./start-debug-chrome.sh` when you need either half independently. The scripts are self-contained and reusable: use `./start-debug-server.sh --html filename.html` for a static project, or `./start-debug-server.sh --pm2 script.js` for a Node.js server that reads `NODE_USE_HTTPS`, `NODE_HTTPS_PORT`, and `NODE_CERTS_DIR`. The suite checks a desktop/mobile viewport-size matrix, all 64 rooms, transparent-pixel nesting, indexed collision, keyboard controls, cone steering and stopping, held mobile action modifiers, touch animation, inventory-cell dropping, settings, door chains, and the piano sequence. It writes screenshots to `/tmp/ankh-js-browser.png` and `/tmp/ankh-js-mobile.png`.
