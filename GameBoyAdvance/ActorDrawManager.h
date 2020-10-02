
#ifndef ACTORDRAWMANAGER_H
#define ACTORDRAWMANAGER_H

#include "Actor.h"
class AnkhActor;

/// handles drawing actors.
/// based on visibility, handles paging in/out of large pools of sprites
class ActorDrawManager
{
public:
   ActorDrawManager() { reset(); }
   inline void init( AnkhActor** ppoActors, u16 nNumActors )
   {
      m_ppoActors = ppoActors;
      m_nNumActors = nNumActors;
      ::InitOAM();
   }
   inline void reset()
   {
      m_ppoActors = NULL;
      m_nNumActors = 0;
      mNumVisible = 0;
      mNumInvisible = 0;
   }
   inline void release()
   {
      reset();
   }

   void update();
   void updateBehaviours();

   /// upload the sprite info to the Object Attribute Memory
   void uploadOamInfo();

   // test intersection with all actors
   struct IntersectResult
   {
      IntersectResult() : actor( NULL ) {}
      Actor* actor;
   };
   bool intersectVisibleActors( const gba::Vec<s32>& target, IntersectResult& result );

   /// intersect actor against all visible actors...
   bool intersectVisibleActors( const Actor& actor, IntersectResult& result, const Actor* exclude = NULL, s32 flags = 0x0 );

   /// intersect box against all visible actors...
   bool intersectVisibleActors( const gba::AABox2<s32>& box, const Actor* actor, IntersectResult& result, const Actor* exclude, s32 flags = 0x0 );

private:
   s32 findFreeMemoryForTileData( u32 size );

   void buildVisibleList();
   void addToDrawList( Actor* actor );

   void removeFromDrawList( Actor* actor );

   /// memory header, the linked list is always in order of the memory allocation.
   struct ActorNode
   {
      ActorNode() : m_nStart( 0 ), m_nSize( 0 ), m_poActor( NULL ), m_poPrev( NULL ), m_poNext( NULL ) {}
      u32 m_nStart;
      u32 m_nSize;
      Actor* m_poActor;
#ifndef WIN32
      gba::Sprite m_oSprite;
#endif
      ActorNode* m_poPrev;
      ActorNode* m_poNext;
   };

   AnkhActor** m_ppoActors;
   u16 m_nNumActors;
   enum { MAX_SPRITES = 128 }; //< we can have up to 128 sprites on the gba.
   SortedList<ActorNode> m_poOnScreenList;
   int mNumVisible, mNumInvisible;
   Pool<ActorNode, MAX_SPRITES> m_aoPool;
};


#endif
