
# ENGINE ARCHITECTURE SPEC v1.0
A Game Engine for Web Browser.

- Features at a glance:
  - 2D map based games with sprites will be supported with the first version
  - Games are defined in a single `world.json` project file.
    - Scene of `Actor`s with `Behavior`s and `Asset`s (like `Map` or `Sprite`s, and `Sound`s)
    - `Game Project Editor` is data driven via pluggable web component, can edit or create new ones
    - `In Game UI` is data driven via pluggable web component, can edit or create new ones
  - Game Project Editing provided by the `editor-backend.js` service
    - create and edit game projects, game editors, in game UIs, game assets (like `Sprite`s, `Map`s, `Sound`s, `Behavior`s)
    - browser-based intuitive WYSIWYG GUI
    - asset management with version control stored to local filesystem.
    - `editor-backend.js` service provides a basic code-editor with preview
      - The only built-in editor provided
        - for creating custom `GameProjectEditor` web-components.
        - for creating custom `InGameUI` web-components.
      - `Game Project Editor` can be created (bootstrapped) from nothing using provided reusable components
      - ... or by copying an existing editor definition and modifying it.
      - `InGameUI` can be created (bootstrapped) from nothing using provided reusable components
      - ... or by copying an existing editor definition and modifying it.
  - Runtime game is created inside `index.html` with a single <game data="world.json"></game> html element


| feature           | about         |
| ---------------- | ------------- |
| **TechStack**:   | JavaScript + HTML + CSS, no or minimal dependencies |
| **Renderer**:    | Canvas 2D + HTML overlay |
| **Editor**:      | Pluggable Editable Web Components + Backend FS abstraction |
| **Philosophy**:  | Data-driven, Behavior-centric, JSON Defs |
| **Project**:     | world.json is the ENTIRE game (with ingame UI and editor) |
| **Code Std**:    | spaces instead of tabs; 2 spaces per tab; curlybrace on same line as conditional; requirements driven; keep it simple stupid; refactor towards simplicity as you go along; CamelCase classes/structs, camelCase functions, snake_case variables, _private variables, ALL_CAPS enums, if () conditionals, for () and while () loops; func( arg, arg2, arg3 ) functions; |


# Conventions

- `designer` is the user making the game design, setting up `world.json` 
- AssetDef is used to generically talk about all asset types, there is no `Asset` or `AssetDef` types in this system (unless the engine needs to use a baseclass for some common code, but this will be hidden from the game `designer`)

---
# CORE DESIGN PRINCIPLES

1. Asset Definitions:  Assets are first defined as "Defs"
   - `ActorDef`, `SpriteDef`, `MapDef`, `SoundDef`, `InGameUIDef`, `EditorDef`.
   - Serve as prototypes (templates) for creating runtime Asset instances of those types.

2. Asset instances:  All runtime objects (`Actor`, `Map`, `Sprite`, `Sound`, `InGameUI`, `GameProjectEditor`)
   - implicitly instantiate an `<Asset>Def`, by refering to their Def via:  `"def": "<DefName>"`.

3. Asset Naming:  We'll use "Def", but not "instance" inside world.json schema,
   - instance types are short/simple in the json text (`Actor`, `Map`, `Sprite`, `Sound`, `InGameUI`, `GameProjectEditor`).
   - def types are longer in the json text (`ActorDef`, `MapDef`, `SpriteDef`, `SoundDef`, `InGameUIDef`, `EditorDef`).

4. `Actors` hold state, which is used by `Behavior` logic.
   - `Behavior`s hold both logic AND it's own state.  `Behavior` logic can use `Actor` state as well.
     - `Behavior` state is internal for that `Behavior`.
     - `Actor` state is can be used by multiple `Behavior`s.
   - AssetDefs hold immutable configuration to initialize an Asset.

5. Triggers are the universal communication mechanism.
   - `Actor` has `emitTrigger( name, params )`, which dispatches down the hierarchy to child `Behavior`(s) and `Actor`(s).

6. The engine's mouse/keyboard input handlers, collision system, and `InGameUI` all emit triggers into the root `Actor`.
   - `Behavior`s can emit triggers into other `Actor`s

7. `world.json` defines the entire project:
   - `<Asset>Defs` - asset definitions (which allows us to spawn instances)
   - `actor` - actor graph - a single root 'world' `Actor` instance with child `Actor` instances
   - `GameProjectEditor` - A game world editor that defines `<GameProjectEditor>` html web component
   - `InGameUI` - An In Game UI that defines `<InGameUI>` html web component

8. `GameProjectEditor` and `InGameUI` are Web Components (just javascript/html/css code)
   - defined via `world.json`'s `InGameUIDef` and `EditorDef`
   - a `world.json` has one `GameProjectEditor` and one `InGameUI` (which instances `EditorDef` and `InGameUIDef` respectively)

9. A reusable `configLoader` pattern throughout the `world.json` schema
   - looks like:  `config: { include: "file.js" }` or `config: { inline: "javascript_code_goes_here()" }`
   - which handles:
     - `include` JS or JSON
     - `inline` JS or JSON

10. Everything is extensible:
    - new Def types can be added easily.
    - `GameProjectEditor` can be replaced
    - `InGameUI` can be replaced
    - `Behavior`s allow new custom logic
    - we'll build libraries of reusable components so building these are easy, but some `GameProjectEditor`/`ingameui`/`Behavior`s will be very game specific, of course.




---
# WORLD.JSON STRUCTURE (TOP LEVEL)
We define a game inside a project file called `world.json`:
```
{
  "actorDefs": { ... },
  "spriteDefs": { ... },
  "mapDefs": { ... },
  "soundDefs": { ... },
  "uiDefs": { ... },
  "editorDefs": { ... },

  "actor": { ... },       // The root "world" Actor
  "ui": { "def": "<UIDefName>" },
  "editor": { "def": "<EditorDefName>" }
}
```

Everything in the project, which defines a game, lives under `world.json`.
We provide the engine code, to run the `world.json` project.

### filesystem structure
Typical dir structure of the `"assets/"` directory (which may be renamed to match the project name):
- `assets/world.json`
- `assets/editor.js`
- `assets/ingameui.js`
- `assets/worldmap.png `
- `assets/[spriteimages].png`  .... (many)
- `assets/[sounds].m4a` .... (many)



---
# WORLD.JSON ASSET HIERARCHY
Assets have a hierarchy, for example:
- The root `Actor` asset defines the game world.
- Each `Actor` instance is defined by it's aggregate children:
  - active logic `Behavior` instances
  - audio/visual instances (e.g. `Sound`, `Sprite`, `Map`)
  - child `Actor` instances
  - data stats (data parameters)

Each Asset has a unique `id` (enforced).
- customizable, or will autogenerate unspecified

From this, a game world exists.

### Conceptual game world actor hierarchy at runtime:
- "World" `Actor` (root)
  - "World" `Behavior`  (scoring, etc...)
  - "World" `Sound` (music or ambient loop)
  - "Map" `Actor`
    - "Map" `Behavior`
    - "Map" `Map`
  - "PlayerCharacter" `Actor`
    - x,y data
    - "PlayerCharacter" `Behavior`
    - "PlayerCharacter" `Sprite`
    - "FootStep" `Sound`

Behavior code/logic can make the world seem to come alive, can give `Actor`s their intelligence:
- `Behavior`s can spawn new `Actor`s
- `Behavior`s can trigger actions on other assets
  - within self, or within the game world (e.g. on child `Actor`s, `Sound` play, `Sprite` animations)

NOTE: actual schema follows below...



---
# WORLD.JSON Asset Definitions (Def)
All Defs use the same pattern:

```
"<AssetDefType>Def": {
  "name": "<DefName>",
  "config": "path/to/file.<js or json>",      // include style config (configLoader checks for string matching path)
  "config": "java_script_goes_here()",        // inline style config (configLoader checks for string !matching path)
  "config": { ... JSON data definition ... }  // json style config (configLoader checks for object)
}
```

### Definition Types:

Asset definitions serve as templates (aka prototypes) that are used when spawning instances.
- `ActorDef` - *defines* how to create an `Actor` instance
- `BehaviorDef` - *defines* how to create an `Behavior` instance
- `SpriteDef` - *defines* how to create an `Sprite` instance
- `MapDef` - *defines* how to create an `Map` instance
- `SoundDef` - *defines* how to create an `Sound` instance
- `InGameUIDef` - *defines* how to create an `InGameUI` html web component instance
- `EditorDef` - *defines* how to create an `GameProjectEditor` html web component instance

