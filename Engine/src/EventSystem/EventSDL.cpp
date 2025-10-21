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
            case SDL_KEYDOWN:
                Application::Get().layerInput->m_Input->SetKeyPressed(e.key.keysym.scancode); 
                break;

            case SDL_KEYUP: 
                Application::Get().layerInput->m_Input->SetKeyReleased(e.key.keysym.scancode);
                break;

                //Mouse
           case SDL_MOUSEBUTTONDOWN:
               Uint8 buttonPressed = e.button.button;
               // left is 1
               // middle is 2
               // right is 3
               // 
                Application::Get().layerInput->m_Input->IsMouseButtonDown();
               break;

  

        }
    }
}
