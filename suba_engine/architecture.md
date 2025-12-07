
# ============================================================
# **ENGINE ARCHITECTURE SPEC v1.0**
# ============================================================

A Game Engine for Web Browser.
- 2D map based games with sprites will be supported with the first version
- Games are defined in a single world.json project file.
  - Scene of Actors with Behaviors and Assets (like Map or Sprites, and Sounds)
  - Editor is data driven via pluggable web component, can edit or create new ones
  - In Game UI is data driven via pluggable web component, can edit or create new ones
- Game Project Editing provided by the editor-backend service
  - create and edit game projects, game editors, in game UIs, game assets (like Sprites, Maps, Sounds, Behaviors)
  - browser-based intuitive WYSIWYG GUI
  - asset management with version control stored to local filesystem.
  - editor-backend service provides a basic code-editor with preview
    - The only built-in editor provided
      - for creating custom Game Project Editor web-components.
      - for creating custom InGameUI web-components.
    - Game Project Editor can be created (bootstrapped) from nothing using provided reusable components
    - ... or by copying an existing editor definition and modifying it.
    - InGameUI can be created (bootstrapped) from nothing using provided reusable components
    - ... or by copying an existing editor definition and modifying it.
- Runtime game is created inside index.html with a single <game data="world.json"></game> html element

```
TechStack:   JavaScript + HTML + CSS
Renderer:    Canvas 2D + HTML overlay
Editor:      Pluggable Editable Web Components + Backend FS abstraction
Philosophy:  Data-driven, Behavior-centric, JSON Defs
Project:     world.json is the ENTIRE game (with ingame UI and editor)
```

---

# ============================================================
# 1. CORE DESIGN PRINCIPLES
# ============================================================

```
1. Asset Definitions:  Assets are defined as "Defs"
   ActorDef, SpriteDef, MapDef, SoundDef, UIDef, EditorDef.
   Serve as prototypes (templates) for creating runtime instances.

2. Asset Instances:  All runtime objects (Actor, Map, Sprite, Sound, UI, Editor)
   implicitly instantiate an AssetDefinition, by refering to their Def via:  "def": "<DefName>".

3. Asset Naming:  We'll use "Def", but not "instance" inside world.json schema,
   instance types are short/simple in the json text (Actor, Map, Sprite, Sound, UI, Editor).
   def types are longer in the json text (ActorDef, MapDef, SpriteDef, SoundDef, UIDef, EditorDef).

4. Actors hold state, which is used by behavior logic.
   Behaviors hold both logic AND it's own state.  Behavior logic can use actor state as well.
   Defs hold immutable configuration.
   
5. Triggers are the universal communication mechanism.
   Actor has emitTrigger( name, params ), which dispatches down the hierarchy to child behavior(s) and actor(s).

6. Engine input and In Game UI both emit triggers into the root Actor.
   Behaviors can emit triggers into other actors

7. world.json defines the entire project:
   - asset defs and instances
   - root world actor with child actors
   - editor
   - in game ui

8. Editor and In Game UI are Web Components (just javascript/html/css code)
   defined via world.json's UIDef and EditorDef
   a world.json has one editor and one ui, set by "editor" and "ui" (which refers to EditorDef and UIDef)

9. One Reusable Loader in the world.json schema
   looks like:  { include: "file.js" } or { inline: "javascript_code_goes_here()" }
   which handles:
    - include JS or JSON
    - inline JS or JSON

10. Everything is extensible:
    new Def types can be added easily.
    editor can be replaced
    ingame ui can be replaces
    behaviors allow new custom logic
    we'll build libraries of reusable components so building these are easy, but some editor/ingameui/behaviors will be very game specific, of course.
```

---

# ============================================================
# 2. WORLD.JSON STRUCTURE (TOP LEVEL)
# ============================================================

```
{
  "actorDefs": { ... },
  "spriteDefs": { ... },
  "mapDefs": { ... },
  "soundDefs": { ... },
  "uiDefs": { ... },
  "editorDefs": { ... },

  "world": { ... },       // The root Actor
  "ui": { "def": "<UIDefName>" },
  "editor": { "def": "<EditorDefName>" }
}
```

Everything in the project lives under **world.json**.
Assets referenced by paths live in `/assets/...`.

---

# ============================================================

# 3. DEFS

# ============================================================

All Defs use the same pattern:

```
"<DefName>": {
    "include": "path/to/file.js"
}
```

OR

```
"<DefName>": "inline JS code or JSON data"
```

OR

