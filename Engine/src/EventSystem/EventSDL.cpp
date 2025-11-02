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
           // Controller
            case SDL_CONTROLLERDEVICEADDED:
            {
                SDL_GameController* pad = SDL_GameControllerOpen(e.cdevice.which);
                if (pad)
                {
                    std::cout << "Controller Added! Total: " << controllers.size() << "\n";

                }
                break;

            }
            case SDL_CONTROLLERDEVICEREMOVED:
            {
                SDL_GameController* pad = SDL_GameControllerOpen(e.cdevice.which);

                if (pad)
                {
                    std::cout << "Controller remove! Total: " << controllers.size() << "\n";
                }


            }
            break;

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
                       // float normalizeLeftXAxis = (float)e.caxis.value / 32768;
                        //std::cout << std::setprecision(2) << fixed << normalizeLeftXAxis << "\n";
                        double val = e.caxis.value;
                        val = (val / 32768);
                        val = std::ceil(val * 100) / 100;
                        Application::Get().layerInput->m_Input->NormaliseLeftXAxis(val);
                        std::cout << val << "\n";
                        break;
                    }
                    case SDL_CONTROLLER_AXIS_LEFTY:
                    {
                        break;
                    }
                    case SDL_CONTROLLER_AXIS_RIGHTX:
                    {
                        break;
                    }
                    case SDL_CONTROLLER_AXIS_RIGHTY:
                    {
                        break;
                    }
                    case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                    {
                        break;
                    }
                    case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                    {
                        break;
                    }
                break;
                }
        }
    }
}
