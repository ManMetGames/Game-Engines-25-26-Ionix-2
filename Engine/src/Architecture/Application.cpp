#include "Application.h"
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

        layerUI = new LayerUI();
        AddLayer(layerUI);

        //Maf test

        float rad = M_PI;
        float deg = 180.0f;

        std::cout << "Rad2Deg: " << Maf::Rad2Deg(rad) << std::end;
        std::cout << "Deg2Rad: " << Maf::Deg2Rad(deg) << std::end;

        std::cout << "Log(8, 2): " << Maf::Log(8.0f, 2.0f) << std::end;
        std::cout << "Log20(100): " << Maf::Log10(100.0f) << std::end;
        
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