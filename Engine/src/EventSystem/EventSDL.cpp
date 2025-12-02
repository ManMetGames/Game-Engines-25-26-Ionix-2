#include "EventSystem/EventSDL.h"
#include "EventSystem/Event.h"
#include "Window/Window.h"
#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include "Architecture/Application.h"
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <Input/MicrophoneManager.h>
#include "Input/ControllerManager.h"

namespace IonixEngine
{
    void EventSDL::PollEventsSDL(const SDL_Event& e, WindowData& windowData)
    {
        //Let ImGui handle the event first if needed:
        ImGui_ImplSDL2_ProcessEvent(&e);

        //Store connected controllers 
        //static std::vector<SDL_GameController*> controllers;

        static std::unordered_map<int, SDL_GameController*> controllers;

        //microphone
        static std::unordered_map<SDL_AudioDeviceID, SDL_AudioSpec> microphones;
        static std::unordered_map<SDL_AudioDeviceID, MicrophoneManager> microphoneManagers;





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


            //Microphone Connected 
        case SDL_AUDIODEVICEADDED:
        {
            const char* deviceName = SDL_GetAudioDeviceName(e.adevice.which, SDL_TRUE);
            if (deviceName)
            {





            }
            break;
        }

        //Microphone disconnected
        case SDL_AUDIODEVICEREMOVED:
        {


        }



        // Controller
        case SDL_CONTROLLERDEVICEADDED:
        {
            SDL_GameController* controller = SDL_GameControllerOpen(e.cdevice.which);
            if (controller)
            {
                int instanceId = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller)); //Stores "Controller just added" in instandID
                controllers[instanceId] = controller;
                Application::Get().layerInput->GetControllerManager()[instanceId] = new ControllerManager(instanceId);

                std::cout << "Player " << instanceId << " connected! Total: " << controllers.size() << "\n";
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
                Application::Get().layerInput->GetControllerManager().erase(instanceId);

                std::cout << "Player " << instanceId << " disconnected! Total: " << controllers.size() << "\n";
            }
            break;
        }


        case SDL_CONTROLLERBUTTONDOWN:
        {
            // We could raise an event with the controller and the id
            // Event gets picked up in the input layer and finds the controller with the id
            // invokes the respective get button down method

            // Probably need to migrate dictionary to the input layer?
            int instanceId = e.cbutton.which;
            if (Application::Get().layerInput->GetControllerManager().count(instanceId))
            {
                Application::Get().layerInput->GetControllerManager()[instanceId]->SetButtonPressed(e.cbutton.button);

                // Raise an event - now we know who pressed the button, and their instanceID
                // This can be picked up and processed in the Input Layer
                if (m_EventCallback)
                {
                    ControllerButtonDownEvent event(Application::Get().layerInput->GetControllerManager()[instanceId], instanceId, e.button.which);
                    m_EventCallback(event);
                }
            }
            break;
        }

        case SDL_CONTROLLERBUTTONUP:
        {
            int instanceId = e.cbutton.which;
            if (Application::Get().layerInput->GetControllerManager().count(instanceId))
            {
                Application::Get().layerInput->GetControllerManager()[instanceId]->SetButtonReleased(e.cbutton.button);

                if (m_EventCallback)
                {
                    ControllerButtonUpEvent event(Application::Get().layerInput->GetControllerManager()[instanceId], instanceId, e.button.which);
                    m_EventCallback(event);
                }
            }
            break;
        }
        case SDL_CONTROLLERAXISMOTION:
            switch (e.caxis.axis)
            {

            case SDL_CONTROLLER_AXIS_LEFTY:
            {
                int instanceId = e.caxis.which;
                if (Application::Get().layerInput->GetControllerManager().count(instanceId))
                {
                    if (m_EventCallback)
                    {
                        ControllerAxisEvent event(Application::Get().layerInput->GetControllerManager()[instanceId], instanceId, e.caxis.value, e.caxis.axis);
                        m_EventCallback(event);
                    }
                }

                break;
            }
            case SDL_CONTROLLER_AXIS_LEFTX:
            {
                int instanceId = e.caxis.which;
                if (Application::Get().layerInput->GetControllerManager().count(instanceId))
                {
                    if (m_EventCallback)
                    {
                        ControllerAxisEvent event(Application::Get().layerInput->GetControllerManager()[instanceId], instanceId, e.caxis.value, e.caxis.axis);
                        m_EventCallback(event);
                    }
                }

                break;
            }
            case SDL_CONTROLLER_AXIS_RIGHTX:
            {

                int instanceId = e.caxis.which;
                if (Application::Get().layerInput->GetControllerManager().count(instanceId))
                {
                    if (m_EventCallback)
                    {
                        ControllerAxisEvent event(Application::Get().layerInput->GetControllerManager()[instanceId], instanceId, e.caxis.value, e.caxis.axis);
                        m_EventCallback(event);
                    }
                }

                break;
            }
            case SDL_CONTROLLER_AXIS_RIGHTY:
            {
                int instanceId = e.caxis.which;
                if (Application::Get().layerInput->GetControllerManager().count(instanceId))
                {
                    if (m_EventCallback)
                    {
                        ControllerAxisEvent event(Application::Get().layerInput->GetControllerManager()[instanceId], instanceId, e.caxis.value, e.caxis.axis);
                        m_EventCallback(event);
                    }
                }
                break;
            }
            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
            {
                int instanceId = e.caxis.which;
                if (Application::Get().layerInput->GetControllerManager().count(instanceId))
                {
                    if (m_EventCallback)
                    {
                        ControllerTriggerEvent event(Application::Get().layerInput->GetControllerManager()[instanceId], instanceId, e.caxis.value, e.caxis.axis);
                        m_EventCallback(event);
                    }
                }
                break;
            }
            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
            {
                int instanceId = e.caxis.which;
                if (Application::Get().layerInput->GetControllerManager().count(instanceId))
                {
                    if (m_EventCallback)
                    {
                        ControllerTriggerEvent event(Application::Get().layerInput->GetControllerManager()[instanceId], instanceId, e.caxis.value, e.caxis.axis);
                        m_EventCallback(event);
                    }
                }
                break;
            }
            break;
            }

        }
    }
}

