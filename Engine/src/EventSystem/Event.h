#pragma once
#include "Architecture/Macros.h"

namespace IonixEngine
{
    class Entity;

    enum class IonixEventType
    {
        None = 0,
        WindowClosed,
        CollisionEnter,
        CollisionExit
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

    // Collision Events
    class CollisionEnterEvent : public IonixEvent
    {
    public:
        Entity* EntityA;
        Entity* EntityB;

        CollisionEnterEvent(Entity* entityA, Entity* entityB)
            : IonixEvent(IonixEventType::CollisionEnter), EntityA(entityA), EntityB(entityB) {}
    };

    class CollisionExitEvent : public IonixEvent
    {
    public:
        Entity* EntityA;
        Entity* EntityB;

        CollisionExitEvent(Entity* entityA, Entity* entityB)
            : IonixEvent(IonixEventType::CollisionExit), EntityA(entityA), EntityB(entityB) {}
    };

    // .. add more as needed...
}