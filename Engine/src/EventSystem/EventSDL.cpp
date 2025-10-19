#include "EventSystem/EventSDL.h"
#include "EventSystem/Event.h"
#include "Window/Window.h"
#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include "Architecture/Application.h"

namespace IonixEngine
{
    void EventSDL::PollEventsSDL(const SDL_Event& e, WindowData& windowData)
    {
        //Let ImGui handle the event first if needed:
        ImGui_ImplSDL2_ProcessEvent(&e);

        switch (e.type)
        {
        case SDL_WINDOWEVENT:
            // Window
            switch (e.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                if (m_EventCallback)
                {
                    WindowClosedEvent event;
                    m_EventCallback(event);
                }
                break;
            }
            break;

            // Keyboard
            case SDL_KEYDOWN:
                Application::Get().layerInput->m_Input->SetKeyPressed(e.key.keysym.scancode); 
                break;

            case SDL_KEYUP: 
                Application::Get().layerInput->m_Input->SetKeyReleased(e.key.keysym.scancode);
                break;

            // Controller
            case SDL_CONTROLLERDEVICEADDED:

                break;
            case SDL_CONTROLLERDEVICEREMOVED:

                break;
        }
    }
}
