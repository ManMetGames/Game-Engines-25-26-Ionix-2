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

              // Mouse
             
            case SDL_MOUSEBUTTONDOWN:
               //Application::Get().layerInput->m_Input->IsMouseButtonDown(e.button.button);
               Application::Get().layerInput->m_Input->SetMousePressed(e.button.button);
               break;

          case SDL_MOUSEBUTTONUP:
              Application::Get().layerInput->m_Input->SetMouseReleased(e.button.button);
              break;

          case SDL_MOUSEWHEEL:
              float scrollY = static_cast<float>(e.wheel.y);
              Application::Get().layerInput->m_Input->SetScrollDiff(scrollY);
              break;

  

        }
    }
}
