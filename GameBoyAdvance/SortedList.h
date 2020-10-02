#ifndef SORTED_LIST
#define SORTED_LIST

/// sorts by m_nStart, make sure your node has this member.
/// also, your node type should have m_poNext, and m_poPrev
template <typename T>
class SortedList
{
public:
   SortedList()
   {
      m_oEntries.m_poNext = &m_oEntries;
      m_oEntries.m_poPrev = &m_oEntries;
      m_oEntries.m_nStart = 0; // smallest number.
      m_nNumNodes = 0;
   }
   inline T* front() { return m_oEntries.m_poNext; }
   inline T* end() { return &m_oEntries; } //< one beyond the last one so you can test for end()
   inline T* rFront() { return m_oEntries.m_poPrev; }
   inline T* rEnd() { return &m_oEntries; } //< one beyond the last one so you can test for end()
   void insert( T* entry )
   {
      //printf( "inserting %d\n", entry->m_nStart );
         
      // sorts incoming records by increasing address
      // first, find node to insert after (iterate backwards)
      T* iter = rFront();
      T* found = iter;
		while (iter != rEnd())
		{
         if (iter->m_nStart <= entry->m_nStart)
			{
            if (entry == iter)
            {
               return; // already in the list, bail out.
            }

				// end insertion
      		iter = rEnd();
            //printf( "found %d\n", found->m_nStart );
			}
			else
			{
				iter = iter->m_poPrev;
            found = iter;
			}
		}

      // do insertion.
      if (found)
      {
         //printf( "hooked up %d after %d\n", entry->m_nStart, found->m_nStart );
         // hook up entry after the one we found (since it is greater than)
		   entry->m_poPrev = found;
		   entry->m_poNext = found->m_poNext;
   		
		   // hook up iter and iter's next
		   found->m_poNext->m_poPrev = entry;
		   found->m_poNext = entry;
         ++m_nNumNodes;
      }
	}

   void remove( T* entry )
   {
      if (entry && entry->m_poNext && entry->m_poPrev)
      {
         T* n = entry->m_poNext;
		   T* p = entry->m_poPrev;

         // remove the references to itself
         p->m_poNext = n;
         n->m_poPrev = p;
         
         // not in the list anymore.
         entry->m_poNext = NULL;
         entry->m_poPrev = NULL;

         // inc
         --m_nNumNodes;
      }
   }

   T m_oEntries;
   unsigned int m_nNumNodes;
};

#endif
