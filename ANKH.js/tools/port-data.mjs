import fs from 'node:fs';
import path from 'node:path';
import {fileURLToPath} from 'node:url';

const project=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'..');
const readJson=file=>JSON.parse(fs.readFileSync(path.join(project,file),'utf8'));
const actors=readJson('data/actors.json');
const wallRows=readJson('data/walls.json');
const mapHeight=wallRows.length,mapWidth=wallRows[0]?.length??0,tileSize=16,atlasColumns=16,roomWidth=192,roomHeight=192;
if(!mapWidth||wallRows.some(row=>!Array.isArray(row)||row.length!==mapWidth))throw new Error('walls.json must be a non-empty rectangular tilemap');
if(wallRows.some(row=>row.some(tile=>!Number.isInteger(tile)||tile<0)))throw new Error('walls.json tile IDs must be non-negative integers');

const tiles=wallRows.flat();

if(tiles.length!==mapWidth*mapHeight)throw new Error('walls.json tile count does not match its dimensions');
if(actors.length!==212)throw new Error(`Expected 212 actors, found ${actors.length}`);
const ids=new Set(actors.map(actor=>actor.id));
if(ids.size!==actors.length)throw new Error('Actor IDs must be unique');

const output=`// GENERATED FILE — DO NOT EDIT.\n`+
  `// Edit actors and behaviors in ../data/actors.json.\n`+
  `// Edit the room wall tilemap in ../data/walls.json.\n`+
  `// From ANKH.js/, regenerate this file with: npm run generate\n`+
  `export const WORLD = ${JSON.stringify({width:mapWidth*tileSize,height:mapHeight*tileSize,roomWidth,roomHeight,tileSize,mapWidth,mapHeight,atlasColumns})};\n`+
  `export const WALL_TILES = ${JSON.stringify(wallRows.flat())};\n`+
  `export const ACTOR_DEFINITIONS = ${JSON.stringify(actors)};\n`;
const destination=path.join(project,'src/data.js');

if(process.argv.includes('--check')){
  if(fs.readFileSync(destination,'utf8')!==output)throw new Error('src/data.js differs from ANKH.js/data source; run npm run generate');
  console.log(`ANKH.js data ok: ${tiles.length} 16x16 tiles and ${actors.length} actors.`);
}else{
  fs.writeFileSync(destination,output);
  console.log(`Generated ${tiles.length} 16x16 tiles and ${actors.length} actors.`);
}