### Definition Schema:
All Defs are defined in this way
- `id` is optional, but must be UNIQUE (exception thrown if not unique; will get assigned a unique GUID if not supplied)
- `config` is mandatory
```
"<AssetType>Def": { id: "<DefName>", "config": <config> }
```

Example ActorDef:
```
ActorDef: {
  "id": "enemySlime",
  "config": {
    "Behaviors": [ { "id": "<idname>", "def": "<behaviordef>", .... }, ... ],
    "Sprites": [ { "id": "<idname>", "def": "<spritedef>", .... }, ... ],
    "Sounds": [ { "id": "<idname>", "def": "<sounddef>", .... }, ... ],
    "Actors": [ { "id": "<idname>", "def": "<actordef>", .... }, ... ],
    "x": 0,
    "y": 0,
    "<datakey>": "<datavalue>",       // many of these
  }
}
```

### When `id` is missing - Auto Assigned GUIDs

**Q:**  When the `assetDefLoader` auto-assigns an IDs with a new GUID, how do designers discover the new names so they can reference them later?
**A:**  They Dont.   `id` wasn't given, there's no expectations.


### Inline Definition Sources
An `<AssetType>Def` can originate from three places, but the loader treats them identically:
1. inline in the Asset
  - where `def` can embed a full definition object typically without id (`{ config }`).
  - Without `id` is only useful for immediate instance and ensures no other instance will try to use it.
  - With `id` will register the instance with the global registry of definitions, and potentially another Asset could reference that `id`, then. 
