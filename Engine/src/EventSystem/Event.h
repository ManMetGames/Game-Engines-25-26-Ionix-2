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
        WindowClosed
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

    // .. add more as needed...
}