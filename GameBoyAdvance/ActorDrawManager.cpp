
#include "ActorDrawManager.h"
#include "AnkhActor.h"
#include "Actor.h"

void ActorDrawManager::update()
{
   buildVisibleList();

   // update positions
   u32 nCount = 0;
   ActorNode* iter = m_poOnScreenList.front();
   while (iter != m_poOnScreenList.end())
   {
      AnkhActor* actor = (AnkhActor*)iter->m_poActor;
#ifndef WIN32
      // get local screen position of the sprite.
      gba::Vec<s32> screen_pos = actor->getScreenPos();
      //if (!actor->m_bIsInWorldSpace)
      //   spew::Trace( "sprite screen pos: %d, %d\n", screen_pos[0], screen_pos[1] );
      iter->m_oSprite.setPos( screen_pos[0], screen_pos[1] );
      iter->m_oSprite.setPriority( actor->depth() );
      iter->m_oSprite.update();
#endif
      iter = iter->m_poNext;
      ++nCount;
   }
}

void ActorDrawManager::updateBehaviours()
{
   // update
   ActorNode* iter = m_poOnScreenList.front();
   while (iter != m_poOnScreenList.end())
   {
      AnkhActor* actor = (AnkhActor*)iter->m_poActor;
      actor->doThing( BehaviourMethod::UPDATE, NULL );
      iter = iter->m_poNext;
   }
}

/// upload the sprite info to the Object Attribute Memory
void ActorDrawManager::uploadOamInfo()
{
   u32 nCount = 0;

   //spew::Trace( spew::ACTOR, 3, "iter: front: %x   end: %x\n", m_poOnScreenList.front(), m_poOnScreenList.end() );
   
   ActorNode* iter = m_poOnScreenList.front();
   int size = m_poOnScreenList.m_nNumNodes;
   for (int x = 0; x < size; ++x)
   {
      if (iter->m_poActor->hasFlags( ACTOR_VISIBLE ))
      {
#ifndef WIN32
         iter->m_oSprite.uploadOamInfo( nCount );
#endif
         ++nCount;
      }
      iter = iter->m_poNext;
   }
}

bool ActorDrawManager::intersectVisibleActors( const gba::Vec<s32>& target, IntersectResult& result )
{
   u32 nCount = 0;
   ActorNode* iter = m_poOnScreenList.front();
   while (iter != m_poOnScreenList.end())
   {
      spew::Trace( "   intersect box:%d,%d %d,%d ... c:%d,%d  target:%d,%d\n", 
         iter->m_poActor->getWorldBox().mMin[0],
         iter->m_poActor->getWorldBox().mMin[1], 
         iter->m_poActor->getWorldBox().mMax[0],
         iter->m_poActor->getWorldBox().mMax[1],
         iter->m_poActor->cpos()[0], 
         iter->m_poActor->cpos()[1], 
         target[0], target[1] );

      if (iter->m_poActor->isOnScreen() && iter->m_poActor->hasFlags( ACTOR_COLLISION ) &&
         gba::intersect( iter->m_poActor->getWorldBox(), target ))
      {
         result.actor = iter->m_poActor;
         return true;
      }
      iter = iter->m_poNext;
      ++nCount;
   }
   return false;
}

static void outputActors( const Actor& a, const Actor& b )
{
   spew::Trace( "min%d,%d max:%d,%d : ",
               a.getWorldMin(0), a.getWorldMin(1),
               a.getWorldMax(0), a.getWorldMax(1) );
   spew::Trace( "min%d,%d max:%d,%d\n",
               b.getWorldMin(0), b.getWorldMin(1),
               b.getWorldMax(0), b.getWorldMax(1) );
}

/// intersect actor against all visible actors...
bool ActorDrawManager::intersectVisibleActors( const Actor& actor, IntersectResult& result, const Actor* exclude, s32 flags )
{
   u32 nCount = 0;
   ActorNode* iter = m_poOnScreenList.front();
   while (iter != m_poOnScreenList.end())
   {
      //if (dbg)
      //{
      //   outputActors( actor, *iter->m_poActor );
      //}

      if (iter->m_poActor->isOnScreen() && 
         iter->m_poActor->hasFlags( flags ) &&
         &actor != iter->m_poActor && 
         exclude != iter->m_poActor &&
         intersect( *iter->m_poActor, actor ))
      {
         result.actor = iter->m_poActor;
         return true;
      }
      iter = iter->m_poNext;
      ++nCount;
   }
   return false;
}

