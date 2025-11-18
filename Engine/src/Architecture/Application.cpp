#include "Application.h"
#include "LayerSystem/Layers/LayerTexture.hpp"
#include "SDL_timer.h"
#include "imgui.h"
#include <chrono>
#include <cstdint>
#include <vector>
#include <backends/imgui_impl_sdlrenderer2.h>
#include <iostream>
#include <third-party/imgui_impl_sdlrenderer2.h>

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
        startTick = SDL_GetPerformanceCounter();
        currentTick = SDL_GetPerformanceCounter();


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
    Camera* cam = new Camera(0.0f, 0.0f, 1.0f, 800, 600, true, 0, 0, 0, 0);
    Camera* cam2 = new Camera(100.0f, 100.0f, 1.0f, 800, 600, false, 0, 0, 0, 0);


    void Application::Run()
    {
        m_Running = true;

	    cam->Init();
	    cam2->Init();
        Scripting::Get().CallHook("OnStart");

        SDL_Renderer* renderer = m_Window->GetSdlRenderer();

        cam->SetBackgroundColor(50, 80, 200, 255);

        //FysicBody testBody = FysicBody();
        

        while (m_Running) {
            /*uint64_t lastTick = currentTick;
            currentTick = SDL_GetPerformanceCounter();
            
            deltaTime = static_cast<double>(currentTick - lastTick) / SDL_GetPerformanceFrequency();
            time += deltaTime;*/
            
            if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_1))
                cam->SetColor(255, 0, 0, 255); // RED

            if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_2))
                cam->SetColor(0, 255, 0, 255); // GREEN

            if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_3))
                cam->SetColor(0, 0, 255, 255); // BLUE

            if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_4))
                cam->SetColor(0, 0, 0, 0); // BLACK

            if (Application::Get().layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_5))
                cam->SetColor(255, 255, 255, 255); // WHITE
            
            cam->ClearBackground(renderer);
            for (auto layer : m_LayerStack.GetLayers()) {
                if(layer)
                    layer->OnUpdate();
            }

            cam->handleInput(deltaTime);
            cam2->handleInput(deltaTime);
            
            Scripting::Get().CallHook("OnUpdate");

			//cam2->RenderToScreen(renderer, 300, 0, 400, 300);

            ImGui::Render();
            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_Window->GetSdlRenderer());
            SDL_RenderPresent(m_Window->m_Renderer);

			

            layerInput->m_Input->CopyCodesEndFrame();
          
            m_Window->OnUpdate();
        }
    
        std::vector<Layer*> layers = m_LayerStack.GetLayers();
        for (size_t i = layers.size() - 1; i > 0; i--) {
            layers[i]->OnDetach();
        }
    }
}
