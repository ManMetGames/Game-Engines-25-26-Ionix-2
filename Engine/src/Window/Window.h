#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>
#include <memory>
#include "Architecture/Macros.h"

namespace IonixEngine
{
    // Forward declarations
    class EventSDL;
    class IonixEvent;

    struct WindowDetails
    {
        std::string Title;
        unsigned int Width;
        unsigned int Height;

        WindowDetails(const std::string& title = "Ionix Engine 2",
            unsigned int width = 960,
            unsigned int height = 640)
            : Title(title), Width(width), Height(height)
        {
        }
    };

    struct WindowData
    {
        std::string Title;
        unsigned int Width, Height;
    };

    class ENGINE_API Window
    {
    public:
        Window(const WindowDetails& details = WindowDetails());
        virtual ~Window();

        void OnUpdate();

        inline SDL_Window* GetSdlWindow() const { return m_Window; }
        inline SDL_Renderer* GetSdlRenderer() const { return m_Renderer; }

        virtual void Init(const WindowDetails& details);
        virtual void ShutDown();

        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;
        WindowData m_Data;

    private:
        EventSDL* m_sdlEvent;
    };
}