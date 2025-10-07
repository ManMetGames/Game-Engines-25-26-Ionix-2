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

        layerScene = new LayerScene();
        AddLayer(layerScene);

        //Initialise layers...
        layerEditor = new LayerEditor();
        AddLayer(layerEditor);

        layerFysics = new LayerFysics();
        AddLayer(layerFysics);

        layerUI = new LayerUI();
        AddLayer(layerUI);

        layerGraphics = new LayerGraphics();
        AddLayer(layerGraphics);
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

        while (m_Running)
        {
            for (auto layer : m_LayerStack.GetLayers())
            {
                if(layer)
                    layer->OnUpdate();
            }

            m_Window->OnUpdate();
        }
    }
}
