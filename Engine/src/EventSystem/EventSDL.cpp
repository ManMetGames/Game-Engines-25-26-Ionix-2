#include "EventSystem/EventSDL.h"
#include "EventSystem/Event.h"
#include "Window/Window.h"
#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include "Architecture/Application.h"
#include <iostream>
#include <iomanip>
#include <unordered_map>

namespace IonixEngine
{
    void EventSDL::PollEventsSDL(const SDL_Event& e, WindowData& windowData)
    {
        //Let ImGui handle the event first if needed:
        ImGui_ImplSDL2_ProcessEvent(&e); 

        //Store connected controllers 
        //static std::vector<SDL_GameController*> controllers;

        static std::unordered_map<int, SDL_GameController*> controllers;
        static std::unordered_map<int, ControllerManager> controllerManagers;

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
            SDL_GameController* controller = SDL_GameControllerOpen(e.cdevice.which);
            if (controller)
            {
                int instanceId = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));
                controllers[instanceId] = controller;
                controllerManagers[instanceId] = ControllerManager();

                std::cout << "Controller " << instanceId << " connected! Total: " << controllers.size() << "\n";
            }
            break;
        }
        case SDL_CONTROLLERDEVICEREMOVED:
        {
            int instanceId = e.cdevice.which;
            if (controllers.count(instanceId))
            {
                SDL_GameControllerClose(controllers[instanceId]);
                controllers.erase(instanceId);
                controllerManagers.erase(instanceId);

                std::cout << "Controller " << instanceId << " disconnected! Total: " << controllers.size() << "\n";
            }
            break;
        }


        case SDL_CONTROLLERBUTTONDOWN:
        {
            int instanceId = e.cbutton.which;
            if (controllerManagers.count(instanceId))
            {
                controllerManagers[instanceId].SetButtonPressed(e.cbutton.button);
                std::cout << "Controller " << instanceId << " pressed button "
                    << static_cast<int>(e.cbutton.button) << "\n";
            }
            break;
        }

            case SDL_CONTROLLERBUTTONUP:
            {
            int instanceId = e.cbutton.which;
            if (controllerManagers.count(instanceId))
            {
                controllerManagers[instanceId].SetButtonReleased(e.cbutton.button);
                std::cout << "Controller " << instanceId << " released button "
                    << static_cast<int>(e.cbutton.button) << "\n";
            }
            break;
            }


                case SDL_CONTROLLER_AXIS_LEFTY:
                {
                    double val = e.caxis.value;
                    float normalised = Application::Get().layerInput->m_ControllerManager->NormaliseStickAxis(val);
                    //std::cout << normalised << "\n";
                    break;
                }
                case SDL_CONTROLLER_AXIS_RIGHTX:
                {
                    double val = e.caxis.value;
                    float normalised = Application::Get().layerInput->m_ControllerManager->NormaliseStickAxis(val);
                    //std::cout << normalised << "\n";
                    break;
                }
                case SDL_CONTROLLER_AXIS_RIGHTY:
                {
                    double val = e.caxis.value;
                    float normalised = Application::Get().layerInput->m_ControllerManager->NormaliseStickAxis(val);
                    //std::cout << normalised << "\n";
                    break;
                }
                case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
                {
                    double val = e.caxis.value;
                    float normalised = Application::Get().layerInput->m_ControllerManager->NormaliseTrigger(val);
                    //std::cout << normalised << "\n";
                    break;
                }
                case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
                {
                    double val = e.caxis.value;
                    float normalised = Application::Get().layerInput->m_ControllerManager->NormaliseTrigger(val);
                    //std::cout << normalised << "\n";
                    break;
                }
                break;
                }
        }
    }

