
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

## 6. UIDef + `<InGameUI>` Example

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

customElements.define("InGameUI", GameUI);

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

