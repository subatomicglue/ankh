#ifndef FIXED_SIZE_POOL
#define FIXED_SIZE_POOL

/// fixed size pool of T elements.
template <typename T, unsigned SIZE>
class Pool
{
public:
   Pool() : m_nSize( SIZE )
   {
      init();
   }

   /// call init before using the pool.
   inline void init()
   {
      for (int x = 0; x < SIZE; ++x)
      {
         m_poNodePool[x] = &m_aoNodes[x];
      }
   }

   /// take an elt from the pool, your responsibility to put it back.
   /// returns NULL if pool is empty
   T* take()
   {
      if (0 < m_nSize)
      {
         // take off the end.
         --m_nSize;
         return m_poNodePool[m_nSize];
      }
      return NULL;
   }

   /// put an element back into the pool after use.
   void putback( T* elt )
   {
      if (m_nSize < SIZE)
      {
         // add back onto the end
         m_poNodePool[m_nSize] = elt;
         ++m_nSize;
      }
   }

   /// get the size of the pool
   u32 size() const { return m_nSize; }

   T m_aoNodes[SIZE];
   T* m_poNodePool[SIZE];
   u32 m_nSize;
};

#endif
