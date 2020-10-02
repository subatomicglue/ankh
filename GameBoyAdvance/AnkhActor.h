#ifndef ANKHACTOR
#define ANKHACTOR

#include "twister.h"
#include "actor.h"
#include "ActorDrawManager.h"
#include "Background.h"
#include "InGameState.h"
#include "Inventory.h"
#include "data.h"

class AnkhActor;
class BehaviourMethod
{
public:
   enum Thing
   {
      TRIGGER = 1, UPDATE = 2, SHOOT = 4, TOUCH = 8,
      COLLIDE = 16, PICKUP = 32, DROP = 64, KARMA = 128,
      UP = 256, DOWN = 512, LEFT = 1024, RIGHT = 2048, STOP = 4096,
      ENTER = 8192, EXIT = 16384, OFFSCREEN = 16384 * 2
   };
   virtual void doThing( BehaviourMethod::Thing thing, AnkhActor* actor, AnkhActor* caller ) = 0;
};

class AnkhActor : public Actor
{
public:
   AnkhActor( const ActorInit& rfoInit ) : Actor( rfoInit )
   {
      reset();
   }

   AnkhActor( const ActorInit& rfoInit, AnkhActor& actor ) : Actor( rfoInit ),
                                    m_poBehaviourMethod( actor.m_poBehaviourMethod )
   {
   }
   
   AnkhActor( const ActorInit& rfoInit, 
               BehaviourMethod* poBehaviourMethod ) : Actor( rfoInit ), 
                                    m_poBehaviourMethod( poBehaviourMethod )
   {
      spew::GBAPrint( "actor init: karma %d\n", m_rfoActorInit.m_nKarmaFromTouching );
   }
   ~AnkhActor()
   {
      delete m_poBehaviourMethod;
      m_poBehaviourMethod = NULL;
   }

   void reset()
   {
      m_poBehaviourMethod = NULL;
   }

   BehaviourMethod* NewBehavior( ActorInit::Type type );

   virtual void doThing( BehaviourMethod::Thing thing, AnkhActor* caller )
   {
      if (m_poBehaviourMethod)
         m_poBehaviourMethod->doThing( thing, this, caller );
   }

   BehaviourMethod* m_poBehaviourMethod;
};


inline void showAll( ActorInit::Type t )
{
   for (int x = 0; x < gActorCount; ++x)
   {
      if (gAllActors[x]->type() == t && gAllActors[x]->m_bIsEnabled)
      {
         gAllActors[x]->show();
      }
   }
}

inline void hideAll( ActorInit::Type t )
{
   for (int x = 0; x < gActorCount; ++x)
   {
      if (t == gAllActors[x]->type())
      {
         gAllActors[x]->m_bIsDrawing = false;
      }
   }
}


#endif