```
"<DefName>": { ... JSON data definition ... }
```

### Def Types:

```
ActorDef
SpriteDef
MapDef
SoundDef
UIDef
EditorDef
```

### Loader Behavior:

The loader is unified and resolves Defs using a common pipeline.

---

# ============================================================

# 4. RUNTIME OBJECTS (IMPLICIT INSTANCES)

# ============================================================

Runtime objects never use the word “instance,” but conceptually they are instantiations of Defs.

### Types:

```
Actor
Map
SpriteInstance
Sound
UI
Editor
```

### Mandatory Field:

```
"def": "<DefName>"
```

Example Actor:

```
{
  "def": "enemySlime",
  "id": "slime004",
  "x": 120,
  "y": 200,
  "behaviors": ["slimeAI"],
  "data": { "hp": 5 }
}
```

---

# ============================================================

# 5. ACTORS

# ============================================================

Actors are the primary runtime entities.

### Actor Fields:

```
id          string
def         ActorDef name
parent      optional actor id
localX,Y    transform
behaviors[] list of behavior names or { name: config }
data{}      arbitrary state from world.json
```

Actors **do not own children.**
Parent relationship is transform-only.

### Transform:

```
worldPos = localPos + parent.localPos + ... root
```

### Root Actor:

```
engine.actor   // singular
```

---

# ============================================================

# 6. BEHAVIORS

# ============================================================

Behavior attachment:

```
"behaviors": [
    "wanderAI",
    { "inventory": { "max": 20 } }
]
```

Behavior code:

```
class WanderAI extends Behavior {
    handle(trigger, ...args) { ... }
    serialize() { return {...}; }
    deserialize(data) { ... }
}
```

Behaviors receive:

```
this.actor
this.world   (root actor)
this.engine
```

---

# ============================================================

# 7. TRIGGER SYSTEM

# ============================================================

Universal communication primitive.

```
actor.emitTrigger("triggerName", arg1, arg2)
```

### Routing:

* If actor defines `"triggers": { ... }` → specific behavior(s)
* Else → all behaviors

### Engine Emits:

```
init
update(dt)
lateUpdate(dt)
```

### Collision Emits:

```
collideBB
collidePixel
enterCollision
exitCollision
collideTile
enterTile
exitTile
```

### UI Emits:

Via mouse/keyboard interactions.

---

# ============================================================

# 8. SPRITEDEF & SPRITEINSTANCE

# ============================================================

### SpriteDef includes:

```
image
frameWidth
frameHeight
states: { walk:1, idle:0, attack:2 }
frameRateHz
direction: forward | reverse | pingpong
ignoreColors: ["#000000", "#ff00ff"]
boundingBox: {x1,y1,x2,y2}
```

### SpriteInstance:

Stored on Actor:

```
actor.sprite.frame
actor.sprite.state
actor.sprite.time
```

---

# ============================================================

# 9. MAPDEF & MAP

# ============================================================

### MapDef includes:

```
tileImage
tileWidth
tileHeight
chunks[]   // arbitrary size and placement
collisionIgnoreColors[]
```

### Map:

Runtime map derived from MapDef, used for:

* Rendering tiles
* Tile collision queries
* Chunk-based culling

---

# ============================================================

# 10. SOUNDDEF & SOUND

# ============================================================

### SoundDef:

```
{
  "path": "assets/sounds/coin.wav",
  "pool_size": 8,        // round-robin pool
  "spatial": true,
  "falloff": "linear"
}
```

### Sound:

Each actor may attach sound instances:

```
actor.sounds["coin"] = new Sound(def)
```

### Sound Behavior:

* Spatial attenuation based on ListenerBehavior
* Ambient sounds bypass attenuation
* Pooling supports overlapping playback

Listener:

```
engine.listenerPosition = actor.getWorldPos()
```

---

# ============================================================

# 11. CAMERA SYSTEM

# ============================================================

Camera is a Behavior on the world Actor.

Camera updates:

```
engine.cameraX
engine.cameraY
```

Types:

* Follow
* Bounded
* Room snap
* Smooth spring

---

# ============================================================

# 12. INPUT SYSTEM

# ============================================================

HTML input → triggers:

```
keydown ArrowLeft → rootActor.emitTrigger("moveLeftDown")
keyup ArrowLeft   → rootActor.emitTrigger("moveLeftUp")
mousedown         → rootActor.emitTrigger("pointerDown", x,y)
```

---

# ============================================================

# 13. GAME UI SYSTEM

# ============================================================

world.json:

