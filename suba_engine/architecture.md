
# ENGINE ARCHITECTURE SPEC v1.0
A Game Engine for Web Browser.

- Features at a glance:
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
        - for creating custom `GameProjectEditor` web-components.
        - for creating custom `InGameUI` web-components.
      - Game Project Editor can be created (bootstrapped) from nothing using provided reusable components
      - ... or by copying an existing editor definition and modifying it.
      - `InGameUI` can be created (bootstrapped) from nothing using provided reusable components
      - ... or by copying an existing editor definition and modifying it.
  - Runtime game is created inside index.html with a single <game data="world.json"></game> html element


| feature           | about         |
| ---------------- | ------------- |
| **TechStack**:   | JavaScript + HTML + CSS, no or minimal dependencies |
| **Renderer**:    | Canvas 2D + HTML overlay |
| **Editor**:      | Pluggable Editable Web Components + Backend FS abstraction |
| **Philosophy**:  | Data-driven, Behavior-centric, JSON Defs |
| **Project**:     | world.json is the ENTIRE game (with ingame UI and editor) |
| **Code Std**:    | spaces instead of tabs; 2 spaces per tab; curlybrace on same line as conditional; requirements driven; keep it simple stupid; refactor towards simplicity as you go along; CamelCase classes/structs, camelCase functions, snake_case variables, _private variables, ALL_CAPS enums, if () conditionals, for () and while () loops; func( arg, arg2, arg3 ) functions; |




---
# CORE DESIGN PRINCIPLES

```
1. Asset Definitions:  Assets are first defined as "Defs"
   ActorDef, SpriteDef, MapDef, SoundDef, UIDef, EditorDef.
   Serve as prototypes (templates) for creating runtime Asset instances of those types.

2. Asset instances:  All runtime objects (Actor, Map, Sprite, Sound, UI, Editor)
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
- `UIDef` - *defines* how to create an `InGameUI` html web component instance
- `EditorDef` - *defines* how to create an `GameProjectEditor` html web component instance

### Definition Schema:
All Defs are defined in this way
- `id` is optional, but must be UNIQUE (exception (or auto uniquify the name) if not unique; will get assigned a GUID if not supplied)
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

### Standardized "config" (configLoader):

All `config` keys in the `world.json` are handled by the same common reusable `configLoader` codepath in the engine:
```
"<AssetDefType>Def": {
  "id": "<DefName>",

  // ... and then, one of the following types of "config":
  "config": "path/to/file.<js or json>",      // include style config (configLoader checks for string matching path)
  "config": "java_script_goes_here()",        // inline style config (configLoader checks for string !matching path)
  "config": { ... JSON data definition ... }  // json style config (configLoader checks for object)
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
    return eval( code )
  }

  // inline style config
  else if (type definition_config == "string" && !path_regex.test( definition_config )) {
    // get the javascript code ready, store it back on top of definition.config
    const let code = definition_config
    return eval( code )
  }

  // json style config
  else if (type definition_config == "object") {
    return definition_config
  }
}
```

The reusable `configLoader` is used to resolve any `config` key inside the `world.json` project file.
```
definition.config_src = definition.config   // preserve source
definition.config = configLoader( definition.config_src )
```

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
- `UI` - spawns an `InGameUI` html web component, from the given Def
- `Editor` - spawns a `GameProjectEditor` html web component, from the given Def

An example def & instance relationship where `Asset` is one of the above types:
```
AssetDef: {
  "id": "enemySlime",
  "config": <configdata>
}

Asset:
{
  "id": "slime004",
  "def": "enemySlime",
  "deftype",                         // automatically set when instantiated, to the AssetDef
  "x": 120,
  "y": 200,
  "<datakey>": "<datavalue>"         // override any of the base ones
}
```



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
```

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

// Instance `UI` is created from that `ingameui` definition:
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
            - `ui` field which populates `UIDef` and `ui` in `world.json`
            - `editor` field which populates `EditorDef` and `editor` in `world.json`
            - Save Button to write `world.json`
            - Buttons to jump to the relevant tab:  Actors, Behaviors, Sprites, Maps, Sounds, UI, Editor
