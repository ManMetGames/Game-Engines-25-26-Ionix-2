#include "EventSystem/EventSDL.h"
#include "EventSystem/Event.h"
#include "Window/Window.h"
#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include "Architecture/Application.h"
#include <iostream>
#include <iomanip>

namespace IonixEngine
{
    void EventSDL::PollEventsSDL(const SDL_Event& e, WindowData& windowData)
    {
        //Let ImGui handle the event first if needed:
        ImGui_ImplSDL2_ProcessEvent(&e); 

        //Store connected controllers 
        static std::vector<SDL_GameController*> controllers;

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
        {
            if (controllers.size() < MAX_CONNECTIONS) //Allow up to 4 controllers
            {
                SDL_GameController* controller = SDL_GameControllerOpen(e.cdevice.which);
                if (controller)
                {
                    controllers.push_back(controller);
                    std::cout << "Controller Connected! Total: " << controllers.size() << "\n";

          case SDL_MOUSEWHEEL:
              float scrollY = static_cast<float>(e.wheel.y);
              Application::Get().layerInput->m_Input->SetScrollDiff(scrollY);
              break;
                }
            }
            break;

        }
        case SDL_CONTROLLERDEVICEREMOVED:
        {
            int instanceId = e.cdevice.which;
            for (auto it = controllers.begin(); it != controllers.end(); ++it)
            {
                if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(*it)) == instanceId)
                {
                    SDL_GameControllerClose(*it);
                    controllers.erase(it);
                    std::cout << "Controller Disconnected! Total: " << controllers.size() << "\n";
                    break;
                }
            }
            break;
        }

            case SDL_CONTROLLERBUTTONDOWN:
                Application::Get().layerInput->m_Input->SetButtonPressed(e.cbutton.button);
                break;
            case SDL_CONTROLLERBUTTONUP:
                Application::Get().layerInput->m_Input->SetButtonReleased(e.cbutton.button);
                break;
            case SDL_CONTROLLERAXISMOTION:
                switch (e.caxis.axis)
                {
                    case SDL_CONTROLLER_AXIS_LEFTX:
                    {
                        double val = e.caxis.value;
                        float normalised = Application::Get().layerInput->m_Input->NormaliseStickAxis(val);
                        //std::cout << normalised << "\n";
                        break;
                    }
                    case SDL_CONTROLLER_AXIS_LEFTY:
                    {
                        double val = e.caxis.value;
                        float normalised = Application::Get().layerInput->m_Input->NormaliseStickAxis(val);
                        //std::cout << normalised << "\n";
                        break;
                    }
                    case SDL_CONTROLLER_AXIS_RIGHTX:
                    {
                        double val = e.caxis.value;
                        float normalised = Application::Get().layerInput->m_Input->NormaliseStickAxis(val);
                        //std::cout << normalised << "\n";
                        break;
                    }
                    case SDL_CONTROLLER_AXIS_RIGHTY:
                    {
                        double val = e.caxis.value;
                        float normalised = Application::Get().layerInput->m_Input->NormaliseStickAxis(val);
                        //std::cout << normalised << "\n";
                        break;
                    }
                    case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                    {
                        double val = e.caxis.value;
                        float normalised = Application::Get().layerInput->m_Input->NormaliseTrigger(val); 
                        //std::cout << normalised << "\n";
                        break;
                    }
                    case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                    {
                        double val = e.caxis.value;
                        float normalised = Application::Get().layerInput->m_Input->NormaliseTrigger(val);
                        //std::cout << normalised << "\n";
                        break;
                    }
                break;
                }
        }
    }
}
