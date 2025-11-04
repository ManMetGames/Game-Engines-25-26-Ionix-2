#include "Application.h"

#include "Fysics/FysicsBody.h"
#include "Fysics/FysicsManager.h"
#include "Fysics/Shapes.h"
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
		      SDL_RenderClear(renderer);
		      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
          
            // delta time calculation
            Uint64 currentTime = SDL_GetTicks64();
            float realDeltaTime = (currentTime - m_LastFrameTime) / 1000.0f; // Convert to seconds
            m_LastFrameTime = currentTime;
            
            // fixed update time accumulation
            m_FixedTimeAccumulator += realDeltaTime;
            
            // variable update loop
            for (auto layer : m_LayerStack.GetLayers())
            {
                if(layer)
                    layer->OnUpdate();
            }
            
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

            if (layerInput->m_Input->IsKeyDown(SDL_SCANCODE_SPACE))
            {
                //std::cout << "Spacebar was pressed once \n";
            }
            if (layerInput->m_Input->IsKeyUp(SDL_SCANCODE_SPACE))
            {
               // std::cout << "Spacebar has been lifted \n";
            }
            if (layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_SPACE))
            {
               // std::cout << "Spacebar is being held down \n";
            }

            //printf("%4.2f %4.2f\n", testBody.GetPosition().x, testBody.GetPosition().y);
            
            Scripting::Get().CallHook("OnUpdate");

            layerInput->m_Input->CopyCodesEndFrame();

                     
            m_Window->OnUpdate();
            SDL_RenderPresent(renderer);
        }

        for (auto layer : m_LayerStack.GetLayers()) {
            layer->OnDetach();
        }
    }
}
