#pragma once
#include "Architecture/Scene/SceneLayer.h"
#include "Window/Window.h"
#include "Macros.h"
#include "LayerSystem/LayerStack.h"
#include "LayerSystem/Layers/LayerEditor.h"
#include "LayerSystem/Layers/LayerUI.h"

namespace IonixEngine
{    
    class ENGINE_API Application
    {
    public:
        static Application& Get();

        Application();
        virtual ~Application();

        bool m_Running = true;

        inline Window& GetWindow() { return *m_Window; }

        void Run();
        void OnEvent(IonixEvent& e);
        
        inline Layer* AddLayer(Layer* layer) 
        {   
            m_LayerStack.PushLayer(layer); 
            layer->OnAttach(); 

            return layer;
        }
        
        LayerScene* layerScene;
        LayerEditor* layerEditor;
        LayerUI* layerUI;

    private:
        static Application* s_Instance;
 
        Window* m_Window;
        LayerStack m_LayerStack;
    };

    Application* CreateApplication();
}
