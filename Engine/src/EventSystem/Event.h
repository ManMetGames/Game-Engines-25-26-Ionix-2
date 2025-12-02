#pragma once
#include "Architecture/Macros.h"
#include <SDL_stdinc.h>
namespace IonixEngine
{
    class Entity;
    class ControllerManager; // forward declare to avoid circular dependency

    enum class IonixEventType
    {
        None = 0,
        WindowClosed,
        CollisionEnter,
        CollisionExit,
        ControllerButtonDown,
        ControllerButtonUp,
        ControllerLAxisX,
        ControllerLAxisY,
        ControllerRAxisX,
        ControllerRAxisY,
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

    // Controller Events
    class ControllerButtonDownEvent : public IonixEvent
    {
    public:
        ControllerManager* controller;
        int instanceId;
        Uint8 button;

        ControllerButtonDownEvent(ControllerManager* controller, int instanceId, Uint8 button)
            : IonixEvent(IonixEventType::ControllerButtonDown), controller(controller), instanceId(instanceId), button(button){}
    };

    class ControllerButtonUpEvent : public IonixEvent
    {
    public:
        ControllerManager* controller;
        int instanceId;
        Uint8 button;

        ControllerButtonUpEvent(ControllerManager* controller, int instanceId, Uint8 button)
            : IonixEvent(IonixEventType::ControllerButtonUp), controller(controller), instanceId(instanceId), button(button) {
        }
    };

    class ControllerAxisEvent : public IonixEvent
    {
    public:
        ControllerManager* controller;
        int instanceId;
        float axis;
        Uint8 direction;

        ControllerAxisEvent(ControllerManager* controller, int instanceId, float axis, Uint8 direction)
            : IonixEvent(IonixEventType::ControllerLAxisX), controller(controller), instanceId(instanceId), axis(axis), direction(direction) {
        }
    };

    // .. add more as needed...
}