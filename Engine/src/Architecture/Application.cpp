#include "Application.h"


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

        layerUI = new LayerUI();
        AddLayer(layerUI);

        Scripting::Get().Init();
        Scripting::Get().GetLuaState().script_file("Scripts/settings.lua");

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
#
        Scripting::Get().CallHook("OnStart");

        while (m_Running)
        {
            for (auto layer : m_LayerStack.GetLayers())
            {
                if(layer)
                    layer->OnUpdate();
            }
            Scripting::Get().CallHook("OnUpdate");
            m_Window->OnUpdate();
        }
    }
}