- WebComponent Code Editor (for editing files like: `ingameui.js`, `editor.js` and new variants of those)
  - A page for writing new `<InGameUI>` and `<GameProjectEditor>` web components
    - presents a color syntax highlighting code editor, and an editor preview pane with controls to browse assets, load and save.
  - by default
    - no `world.json` active:  displays default `editor.js` from the asset data endpoint
    - `world.json` active:  displays `editor` from the `world.json`
  - 2 panes:
    - 1 big pane with 2 sub tabs:
      - Code Editor for the selected `EditorDef` or `UIDef`
      - preview pane (shows the active `<GameProjectEditor></GameProjectEditor>` or `<InGameUI></InGameUI>` being edited)
        - displays the `UI` as defined by `UIDef` in the active `world.json`, as the `<InGameUI>` html element
        - displays the `Editor` as defined by `EditorDef` in the active `world.json`, as the `<GameProjectEditor>` html element
        - if eval() of the code fails, nothing will preview until eval() passes (show the error instead of preview)
    - data panel
      - listview with list of all `EditorDef`s in `world.json` (if one is active)
        - selected `EditorDef` resets the preview pane, and the Code Editor to that `EditorDef`
        - New:    Button to creates a new `EditorDef`
        - Delete: Button to delete selected `EditorDef`
        - Use:    Button to make the currect selected `EditorDef` active as `editor` in `world.json`
      - listview with list of all `UIDef`s in `world.json` (if one is active)
        - selected `UIDef` resets the preview pane, and the Code Editor to that `UIDef`
        - New:    Button to creates a new `UIDef`
        - Delete: Button to delete selected `UIDef`
        - Use:    Button to make the currect selected `UIDef` active as `ui` in `world.json`
      - Edit Button to jump to my Code Editor tab
      - Preview Button to jump to my preview pane tab
      - Save Button to write `world.json` and the asset
        - inline: into world.json
        - include: into asset system
- GameProjectEditor (active (not greyed out) when there's an active `world.json`)
  - A page with a single pane for editor or game-preview, `[new project]`, `[load project]`, `[save project]`, `[Edit Mode]` or `[Game Mode]` buttons
  - upon Project Load, the editor.js is read from the project, and `<GameProjectEditor></GameProjectEditor>` is then added to the page.
  - `[Edit Mode]` or `[Game Mode]` buttons toggle between `<game data="world.json"></game>` or `<GameProjectEditor data="{ entire world json object }"></GameProjectEditor>` in the page.
  - will display the `world.json`'s `editor` field's `editor.js` web component `<GameProjectEditor></GameProjectEditor>`

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
  - Each **preview** type (that we may like to view in a "preview pane") is a web component  (`Actor`, `Behavior`, `Sprite`, `Map`, `Sound`, `UI`, `Editor`, `Code`, `Image`, `Game`, `ActorPlacement`)
  - Reusable Widget Library components:
    - Each widget in a "data pane" is a web component, for example:
      1. **Data Editors**
        - For editing `AssetDef` and `Asset` instance types (where Asset == `Actor`, `Behavior`, `Sprite`, `Map`, `Sound`, `UI`, `Editor`)
        - ideally there is one `Asset` Editor web-component only, and can "drive it with data" to instantiate an editor for each type... (where Asset == `Actor`, `Behavior`, `Sprite`, `Map`, `Sound`, `UI`, `Editor`)
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
      - listview with list of all `SoundDef`s in `world.json`
      - `SpriteDef` editor (datadriven by the base `SpriteDef` config structure), for example:
        - Upload new sprite image to assets backend
        - Select a sprite image from assets backend, and set it into the `SpriteDef`
        - all other fields from the the base `SpriteDef` config structure
  - Saves to `world.json` definitions
  - Reuse the web-component `<JavaScriptEditor>` code editor for JavaScript
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
      - Code Editor for the selected `UIDef`
      - preview pane (shows the active `<InGameUI></InGameUI>` being edited)
        - displays the `UI` as defined by `UIDef` in the active `world.json`, as the `<InGameUI>` html element
        - Controls below the `<InGameUI></InGameUI>`:
          - behavior trigger buttons (one per named trigger in the child behaviors)
    - data panel
      - listview with list of all `UIDef`s in `world.json`
        - selected `UIDef` resets the preview pane, and the Code Editor to that `UIDef`
        - New:    Button to creates a new `UIDef`
        - Delete: Button to delete selected `UIDef`
        - Use:    Button to make the currect selected `UIDef` active as `ui` in `world.json`
      - Edit Button to jump to my Code Editor tab
      - Preview Button to jump to my preview pane tab
      - Save Button to write `world.json`
  - Saves to `world.json` definitions




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
When actors collide, the Collision Enginer will emit the following events onto the 2 `Actor`s
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
Saved:
* Actor positions
* Actor data
* Behavior.serialize()
* Sprite/Sound playback states
* World/global flags
* Parent relationships

Load:
1. Load world.json
2. Load save.json
3. Reinstantiate runtime
4. Apply state

---
# END SPEC v1.0









# ##############################################################################









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

