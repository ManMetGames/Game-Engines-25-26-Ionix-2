#include "Application.h"
#include "Fysics/FysicsBody.h"
#include "Fysics/FysicsManager.h"
#include "Fysics/Shapes.h"
//#include "Fysics/NavMef.h"
#include "LayerSystem/Layers/LayerTexture.hpp"
#include "SDL_timer.h"
#include "imgui.h"
#include <chrono>
#include <cstdint>
#include <vector>
#include <backends/imgui_impl_sdlrenderer2.h>
#include <iostream>
#include <third-party/imgui_impl_sdlrenderer2.h>
#include "Input/ControllerManager.h"

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

		firebaseLeaderboard = new FirebaseLeaderboard();
		firebaseLeaderboard->Init();
        //FirebaseLeaderboard::SubmitScore("Zulfaqaar", 90);
        //FirebaseLeaderboard::RetrieveTopScores(5);
        
        //layerNavigation = new LayerNavigation();  
        //AddLayer(layerNavigation);

        Scripting::Get().Init();
        // Safely load the Lua settings file
        auto& lua = Scripting::Get().GetLuaState();
        try
        {
            lua.script_file("Scripts/Settings.lua");
        }
        catch (const sol::error& e)
        {
            // Prefer your own logging system if you have one
            std::cerr << "Lua error while loading Scripts/Settings.lua: "
                      << e.what() << std::endl;
            // Optionally: set a flag or fall back to default settings here.
        }
        catch (const std::exception& e)
        {
            std::cerr << "Std exception while loading Scripts/Settings.lua: "
                      << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown exception while loading Scripts/Settings.lua"
                      << std::endl;
        }
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
    Camera* cam = new Camera(0.0f, 0.0f, 0);
    Camera* cam2 = new Camera(100.0f, 100.0f, 1);

    void Application::Run()
    {
        m_Running = true;
        Scripting::Get().CallHook("OnStart");

	    cam->Init();
	    cam2->Init();

        currentCam = cam;
        
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
                Scripting::Get().CallHook("OnFixedUpdate");
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
            //Hit box Gizmos:
            //Get().layerFysics->GetFysicsManager()->GetWorld()->DebugDraw();
            SDL_RenderPresent(m_Window->m_Renderer);

            layerInput->m_Input->CopyCodesEndFrame();

            //need a way of itterating through unorder map
            //for (int i = 0; i < 4; i++)
            //{
            //    if (layerInput->GetControllerManager()[0])
            //    {
            //        layerInput->GetControllerManager()[0]->CopyCodesEndFrame();
            //    }
            //}

            if (layerInput->GetControllerManager()[0])
            {
                layerInput->GetControllerManager()[0]->CopyCodesEndFrame();
            }
            if (layerInput->GetControllerManager()[1])
            {
                layerInput->GetControllerManager()[1]->CopyCodesEndFrame();
            }
            if (layerInput->GetControllerManager()[2])
            {
                layerInput->GetControllerManager()[2]->CopyCodesEndFrame();
            }
            if (layerInput->GetControllerManager()[3])
            {
                layerInput->GetControllerManager()[3]->CopyCodesEndFrame();
            }
            m_Window->OnUpdate();
        }
    
        std::vector<Layer*> layers = m_LayerStack.GetLayers();
        for (size_t i = layers.size() - 1; i > 0; i--) {
            layers[i]->OnDetach();
        }
    }
}
