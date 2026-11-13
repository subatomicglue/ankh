import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '../..');
const source = fs.readFileSync(path.join(root, 'GameBoyAdvance/data.cpp'), 'utf8');
const mapSource = fs.readFileSync(path.join(root, 'GameBoyAdvance/art/walls.map.c'), 'utf8');

const numeric = expression => {
  if (!/^[\d\s()+*/.-]+$/.test(expression)) throw new Error(`Unsafe numeric expression: ${expression}`);
  return Function(`"use strict"; return (${expression})`)();
};
const spriteBoxes = Object.fromEntries([...source.matchAll(/const gba::SpriteAsset (\w+)Sprite = \{ gba::Vec<s32>\(\s*([^,]+),\s*([^)]+)\)/g)]
  .map(([, name, width, height]) => [name, [numeric(width), numeric(height)]]));
const spriteImages = Object.fromEntries([...source.matchAll(/const gba::SpriteAsset (\w+)Sprite = \{[^\n]*,\s*(\w+)\s*\};/g)]
  .map(([, name, image]) => [name, image]));
const bitmapFrames = Object.fromEntries([...source.matchAll(/const gba::BitmapAsset (\w+) = \{\s*(\d+),\s*(\d+),/g)]
  .map(([,name,tx,ty])=>[name,Number(tx)*Number(ty)]));

const behaviorByActor = {};
for (const match of source.matchAll(/(\w+)Method\s+(\w+)BehaviourMethod(?:\(([^;]*)\))?;/g)) {
  const [, type, actor, args = ''] = match;
  behaviorByActor[actor] = { type: type.toLowerCase(), args: args.trim() };
}

const actors = [];
const actorPattern = /const ActorInit (\w+)Init = \{\s*"([^"]+)",\s*ActorInit::(\w+),\s*&([a-zA-Z0-9_]+)Sprite,\s*(-?\d+),\s*(-?\d+),\s*(-?\d+),\s*(-?\d+),\s*(-?\d+),\s*(-?\d+)\s*,\s*([^}]+)\}/g;
for (const m of source.matchAll(actorPattern)) {
  const [, id, name, type, sprite, x, y, roomX, roomY, touchKarma, shotKarma, flags] = m;
  const flagNames = [...flags.matchAll(/ACTOR_[A-Z_]+/g)].map(x => x[0]);
  actors.push({
    id, name, type, sprite, x: +x, y: +y, roomX: +roomX, roomY: +roomY,
    touchKarma: +touchKarma, shotKarma: +shotKarma,
    flags: flagNames, box: spriteBoxes[sprite] ?? [8, 8], image: spriteImages[sprite] ?? sprite,
    frames: bitmapFrames[spriteImages[sprite] ?? sprite] ?? 1,
    behavior: behaviorByActor[id] ?? { type: 'none', args: '' },
  });
}

const mapBody = mapSource.slice(mapSource.indexOf('{') + 1, mapSource.lastIndexOf('}'));
const tiles = (mapBody.match(/\b\d+\b/g) ?? []).map(Number);
if (tiles.length !== 192 * 192) throw new Error(`Expected 36864 map tiles, found ${tiles.length}`);
if (actors.length !== 212) throw new Error(`Expected 212 actors, found ${actors.length}`);
const allActorsBody=source.match(/AnkhActor\* gAllActors\[212\]\s*=\s*\{([^}]+)\}/)?.[1]??'';
const allActorIds=[...allActorsBody.matchAll(/&(\w+)/g)].map(match=>match[1]);
if(JSON.stringify(allActorIds)!==JSON.stringify(actors.map(actor=>actor.id)))throw new Error('Generated actor declaration order differs from gAllActors');

const output = `// Generated from GameBoyAdvance/data.cpp and art/walls.map.c.\n` +
  `export const WORLD = { width: 1536, height: 1536, roomWidth: 192, roomHeight: 192, tileSize: 8 };\n` +
  `export const WALL_TILES = ${JSON.stringify(tiles)};\n` +
  `export const ACTOR_DEFINITIONS = ${JSON.stringify(actors, null, 2)};\n`;
const destination=path.join(root,'ANKH.js/src/data.js');
if(process.argv.includes('--check')){
  if(fs.readFileSync(destination,'utf8')!==output)throw new Error('src/data.js differs from the GBA actor or map declarations; run npm run generate');
  const files=[['walls.bmp','assets/walls.bmp'],...new Set(actors.map(actor=>actor.image))].map(name=>Array.isArray(name)?name:[`${name}.bmp`,`assets/sprites/${name}.bmp`]);
  for(let title=1;title<=9;title++)files.push([`title${title}.bmp`,`assets/titles/title${title}.bmp`]);
  for(const [sourceName,destinationName] of files){
    const original=fs.readFileSync(path.join(root,'GameBoyAdvance/art',sourceName)),ported=fs.readFileSync(path.join(root,'ANKH.js',destinationName));
    if(!original.equals(ported))throw new Error(`${destinationName} differs from GameBoyAdvance/art/${sourceName}`);
  }
  console.log(`Parity data ok: ${tiles.length} tiles and ${actors.length} actors.`);
}else{
  fs.writeFileSync(destination,output);
  console.log(`Ported ${tiles.length} tiles and ${actors.length} actors.`);
}
