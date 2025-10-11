#include "Application.h"
#include <chrono>


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

        Scripting::Get().CallHook("OnStart");

        double t = 0.0;
        const double dt = 1.0 / 60.0;

        double currentTime = GetTimeSeconds();
        double accumulator = 0.0;

        while (m_Running)
        {
            double newTime = GetTimeSeconds();
            double frameTime = newTime - currentTime;

            currentTime = newTime;
            accumulator += frameTime;

            while (accumulator >= dt)
            {


                //Scripting::Get().CallHook("OnUpdate");
                t += dt;
                accumulator -= dt;
            }
            for (auto layer : m_LayerStack.GetLayers())
            {
                if (layer)
                    layer->OnUpdate();
            }
            m_Window->OnUpdate();
        }
    }

    double Application::GetTimeSeconds()
    {
        using namespace std::chrono;
        static auto start = high_resolution_clock::now();
        auto now = high_resolution_clock::now();
        duration<double> elapsed = now - start;
        return elapsed.count();
    }
}