2. inline in the Def (reuse by all Assets in that one `world.json`)
3. on the filesystem (reuse by multiple `world.json`'s).  Incremental levels of reuse here.

Regardless of where the definition is discovered, `assetDefLoader` normalizes it (assigns GUIDs when id is missing) and registers it in the global definition registry for that type. Registration order follows file order, so referencing a def before it is encountered will still fail; designers should declare inline defs before they are consumed elsewhere.

Benefit of registering the definition, is discovery, and inventory, for tooling to be able to list out all unique definitions in the system.

### Runtime asset paths
- `world.json` relative path will be specified to `<game data="world.json"></game>`
- assets that are included from `world.json` will be relative to `world.json`, engine will parse off the path, and append to every asset included from `world.json`
- asset includes are only ever fetched once, any subsequent references in `world.json` to that asset include will refer to the first one fetched.  Caching is described in `configLoader` below.

### Standardized "config" (configLoader):

All `config` keys in the `world.json` are handled by the same common reusable `configLoader` codepath in the engine:
```
"<AssetDefType>Def": {
  "id": "<DefName>",

  // ... and then, one of the following types of "config":
  "config": "path/to/file.<js or json>",      // include-style config (configLoader checks for string matching assetpath pattern)
  "config": "java_script_goes_here()",        // inline-style config (configLoader checks for string !matching assetpath pattern)
  "config": { ... JSON data definition ... }  // json-style config (configLoader checks for object)
}
```

Example `configLoader`:
```
// resolves the definition.config from the world.json into the data needed at runtime 
function configLoader( definition_config ) {
  // include style config
  if (type definition_config == "string" && path_regex.test( definition_config )) {
    // get the codefile.js or datafile.json ready, store it back on top of definition.config
    const let code = fetch( `<urlprefix>/${definition_config}` )
    return (code && code.length > 2) ? (code.trim()[0] == '{' ? JSON.parse( code ) : eval( code )) : undefined
  }

  // inline style config
  else if (type definition_config == "string" && !path_regex.test( definition_config )) {
    // get the javascript code ready, store it back on top of definition.config
    const let code = definition_config
    return (code && code.length > 2) ? eval( code.trim() ) : undefined
  }

  // json style config
  else if (type definition_config == "object") {
    return definition_config
  }

  return undefined; // did nothing.
}
```

The reusable `configLoader` is used to resolve any `config` key inside the `world.json` project file.
```
definition.config_src = definition.config   // preserve source
definition.config = configLoader( definition.config_src )
```
This lends to asset Caching:
- asset includes are only ever fetched once, any subsequent references in `world.json` to that same asset-include will refer to the first one fetched.  `definition.config_src` is the cache.  The `configLoader` will check if `definition.config_src` exists, and if so, use it, and if not, it'll do the asset fetch. 

As illustrated above, we will generically use `config` for all Asset definitions, to allow users to perform either:
- inline `config` editing (easy iteration)
- moving inline `config` out to reusable files (e.g. to include in another game project's world.json file)


Example reusable `assetDefLoader`:
```
function assetDefLoader( type, definition ) {
  if (definition.id == undefined) definition.id = generateGUID();
  if (engine.lookupDef( type, definition.id ) != undefined) {
    // throw; // ideally we never throw, but we may want the app to STOP hard until this is resolved... maybe.
    let guid = generateGUID();
    log( `WARNING: type:${type} id:${definition.id} is not unique, changing to ${guid}` )
    definition.id = guid;
  }
  definition.config_src = definition.config   // preserve source
  definition.config = configLoader( definition.config_src )

  switch (type) {
    case "ActorDef": /* do any custom extra stuff.... */ return;
    case "BehaviorDef": /* do any custom extra stuff.... */ return;
    case "SpriteDef": /* do any custom extra stuff.... */ return;
    case "MapDef": /* do any custom extra stuff.... */ return;
    case "SoundDef": /* do any custom extra stuff.... */ return;
    case "UIDef": /* do any custom extra stuff.... */ return;
    case "EditorDef": /* do any custom extra stuff.... */ return;
  }

  // will be used by spawn( type, instance_config ) functions...
  engine.defs[type].push_back( definition )
}
```

Engine's `world.json` project file loader will index all definitions at load time
```
function loadProjectFile( file ) {
  return loadProjectData( require( file ) );
}

function loadProjectData( data ) {
  Object.keys( world_json.actorDefs ).forEach( r => assetDefLoader( "ActorDef", r ) );
  Object.keys( world_json.spriteDefs ).forEach( r => assetDefLoader( "SpriteDef", r ) );
  Object.keys( world_json.mapDefs ).forEach( r => assetDefLoader( "MapDef", r ) );
  Object.keys( world_json.soundDefs ).forEach( r => assetDefLoader( "SoundDef", r ) );
  Object.keys( world_json.uiDefs ).forEach( r => assetDefLoader( "UIDef", r ) );
  Object.keys( world_json.editorDefs ).forEach( r => assetDefLoader( "EditorDef", r ) );
}
```

After `loadProjectFile( "world.json" )`, then we can spawn Asset instances:
```
this.engine.spawn( "ActorDef", { def: "name", ...instance_config... } );
```


---
# WORLD.JSON Asset Instances
Asset instances are the Runtime objects during gameplay.
Definitions serve as templates (prototypes) that can be used to spawn instances.
Asset instances conceptually are instantiations of Defs.

`Actor` assets are the unit of "game object", which holds what you see and hear, and contain active intelligence logic.
- Assets under the Actor, must be named uniquely per Asset type
- Asset siblings (of the same type) must always be uniquely named.

We'll never use the word “instance” in our type names, for brevity / readability. 

### Asset Instance Types:

- `Actor` - spawns an `Actor`, from the given Def
- `Behavior` - spawns an `Behavior` for an `Actor`, from the given Def
- `Sprite` - spawns a `Sprite`, from the given Def
- `Map` - spawns a `Map`, from the given Def
- `Sound` - spawns a `Sound`, from the given Def
- `InGameUI` - spawns an `InGameUI` html web component, from the given Def
- `GameProjectEditor` - spawns a `GameProjectEditor` html web component, from the given Def

An example def & instance relationship where `Asset` is one of the above types:
```
AssetDef: {
  "id": "enemySlime",
  "config": <configdata>
}

Asset:
{
  "id": "slime004",
  "def": "enemySlime",               // or an inline AssetDef object, see below
  "deftype",                         // automatically set when instantiated, to the AssetDef
  "x": 120,
  "y": 200,
  "<datakey>": "<datavalue>"         // override any of the base ones
}
```

The def field accepts either the AssetDef's `id` identifier, or, an inline definition object for the AssetDef:
```
Asset:
{
  "id": "slime004",
  "def": {
    "id": "enemySlime-Alt",
    "config": { ...full ActorDef config... }
  }
}
```
When an inline object is provided, the engine runs it through the same `assetDefLoader`, registers it globally (assigning a GUID if id is omitted), and then instantiates the resulting `def`. Other assets can reference that new `id` later as long as they appear *after* this inline `def` in `world.json`.



---
# ASSET TYPE: ACTOR
Actors are the primary runtime entities, they are the runtime game elements.
Actors contain a collection of several Assets.

Schema
```
// Actor Definition, template used for spawning new Actor instances
ActorDef: {
  "id": "<definition name>",
  "config": {
    "Behaviors": [ { "id": "<idname>", "def": "<behaviordef>", .... }, ... ],
    "Sprites": [ { "id": "<idname>", "def": "<spritedef>", .... }, ... ],
    "Sounds": [ { "id": "<idname>", "def": "<sounddef>", .... }, ... ],
    "Actors": [ { "id": "<idname>", "def": "<actordef>", .... }, ... ],
    "x": 0,
    "y": 0,
    "<datakey>": "<datavalue>",       // many of these
  }
}

// Actor instance config
Actor: {
  id,         // unique instance name
  def,        // ActorDef name
  deftype,    // automatically set when instantiated, to the AssetDef
  parent,     // automatically populated by engine based on hierarchy in world.json
  x,          // x position, relative to parent. 2D sprites dont have rotation
  y,          // y position, relative to parent. 2D sprites dont have rotation
  "<datakey>": "<datavalue>",       // many of these
}

// Each Actor may attach Actor instances:
"actors": {
    "enemy": { def: "enemy", ...Actor instance config goes here... },
    ...
}

// or from javascript:
actor.actors["enemy"] = new Actor( def )
actor.actors["enemy"] = engine.spawn( "ActorDef", { def: "enemy", ...instance_config... } ) // from "enemy" def
```

Actors have children.
Asset Positions are relative to parent Actor

### Asset Transforms:

`Actor`s have children.  Asset Positions are relative to parent `Actor`, all Assets have `getPos()` which does that math, using parent.  `getLocalPos`, `setLocalPos` also available.

```
function getPos() {
  return add( [x,y], parent.getPos() )
}
```

### Root Actor defines the Game World:

`world.json` has 
```
{
  actor: {
    id: 'world',                  // instance name
    def: 'world',                 // uses this definition
    x, y,                         // position
    actors: [ ... ],              // child actors go here ...
    "<datakey>":  "<datavalue>",  // additional data fields go here ...
  }
}
```

Behavior logic (javascript) can access the root 'world' actor using:
```
this.engine.actor
```

### Spawning Actors
inside of Behavior logic we can spawn a new actor:
```
// spawn an enemy into the world
this.engine.spawn( "ActorDef", { def: "creature", parent: this.engine.actor, pos: this.getPos() } );

// spawn an object local to self
this.engine.spawn( "ActorDef", { def: "sword", parent: this.parent, pos: [0,20] } );
```

### Child Actors
Both `ActorDef` and `Actor` instances can have child `Actor`s
- `ActorDef` specifies the defaults (e.g. 'car' `ActorDef` with 4 child 'wheel' `Actor`s)
- `Actor` instance specifies customization (e.g. 'car' `Actor` with a monster `Actor` in the driver position)
  - add to the child `actors` list from the base `ActorDef`



---
# ASSET TYPE: Behavior
Behaviors give Actors intelligence, allowing the game designer to add logic to actors.
Each Behavior has 1..N trigger handlers, each which runs code.

Schema
```
// Behavior Definition, template used for spawning new Behavior instances
BehaviorDef: {
  "id": "<definition name>",
  "config": {
    "<datakey>": "<datavalue>",       // many of these
    "triggers": {
      "init": { "config": <configloader> },               // inline js or included js
      "update": { "config": <configloader> },             // inline js or included js
      "<other triggers>": { "config": <configloader> },   // inline js or included js
    }
  }
}

// Behavior instance config
Behavior: {
  id,         // unique instance name
  def         // definition id to clone
  deftype,    // automatically set when instantiated, to the AssetDef
  parent,     // automatically populated by engine based on hierarchy in world.json
  x,          // x position, relative to parent.
  y,          // y position, relative to parent.
  "<datakey>": "<datavalue>",       // many of these
}

// Each Actor may attach Behavior instances:
"behaviors": {
    "wanderAI",                             // BehaviorDef 'id' alone, instance a "wanderAI" BehaviorDef.
    "inv": { def: "inventory", "max": 20 }, // Behavior instance of "inventory" BehaviorDef, customizable here.
    ...
}

// or from javascript:
actor.behaviors["inv"] = new Behavior( def )
actor.behaviors["wanderAI"] = engine.spawn( "BehaviorDef", { def: "wanderAI", ...instance_config... } ) // from "wanderAI" def
```

Behavior trigger js code may freely use the provided standard library, including but not limited to:
```
this.parent         // the Behavior's Actor
this.engine.actor   // the root "world" Actor
this.getPos         // position of my Actor
this.getLocalPos    // local position of my Actor
this.setLocalPos    // modify the local position of my Actor
this[key] = value;
if (this[key)) { /* do things */ }
```

### JS scope
Scope is the entire engine, your behavior can ruin things.
Use this to limit your behavior code's actions to only the data exposed to your behavior

WARNING: be careful!  you can footgun yourself here (ow!).

### Triggers
See Behavior schema above for defining custom triggers.
See Trigger section for engine generated triggers that you should expect to emit into your Behaviors.


---
# ASSET TYPE: Sprite
A Sprite asset defines a 2D pixel map that draws into the Game Viewport.
The sprite is defined by
- it's image which contains frames to draw
- configuration data (see Schema for details)

Schema
```
// Sprite Definition, template used for spawning new Sprite instances
SpriteDef: {
  "id": "<definition name>",
  "config": {
    image                                      // relative path to the image
                                               // image is grid of frames: horz is animation, vert is states
    frameWidth                                 // width of sprite (single frame)
    frameHeight                                // height of sprite (single frame)
    states: { walk:1, idle:0, attack:2 }       // name some states 0..n, by horz line in the image
    frameRateHz                                // framerate to flip frames in the image
    direction: "fwd", "rev", "rand",           // direction to iterate through frames
             "fwd_once", "rev_once",           // direction to iterate through frames
    nocollide_colors: ["#000000", "#ff00ff"]  // collision engine will ignore these pixels
    boundingBox: {x1,y1,x2,y2}                 // collision engine will only consider this bounding box
  }
}

// Sprite instance config
Sprite: {
  "id": "<instance unique name>",
  "def": "<definition id to clone>",
  "deftype",                  // automatically set when instantiated, to the AssetDef
  "parent",                   // automatically populated by engine based on hierarchy in world.json
}


// Each Actor may attach Sprite instances:
"sprites": {
    "goon": { def: "goon", ...Sprite instance config goes here... },
    ...
}

// or from javascript:
actor.sprites["goon"] = new Sprite( def )
actor.sprites["goon"] = engine.spawn( "SpriteDef", { def: "goon", ...instance_config...} ) // from "goon" def
```

### Triggers supported
```
play, { state: <state>, dir: <dir> }
stop
setState, <state>
```


---
# ASSET TYPE: Map
Runtime 2D pixel tile based world map:
* Rendering 2D map tiles
* Tile collision queries
* Chunk-based enable or culling


Schema
```
// Map Definition, template used for spawning new Map instances
MapDef: {
  "id": "<definition name>",
  "config": {
    chunks: [{
      id,                                           // name of the chunk
      enable,                                       // true or false
      x,y,                                          // position offset (relative to parent)
      tileImage,                                    // relative path to the image with grid of map tiles
      tileWidth                                     // width of a single tile
      tileHeight                                    // height of a single tile
      tileMap                                       // array of tiles
      tileMapWidth,                                 // how many tiles horizontally in the tileMap
      tileMapHeight,                                // how many tiles vertically in the tileMap
      nocollide_colors: ["#000000", "#ff00ff"]  // collision engine will ignore these pixels
      bounding_box,                                 // automatically computed
    }, ... ]
  }
}

// Map instance config
Map: {
  "id": "<instance unique name>",
  "def": "<definition id to clone>",
  "deftype",                  // automatically set when instantiated, to the AssetDef
  "parent",                   // automatically populated by engine based on hierarchy in world.json
  x,y,                        // position offset (relative to parent)
}

// Each actor may attach Map instances:
"maps": {
  "world": { def: "world", ...Map instance config goes here... }
}

// or from javascript:
actor.maps["world"] = new Map(def)
actor.maps["world"] = engine.spawn( "MapDef", { def: "world", ...instance_config... } ) // from "world" def
```

### Triggers supported
```
// turn on or off portions of the map.  could be used for auto-culling too
enableChunk, { id: "<id>", enable: <boolean> }

// change map tiles at runtime, reveal a hole in the wall, hidden staircase, create a maproom, etc...
setTile, { tilex, tiley, tileid }
```

---
# ASSET TYPE: Sound
A Sound asset defines a sound that emits from the user's speakers.
The sound is defined by
- its audio file which contains the audio to play
- configuration data (see Schema for details)

Schema
```
// Sound Definition, template used for spawning new Sound instances
SoundDef: {
  "id": "<definition name>",
  "config": {
    audio,                                     // asset path to the audio file
                                               // image is grid of frames: horz is animation, vert is states
    spatial,                                   // true: distance-attenuated, false: ambience, music
    x,y,                                       // position of the sound
    size,                                      // full volume: within size;  attenuated: within size + falloff
    falloff_size,                              // sound gets quieter as move away, distance
    falloff_type,                              // "linear", "exponential" falloff
    loop,                                      // loop the sound, true/false
    voices,                                    // round robin pool of voices, from 0..n
    gain,                                      // how loud, from 0 to 1
  }
}

// Sound instance config
Sound: {
  "id": "<instance unique name>",
  "def": "<definition id to clone>",
  "deftype",                  // automatically set when instantiated, to the AssetDef
  "parent",                   // automatically populated by engine based on hierarchy in world.json
  x,y,                        // position of the sound
}

// Each actor may attach sound instances:
"sounds": {
  "coin": { def: "coin", ...Sound instance config goes here... }
}

// or from javascript:
actor.sounds["coin"] = new Sound(def)
actor.sounds["coin"] = engine.spawn( "SoundDef", { def: "coin", { ...instance_config...} ) // from "coin" def
```

### Triggers supported
```
play              // play the sound
stop              // stop all voices (within the sound node)
```

### Sound Behavior:
* 2D Distance Attentuation:
  * Spatial attenuation based on ListenerBehavior which updates engine.listenerPosition
  * Ambient sounds bypass attenuation
* 2D Stereo Panning
  * Stereo L/R speaker pan/positioning based on ListenerBehavior which updates engine.listenerDirection
* Voice Pooling enables overlapping playback (in quick succession)

### 2D Distance Attenuation of the Sound's Volume
```
distance = the Listener's distance from the sound node

distance == 0:                    100% volume  // inside the sound node, full volume
distance == size/2:               100% volume  // inside the sound node, full volume
distance == size:                 100% volume  // inside the sound node, full volume
distance == size+falloff_size/2:   50% volume  // volume from 1..0, using [falloff] curve, as move away
distance == size+falloff_size:      0% volume  // volume from 1..0, using [falloff] curve, as move away
distance == size+falloff_size+N:    0% volume  // can't hear the sound node
```

### ListenerBehavior
There will be a Listener `Behavior` called `ListenerBehavior`, which contains this code:
```
// update the listenerPosition
engine.listenerPosition = this.engine.actor.get( "#PlayerCharacter" ).getWorldPos()
engine.listenerDirection = this.engine.actor.get( "#PlayerCharacter" ).getDirection()

// built in, called inside the engine...
this.engine.updateSoundAttenuations( this.engine.actor, this.engine.listenerPosition );
```

---
# ASSET TYPE: UI
This is a HTML web component definition, which defines the `InGameUI` web component.
The engine will create:
```
<game></game>
```
And that `<game>` will create `<InGameUI>` into it's DOM.

Schema
```
// UI Definition, template used for spawning new UI instances (web-component)
"UIDef": { "id": "ingameui", "config": { "include": "ingameui.js" } }

// Instance `InGameUI` is created from that `ingameui` definition:
// there is only one instance, at the top-level of `world.json`
"ui": { "def": "ingameui" }
```

Runtime (utilized by `<game>`):
```
<InGameUI></InGameUI>
```

`<InGameUI>` receives from it's parent `<game>`:
```
engine
engine.actor  // the root Actor which defines the game 'world'
```

`InGameUI` emits triggers into the root game world in `engine.actor`

- see The BuiltInEditor section for details around the `WebComponent Code Editor` which can be used to edit or create an `ingameui.js` file needed to define `InGameUI`

---
# ASSET TYPE: Editor
This is a HTML web component definition, which defines the `GameProjectEditor` web component.

world.json Schema
```
"editorDefs": {
  { "id": "editor", "config": { "include": "editor.js" } }
}
```

- see The BuiltInEditor section, for details on the default/supplied `editor.js` (which defines `GameProjectEditor`) that people may use in their `world.json`.

---
# The BuiltInEditor - editor-backend.js
The engine comes with a `editor-backend.js` which runs a service for editing game projects.
The game designer should run with:
```
> node editor-backend.js
```
or
```
> npm start
```

The `editor-backend.js` when `https://localhost:8080` is loaded, displays a page with top-level tabs:
- defines the root level `<BuiltInEditor>` html web component
- several child web components under that
  - Each **tab** is a web component
- Three Tabs:
  - **Asset Mgmt & Preview**
    - initializes with `Asset Mgmt & Preview` tab focused
  - **WebComponent Code Editor**
    - always available
  - **GameProjectEditor**
    - the `GameProjectEditor` tab is hidden (greyed out) until there is an active `world.json` selected
- Asset Mgmt & Preview
  - A page for
    - Viewing the asset filesystem, asset's versioning history, plus controls for create/delete/move of asset files, plus version mgmt (delete, create new version from an older version, etc.)
    - Preview for selected asset
  - 2 views:
    - Preview Pane:
      - Display whatever is clicked on in the asset view, create the instance of that asset, and view there
        - `world.json` shows a game preview, with a [PLAY] button
          - Clicking "PLAY" reloads `world.json` into the runtime engine's `<game></game>` web component
        - png/jpeg/jpg/gif/bmp shows an image
        - js files shows a code editor (view only)
    - Data Pane:
      - hierarchical listview - assets
        - entire asset filesystem (files and folders) as a tree
        - on select, changes "version" view to display all the versions for that file
        - on select, changes "Editor" view to display an editor for that file (if available)
        - on drag of a [file or folder] to a folder, moves to that folder
        - button "new dir" creates a new directory
        - button "load world"
          - makes the selected `world.json` the "active" for the entire editor
          - makes a new top level sibling tab "Editor" appear containing the `<GameProjectEditor></GameProjectEditor>` from `world.json`
        - button "clone" makes a `<filename>.ext` clone `<filename>-cloned.ext`
      - listview - versions
        - for each item in the tree, when selected, changes the "versions" box to contain a list of all versions for that asset
        - on select, able to "create new version" which adds a new version
        - on select, able to "delete" a version, which asks if you're sure, and then renumbers them
      - Selected Editor
        - displays whatever asset was selected, allowing edit if possible
          - `world.json` typically
            - `InGameUI` field which populates `InGameUIDef` and `InGameUI` in `world.json`
            - `GameProjectEditor` field which populates `EditorDef` and `GameProjectEditor` in `world.json`
            - Save Button to write `world.json`
            - Buttons to jump to the relevant tab:  Actors, Behaviors, Sprites, Maps, Sounds, UI, Editor
- WebComponent Code Editor (for editing files like: `ingameui.js`, `editor.js` and new variants of those)
  - A page for writing new `<InGameUI>` and `<GameProjectEditor>` web components
    - presents a color syntax highlighting code editor, and an editor preview pane with controls to browse assets, load and save.
  - by default
    - no `world.json` active:  displays default `editor.js` from the asset data endpoint
    - `world.json` active:  displays `GameProjectEditor` from the `world.json`
  - 2 panes:
    - 1 big pane with 2 sub tabs:
      - Code Editor for the selected `EditorDef` or `InGameUIDef`
        - A HTML web component that implements the `<JavascriptCodeEditor>` HTML element
      - preview pane (shows the active `<GameProjectEditor></GameProjectEditor>` or `<InGameUI></InGameUI>` being edited)
        - displays the `InGameUI` as defined by `InGameUIDef` in the active `world.json`, as the `<InGameUI>` html element
        - displays the `GameProjectEditor` as defined by `EditorDef` in the active `world.json`, as the `<GameProjectEditor>` html element
        - if eval() of the code fails, nothing will preview until eval() passes (show the error instead of preview)
    - data panel
      - listview with list of all `EditorDef`s in `world.json` (if one is active)
        - selected `EditorDef` resets the preview pane, and the Code Editor to that `EditorDef`
        - New:    Button to creates a new `EditorDef`
        - Delete: Button to delete selected `EditorDef`
        - Use:    Button to make the currect selected `EditorDef` active as `GameProjectEditor` in `world.json`
      - listview with list of all `InGameUIDef`s in `world.json` (if one is active)
        - selected `InGameUIDef` resets the preview pane, and the Code Editor to that `InGameUIDef`
        - New:    Button to creates a new `InGameUIDef`
        - Delete: Button to delete selected `InGameUIDef`
        - Use:    Button to make the currect selected `InGameUIDef` active as `InGameUI` in `world.json`
      - Edit Button to jump to my Code Editor tab
      - Preview Button to jump to my preview pane tab
      - Save Button to write `world.json` and the asset
        - inline: into world.json
        - include: into asset system
- GameProjectEditor (active (not greyed out) when there's an active `world.json`)
  - A page with a single pane for editor or game-preview, `[new project]`, `[load project]`, `[save project]`, `[Edit Mode]` or `[Game Mode]` buttons
  - upon Project Load, the editor.js is read from the project, and `<GameProjectEditor></GameProjectEditor>` is then added to the page.
  - `[Edit Mode]` or `[Game Mode]` buttons toggle between `<game data="world.json"></game>` or `<GameProjectEditor data="{ entire world json object }"></GameProjectEditor>` in the page.
  - will display the `world.json`'s `GameProjectEditor` field's `editor.js` web component `<GameProjectEditor></GameProjectEditor>`

Reusable Widget Library
- reusable library of web components
- any tab could utilize these reusable components
- Web Components in the Reusable Widget Library:
  - Code Editor (reusable web component, for use in the WebComponent Code Editor)
    - Reusable HTML web component
    - `<code-editor></code-editor>` configure with the [inline: world.json + json path to the inline code] or [include: asset path]
    - Save, Undo, Redo buttons
    - color syntax highlighting for javascript and json only (one of the few dependencies we will bring in)
    - ctrl-z and shift-ctrl-z keybindings for undo/redo
    - ctrl-s for keybinding for save
    - knows how to save:
      - inline: `world.json/<json path to the inline code>`  (updates `world.json`)
      - include: `<asset path>`  (updates via PUT to the `save/<asset path>` data endpoint)
  - see `editor.js` description below for more of these components......

### The included editor.js (implements GameProjectEditor)
The engine comes with an `editor.js` `GameProjectEditor` component.
A user may create new `GameProjectEditor`s (e.g. `myeditor.js`) using
- the `editor-backend.js`'s built in "WebComponent Code Editor" (to get asset management w/ version control)
or
- using their favorite code editor to directly edit on their own local filesystem (without asset mgmt).

### Structure inside the included `editor.js`
- defines the root level `<GameProjectEditor>` html web component
- initializes with "Asset Mgmt & Preview" tab when there is no active `world.json` selected, all other tabs are hidden until then.
- several child web components under that
  - Each **tab** is a web component
  - Each **pane** is a web component
  - Each **preview** type (that we may like to view in a "preview pane") is a web component  (`Actor`, `Behavior`, `Sprite`, `Map`, `Sound`, `InGameUI`, `GameProjectEditor`, `Code`, `Image`, `Game`, `ActorPlacement`)
  - Reusable Widget Library components:
    - Each widget in a "data pane" is a web component, for example:
      1. **Data Editors**
        - For editing `AssetDef` and `Asset` instance types (where Asset == `Actor`, `Behavior`, `Sprite`, `Map`, `Sound`, `InGameUI`, `GameProjectEditor`)
        - ideally there is one `Asset` Editor web-component only, and can "drive it with data" to instantiate an editor for each type... (where Asset == `Actor`, `Behavior`, `Sprite`, `Map`, `Sound`, `InGameUI`, `GameProjectEditor`)
      2. **listviews**  (for flat lists)
        - the data is configurable via HTML element parameter data="" & also web-component API method call setData(...)
        - with `onchange="myfunc(e)"` HTML element parameter for detecting changes
        - with callback for detecting selection changes (using a web-component API method addListener(...))
      3. **hierarchical listview**  (for hierachical lists)
        - the data is configurable via HTML element parameter data="" & also web-component API method call setData(...)
        - with `onchange="myfunc(e)"` HTML element parameter for detecting changes
        - with callback for detecting selection changes (using a web-component API method addListener(...))
    - **Code Editor** is a web component
      - the data is configurable via HTML element parameter data="" & also web-component API method call setData(...)
      - with `onchange="myfunc(e)"` HTML element parameter for detecting changes
      - with callback for detecting changes (using a web-component API method addListener(...))
- Structure under `<GameProjectEditor>`
  - Tabs:
    - Actor Definition Editor
    - Behavior Definition Editor
    - Sprite Definition Editor
    - Map Definition Editor
    - Sound Definition Editor
    - Asset Mgmt & Preview
- Actor Definition Editor
  - 2 panes (left and right):
    - preview pane (2d canvas) - left
      - represents an `Actor` instance spawned from selected `ActorDef`
      - Controls
        - behavior trigger buttons (one per named trigger in the child behaviors)
    - data pane - right
      - listview with list of all `ActorDef`s in `world.json`
      - `ActorDef` editor (datadriven by the base `ActorDef` config structure), for example:
        - Add/Remove to/from child `AssetDef` lists (`actors`, `behaviors`, `sprites`, `maps`, `sounds`), by picking from relevant `AssetDef` list
        - Edit behavior custom data fields on the Actor, exposed by the child `BehaviorDef` definition(s) (e.g. health, speed, faction, flags, etc.)
        - Editing of each `AssetDef` is auto-driven by it's respective `AssetDef` definition.
  - Saves to `world.json` definitions
- Behavior Definition Editor
  - 2 panes (left and right):
    - preview pane (2d canvas)
      - represents an `Actor` instance spawned from selected `ActorDef`
      - Controls
        - play and stop buttons, trigger buttons (one per named trigger in the Actor's Behaviors)
    - data pane
      - listview with list of all `ActorDef`s in `world.json`
      - `ActorDef` editor (datadriven by the base `ActorDef` config structure)
      - `BehaviorDef` editor (datadriven by the base `BehaviorDef` config structure), for example:
        - shows the top-level structure of a `BehaviorDef`
  - Saves to `world.json` definitions
  - Reuse the web-component `<JavascriptCodeEditor>` code editor for JavaScript
  - Edit the BehaviorDef fields, for example:
    - Create triggers, edit code for the trigger
- Sprite Definition Editor
  - 2 panes (left and right):
    - preview pane (2d canvas)
      - represents a `Sprite` instance spawned from selected `SpriteDef`
      - Controls
        - play and stop buttons, state trigger buttons (one per named state)
    - data pane
      - listview with list of all `SoundDef`s in `world.json`
      - `SpriteDef` editor (datadriven by the base `SpriteDef` config structure), for example:
        - Upload new sprite image to assets backend
        - Select a sprite image from assets backend, and set it into the `SpriteDef`
        - all other fields from the the base `SpriteDef` config structure
  - Saves to `world.json` definitions
- Map Definition Editor
  - 2 panes (left and right):
    - preview pane (2d canvas)
      - represents a `Map` instance spawned from selected `MapDef`
      - Controls
        - LeftMouseClick to Paint tileIDs onto the MapTiles
        - RightMouseClick to pan the map
        - MouseScroll to zoom the map
    - data pane
      - listview with list of all `MapDef`s in `world.json`
      - `MapDef` editor (datadriven by the base `MapDef` config structure), for example:
        - Upload new map tile image to assets backend
        - Select a map tile image from assets backend, and set it into the `MapDef`
        - Create or Delete chunks
        - Edit chunk:
          - Visually select a tileID from the MapImage, then Paint tileIDs onto the MapTiles 
          - Edit chunk x,y position
        - all other fields from the the base `MapDef` config structure
  - Saves to `world.json` definitions
- Sound Definition Editor
  - 2 panes (left and right):
    - preview pane (audio icon)
      - represents a `Sound` instance spawned from selected `SoundDef`
      - Controls
        - play and stop buttons
    - data pane
      - listview with list of all `SoundDef`s in `world.json`
      - `SoundDef` editor (datadriven by the base `SoundDef` config structure), for example:
        - Upload new sound file to assets backend
        - Select a sound file from assets backend, and set it into the `SoundDef`
        - all other fields from the the base `SoundDef` config structure
  - Saves to `world.json` definitions
- Actor placement Editor
  - 2 panes (left and right):
    - preview pane (2D canvas)
      - represents `Actor`s instances overlaid over a `Map` instance
      - Controls:
        - Move Map:  RightMouseClick on map pans the map
        - Create Actor: LeftMouseClick to create Actor (with ActorDef listview's selection)
        - Edit Actor: LeftMouseClick an Actor to edit that Actor and auto-scroll in the Actor Graph
        - Delete Actor: Press "delete" or "backspace" while Actor is selected to remove
        - Move Actor: LeftMouseClick on actor and drag with mouse to move the actor, ctrl-x/c/v works to cut/copy/paste, paste goes to center of the map
    - data pane
      - listview - All `ActorDef`s
        - Visually see the `world.json` list of `ActorDefs`
        - Workflows
          - select one, then click the preview pane to place
          - select one, which populates the Editor, then use Editor to edit.
      - hierarchical listview - Actor Graph (instances) 
        - Visually see the `world.json` hierarchical `Actor` graph (from the root)
        - Workflows:
          - Drag `Actor`s onto other `Actor`s to affect parent/child relationship
          - listview elements support Collapse / Expand
          - select one, which populates the Editor, then use Editor to edit.
      - Selected Editor
        - Shows an editor for the active `Actor` instance or `ActorDef` selection, whichever was last selected
      - Save button writes a new `world.json` to assetmanagement filesystem
  - Saves to `world.json`
    - expected to replace entire actor hierarchy (new child relationships, new x,y positions, new/removed Actors)
- UI Definition Editor
  - 2 panes (left and right):
    - 1 big pane with 2 sub tabs:
      - Code Editor for the selected `InGameUIDef`
      - preview pane (shows the active `<InGameUI></InGameUI>` being edited)
        - displays the `InGameUI` as defined by `InGameUIDef` in the active `world.json`, as the `<InGameUI>` html element
        - Controls below the `<InGameUI></InGameUI>`:
          - behavior trigger buttons (one per named trigger in the child behaviors)
    - data panel
      - listview with list of all `InGameUIDef`s in `world.json`
        - selected `InGameUIDef` resets the preview pane, and the Code Editor to that `InGameUIDef`
        - New:    Button to creates a new `InGameUIDef`
        - Delete: Button to delete selected `InGameUIDef`
        - Use:    Button to make the currect selected `InGameUIDef` active as `InGameUI` in `world.json`
      - Edit Button to jump to my Code Editor tab
      - Preview Button to jump to my preview pane tab
      - Save Button to write `world.json`
  - Saves to `world.json` definitions
- World Editor (world Actor Placement and Editing)
  - **Purpose:** A unified workspace for laying out the `Actor` hierarchy defined under `world.json` root `actor`.
  - 2 Panes (left and right):
    - **Viewport pane (left)**:
      - `WorldViewport`: a black viewport renders the static (initialization-time) game world using the engine runtime.  (`<WorldViewport>` as a reusable web component)
      - pan: right-drag; zoom: mousewheel/pinch.
      - play button below the Viewport replaces the viewport with a `<game>` for play testing
      - stop button below the Viewport replaces the `<game>` with the world editing viewport
    - **Data pane (right)**: visually stacked editors that react to the current selection.
      - Default Top section:
        - **ActorDef list**:  listview of all `ActorDef`s in the active `world.json`, showing all `<AssetType>Def` lists, with search/ filter
        - **Actor Graph**: hierarchical listview of all `Actor` instances in `world.json` root `actor`-graph, with search/filter.
        - drag and drop is used to move inline `def` from the instance to the `world.json` `<AssetType>Def` list, and vice-versa.
          - move from the `def` Asset instance:  replaces that Asset instance's `def` with the `id`; if no `id` existed yet, generate a GUID, and use that.
          - move into the `def` Asset instance:  replaces that Asset instance's inline `def` using the item's data from the `<AssetType>Def` list.
        - edit of an AssetDef's `id` updates all Asset instance `def` references (in the actor-graph) to that new `id` name.
        - resize:  can drag-expand the bottom border of each editor-widget to be taller (vert resize as a reusable web component that every data pane widget can use)
      - **Below**: contextual editors that appear as you select-drill into instances/defs/fields; each editor can spawn child editors beneath it (accordion style).
        - TODO: in code, we have a decision whether to add as a child or as a sibling.   child may be easier...
        - resize:  can drag-expand the bottom border of each editor-widget to be taller (vert resize as a reusable web component that every data pane widget can use)
    - **...with a reactive layout**:
      - panes fit the browser reactively, if too narrow (e.g. on mobile) then left pane is on top, and right pane is on bottom.
      - divider between the panes is positionable, which can resize both panes at once.
  - New Placement workflow:
    1. Click explicit mode toggle “Place Actor” button. (found in the floating pills)
    2. Optionally toggle “snap to parent” button. (found in the floating pills), if unchecked, new actors always attach under the root `Actor` regardless of what’s selected, which prevents accidental deep nesting.
    3. Select an `ActorDef` in the right-hand datapane listview.
    4. Optionally select an `Actor` instance in the right-hand datapane hierarchical listview. (if selected, new placed `Actor` will become a child under this `Actor`)
    5. Clicking the `WorldViewport`, receives the selected data; and x,y world position clicked on (mouse x,y position is translated into "game world" x,y)
    6. A new `Actor` instance data (derived from the selected `ActorDef` data) is appended to `world.json` under the selected `actor`'s children (or under root if no `Actor` selection), setting the x,y data for that `Actor` instance.
    7. The new instance appears immediately; undo/redo integrates with the editor’s history stack.
    8. After a placement, stay in placement mode but show a floating “Done” pill so designers can exit quickly (handy on mobile where hitting ESC isn’t an option).
  - Copy/Paste Placement workflow:
    1. Select an `Actor` instance in either a. the right-hand datapane hierarchical listview, or, b. from the `WorldViewport`.
    2. Press ctrl-c to copy, or long-press on the `Actor` to copy, or use on-screen copy/paste buttons. (found in the floating pills)
    3. Upon paste (ctrl-v, or paste button pill), set mode status "place actor".  Show a ghost actor following the cursor/finger. The next tap drops it and sets the world x/y.  cancel the mode status "place actor" with escape or "x" on the status
      - the `WorldViewport` receives the copied data; and x,y world position of the mouse cursor or tap location
      - mouse/tap x,y position is translated into "game world" x,y
    4. A new `Actor` instance data (derived from the selected `ActorDef` data) is appended to `world.json` as a sibling of the selected `Actor` instance, setting the x,y data for that `Actor` instance offset in x by the bounding box width (they might stack up on top of each other, that's ok).
    5. The new instance appears immediately; undo/redo integrates with the editor’s history stack.
  - Selection workflow:
    1. Clear the ActorDef list selection (or press Esc) to leave placement mode.
    2. Click any Actor in the viewport to select/focus it. The selection outline highlights in-scene.
    3. Data pane updates with:
      - In the actor-graph hierachical listview, shows the focused `Actor` highlighted and scrolled into view.
      - Below that we've added an editor card to represent the selected `Actor` instance (shows def, position, custom data, child asset references).
      - Upon selection of sub features (def, child assets, data fields) of that `Actor` instance, then, more Nested editors will appear under that card, for those linked assets (Sprite instances, Behavior instances, etc.). Clicking a child drills deeper, pushing another editor card under the parent.
      - Links back to source `ActorDef` so designers can jump between instance tweaks and definition edits.
  - Editing model:
    - Each editor widget edits JSON data only; it doesn’t execute code directly.
    - The widget is given the JSON pointer (`world.json/<path>`) so saves route to the correct spot (inline instance, inline def, or include file).
    - Undo/redo spans all widgets so “drill-down edits” roll back cohesively.
  - Generic `<asset-editor>`:
    - Provide one reusable `<asset-editor>` component that accepts:
      - data: the current JSON payload.
      - schema: field definitions (labels, types, validation, nested repeaters). Each asset type (Actor, Sprite, Map, Sound, Behavior, UIDef, EditorDef) supplies its schema via a registry so the UI   - adapts automatically.
      - onChange callback that emits the new JSON plus metadata indicating whether the payload should be written inline or to an include file.
    - Specialized editors (e.g., sprite image picker, tilemap painter) plug into slots defined by the schema so high-fidelity tools coexist with the generic form.
    - Benefits: single code path for validation, consistent UX, easier maintenance as new asset types arrive.
  - Here’s the full set of UI/web components the spec now implies, grouped by role. Everything lives under the World Editor umbrella so they can be re-used in other tabs.
    - Core World Editor Shell
      - `world-editor`: owns the tab, orchestrates viewport/data pane, undo/redo.
      - `world-viewport`: renders runtime scene, handles pan/zoom, placement ghost, selection outline.
      - `actor-pills-toolbar`: floating controls (Place Actor, Snap to Parent, copy/paste buttons, “Done” chip, status indicator).
      - `actor-hierarchy-list`: tree view of the current world.json.actor hierarchy (select, expand/collapse, drag/drop reorder).
      - `actordef-list`: flat list of all ActorDefs; supports search/filter.
      - `editor-card-stack`: container that shows stacked contextual editors as you drill down (push/pop cards).
    - Generic Editing Infrastructure
      - `<asset-editor>` (generic data editing form). Which auto-configures based on schemas for the following:
        - `ActorDef`
        - `Actor` instance
        - `BehaviorDef`
        - `Behavior` instance (instance overrides, data fields)
        - `SpriteDef`
        - `Sprite` instance
        - `MapDef`
        - `Map` instance (chunk positions, enable flags)
        - `SoundDef`
        - `Sound` instance
        - `InGameUIDef`
        - `InGameUI` instance
        - `GameProjectEditorDef`
        - `GameProjectEditor` instance
      - Each schema should describe
        - primitive fields (bool, number (with min/max), string, options list, key/value pair(s))
        - child Asset instance lists (`Behaviors`, `Sprites`, etc.)
        - optionally if a special editor is needed upon click, e.g.
          - `Behavior` `triggers` editor (key/value pairs), where clicking the trigger key's value pops up a `javascript-code-editor`, which edits the string field for that trigger key's value.
          - `Map`'s `maptile` editor (string type), clicking the value pops up a
          - `Sound` file (string type), clicking raises a `file-requestor` with (`wav`|`m4a`|`mp3`) extension filter
      - `<asset-editor>` provides a callback hook on change, so that changes get propagated to the right location:
        - in the asset vs in the definitions
        - inline vs assetfilesys
    - Specialized / One-off Editors
      - `file-requestor`: pops up a dialog with the file, certain types will have a preview available (audio or image).
        - configure with extensions to filter from the backend `<file-requestor exts="['mp3', 'm4a', 'wav']">`
      - `javascript-code-editor`: existing component used for Behavior, UI, and Editor defs (syntax highlighting, save hooks).
      - `sprite-state-preview`: canvas preview with play/stop controls, frame/state selector.
      - `sprite-image-picker`: chooser for sprite sheets from the asset backend, shows thumbnails, enforces frame dimensions.
      - `map-chunk-editor`: provides tilemap painting, chunk creation/deletion, per-chunk transform editing.
      - `tilepicker-panel:` palette of tiles taken from the map’s tileset image (works inside the chunk editor).
      - `sound-preview-panel`: play/stop buttons, waveform visualization, gain/falloff sliders tied to SoundDef.
      - `asset-reference-picker`: generic dialog for selecting existing defs/instances when wiring child assets (used inside `Actor` editor for choosing child `BehaviorDef`/`SpriteDef` references).
      - `inline-include-toggle`: small widget that lets designers flip between inline JSON and include file storage for a particular sub-object.
      - Utility/Support Components
        - `selection-overlay:` draws outlines/bounding boxes for the currently selected Actor(s) in the viewport.
        - `ghost-actor-preview`: appears during placement/copy workflows; follows pointer and snaps to grid when needed.
        - `undo-redo-controls`: toolbar or keyboard handler integrated with the world editor history stack.
        - `status-toast` / `guidance-banner`: to notify users (e.g., “Placement mode active”, “Snap-to-parent off”).
        - `asset-schema-registry`: not a visual component but a module providing schema definitions to `<asset-editor>` so it can render the correct controls per asset type.



---
# ACTOR QUERY

Similar to HTML DOM query selector, our runtime `engine`'s root actor-graph also has a query API:
```
this.engine.actor.get( str )
```
- if `str` matches pattern "#InstanceID" returns asset instance with `id: InstanceID` (0..1)
- if `str` matches pattern "AssetDef[DefName]" returns asset instances with `def: DefName` (0..N)
- if `str` matches pattern "AssetDef" returns asset instances with `deftype: AssetDef` (0..N)

Paths:
```
this.engine.actor.get( "#PlayerCharacter" )                     // get the PC instance in the world
this.engine.actor.get( "#PlayerCharacter/SoundDef[coinsound]" ) // get the PC Actor's coinsound Sound
this.engine.actor.get( "#PlayerCharacter/ActorDef[Sword]" )     // get the PC Actor's Sword Actor
this.engine.actor.get( "#PlayerCharacter/SoundDef" )            // get the PC Actor's Sounds
this.engine.actor.get( "ActorDef[ememy]" )                      // get all enemy Actors in the world
```



---
# TRIGGER SYSTEM
Universal communication primitive within the `engine`'s actor-graph.

Basic pattern / usage:
```
actor.emitTrigger( "triggerName", arg1, arg2 )
```

### Routing:

* `Actor.emitTrigger()` will
  * call any registered triggers on self
  * look at all of `Actor`'s children (`Behavior`, `Sound`, `Sprite`, `Actors`), call `emitTrigger()` on all of them

### Standard Triggers - emitted from Engine Lifecycle:
```
this.engine.actor.emitTrigger( "init" );
this.engine.actor.emitTrigger( "update", dt );
this.engine.actor.emitTrigger( "lateUpdate", dt );
```

### Standard Triggers - emitted from Engine Collision:
When actors collide, the Collision Engine will emit the following events onto the 2 `Actor`s
```
actor.emitTrigger( "collide", ...data... )
actor.emitTrigger( "collide_start", ...data... )
actor.emitTrigger( "collide_end", ...data... )
```

### Triggers - emitted from UI:
```
<bespoke trigger names defined inside the custom InGameUI.js for the game project>
```
...Via mouse/keyboard interactions with the UI

### Triggers - emitted from the game Behaviors:
```
<bespoke trigger names defined inside world.json for the game project>
```
...Via custom game events...



---
# Collision Engine details
The runtime `engine` will run a collision pass each `update()` (frame step) before `Behavior`s are serviced.
The collision engine will look at the scene's graph, and perform `Actor` to `Actor` checks will be performed using their visible assets `Sprite` and `Map`, emmiting triggers when collisions occur.

Details:
- `Sprite` assets have both bounding box and collidable pixels
  - **bounding box**:  efficient test before testing pixles
    - will be auto-marked based on the pixel map, as:
      - a. fully uncollidable
      - b. fully collidable
      - c. contining collidable pixels
  - **collidable pixels**:  the `Sprite` asset has a list of colors to ignore
- `Map` assets have tiles and collidable pixels
  - **tiles**: efficient test before testing pixles
    - will be auto-marked based on the pixel map, as:
      - a. fully uncollidable
      - b. fully collidable
      - c. contining collidable pixels
  - **collidable pixels**: the `Map` asset has a list of colors to ignore
- `Actor` auto computes it's own bounding box from it's children
  - **bounding box**: returns the Union of all children's bounding boxes
  - **collidable pixels**: iterates through the children in order, performing pixel collision, first hit of opaque pixel, returns positive for the collision.

### Collision Direction
- In many cases, a direction vector upon collision is nice to have for physical interactions
  - **bounding box** alone, the side of the collision can return a normalized up/down/left/right direction vector.
  - **collidable pixels**:  
    - Variants we could expose
      - Option[fast]: the vector returned can be a normalized vector from the centroid of the bounding box, to the first pixel collided.
      - Option[accuracy]: A more accurate test would be to use:  the centroid/average of all pixels collided, instead of the first pixel collided.
      - Option[constrain]: Could ultimately constrain/snap the normalized vector to up/down/left/right direction vector


---
# Visibility Engine details
The runtime `engine` will run a visiblity pass each `update()` (frame step) before `draw()`.

- `Sprite` assets will be tested against
  1. the viewport: only draw if within.
- `Map` assets will be tested against
  1. the viewport: only draw if within.
- `Sound` assets will be tested against
  1. distance from Listener:  only play if positional & within, or non-positional.


---
# CAMERA SYSTEM
`Camera<Type>` is a `Behavior` on the world `Actor`

`Camera<Type>` `Behavior`s simply update the engine's primitive camera transform:
```
engine.camera.x
engine.camera.y
```

The `Camera<Type>` `Behavior`s:
* `CameraFollow` - configure with an `Actor` instance to follow, keeps that `Actor` centered as it moves around
* `CameraBounded` - configure with an `Actor` instance, snaps to view `CameraBoundingVolume` when entered
* `CameraRoom` - configure with an `Actor` instance, snaps to view `Map` chunk when entered 

All `Camera<Type>` `Behavior`s have the following parameters:
* `distance` - distance Actor can move before the camera moves, creates a circle around the tracked `Actor`, camera will move only the amount to put the `Actor` back at the edge of the circle.  If 0, then precise tracking.
* `delay` - delay before applying the spring
* `outer_distance` - if > `distance` then the spring only takes effect between `distance` and `outer_distance`
* `factor` - amount to move the camera each timestep (scaled by time) back to 0 distance from `Actor`.
* `curve`:
  - `linear`: move the camera each timestep a fixed `factor` velocity toward the `Actor`
  - `exp`: move the camera each timestep a fractional `factor` of distance from camera to the `Actor`


---
# INPUT SYSTEM
The engine registers HTML/Javascript key handlers, maps those to triggers onto the root world `Actor`
```
keydown ArrowLeft → rootActor.emitTrigger("moveLeftDown")
keyup ArrowLeft   → rootActor.emitTrigger("moveLeftUp")
mousedown         → rootActor.emitTrigger("pointerDown", x,y)
```

Example how the engine maps:
```
window.addEventListener( "keydown", e => {
    engine.actor.emitTrigger( "player", "inputKey", { key: e.key, state: buttonStateHelper( e.key, e ) } );
});
window.addEventListener( "keyup", e => {
    engine.actor.emitTrigger( "player", "inputKey", { key: e.key, state: buttonStateHelper( e.key, e ) } );
});
window.addEventListener( "mousedown", e => {
    engine.actor.emitTrigger( "player", "inputMouse", { key: e.button, state: buttonStateHelper( e.button, e ) } );
});
window.addEventListener( "mouseup", e => {
    engine.actor.emitTrigger( "player", "inputMouse", { key: e.button, state: buttonStateHelper( e.button, e ) } );
});
```

Where `buttonStateHelper( ... )` computes and returns a 4-state edge tracking of the 2-state true/false button:
- `DOWN`, `UP`, `JUST_DOWN`, `JUST_UP`


---
# GAME PROJECT FILESYSTEM
Every Game Project exists inside an `assets/` directory.
It may be named anything `mygame/`, `zelda/`
In this document, we will simply refer to these game project directories as `assets/`
When using the Built In Editor (`editor-backend.js`) then there will also be historical versions (old copies) of assets stored under `assets/.versions`.

Filesystem:
```
> ls
assets/
assets/world.json
assets/<images>.[jpg|gif|png]
assets/<sounds>.[m4a|mp3]
assets/<other include files>.<ext>
assets/editor.js
assets/ingameui.js
assets/.versions/<mirrored tree>/
<other asset directories, one per game>/
...
```

# EDITOR BACKEND FS ABSTRACTION

The BuiltInEditor (`editor-backend.js`) serves data endpoints for
- **list** the entire tree of gameprojects (e.g. `assets/` dirs) and their assets (via HTTP GET `/path/to/dir`)
  - returns JSON recursive/nested hierarchy of paths (unless `?recursive=false` is passed)
  - returns success (status=200) or fail (status=400)
  - NOTE: getting/listing implemented with one GET endpoint, which determines what to do by the type of the dir/file under `/path/to`
- **load** the data for an named asset (via HTTP GET `/path/to/asset`)
  - returns the asset raw data with correcly set mimetype
  - returns success (status=200) or fail (status=400)
  - NOTE: getting/listing implemented with one GET endpoint, which determines what to do by the type of the dir/file under `/path/to`
- **save** the data for a named asset (via HTTP POST `/path/to/asset`, with related body mime data)
  - On save:
    1. Copy existing canonical file to next version
    2. Overwrite canonical file
  - returns success (status=200) or fail (status=400)
- **rename** a dir or asset (via HTTP PUT { body: {command: "rename", from: /old/path, to: /new/path }})
  * Check for collisions in the canonical, or in the versions, if none, then:
    * Rename canonical
    * Rename all versions

### versioning on the filesystem:
There is a canonical file:
```
assets/world.json
```
And versions of that same file (from past saves):
```
assets/.versions/world.1.json
assets/.versions/world.2.json
assets/.versions/world.3.json
```

All Paths will be sanitized on load and save.


---
# RENDERER

The engine has a 2D canvas renderer embedded into visual `Asset` instances like `Actor`, `Sprite` and `Map`.
`Actor`s know how to `draw()` themselves (`this.engine.actor.draw()`), to avoid using `emitTrigger()` which uses string compares.
(TODO: we might want to optimize `emitTrigger( "update", dt )` similarly... need to think about data driven behaviors though, maybe can pre-cache grab a function pointer to a behavior's "update" trigger and store in it's `update()` function)

`draw()` recursively iterates down the engine's root actor-graph (`this.engine.actor`), and for every `Actor`, `Sprite` and `Map` instance, draws them into the 2D Canvas.

Outside of this canvas renderer is the HTML overlay defined by `<InGameUI>`, which is rendered by the web browser.

---
# SOUND SYSTEM

Handled by the browser.   individual `Sound` instances manage their own pools, and manage their own stereo + attentuation.

`Sound` instances utilize `update()` to trigger themselves, do their computation/updates.

### Mobile Audio Unlock
Many mobile browsers (iOS Safari, Android Chrome/Firefox) gate audio playback until a user interacts with the page. The engine therefore treats audio unlock as part of startup:

1. Shared AudioContext
  - All `Sound` assets route through a single `engine.audioContext`.
  - Creation is deferred until the first trusted gesture to avoid iOS suspending it automatically.
2. Unlock gesture listeners
  - `<game>` registers `mousedown` and `keydown` listeners.
  - On the first gesture, it calls `engine.ensureAudioUnlocked()` which:
    - Creates/Resumes the shared AudioContext.
    - Plays a silent buffer (Web Audio) to satisfy iOS’s “user-initiated” requirement.
    - Removes the temporary listeners.
    - Sets `engine.audioUnlocked = true` and emits `audioUnlocked` on the root `Actor`.
3. UI affordance
  - Until `audioUnlocked`, the default `<InGameUI>` overlay displays “Tap to enable audio.” Designers can override or restyle this, but they should ensure at least one obvious interaction exists.
4. Queued playback
  - `Sound.play()` checks `engine.audioUnlocked`.
  - If `false`:
    - BGM/SFX requests are queued per `Sound` instance (latest request wins).
    - Once unlocked, the queue flushes automatically.
  - If designers prefer manual control, they can listen for `audioUnlocked` and emit their own play triggers.
5. Platform notes
  - **iOS Safari**: Requires the silent buffer trick plus a resumed AudioContext. After unlock, normal behavior (looping music, multiple voices) works without further gestures.
  - **Android Chrome/Firefox**: Similar requirement but less strict; the same unlock flow covers both.
  - Navigating away or reloading the page resets the lock; the engine re-arms the listeners each time.
6. API Surface
  - `engine.ensureAudioUnlocked()` returns a promise that resolves once audio is available.
  - `engine.audioUnlocked` boolean reflects current state.
  - Root actor receives `audioUnlocked` trigger so `Behaviors`/`InGameUI`s can react (e.g., start music, hide overlays).

---
# UPDATE LOOP
Variable dt:
```
dt = min(actualDt, 1/30) // avoid tiny or zero dt
```

Pipeline:
```
// inittime
this.engine.actor.emitTrigger( "init" ) // first frame only (init the actor graph, all child assets)
this.engine.ui.emitTrigger( "init" ) // first frame only (init the ui)
this.engine.ui.addTriggerListener( (trigger) => {
  if (trigger.name == "quit")) {
    this.engine.running = false
  } else {
    this.engine.actor.emitTrigger( trigger.name, trigger.args )
  }
})

// runtime
while (this.engine.running) {
  this.engine.camera.update(dt)
  this.engine.collision.update()
  this.engine.ui.emitTrigger( "update", dt ) // every frame (updates the ui)
  this.engine.actor.update( this.engine, dt ) // every frame (updates the actor graph, all child assets)
  this.engine.actor.draw( this.engine ) // every frame (draws the actor graph, all child assets)
}
```

---
# SAVE / LOAD RUNTIME STATE

The game runs in a webpage without a backend.
Therefore, there must be a strategy to save state using the browser's `localStorage`.

TODO: Options To Consider:
- save entire root `Actor` instance graph hierarchy to `localStorage` (TODO: is there enough space?  what's our limit?).  Only save state relevant to gameplay, Some state is not worth saving out, like the exact animation frame.
- add a generic "save" `Behavior` only to those Actors we want to save, which uses `localStorage`
- mark selected data fields in `Actor` and `Behavior` for save
- allow the user to download a save file, which they can upload to restore (not ideal, forces them to deal with files on mobile)

---
# TODO: QUESTIONS to INVESTIGATE

- on iOS mobile, they block audio unless you tap a button, how do we ensure that game audio plays, given the structure above?

---
# END SPEC v1.0
