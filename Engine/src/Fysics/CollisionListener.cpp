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
            CheckCollisionEntityMap(entityA, entityB);
            //std::cout << "Entity "<< entityA->id << " collided with " << entityB->id << std::endl;
        }
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
