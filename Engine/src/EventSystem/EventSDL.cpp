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
                controllerManagers[instanceId] = ControllerManager();

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
                controllerManagers.erase(instanceId);

                std::cout << "Player " << instanceId << " disconnected! Total: " << controllers.size() << "\n";
            }
            break;
        }


        case SDL_CONTROLLERBUTTONDOWN:
        {
            int instanceId = e.cbutton.which;
            if (controllerManagers.count(instanceId))
            {
                controllerManagers[instanceId].SetButtonPressed(e.cbutton.button);
                std::cout << "Player " << instanceId << " pressed button "
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
                std::cout << "Player " << instanceId << " released button "
                    << static_cast<int>(e.cbutton.button) << "\n";
            }
            break;
        }
        case SDL_CONTROLLERAXISMOTION:
            switch (e.caxis.axis)
            {

            case SDL_CONTROLLER_AXIS_LEFTY:
            {
                int instanceId = e.caxis.which;
                if (controllerManagers.count(instanceId))
                {
                    double val = e.caxis.value;
                    float normalised = controllerManagers[instanceId].NormaliseStickAxis(val);
                    std::cout << "Player " << instanceId << " Left Stick Y Moved " 
                        << static_cast<float>(normalised) << "\n";
                   
                }
                
                break;
            }
            case SDL_CONTROLLER_AXIS_LEFTX:
            {
                int instanceId = e.caxis.which;
                if (controllerManagers.count(instanceId))
                {
                    double val = e.caxis.value;
                    float normalised = controllerManagers[instanceId].NormaliseStickAxis(val);
                    std::cout << "Player " << instanceId << " Left Stick X Moved "
                        << static_cast<float>(normalised) << "\n";
                   
                }
               
                break;
            }
            case SDL_CONTROLLER_AXIS_RIGHTX:
            {

                int instanceId = e.caxis.which;
                if (controllerManagers.count(instanceId))
                {
                    double val = e.caxis.value;
                    float normalised = controllerManagers[instanceId].NormaliseStickAxis(val);
                    std::cout << "Player " << instanceId << " Right Stick X Moved "
                        << static_cast<float>(normalised) << "\n";
                    
                }
               
                break;
            }
            case SDL_CONTROLLER_AXIS_RIGHTY:
            {
                int instanceId = e.caxis.which;
                if (controllerManagers.count(instanceId))
                {
                    double val = e.caxis.value;
                    float normalised = controllerManagers[instanceId].NormaliseStickAxis(val);
                    std::cout << "Player " << instanceId << " Right Stick Y Moved "
                        << static_cast<float>(normalised) << "\n";
                    
                }
                break;
            }
            case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
            {
                int instanceId = e.caxis.which;
                if (controllerManagers.count(instanceId))
                {
                    double val = e.caxis.value;
                    float normalised = controllerManagers[instanceId].NormaliseTrigger(val);
                    std::cout << "Player " << instanceId << " Left Trigger pressed "
                        << static_cast<float>(normalised) << "\n";
                    
                }
                break;
            }
            case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
            {
                int instanceId = e.caxis.which;
                if (controllerManagers.count(instanceId))
                {
                    double val = e.caxis.value;
                    float normalised = controllerManagers[instanceId].NormaliseTrigger(val);
                    std::cout << "Player " << instanceId << " Right Trigger pressed "
                        << static_cast<float>(normalised) << "\n";
                   
                }
                break;
            }
            break;
            }

        }
        }
    }

