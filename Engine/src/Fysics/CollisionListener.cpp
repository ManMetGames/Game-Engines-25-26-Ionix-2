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
}
