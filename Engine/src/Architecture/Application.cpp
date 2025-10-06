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

        //Joe Maf Util Test
        //-----------------------------------

        float nom1 = 4.0f;
        float nom2 = 9.0f;
        float nom3 = 12.0f;
        float t = 0.25f;

        Maf::mafVector2<float> vector2(nom1, nom2); //create maf vec2
        Maf::mafVector3<float> vector3(nom1, nom2, nom3); //create maf vec 3

        std::cout << "maf-vector2 x:" << vector2[0] << " & y:" << vector2[1] << std::endl; //reading vector x & y

        vector2[0] = 9.f; //writing 9.0f into index 'x'
        std::cout << "maf-changed vector2 x:" << vector2[0] << " & y:" << vector2[1] << std::endl; //reading changed vector x & y

        std::cout << "maf-vector2 x:" << vector3[0] << " & y:" << vector3[1] << " & z:" << vector3[2] << std::endl; //reading vec3 x & y & z

        std::cout << "maf-Lerp nom 1&2: " << Maf::mafLerp(nom1, nom2, t) << std::endl; // lerping

        Maf::mafVector2<float> a(0.f, 0.f); //lerping two vectors
        Maf::mafVector2<float> b(10.f, 20.f);
        
        Maf::mafVector2<float> c = Maf::mafVector2<float>::maf2Lerp(a, b, t);

        std::cout << "maf - mafVector2 Lerp: (" << c.x << ", " << c.y << ")\n";

        //Jake Maf Utils Test
        //---------------------------------

        float testnum1 = 10.0f;
        float testnum2 = 2.0f;
        float testnum3 = 3.0f;

        float modTest = Maf::mafMod(testnum1, testnum2);
        float divTest = Maf::mafDiv(testnum1, testnum3);

        std::cout << "Mod Test: " << testnum1 << " MOD " << testnum2 << " = " << modTest << "\n";
        std::cout << "Div Test: " << testnum1 << " DIV " << testnum3 << " = " << divTest << "\n";

        //-----------------------------------

        AddLayer(layerEditor);

        layerUI = new LayerUI();
        AddLayer(layerUI);

        //Josh maf utils test
        //------------------------------

        float number1 = 5.5f;
        float number2 = 12.0f;
        float number3 = -15.0f;

        Maf::mafVector2<float> vecA(5.0f, 15.0f);
        Maf::mafVector3<float> vecB(10.0f, 20.0f, 30.0f);

        float vec2SqrMag = Maf::mafSqrMagnitudeVec2(vecA);
        float vec3SqrMag = Maf::mafSqrMagnitudeVec3(vecB);

        float clampTest1 = Maf::mafClamp(number1, 0.0f, 10.0f);
        float clampTest2 = Maf::mafClamp(number2, 0.0f, 10.0f);
        float clampTest3 = Maf::mafClamp(number3, 0.0f, 10.0f);

        std::cout << "Clamp Test 1: " << clampTest1 << "\n";
        std::cout << "Clamp Test 2: " << clampTest2 << "\n";
        std::cout << "Clamp Test 3: " << clampTest3 << "\n";

        std::cout << "Vec2 Sqr Magnitude: " << vec2SqrMag << "\n";
        std::cout << "Vec3 Sqr Magnitude: " << vec3SqrMag << "\n";
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