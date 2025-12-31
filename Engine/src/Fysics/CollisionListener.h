#pragma once
#include "box2d.h"
#include <functional>

namespace IonixEngine
{
    class FysicsManager;
    class IonixEvent;

    class CollisionListener : public b2ContactListener
    {
    public:
        using EventCallback = std::function<void(IonixEvent&)>;

        CollisionListener(FysicsManager* manager);

        // Box2D contact callbacks
        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;

        void SetEventCallback(const EventCallback& callback) { m_EventCallback = callback; }

    private:
        FysicsManager* fysicsManager;
        EventCallback m_EventCallback;
    };
}
