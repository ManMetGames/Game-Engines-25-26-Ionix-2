#pragma once
#include "Architecture/Macros.h"

namespace IonixEngine
{
    enum class IonixEventType
    {
        None = 0,
        WindowClosed,
        WindowMinimized,
        WindowMaximized
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

    class WindowMinimizedEvent : public IonixEvent
    {
    public:
        WindowMinimizedEvent() : IonixEvent(IonixEventType::WindowMinimized) {}
    };

    class WindowMaximizedEvent : public IonixEvent
    {
    public:
        WindowMaximizedEvent() : IonixEvent(IonixEventType::WindowMaximized) {}
    };

    // .. add more as needed...
}