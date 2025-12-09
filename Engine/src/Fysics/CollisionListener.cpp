#include "CollisionListener.h"
#include "FysicsManager.h"
#include "Architecture/ECS/Entity.hpp"
#include "EventSystem/Event.h"

namespace IonixEngine
{
    CollisionListener::CollisionListener(FysicsManager* manager)
        : fysicsManager(manager)
    {
    }

    void CollisionListener::BeginContact(b2Contact* contact)
    {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        Entity* entityA = fysicsManager->GetEntityFromBody(bodyA);
        Entity* entityB = fysicsManager->GetEntityFromBody(bodyB);

 

        if (entityA && entityB && m_EventCallback)
        {
            // Create and dispatch collision event through callback
            CollisionEnterEvent event(entityA, entityB);
            m_EventCallback(event);
            if (!CheckActiveCollisions(entityA, entityB))
            {
                activeCollisions[entityA->id].insert(entityB->id);
                activeCollisions[entityB->id].insert(entityA->id);
            }
            CheckTrigger(entityA, entityB);
        }
    }

    void CollisionListener::CheckTrigger(Entity* entityA, Entity* entityB)
    {
                
        if (fysicsManager->GetFixtureFromEntity(entityA)->IsSensor() || fysicsManager->GetFixtureFromEntity(entityB)->IsSensor())
        {
            Scripting::Get().CallHook("OnTriggerEnter", entityA, entityB);
        }

        else
        {
            Scripting::Get().CallHook("OnCollisionEnter", entityA, entityB);
        }
    }

    void CollisionListener::CheckTriggerExit(Entity* entityA, Entity* entityB)
    {
                
        if (fysicsManager->GetFixtureFromEntity(entityA)->IsSensor() || fysicsManager->GetFixtureFromEntity(entityB)->IsSensor())
        {
            Scripting::Get().CallHook("OnTriggerExit", entityA, entityB);
        }

        else
        {
            Scripting::Get().CallHook("OnCollisionExit", entityA, entityB);
        }
    }

    bool CollisionListener::CheckActiveCollisions(Entity* entityA, Entity* entityB)
    {
        for (const auto& pair : Application::Get().layerFysics->GetFysicsManager()->GetCollisionListener()->activeCollisions)
        {
            for (EntityID entB: pair.second )
            {
                EntityID entA = pair.first;   // key
                // Do your check here
                if (entA == entityA->id && entB == entityB->id) {
                    return true;
                }
                return false;
            }
        }
        return false;
    }

    void CollisionListener::AddEntityBodiesToDestroy(b2Body* entityToDestroy)
    {
        entityBodiesToDestroy.push_back(entityToDestroy);
    }

    void CollisionListener::EndContact(b2Contact* contact)
    {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        Entity* entityA = fysicsManager->GetEntityFromBody(bodyA);
        Entity* entityB = fysicsManager->GetEntityFromBody(bodyB);


        
        if (entityA && entityB && m_EventCallback)
        {
            // Create and dispatch collision event through callback
            CollisionExitEvent event(entityA, entityB);
            m_EventCallback(event);
            CheckTriggerExit(entityA, entityB);

            activeCollisions[entityA->id].erase(entityB->id);
            activeCollisions[entityB->id].erase(entityA->id);
        }
    }

    void CollisionListener::CheckCollisionEntityMap(Entity* colA, Entity* colB)
    {
        EntityID a = colA->id;
        EntityID b = colB->id;
        for (const auto& pair : collisionEntityMap) {
            for (EntityID entB: pair.second )
            {
                EntityID entityA = pair.first;   // key
                

                // Do your check here
                if (entityA == a && entB == b) {
                    std::cout << "Entities are paired" << std::endl;
                    // Example: print, compare, or run logic
                }
            }
        }
    }

    void CollisionListener::AddToCollisionMap(Entity* entityA, Entity* entityB)
    {
        collisionEntityMap[entityA->id].push_back(entityB->id);
        
    }
}

