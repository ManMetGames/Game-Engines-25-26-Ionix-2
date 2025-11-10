#pragma once
#include "Architecture/Macros.h"
#include "Architecture/ECS/Entity.hpp"
#include <box2d.h>
namespace IonixEngine
{
    enum class IonixEventType
    {
        None = 0,
        WindowClosed,
        EntityCollision,
        CollisionBegin,
        CollisionEnd,
        // ...
    };

    // Event class
    class IonixEvent
    {
    public:
        IonixEventType Type = IonixEventType::None;
        bool Handled = false;

        IonixEvent(IonixEventType type) : Type(type) {}
        virtual ~IonixEvent() = default;
    };

    // Window Events
    class WindowClosedEvent : public IonixEvent
    {
    public:
        WindowClosedEvent() : IonixEvent(IonixEventType::WindowClosed) {}
    };

    // Physics Events
    class EntityCollisionEvent : public IonixEvent
    {
    public:
        EntityCollisionEvent(b2Fixture* fixA, b2Fixture* fixB) : IonixEvent(IonixEventType::EntityCollision), fixtureA(fixA), fixtureB(fixB) 
        {
        	bodyA = fixtureA->GetBody();
            bodyB = fixtureB->GetBody();
        }

    private:
        b2Fixture* fixtureA;
        b2Fixture* fixtureB;
        b2Body* bodyA;
        b2Body* bodyB;
    };


    // .. add more as needed...
}