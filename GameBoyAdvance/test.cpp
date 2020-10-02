
#include <stdio.h>
int main()
{
   int a = 0, b = 2;
   for (int x = 0; x < 50; ++x)
   {
      a = (a+1) % b;
      printf( "%d\n", a );
   }
   return 0;
}
