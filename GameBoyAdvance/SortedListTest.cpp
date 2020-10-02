#include "SortedList.h"

struct bok
{
   bok* m_poNext, *m_poPrev;
   int m_nStart;
   int m_nSize;
};

void print( SortedList<bok>& m_poDrawList )
{
   bok* iter = m_poDrawList.front();
   int nCount = 0;
   char buf[256];
   printf( "\n---------\nlist:\n" );
   while (iter != m_poDrawList.end() && nCount <= m_poDrawList.m_nNumNodes)
   {
      sprintf( buf, "%d/%d: start:%d \n", nCount, m_poDrawList.m_nNumNodes, iter->m_nStart );
      printf( buf );
      iter = iter->m_poNext;
      ++nCount;
   }
   printf( "\n" );
}
int main()
{
   SortedList<bok> m_poDrawList;
   bok a, b, c, d;
   a.m_nStart = 0;
   b.m_nStart = 64;
   c.m_nStart = 128;
   d.m_nStart = 1024;

   printf( "inserting %d\n", b.m_nStart );
   m_poDrawList.insert( &b );
   print( m_poDrawList );

   printf( "inserting %d\n", c.m_nStart );
   m_poDrawList.insert( &c );
   print( m_poDrawList );
   
   printf( "inserting %d\n", d.m_nStart );
   m_poDrawList.insert( &d );
   print( m_poDrawList );
   
   printf( "inserting %d\n", a.m_nStart );
   m_poDrawList.insert( &a );
   print( m_poDrawList );


   printf( "inserting (again) %d\n", b.m_nStart );
   m_poDrawList.insert( &b );
   print( m_poDrawList );


   printf( "removing %d\n", b.m_nStart );
   m_poDrawList.remove( &b );
   print( m_poDrawList );

   printf( "removing %d\n", c.m_nStart );
   m_poDrawList.remove( &c );
   print( m_poDrawList );
   
   printf( "removing %d\n", d.m_nStart );
   m_poDrawList.remove( &d );
   print( m_poDrawList );
   
   printf( "removing %d\n", a.m_nStart );
   m_poDrawList.remove( &a );
   print( m_poDrawList );

   return 0;
}