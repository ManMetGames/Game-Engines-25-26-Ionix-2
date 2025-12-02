#pragma once
#include "box2d.h"
#include <functional>
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

        CollisionListener(FysicsManager* manager);

        // Box2D contact callbacks
        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
        void SetEventCallback(const EventCallback& callback) { m_EventCallback = callback;}
        void CheckTrigger(Entity* entityA, Entity* entityB);

        void AddToCollisionMap(Entity* entityA, Entity* entityB);
        void CheckCollisionEntityMap(Entity* colA, Entity* colB);

    private:
        FysicsManager* fysicsManager;
        EventCallback m_EventCallback;
    };
}
