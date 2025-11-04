#include "Application.h"

#include "Fysics/FysicsBody.h"
#include "Fysics/Shapes.h"
#include "LayerSystem/Layers/LayerTexture.hpp"
#include <backends/imgui_impl_sdlrenderer2.h>
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
        
        Scripting::Get().Init();
        Scripting::Get().GetLuaState().script_file("Scripts/Settings.lua");
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

    Camera* cam = new Camera();
    SDL_Rect* rect = new SDL_Rect();

    void Application::Run()
    {
        m_Running = true;

        bool isLMouseDown = false;
        bool isRMouseDown = false;
        bool isMMouseDown = false;

        Scripting::Get().CallHook("OnStart");

        SDL_Renderer* renderer = m_Window->GetSdlRenderer();
        //FysicBody testBody = FysicBody();

        while (m_Running)
        {
			 SDL_RenderClear(renderer);
			 SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
          
            cam->handleInput(1.0f);
            cam->apply(*rect);
            cam->renderTexture(0, 0, rect, renderer);
             

            for (auto layer : m_LayerStack.GetLayers())
            {
                if(layer)
                    layer->OnUpdate();
            }
            
            Scripting::Get().CallHook("OnUpdate");

            layerInput->m_Input->CopyCodesEndFrame();
                     
            m_Window->OnUpdate();
            ImGui::Render();
            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), Application::Get().GetWindow().m_Renderer);
            SDL_RenderPresent(renderer);
        }

        for (auto layer : m_LayerStack.GetLayers()) {
            layer->OnDetach();
        }
    }
}