```
"ui": { "def": "mainUI" }
```

UIDef:

```
"mainUI": { "include": "ui/ui.js" }
```

Runtime:

```
<game-ui></game-ui>
```

GameUI receives:

```
engine
world
rootActor
```

UI emits triggers into rootActor.

---

# ============================================================

# 14. EDITOR SYSTEM

# ============================================================

world.json:

```
"editor": { "def": "defaultEditor" }
```

EditorDef:

```
"defaultEditor": { "include": "editor/mainEditor.js" }
```

Runtime:

```
<world-editor></world-editor>
```

Editor consists of Web Components:

```
<sprite-editor>
<tilemap-editor>
<actor-editor>
<behavior-editor>
<code-editor>
<file-selector>
<engine-preview>
```

Editor preferences saved in localStorage.

---

# ============================================================

# 15. EDITOR BACKEND FS ABSTRACTION

# ============================================================

Versioned assets stored under:

```
assets/.versions/<mirrored tree>/
```

On save:

1. Copy existing file to next version
2. Overwrite canonical
3. Update version listing

On rename:

* Check collisions
* Rename canonical
* Rename all versions

Paths sanitized.

---

# ============================================================

# 16. UPDATE LOOP

# ============================================================

Variable dt:

```
dt = min(actualDt, 1/30)
```

Pipeline:

```
emit init (first frame)
emit update(dt)
collision.update()
sprites.update(dt)
sounds.update(dt)
camera.update(dt)
ui.update(dt)
renderer.draw()
```

---

# ============================================================

# 17. SAVE / LOAD RUNTIME STATE

# ============================================================

Saved:

* Actor positions
* Actor data
* Behavior.serialize()
* Sound playback states (optional)
* World/global flags
* Parent relationships

Load:

1. Load world.json
2. Load save.json
3. Reinstantiate runtime
4. Apply state

---

# ============================================================

# END SPEC v1.0

# ============================================================



















## 1. SpriteDef Examples

```json
{
  "spriteDefs": {
    "heroSprite": {
      "image": "assets/sprites/hero.png",
      "frameWidth": 16,
      "frameHeight": 16,

      "states": {
        "idle": 0,
        "walk": 1,
        "attack": 2
      },

      "frameRateHz": 8,
      "direction": "forward",

      "ignoreColors": ["#000000", "#ff00ff"],
      "boundingBox": { "x1": 2, "y1": 2, "x2": 13, "y2": 15 }
    },

    "slimeSprite": {
      "image": "assets/sprites/slime.png",
      "frameWidth": 16,
      "frameHeight": 16,

      "states": {
        "idle": 0,
        "hop": 1
      },

      "frameRateHz": 4,
      "direction": "pingpong",
      "ignoreColors": ["#000000"]
    }
  }
}
```

Notes:

* `states` maps state names → row indices
* `ignoreColors` supports pixel-precise collision
* `boundingBox` is in sprite pixel space

---

## 2. MapDef Examples

```json
{
  "mapDefs": {
    "overworld": {
      "tileImage": "assets/tiles/overworld.png",
      "tileWidth": 16,
      "tileHeight": 16,

      "collisionIgnoreColors": ["#000000"],

      "chunks": [
        {
          "x": 0,
          "y": 0,
          "width": 64,
          "height": 64,
          "tiles": [
            [ 1, 1, 1, 1, 1, 1, 1, 1 ],
            [ 1, 2, 2, 2, 2, 2, 2, 1 ],
            [ 1, 2, 3, 3, 3, 3, 2, 1 ],
            [ 1, 2, 3, 4, 4, 3, 2, 1 ],
            [ 1, 2, 3, 3, 3, 3, 2, 1 ],
            [ 1, 2, 2, 2, 2, 2, 2, 1 ],
            [ 1, 1, 1, 1, 1, 1, 1, 1 ]
          ]
        }
      ]
    },

    "dungeon1": {
      "tileImage": "assets/tiles/dungeon.png",
      "tileWidth": 16,
      "tileHeight": 16,
      "collisionIgnoreColors": ["#000000"],

      "chunks": [
        {
          "x": 0,
          "y": 0,
          "width": 32,
          "height": 32,
          "tiles": [ /* big 2D array of tile indices */ ]
        }
      ]
    }
  }
}
```

Notes:

* `chunks` are positioned in tile coordinates via `x`, `y`.
* `collisionIgnoreColors` is used at load time to precompute per-tile solidity.

---

## 3. SoundDef Examples

