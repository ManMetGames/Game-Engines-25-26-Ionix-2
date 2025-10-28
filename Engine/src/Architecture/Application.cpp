#include "Application.h"

#include "Fysics/FysicBody.h"
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
        
        FysicBody testBody = FysicBody();
        

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
                //std::cout << "Spacebar was pressed once \n";
                testBody.AddForceToCenter(b2Vec2(0.f, 5000.0f));
            }
            if (layerInput->m_Input->IsKeyUp(SDL_SCANCODE_SPACE))
            {
               // std::cout << "Spacebar has been lifted \n";
            }
            if (layerInput->m_Input->IsKeyHeld(SDL_SCANCODE_SPACE))
            {
               // std::cout << "Spacebar is being held down \n";
            }

            SoundManager& sm = SoundManager::GetInstance();
            sm.Init();
            sm.LoadSound("test", "../Client/test.wav");
            //printf("%4.2f %4.2f\n", testBody.GetPosition().x, testBody.GetPosition().y);
            

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
