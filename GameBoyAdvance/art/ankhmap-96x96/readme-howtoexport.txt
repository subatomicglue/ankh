
if you lose the fmp file, you can get it into mappy this way:
- you can import the bmp using file/import, 
- and a txt file using "custom"/read text file


when exporting, save it, then 
- type "walltiles" for the name
- choose "data as text for GBA (.txt)",
- choose "Graphics Blocks as picture (BMP)"
- choose (bmpwidth/blocksize) (it's 16 for 256x256) blocks a row
- skip block 0
- choose OK
  
- type "walls_Map"
- choose this layer only, 2d format, nothing else (deselect everything)
- choose Use 'const', Maparray(s)

- NOTE: that the output bitmap matches the walls.bmp in ART directory...

- NOTE: this is only a 96x96 map (16px tiles).  
  use the converter (or write one) to split the tiles into 8x8...