```json
{
  "soundDefs": {
    "coinPickup": {
      "path": "assets/sounds/coin.wav",
      "pool_size": 8,
      "spatial": true,
      "falloff": "linear"
    },

    "playerHit": {
      "path": "assets/sounds/player_hit.wav",
      "pool_size": 4,
      "spatial": true,
      "falloff": "linear"
    },

    "ambientWind": {
      "path": "assets/sounds/wind_loop.ogg",
      "pool_size": 1,
      "spatial": false
    }
  }
}
```

Notes:

* `pool_size` is the pool per **Sound**, duplicated per bound Actor.
* `spatial: true` = distance attenuation via ListenerBehavior.
* `spatial: false` = no attenuation (ambient).

---

## 4. ActorDef + Actor Examples

### ActorDef section

```json
{
  "actorDefs": {
    "player": {
      "spriteDef": "heroSprite",

      "behaviors": [
        "playerControl",
        "inventory",
        "playerHealth"
      ],

      "data": {
        "maxHealth": 10,
        "moveSpeed": 60,
        "faction": "hero"
      },

      "sounds": {
        "hit": "playerHit"
      }
    },

    "enemySlime": {
      "spriteDef": "slimeSprite",

      "behaviors": [
        "slimeAI",
        "enemyHealth"
      ],

      "data": {
        "maxHealth": 3,
        "moveSpeed": 40,
        "faction": "enemy"
      },

      "sounds": {
        "hit": "slimeHit"
      }
    },

    "coin": {
      "spriteDef": "coinSprite",

      "behaviors": [
        "pickupItem"
      ],

      "data": {
        "value": 1
      },

      "sounds": {
        "pickup": "coinPickup"
      }
    }
  }
}
```

Notes:

* `data` in ActorDef are default fields; Actors can override.
* `sounds` maps semantic names → SoundDef names (optional, but nice).

### Actor objects in the world

```json
{
  "world": {
    "def": "worldActorDef",
    "id": "worldRoot",

    "mapDef": "overworld",

    "behaviors": [
      "worldContainer",
      "collisionSystem",
      "camera",
      "spatialHash",
      { "game": { "startMap": "overworld" } },
      "listener"   // sets engine.listenerPosition
    ],

    "actors": [
      {
        "def": "player",
        "id": "player",
        "x": 80,
        "y": 120,
        "data": {
          "health": 10
        },
        "tags": ["player", "controllable"]
      },

      {
        "def": "enemySlime",
        "id": "slime_001",
        "x": 160,
        "y": 140,
        "data": {
          "maxHealth": 5   // override default
        },
        "tags": ["enemy"]
      },

      {
        "def": "coin",
        "id": "coin_001",
        "x": 200,
        "y": 100,
        "tags": ["pickup", "gold"]
      }
    ]
  }
}
```

Notes:

* `world` is an Actor with its own `def` (e.g. `"worldActorDef"`).
* `world.actors` is a pure data list that the Game behavior / worldContainer behavior will instantiate into runtime Actors.

---

## 5. Behavior JS Examples

### 5.1 PlayerControlBehavior

```js
// behaviors/playerControl.js
class PlayerControl extends Behavior {
    handle(trigger, ...args) {
        switch (trigger) {
            case "update":
                this.updateMovement(args[0]); // dt
                break;

            case "moveLeftDown":
                this.moveLeft = true;
                break;
            case "moveLeftUp":
                this.moveLeft = false;
                break;

            case "moveRightDown":
                this.moveRight = true;
                break;
            case "moveRightUp":
                this.moveRight = false;
                break;

            case "moveUpDown":
                this.moveUp = true;
                break;
            case "moveUpUp":
                this.moveUp = false;
                break;

            case "moveDownDown":
                this.moveDown = true;
                break;
            case "moveDownUp":
                this.moveDown = false;
                break;
        }
    }

    updateMovement(dt) {
        const data = this.actor.data;
        const speed = data.moveSpeed ?? 60;
        let dx = 0;
        let dy = 0;

        if (this.moveLeft)  dx -= speed * dt;
        if (this.moveRight) dx += speed * dt;
        if (this.moveUp)    dy -= speed * dt;
        if (this.moveDown)  dy += speed * dt;

        this.actor.localX += dx;
        this.actor.localY += dy;

        if (dx !== 0 || dy !== 0) {
            this.actor.emitTrigger("playerMoved", this.actor.localX, this.actor.localY);
        }
    }
}

engine.registerBehavior("playerControl", PlayerControl);
```

---

### 5.2 InventoryBehavior

