#include "Window/Window.h"
#include "EventSystem/EventSDL.h"
#include "EventSystem/Event.h"
#include "Architecture/Application.h"
#include <iostream>
#include <SDL_mixer.h>

namespace IonixEngine
{
    Window::Window(const WindowDetails& props)
        : m_sdlEvent(new EventSDL())
    {
        Init(props);

        // Set up event callback to forward events to the Application
        m_sdlEvent->SetEventCallback([](IonixEvent& e) {
            Application::Get().OnEvent(e);
            });
    }

    Window::~Window()
    {
        ShutDown();
    }

    void Window::Init(const WindowDetails& details)
    {
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        // Initialize SDL Mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
            SDL_Log("Unable to initialize SDL_mixer: %s", Mix_GetError());
            return;
        }

        // Create SDL window
        m_Window = SDL_CreateWindow(
            details.Title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            static_cast<int>(details.Width),
            static_cast<int>(details.Height),
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );
        if (!m_Window) {
            std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
            return;
        }

        // Create SDL renderer
        m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!m_Renderer) {
            std::cerr << "Failed to create SDL renderer: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(m_Window);
            return;
        }

        // Store window data
        m_Data.Title = details.Title;
        m_Data.Width = details.Width;
        m_Data.Height = details.Height;
        SDL_SetWindowData(m_Window, "customData", &m_Data);
    }

    void Window::ShutDown()
    {
        if (m_Renderer) {
            SDL_DestroyRenderer(m_Renderer);
            m_Renderer = nullptr;
        }
        if (m_Window) {
            SDL_DestroyWindow(m_Window);
            m_Window = nullptr;
        }
        Mix_CloseAudio();
        SDL_Quit();
    }

    void Window::OnUpdate()
    {
        // SDL polling entry point
        SDL_Event e;

        while (SDL_PollEvent(&e) != 0)
            m_sdlEvent->PollEventsSDL(e, m_Data);

        // Extend as required...
    }
}