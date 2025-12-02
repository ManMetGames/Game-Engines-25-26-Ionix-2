#pragma once
#include "box2d.h"
#include <functional>
#include <unordered_set>
#include <string>

#include "Architecture/ECS/Entity.hpp"

namespace IonixEngine
{
    class CollisionEnterEvent;
    class FysicsManager;
    class IonixEvent;

    class CollisionListener : public b2ContactListener
    {
    public:
        using EventCallback = std::function<void(IonixEvent&)>;
        std::unordered_map<EntityID, std::vector<EntityID>> collisionEntityMap;
        std::unordered_map<EntityID, std::unordered_set<EntityID>> activeCollisions; 
        std::vector<b2Body*> entityBodiesToDestroy;


        CollisionListener(FysicsManager* manager);

        // Box2D contact callbacks
        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
        void SetEventCallback(const EventCallback& callback) { m_EventCallback = callback;}
        void CheckTrigger(Entity* entityA, Entity* entityB);
        void CheckTriggerExit(Entity* entityA, Entity* entityB);

        void AddToCollisionMap(Entity* entityA, Entity* entityB);
        void CheckCollisionEntityMap(Entity* colA, Entity* colB);
        bool CheckActiveCollisions(Entity* entityA, Entity* entityB);

        void AddEntityBodiesToDestroy(b2Body* entityToDestroy);

    private:
        FysicsManager* fysicsManager;
        EventCallback m_EventCallback;
    };
}
