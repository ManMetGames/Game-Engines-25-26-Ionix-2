#include "Application.h"
#include "LayerSystem/Layers/LayerTexture.hpp"


namespace IonixEngine {
    Application* Application::s_Instance = nullptr;
}

namespace IonixEngine
{
    Application& Application::Get() { return *s_Instance; }

    Application::Application()
        : m_Window(new Window())
    {
        s_Instance = this;


        //Initialise layers...
        layerEditor = new LayerEditor();
        AddLayer(layerEditor);

        layerInput = new LayerInput();
        AddLayer(layerInput);

        layerFysics = new LayerFysics();
        AddLayer(layerFysics);

        layerSound = new LayerSound();
        AddLayer(layerSound);

        layerUI = new LayerUI();
        AddLayer(layerUI);

        layerTexture = new LayerTexture();
        AddLayer(layerTexture);

        layerGraphics = new LayerGraphics();
        AddLayer(layerGraphics);

        layerScene = new LayerScene();
        AddLayer(layerScene);
        //Scripting::Get().Init();
        //Scripting::Get().GetLuaState().script_file("Scripts/settings.lua");
    }
        
    Application::~Application() 
    {
        delete m_Window;
        m_Window = nullptr;
    }

    void Application::OnEvent(IonixEvent& e)
    {
        for (auto layer : m_LayerStack.GetLayers()) {
            layer->OnEvent(e);
            if (e.Handled)
                break;
        }

    }

    void Application::Run()
    {
        m_Running = true;

        //Scripting::Get().CallHook("OnStart");
        SDL_Renderer* renderer = m_Window->GetSdlRenderer();

        while (m_Running)
        {
			      SDL_RenderClear(renderer);
			      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
          
            for (auto layer : m_LayerStack.GetLayers())
            {
                if(layer)
                    layer->OnUpdate();
            }

            // Scripting::Get().CallHook("OnUpdate");

            // Keyboard
            if (layerInput->m_Input->IsKeyDown(SDL_SCANCODE_SPACE))
            {
                std::cout << "Spacebar was pressed once \n";
            }
            else if (layerInput->m_Input->IsKeyUp(SDL_SCANCODE_SPACE))
            {
                std::cout << "Spacebar has been lifted \n";
            }
            else if (layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_SPACE))
            {
                std::cout << "Spacebar is being held down \n";
            }

            // Controller
                // Buttons
                    // A Button
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_A))
            {
                std::cout << "Button A on the controller was pressed once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_A))
            {
                std::cout << "Button A on the controller has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_A))
            {
                std::cout << "Button A on the controller is being held down \n";
            }
                    // B Button
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_B))
            {
                std::cout << "Button B on the controller was pressed once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_B))
            {
                std::cout << "Button B on the controller has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_B))
            {
                std::cout << "Button B on the controller is being held down \n";
            }
                    // X Button
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_X))
            {
                std::cout << "Button X on the controller was pressed once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_X))
            {
                std::cout << "Button X on the controller has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_X))
            {
                std::cout << "Button X on the controller is being held down \n";
            }
                    // Y Button
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_Y))
            {
                std::cout << "Button Y on the controller was pressed once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_Y))
            {
                std::cout << "Button Y on the controller has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_Y))
            {
                std::cout << "Button Y on the controller is being held down \n";
            }

                // D-Pad
                    // Up
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_DPAD_UP))
            {
                std::cout << "Up on the controller D-Pad was pressed once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_DPAD_UP))
            {
                std::cout << "Up on the controller D-Pad has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_UP))
            {
                std::cout << "Up on the controller D-Pad is being held down \n";
            }
                    // Down
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
            {
                std::cout << "Down on the controller D-Pad was pressed once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
            {
                std::cout << "Down on the controller D-Pad has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
            {
                std::cout << "Down on the controller D-Pad is being held down \n";
            }
                    // Left
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
            {
                std::cout << "Left on the controller D-Pad was pressed once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
            {
                std::cout << "Left on the controller D-Pad has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_LEFT))
            {
                std::cout << "Left on the controller D-Pad is being held down \n";
            }
                    // Right
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
            {
                std::cout << "Right on the controller D-Pad was pressed once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
            {
                std::cout << "Right on the controller D-Pad has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
            {
                std::cout << "Right on the controller D-Pad is being held down \n";
            }

                // Touchpad
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_TOUCHPAD))
            {
                std::cout << "Touchpad was pressed once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_TOUCHPAD))
            {
                std::cout << "Touchpad has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_TOUCHPAD))
            {
                std::cout << "Touchpad is being held down \n";
            }

               // Bumpers
                    // Left
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
            {
                std::cout << "the left bumper was pressed once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
            {
                std::cout << "the left bumper has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
            {
                std::cout << "the left bumper is being held down \n";
            }
                    // Right
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
            {
                std::cout << "the right bumper was pressed once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
            {
                std::cout << "the right bumper has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
            {
                std::cout << "the right bumper is being held down \n";
            }

                // Sticks
                    // Pressing Left
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_LEFTSTICK))
            {
                std::cout << "the left stick was pressed down once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_LEFTSTICK)) 
            {
                std::cout << "the left stick has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_LEFTSTICK))
            {
                std::cout << "the left stick is being held down \n";
            }
                    // Pressing Right
            if (layerInput->m_Input->isButtonDown(SDL_CONTROLLER_BUTTON_RIGHTSTICK))
            {
                std::cout << "the right stick was pressed down once \n";
            }
            else if (layerInput->m_Input->isButtonUp(SDL_CONTROLLER_BUTTON_RIGHTSTICK))
            {
                std::cout << "the right stick has been lifted \n";
            }
            else if (layerInput->m_Input->isButtonHeld(SDL_CONTROLLER_BUTTON_RIGHTSTICK))
            {
                std::cout << "the right stick is being held down \n";
            }

            //if (layerInput->m_Input->NormaliseLeftXAxis(SDL_CONTROLLER_AXIS_LEFTX))
            //{
             //  std::cout << std::setprecision(2) << fixed << val << "\n";
            //}

            layerInput->m_Input->CopyCodesEndFrame();

            // Scripting::Get().CallHook("OnUpdate");
          
            m_Window->OnUpdate();
            SDL_RenderPresent(renderer);
        }

        for (auto layer : m_LayerStack.GetLayers()) {
            layer->OnDetach();
        }
    }
}
