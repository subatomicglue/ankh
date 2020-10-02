
#ifndef DRAW_SCREEN_H
#define DRAW_SCREEN_H

// call this function to draw the screen with an image.
// For input data:
// use gfx2gba to convert bitmaps to paletized (16bit, 256 color), and
// uncompressed.  gfx2gba -fsrc -ppalette image.bmp
inline void drawScreen( const unsigned short pal[], const unsigned char bmp[] )
{
    unsigned short* Screen = (unsigned short*)0x6000000;
    //unsigned short* BackBuf = (unsigned short*)0x600A000;
    *(unsigned long*)0x04000000 = 0x3 | 0x400; // REG_DISPCNT = BGMODE3 | BG2ENABLE

   u16 rgb = 0;
   for(u16 y = 0; y < 160; y++)  //loop through all y
   {
      u16 y240 = y*240;
      for(u16 x = 0; x < 240;)   //loop through all x
      {
         // todo, preconvert the bmp/pal lookup into an array
         //       save into a .cpp file... memcpy/DMA to Screen instead of for loop...

         u16 xPlusY240 = x + y240;
         //Screen[x+y*240] = pal[bmp[rgb++]]; x++;
         Screen[xPlusY240+0] = pal[bmp[rgb+0]]; 
         Screen[xPlusY240+1] = pal[bmp[rgb+1]]; 
         Screen[xPlusY240+2] = pal[bmp[rgb+2]]; 
         Screen[xPlusY240+3] = pal[bmp[rgb+3]];
         Screen[xPlusY240+4] = pal[bmp[rgb+4]];
         Screen[xPlusY240+5] = pal[bmp[rgb+5]];
         Screen[xPlusY240+6] = pal[bmp[rgb+6]];
         Screen[xPlusY240+7] = pal[bmp[rgb+7]];
         x += 8;
         rgb += 8;
      }
   }
   REG_DISPCNT &= ~BACKBUFFER;
}

#endif
