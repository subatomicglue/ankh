# Game Designer Guide

The engine keeps editable game rules and content separate from generated runtime data and platform-specific code.

## Authoring workflow

1. Edit actor instances in `data/actors.json`.
2. Edit the wall tilemap in `data/walls.json`.
3. Edit named sound slots in `data/sounds.json`.
4. Edit actor reactions in `src/behaviors.js`.
5. Run `npm run generate` after actor or wall changes.
6. Run `npm test`, then play-test the affected rooms.

`src/data.js` is generated. Never edit it directly. `data/sounds.json` is loaded directly and does not require generation.

## Actor data

Each actor entry describes one placed instance: identity, type, position, artwork, collision flags, scores, and behavior initialization.

```json
{
  "id": "switch-a",
  "type": "switch",
  "x": 48,
  "y": 64,
  "roomX": 0,
  "roomY": 0,
  "flags": ["ACTOR_COLLISION_PHYSICS", "ACTOR_VISIBLE"],
  "box": [8, 8],
  "image": "switch",
  "frames": 1,
  "behavior": {
    "type": "trigger",
    "args": "door-a, BehaviourMethod::TRIGGER, BehaviourMethod::SHOOT"
  }
}
```

Room indices are zero-based. Actor `x` and `y` are world pixels; `roomX` and `roomY` identify the containing room.

Keep instance-specific choices on the actor:

- target actor IDs;
- speeds, timers, directions, and damage;
- sound slot IDs;
- behavior modes and starting state.

Do not create a separate hardcoded mapping table when the choice belongs to one actor instance.

## Behaviors are game design

The game sends events to actors. A behavior receives an event and describes that actor's reaction.

Typical events include:

- `update` for time-based behavior;
- `shoot`, `touch`, and `collide` for interactions;
- `pickup` and `drop` for inventory;
- `run`, `stop`, and `step` for movement state;
- `trigger` for actor-to-actor signaling.

Keep behavior code small and readable:

```js
handle(event, actor, caller, game) {
  if (event !== 'shoot') return;
  game.actorById.get(this.target)?.event('trigger', game, actor);
}
```

Prefer sending an event to another actor over changing that actor's internal state directly. The source owns the decision to send; the target owns its reaction.

A behavior may:

- read its actor and initialization data;
- keep small runtime state such as a counter or direction;
- query actors through the game API;
- request movement, collision tests, shots, inventory changes, scores, or sounds;
- send events to other actors.

A behavior should not manipulate the DOM, canvas, browser input, `AudioContext`, renderer internals, or another behavior's private fields.

Reset every piece of behavior runtime state in `reset()` so restart behavior is reproducible.

## Sound from behaviors

Play a named slot through the concise behavior-facing API:

```js
game.sound('effect-name');
```

The returned handle stops that exact playback when an actor must retain ownership of it:

```js
const playback = game.sound('moving-effect');
playback.stop();
```

Stop every voice in a named slot only when that is the intended game rule:

```js
game.sounds.stop('ambient-effect');
```

The actor initiating an action should normally request its immediate feedback sound. The reacting actor should request sounds associated with its own later state changes. This remains correct when actors are in different rooms or update at different times.

## Sound slots

Each `data/sounds.json` entry defines one named sound policy:

```json
{
  "id": "effect-name",
  "src": [
    "assets/sounds/effect-1.m4a",
    "assets/sounds/effect-2.m4a"
  ],
  "next_src": "rnd",
  "loop": "once_fwd",
  "polyphony": 2,
  "positional": false,
  "vol": 1
}
```

Source selection supports `fwd`, `back`, and `rnd`. Looping supports `once_fwd`, `once_bck`, `repeat_fwd`, and `repeat_bck`.

Use a returned playback handle when individual concurrent voices must stop independently. Change slot polyphony through the sound manager only when runtime capacity genuinely depends on an engine-managed pool.

## Walls and pixel collision

`data/walls.json` is the editable rectangular wall tilemap. Preserve its compact row layout so room geometry remains easy to inspect and edit.

Collision follows indexed artwork pixels:

- palette index `0` is transparent and passable;
- any nonzero index is opaque for collision;
- visually black but collidable pixels must use a nonzero near-black palette entry.

Do not duplicate wall collision shapes in behavior data. Rendering and collision should consume the same canonical tile and sprite artwork.

## When to extend the engine

Remain in the game-designer role while a rule can be expressed with actor data, behavior state, existing events, and existing game APIs.

Switch to the engine-architect role only when the behavior needs a reusable capability the engine does not expose. Warning signs include:

- a behavior would need browser, renderer, audio, or input internals;
- several behaviors repeat the same low-level collision or movement algorithm;
- the rule needs a general spatial query, scheduler, lifecycle event, or resource manager;
- implementing the rule would make behavior code describe mechanism instead of gameplay.

When engine support is necessary:

1. State the desired behavior as a simple game rule.
2. Identify the smallest platform-neutral capability missing from the game API.
3. Implement that mechanism behind a narrow engine method or manager.
4. Test the engine capability independently.
5. Return to the designer role and call it concisely from the behavior.

Engine code must not know puzzle sequences, particular room coordinates, or specific actor IDs. Those belong in JSON and behaviors.

Input adapters translate devices into generic commands outside the game core. Renderers translate game state into pixels outside behaviors. Audio internals remain inside the sound manager.

## Debugging and verification

Use debug mode to inspect the real actor initialization data, runtime behavior state, wall tile, and collision pixel under the pointer.

Run:

- `npm run generate` after actor or wall edits;
- `npm test` for data and engine checks;
- `npm run start-debug`, then `npm run test:browser`, for browser integration and responsive-layout checks.
