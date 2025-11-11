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
        EntityCollisionEvent(Entity* entA, Entity* entB) : IonixEvent(IonixEventType::EntityCollision), entityA(entA), entityB(entB)
        {
            if (entA) entA->Collision(entB);
            if (entB) entB->Collision(entA);
        }

    private:
        Entity* entityA;
        Entity* entityB;
    };


    // .. add more as needed...
}