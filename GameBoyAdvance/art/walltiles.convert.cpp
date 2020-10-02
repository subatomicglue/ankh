
#include <fstream>

typedef unsigned short u16;
typedef unsigned char u8;
#include "ankhmap-96x96/walls.txt" // mappy 96x96 map, need to split to 192x192

#define MAP walls_map

int convertID( int id, int origx, int origy, int destx, int desty, int offsetx, int offsety )
{
   int x = id % origx;
   int y = id / origx;
   int scalex = destx / origx;

   return (x * scalex + y * destx * scalex) + (offsetx + offsety * destx);
}

int main()
{
   u16 new_Map[ 96 * 96 * 4 ];

   int tiles_in_a_row = 16;

   for (int y = 0; y < 96; ++y)
   for (int x = 0; x < 96; ++x)
   {
      new_Map[(x*2 + 0) + (y*2 + 0) * 96*2] = convertID( MAP[x + y * 96], 16, 16, 32, 32, 0, 0 );
      new_Map[(x*2 + 1) + (y*2 + 0) * 96*2] = convertID( MAP[x + y * 96], 16, 16, 32, 32, 1, 0 );
      new_Map[(x*2 + 0) + (y*2 + 1) * 96*2] = convertID( MAP[x + y * 96], 16, 16, 32, 32, 0, 1 );
      new_Map[(x*2 + 1) + (y*2 + 1) * 96*2] = convertID( MAP[x + y * 96], 16, 16, 32, 32, 1, 1 );
   }

   std::ofstream f( "walls.map.c" );

   f << "typedef unsigned short u16;\n"
      << "typedef unsigned char u8;\n\n";
   f << "extern const u16 walls_Map[];\n"
      << "const u16 walls_Map[96*96*4] = {\n";
   for (int y = 0; y < 96 * 2; ++y)
   {
      f << "\n   ";
      for (int x = 0; x < 96 * 2; ++x)
      {
         int val = new_Map[x + y * 96 * 2];
         f << new_Map[x + y * 96 * 2] << ",";
      }
      
   }
   f << "};\n\n";

   /*
   f << "extern const u8 walls_Bitmap[];\n";
   f << "const u8 walls_Bitmap[256*256] = {\n";
   for (int y = 0; y < 256*256; ++y)
   {
      f << (int)walls_Bitmap[y] << ",";
   }
   f << "};\n\n";

   f << "extern const u16 walls_Palette[];\n";
   f << "const u16 walls_Palette[256] = {\n";
   for (int y = 0; y < 256; ++y)
   {
      f << (int)walls_Palette[y] << ",";
   }
   f << "};\n\n";
   */

   return 0;
}
