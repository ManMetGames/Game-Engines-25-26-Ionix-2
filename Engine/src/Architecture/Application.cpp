#include "Application.h"

#include "Fysics/FysicsBody.h"
#include "Fysics/FysicsManager.h"
#include "Fysics/Shapes.h"
#include "Fysics/NavMef.h"
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

        layerNavigation = new LayerNavigation();  
        AddLayer(layerNavigation);

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

    void Application::Run()
    {
        m_Running = true;
        Scripting::Get().CallHook("OnStart");

        SDL_Renderer* renderer = m_Window->GetSdlRenderer();

        // timings initialisation for fixed update
        m_LastFrameTime = SDL_GetTicks64();
        m_FixedTimeAccumulator = 0.0f;
        m_FixedTimeStep = 1.0f / 60.0f; // 60 Hz
        
        while (m_Running)
        {
            
            uint64_t lastTick = currentTick;
            currentTick = SDL_GetPerformanceCounter();
            
            deltaTime = static_cast<double>(currentTick - lastTick) / SDL_GetPerformanceFrequency();
            time += deltaTime;
		    
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
            SDL_RenderClear(renderer);

            // fixed update time accumulation
            m_FixedTimeAccumulator += deltaTime;
            
            // fixed update loop
            while (m_FixedTimeAccumulator >= m_FixedTimeStep)
            {
                for (auto layer : m_LayerStack.GetLayers())
                {
                    if(layer)
                        layer->OnFixedUpdate();
                }
                m_FixedTimeAccumulator -= m_FixedTimeStep;
            }
            
            // variable update loop
            for (auto layer : m_LayerStack.GetLayers())
            {
                if(layer)
                    layer->OnUpdate();
            }
            
            
            Scripting::Get().CallHook("OnUpdate");
            ImGui::Render();
            
            ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), m_Window->GetSdlRenderer());
            Get().layerFysics->GetFysicsManager()->GetWorld()->DebugDraw();

            SDL_RenderPresent(m_Window->m_Renderer);

           /*if (layerInput->m_Input->IsMouseButtonDown(SDL_BUTTON_LEFT))
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

           MouseCoords mc = layerInput->m_Input->GetMousePosition();
           std::cout << "Mouse X Pos: " << mc.x << " Mouse Y Pos: " << mc.y << std::endl;
           */

            layerInput->m_Input->CopyCodesEndFrame();
          
            m_Window->OnUpdate();
        }
    
        std::vector<Layer*> layers = m_LayerStack.GetLayers();
        for (size_t i = layers.size() - 1; i > 0; i--) {
            layers[i]->OnDetach();
        }
    }
}
