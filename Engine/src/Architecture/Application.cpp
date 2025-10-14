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
            m_Window->OnUpdate();
            SDL_RenderPresent(renderer);
        }

        for (auto layer : m_LayerStack.GetLayers()) {
            layer->OnDetach();
        }
    }
}
