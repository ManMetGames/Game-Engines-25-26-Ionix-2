#pragma once
#include "Architecture/Macros.h"
#include "Architecture/ECS/Entity.hpp"
namespace IonixEngine
{
    enum class IonixEventType
    {
        None = 0,
        WindowClosed,
        EntityCollision
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
        EntityCollisionEvent(Entity ent1, Entity ent2) : IonixEvent(IonixEventType::EntityCollision), ent1(ent1), ent2(ent2) {}

    private:
        Entity ent1, ent2;
    };


    // .. add more as needed...
}