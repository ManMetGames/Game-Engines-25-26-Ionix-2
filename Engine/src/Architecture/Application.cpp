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
        m_LayerStack.PushLayer(layerEditor);

        //Bens Maths Utils Testing
        //-----------------------------------

        float num1 = 20.0f;
        float num2 = 8.0f;
        float num3 = 2.26f;
        float num4 = 64;
        
        float abs = Maf::mafAbs(num1);
        float min = Maf::mafMin(num1, num2);
        float max = Maf::mafMax(num1, num2);
        float round = Maf::mafRound(num3);
        float sqrt = Maf::mafSqrt(num4);

        std::cout << "Absolute: " << abs << "\n";
        std::cout << "Min: " << min << "\n";
        std::cout << "Max: " << max << "\n";
        std::cout << "Round: " << round << "\n";
        std::cout << "Sqrt: " << sqrt << "\n";

        //---------------------------------
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