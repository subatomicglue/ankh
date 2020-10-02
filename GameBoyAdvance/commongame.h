
#ifndef GAME_COMMON_INCLUDED_H
#define GAME_COMMON_INCLUDED_H

#include "InGameState.h"
#include "ActorDrawManager.h"
#include "Background.h"

// todo try putting this into ingamestate.h   resolving the include dependency hell...

inline bool intersectAllActors( const AnkhActor& actor, s32 actorFlags = 0x0 )
{
   ActorDrawManager::IntersectResult result;
   return gInGameState.m_oActorDrawManager.intersectVisibleActors( actor, result, &actor, actorFlags );
}

inline bool intersectAllActors( const AnkhActor& actor, AnkhActor* &hitactor, const AnkhActor* exclude, s32 actorFlags = 0x0 )
{
   ActorDrawManager::IntersectResult result;
   bool retval = gInGameState.m_oActorDrawManager.intersectVisibleActors( actor, result, exclude, actorFlags );
   hitactor = ((AnkhActor*)result.actor);
   return retval;
}

inline bool intersectAllActors( const gba::AABox2<s32>& box, const Actor* actor, AnkhActor* &hitactor, const AnkhActor* exclude, s32 actorFlags = 0x0 )
{
   ActorDrawManager::IntersectResult result;
   bool retval = gInGameState.m_oActorDrawManager.intersectVisibleActors( box, actor, result, exclude, actorFlags );
   hitactor = ((AnkhActor*)result.actor);
   return retval;
}


inline bool intersectMap( const AnkhActor& actor )
{
   return intersect( gInGameState.mMap, (const Actor&)actor );
}


#endif