```js
// behaviors/inventory.js
class Inventory extends Behavior {
    constructor(actor, world, config) {
        super(actor, world, config);
        this.max = config?.max ?? 16;
        this.items = [];
    }

    serialize() {
        return { items: this.items.slice(0) };
    }

    deserialize(data) {
        this.items = data.items ?? [];
    }

    handle(trigger, ...args) {
        switch (trigger) {
            case "pickup":
                this.tryPickup(args[0]); // itemActor
                break;
            case "useItem":
                this.useItem(args[0]); // slot index
                break;
        }
    }

    tryPickup(itemActor) {
        if (this.items.length >= this.max) return;
        this.items.push(itemActor.id);

        // remove from world container, hide sprite, etc.
        this.world.emitTrigger("removeActor", itemActor);
    }

    useItem(slotIndex) {
        const id = this.items[slotIndex];
        if (!id) return;

        // let some other behavior handle "useItemOnSelf"
        this.actor.emitTrigger("useItemOnSelf", id);
    }
}

engine.registerBehavior("inventory", Inventory);
```

---

### 5.3 Simple SlimeAI Behavior

```js
// behaviors/slimeAI.js
class SlimeAI extends Behavior {
    constructor(actor, world, config) {
        super(actor, world, config);
        this.direction = 1;
        this.timer = 0;
    }

    handle(trigger, ...args) {
        if (trigger === "update") {
            const dt = args[0];
            this.timer += dt;
            if (this.timer > 2.0) {
                this.direction *= -1;
                this.timer = 0;
            }

            const speed = this.actor.data.moveSpeed ?? 40;
            this.actor.localX += this.direction * speed * dt;
        }
    }
}

engine.registerBehavior("slimeAI", SlimeAI);
```

---

## 6. UIDef + `<game-ui>` Example

### UIDef in world.json

```json
{
  "uiDefs": {
    "defaultUI": {
      "include": "ui/defaultUI.js"
    }
  },

  "ui": {
    "def": "defaultUI"
  }
}
```

### ui/defaultUI.js

```js
// ui/defaultUI.js
class GameUI extends HTMLElement {
    constructor() {
        super();
        this.attachShadow({ mode: "open" });
        this.shadowRoot.innerHTML = `
            <style>
              .hud { position: fixed; top: 8px; left: 8px; color: white; }
              .inventory { position: fixed; bottom: 8px; left: 8px; color: white; }
              button { margin-right: 4px; }
            </style>
            <div class="hud">
              Health: <span id="health">-</span>
            </div>
            <div class="inventory">
              <button id="useSlot0">Use Slot 0</button>
              <button id="useSlot1">Use Slot 1</button>
            </div>
        `;
    }

    initialize(engine, world, rootActor) {
        this.engine = engine;
        this.world = world;
        this.rootActor = rootActor;

        const use0 = this.shadowRoot.getElementById("useSlot0");
        const use1 = this.shadowRoot.getElementById("useSlot1");
        use0.addEventListener("click", () => {
            this.rootActor.emitTrigger("useItem", 0);
        });
        use1.addEventListener("click", () => {
            this.rootActor.emitTrigger("useItem", 1);
        });

        this.healthSpan = this.shadowRoot.getElementById("health");
    }

    update(dt) {
        // example: read player health each frame (could be optimized with events)
        const game = this.world.get_behavior("game");
        const player = game.findActorById("player");
        if (player && this.healthSpan) {
            this.healthSpan.textContent = player.data.health ?? "?";
        }
    }
}

customElements.define("game-ui", GameUI);

// register with engine's UIDef loader if needed
engine.registerUIDef("defaultUI", GameUI);
```

---

## 7. EditorDef + `<world-editor>` Example

### EditorDef in world.json

```json
{
  "editorDefs": {
    "defaultEditor": {
      "include": "editor/mainEditor.js"
    }
  },

  "editor": {
    "def": "defaultEditor"
  }
}
```

### editor/mainEditor.js