/// intersect box against all visible actors...
bool ActorDrawManager::intersectVisibleActors( const gba::AABox2<s32>& box, const Actor* actor, IntersectResult& result, const Actor* exclude, s32 flags )
{
   u32 nCount = 0;
   ActorNode* iter = m_poOnScreenList.front();
   while (iter != m_poOnScreenList.end())
   {
      //if (dbg)
      //{
      //   outputActors( actor, *iter->m_poActor );
      //}

      if (iter->m_poActor->isOnScreen() && 
         iter->m_poActor->hasFlags( flags ) &&
         actor != iter->m_poActor && 
         exclude != iter->m_poActor &&
         intersect( box, *iter->m_poActor ))
      {
         result.actor = iter->m_poActor;
         return true;
      }
      iter = iter->m_poNext;
      ++nCount;
   }
   return false;
}


s32 ActorDrawManager::findFreeMemoryForTileData( u32 size )
{
   // debug output...
   /*
   {
      ActorNode* iter = m_poOnScreenList.front();
      u32 nCount = 0;
      char buf[256];
      print( "\n---------\nlist:\n" );
      while (iter != m_poOnScreenList.end())
      {
         sprintf( buf, "%d: [%s] start:%d size:%d\n", nCount, iter->m_poActor->name(), iter->m_nStart, iter->m_nSize );
         print( buf );
         iter = iter->m_poNext;
         ++nCount;
      }
      print( "\n" );
   }
   */

   const u32 SPRITE_DATA_START = 0; // starts at 0... 
   const u32 SPRITE_DATA_SIZE = 0x4000;
   u32 endOfMem = SPRITE_DATA_START;
   u32 beginningOfNextMem = SPRITE_DATA_SIZE;
   for (ActorNode* iter = m_poOnScreenList.front();
        iter != m_poOnScreenList.end();
        iter = iter->m_poNext)
   {
      beginningOfNextMem = iter->m_nStart;
      if (size <= (beginningOfNextMem - endOfMem))
      {
         return endOfMem;
      }
      endOfMem = iter->m_nStart + iter->m_nSize;
   }
   if (size < (beginningOfNextMem - endOfMem))
   {
      return endOfMem;
   }
   return -1;
}

void ActorDrawManager::buildVisibleList()
{
   for (u16 x = 0; x < m_nNumActors; ++x)
   {
      if (m_ppoActors[x]->m_bIsOnScreen &&
          (!m_ppoActors[x]->isOnScreen() ||
           !m_ppoActors[x]->m_bIsDrawing ||
           !m_ppoActors[x]->m_bIsEnabled))
      {
         removeFromDrawList( m_ppoActors[x] );
         m_ppoActors[x]->m_bIsOnScreen = false;
         m_ppoActors[x]->doThing( BehaviourMethod::OFFSCREEN, NULL );
      }
      else if (!m_ppoActors[x]->m_bIsOnScreen &&
               m_ppoActors[x]->isOnScreen() &&
               m_ppoActors[x]->m_bIsDrawing &&
               m_ppoActors[x]->m_bIsEnabled)
      {
         addToDrawList( m_ppoActors[x] );
         m_ppoActors[x]->m_bIsOnScreen = true;
      }
   }
}

void ActorDrawManager::addToDrawList( Actor* actor )
{
   s32 freemem = findFreeMemoryForTileData( actor->bitmap()->ps() );
   if (-1 != freemem)
   {
      ActorNode* an = m_aoPool.take();
      if (an)
      {
         an->m_nSize = actor->bitmap()->ps();
         an->m_nStart = freemem;
         an->m_poActor = actor;
#ifndef WIN32
         an->m_oSprite.init( actor->bitmap(), an->m_nStart, actor->depth() );
         an->m_oSprite.setPos( actor->m_oPos[0], actor->m_oPos[1] );
#endif
         m_poOnScreenList.insert( an );
         int visible = (int)actor->hasFlags( ACTOR_VISIBLE );
         mNumVisible += visible;
         mNumInvisible += !visible;
      }
   }
}

void ActorDrawManager::removeFromDrawList( Actor* actor )
{
   // find the actor node to remove:
   ActorNode* iter = m_poOnScreenList.front();
   //u16 nCount = 0;
   while (iter != m_poOnScreenList.end() && iter->m_poActor != actor)
   {
      iter = iter->m_poNext;
      //++nCount;
   }

   // remove it
   if (iter != m_poOnScreenList.end())
   {
      int visible = (int)iter->m_poActor->hasFlags( ACTOR_VISIBLE );
#ifndef WIN32
      iter->m_oSprite.release();
      if (visible)
      {
         iter->m_oSprite.uploadOamInfo( mNumVisible - 1 ); // the last one is going to become unused, overwrite it.
      }
#endif
      mNumVisible -= visible;
      mNumInvisible -= !visible;
      m_poOnScreenList.remove( iter );
      m_aoPool.putback( iter );
   }
}



