
typedef unsigned short u16;

#include <stdio.h>
#include "walls.TXT"

#define MAP walls_map

int main()
{
   int x, y;
   FILE* f = fopen( "mappy-import.txt", "w" );
   for (y = 0; y < 96; ++y)
   {
      for (x = 0; x < 96; ++x)
      {
         fprintf( f, "%d", MAP[x + y*96] );
      }
      fprintf( f, "\n" );
   }
   fclose( f );
}
