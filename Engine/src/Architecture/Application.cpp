#include "Application.h"
#include "LayerSystem/Layers/LayerTexture.hpp"
#include <iostream>


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

        bool isLMouseDown = false;
        bool isRMouseDown = false;
        bool isMMouseDown = false;

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

            if (layerInput->m_Input->IsKeyDown(SDL_SCANCODE_SPACE))
            {
                std::cout << "Spacebar was pressed once \n";
            }
            else if (layerInput->m_Input->IsKeyUp(SDL_SCANCODE_SPACE))
            {
                std::cout << "Spacebar has been lifted \n";
            }
            if (layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_SPACE))
            {
                std::cout << "Spacebar is being held down \n";

            }

           if (layerInput->m_Input->IsMouseButtonDown(SDL_BUTTON_LEFT))
           {
               if (!isLMouseDown)
               {
                   std::cout << "L-Mouse Button Down pressed \n";
                   isLMouseDown = true;
               }
           }

           else if(layerInput->m_Input->IsMouseButtonUp(SDL_BUTTON_LEFT))
           {
             std::cout << "L-Mouse Button released \n";
             isLMouseDown = false;
           }


           if (layerInput->m_Input->IsMouseButtonDown(SDL_BUTTON_RIGHT))
           {
               if (!isRMouseDown) {
                   std::cout << "R-Mouse Button Down pressed \n";
                   isRMouseDown = true;
               }
           }

           else if (layerInput->m_Input->IsMouseButtonUp(SDL_BUTTON_RIGHT))
           {
              
               std::cout << "R-Mouse Button released \n";
               isRMouseDown = false;
           }

           if (layerInput->m_Input->IsMouseButtonDown(SDL_BUTTON_MIDDLE))
           {
               if (!isMMouseDown) {
                   std::cout << "M-Mouse Button Down pressed \n";
                   isMMouseDown = true;
               }
           }

           else if (layerInput->m_Input->IsMouseButtonUp(SDL_BUTTON_MIDDLE))
           {
               std::cout << "M-Mouse Button released \n";
               isMMouseDown = false;
           }

           if (layerInput->m_Input->ScrollWheel(SDL_MOUSEWHEEL))
           {
               std::cout << "Scroll Wheel is scrolling.\n";
           }
           

          



           // MouseCoords mc = layerInput->m_Input->GetMousePosition();
           // std::cout << "Mouse X Pos: " << mc.x << " Mouse Y Pos: " << mc.y << std::endl;
            
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
