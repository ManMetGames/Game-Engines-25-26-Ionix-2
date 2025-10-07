#include "EventSystem/EventSDL.h"
#include "EventSystem/Event.h"
#include "Window/Window.h"
#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>

namespace IonixEngine
{
    void EventSDL::PollEventsSDL(const SDL_Event& e, WindowData& windowData)
    {
        // Let ImGui handle the event first if needed:
        // ImGui_ImplSDL2_ProcessEvent(&e);

        switch (e.type)
        {
        case SDL_WINDOWEVENT:
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                if (m_EventCallback)
                {
                    WindowClosedEvent event;
                    m_EventCallback(event);
                }
                break;

            case SDL_WINDOWEVENT_MINIMIZED:
                if (m_EventCallback)
                {
                    WindowMinimizedEvent event;
                    m_EventCallback(event);
                }
                break;
            


            case SDL_WINDOWEVENT_MAXIMIZED:
                if (m_EventCallback)
                {
                    WindowMaximizedEvent event;
                    m_EventCallback(event);
                }
                break;

            case SDL_WINDOWEVENT_RESIZED:
                if (m_EventCallback)
                {
                    WindowResizedEvent event(e.window.data1, e.window.data2);
                    m_EventCallback(event);
                }
                break;

            case SDL_WINDOWEVENT_MOVED:
                if (m_EventCallback)
                {
                    WindowMovedEvent event(e.window.data1, e.window.data2);
                    m_EventCallback(event);
                }
                break;

                
            case SDL_WINDOWEVENT_HIDDEN:
                if (m_EventCallback)
                {
                    WindowHiddenEvent event;
                    m_EventCallback(event);
                }
                break;
                //Window Restored
            case SDL_WINDOWEVENT_RESTORED: 
                if (m_EventCallback)
                {
                    WindowRestoredEvent event;
                    m_EventCallback(event);
                }

            }
          

        }

   

    }
}