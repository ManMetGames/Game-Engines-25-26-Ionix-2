#pragma once
#include <SDL.h>
#include <functional>

namespace IonixEngine
{
    // Forward declarations
    struct WindowData;
    class IonixEvent;

    class EventSDL
    {
    public:
        using EventCallback = std::function<void(IonixEvent&)>;

        EventSDL() = default;
        ~EventSDL() = default;

        void PollEventsSDL(const SDL_Event& e, WindowData& windowData);
        void SetEventCallback(const EventCallback& callback) { m_EventCallback = callback; }

    private:
        EventCallback m_EventCallback;
    };
}