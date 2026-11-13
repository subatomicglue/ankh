export async function loadIndexedBitmap(url) {
  const buffer = await (await fetch(url)).arrayBuffer();
  const view = new DataView(buffer);
  if (view.getUint16(0, true) !== 0x4d42) throw new Error(`${url} is not a BMP`);
  const offset=view.getUint32(10,true), width=view.getInt32(18,true), signedHeight=view.getInt32(22,true);
  const height=Math.abs(signedHeight), bits=view.getUint16(28,true);
  if (bits !== 8) throw new Error(`${url}: expected indexed 8-bit BMP, got ${bits}`);
  const stride=Math.ceil(width/4)*4, pixels=new Uint8Array(width*height), source=new Uint8Array(buffer);
  for(let y=0;y<height;y++) {
    const sourceY=signedHeight>0?height-1-y:y;
    pixels.set(source.subarray(offset+sourceY*stride,offset+sourceY*stride+width),y*width);
  }
  return { width, height, pixels, pixel(x,y){return x<0||y<0||x>=width||y>=height?0:pixels[y*width+x];} };
}

export class AssetStore {
  constructor(base='.') { this.base=base;this.bitmaps=new Map(); }
  async load(names) {
    this.walls=await loadIndexedBitmap(`${this.base}/assets/walls.bmp`);
    await Promise.all([...names].map(async name=>this.bitmaps.set(name,await loadIndexedBitmap(`${this.base}/assets/sprites/${name}.bmp`))));
  }
  actorPixel(actor,x,y) { return this.bitmaps.get(actor.spriteFile)?.pixel(x,y)??0; }
}
