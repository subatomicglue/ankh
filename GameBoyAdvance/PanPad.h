
#ifndef PANPAD_H
#define PANPAD_H

class PanPad
{
public:
   PanPad( int x, int y, int width, int height, int offsetx, int offsety )
   {
      //AnkhActor::
   }


   /// todo: split off the constants into a PanPadInit that can stay in ROM...
   s32 mX;
   s32 mY;
   s32 mWidth;
   s32 mHeight;
   s32 mOffsetX;
   s32 mOffsetY;
};

/*
class Manager
{
public:
   void buildVisibleList()
   {
      for (u16 x = 0; x < m_nNumActors; ++x)
      {
         if (m_ppoActors[x]->m_bIsOnScreen &&
            (!m_ppoActors[x]->isOnScreen() ||
            !m_ppoActors[x]->m_bIsDrawing))
         {
            removeFromDrawList( m_ppoActors[x] );
            m_ppoActors[x]->m_bIsOnScreen = false;
         }
         else if (!m_ppoActors[x]->m_bIsOnScreen &&
                  m_ppoActors[x]->isOnScreen() &&
                  m_ppoActors[x]->m_bIsDrawing)
         {
            addToDrawList( m_ppoActors[x] );
            m_ppoActors[x]->m_bIsOnScreen = true;
         }
      }
   }

   void addToDrawList( Actor* actor )
   {
      ActorNode* an = m_aoPool.take();
      if (an)
      {
         an->m_nSize = actor->bitmap()->ps();
         an->m_poActor = actor;
         m_poDrawList.insert( an );
      }
   }

   void removeFromDrawList( Actor* actor )
   {
      ActorNode* iter = m_poDrawList.front();
      u16 nCount = 0;
      while (iter != m_poDrawList.end() && iter->m_poActor != actor)
      {
         iter = iter->m_poNext;
         ++nCount;
      }
      if (iter != m_poDrawList.end())
      {
         m_poDrawList.remove( iter );
         m_aoPool.putback( iter );
      }
   }
};
*/

#endif