```js
// editor/mainEditor.js
class WorldEditor extends HTMLElement {
    constructor() {
        super();
        this.attachShadow({ mode: "open" });
        this.shadowRoot.innerHTML = `
          <style>
            .layout { display: grid; grid-template-columns: 220px 1fr 300px; height: 100vh; }
            .sidebar { border-right: 1px solid #444; overflow-y: auto; }
            .center { border-right: 1px solid #444; position: relative; }
            .rightbar { overflow-y: auto; }
          </style>
          <div class="layout">
            <div class="sidebar">
              <file-selector id="files"></file-selector>
            </div>
            <div class="center">
              <engine-preview id="preview"></engine-preview>
            </div>
            <div class="rightbar">
              <sprite-editor id="spriteEditor"></sprite-editor>
              <actor-editor id="actorEditor"></actor-editor>
              <code-editor id="codeEditor"></code-editor>
            </div>
          </div>
        `;
    }

    initialize(projectWorldJson, backendAPI) {
        this.projectWorldJson = projectWorldJson;
        this.backendAPI = backendAPI;

        const fileSelector = this.shadowRoot.getElementById("files");
        const preview = this.shadowRoot.getElementById("preview");
        const spriteEditor = this.shadowRoot.getElementById("spriteEditor");
        const actorEditor = this.shadowRoot.getElementById("actorEditor");
        const codeEditor = this.shadowRoot.getElementById("codeEditor");

        // wire up events, e.g.:
        fileSelector.addEventListener("fileSelected", (e) => {
            const path = e.detail.path;
            // open in appropriate editor
        });

        preview.loadWorld(projectWorldJson);
    }
}

customElements.define("world-editor", WorldEditor);

// registered via editorDefs loader
engine.registerEditorDef("defaultEditor", WorldEditor);
```

---

## 8. Full Example `world.json` (Minimal but End-to-End)

Here’s a compact but coherent example tying it all together:

```json
{
  "spriteDefs": {
    "heroSprite": {
      "image": "assets/sprites/hero.png",
      "frameWidth": 16,
      "frameHeight": 16,
      "states": { "idle": 0, "walk": 1 },
      "frameRateHz": 8,
      "direction": "forward",
      "ignoreColors": ["#000000"],
      "boundingBox": { "x1": 2, "y1": 2, "x2": 13, "y2": 15 }
    },
    "slimeSprite": {
      "image": "assets/sprites/slime.png",
      "frameWidth": 16,
      "frameHeight": 16,
      "states": { "idle": 0, "hop": 1 },
      "frameRateHz": 4,
      "direction": "pingpong",
      "ignoreColors": ["#000000"]
    }
  },

  "mapDefs": {
    "overworld": {
      "tileImage": "assets/tiles/overworld.png",
      "tileWidth": 16,
      "tileHeight": 16,
      "collisionIgnoreColors": ["#000000"],
      "chunks": [
        {
          "x": 0,
          "y": 0,
          "width": 8,
          "height": 7,
          "tiles": [
            [1,1,1,1,1,1,1,1],
            [1,2,2,2,2,2,2,1],
            [1,2,3,3,3,3,2,1],
            [1,2,3,4,4,3,2,1],
            [1,2,3,3,3,3,2,1],
            [1,2,2,2,2,2,2,1],
            [1,1,1,1,1,1,1,1]
          ]
        }
      ]
    }
  },

  "soundDefs": {
    "coinPickup": {
      "path": "assets/sounds/coin.wav",
      "pool_size": 8,
      "spatial": true,
      "falloff": "linear"
    }
  },

  "actorDefs": {
    "worldActorDef": {
      "mapDef": "overworld",
      "behaviors": [
        "worldContainer",
        "collisionSystem",
        "camera",
        "spatialHash",
        "listener",
        { "game": { "startMap": "overworld" } }
      ]
    },

    "player": {
      "spriteDef": "heroSprite",
      "behaviors": ["playerControl", "inventory", "playerHealth"],
      "data": { "maxHealth": 10, "moveSpeed": 60 },
      "sounds": { "pickup": "coinPickup" }
    },

    "enemySlime": {
      "spriteDef": "slimeSprite",
      "behaviors": ["slimeAI", "enemyHealth"],
      "data": { "maxHealth": 3, "moveSpeed": 40 }
    }
  },

  "uiDefs": {
    "defaultUI": {
      "include": "ui/defaultUI.js"
    }
  },

  "editorDefs": {
    "defaultEditor": {
      "include": "editor/mainEditor.js"
    }
  },

  "world": {
    "def": "worldActorDef",
    "id": "worldRoot",

    "actors": [
      {
        "def": "player",
        "id": "player",
        "x": 80,
        "y": 120,
        "data": { "health": 10 },
        "tags": ["player"]
      },
      {
        "def": "enemySlime",
        "id": "slime_001",
        "x": 160,
        "y": 140,
        "tags": ["enemy"]
      }
    ]
  },

  "ui": {
    "def": "defaultUI"
  },

  "editor": {
    "def": "defaultEditor"
  }
}
```

