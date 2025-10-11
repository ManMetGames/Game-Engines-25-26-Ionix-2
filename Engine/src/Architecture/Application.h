#pragma once
#include "Window/Window.h"
#include "Macros.h"
#include "LayerSystem/LayerStack.h"
#include "LayerSystem/Layers/LayerEditor.h"
#include "Maf/MafUtils.h"
#include <iostream>
#include "LayerSystem/Layers/LayerUI.h"
#include "LayerSystem/Layers/LayerGraphics.h"
#include "LayerSystem/Layers/SceneLayer.h"
#include "LayerSystem/Layers/LayerFysics.h"
#include"Scripting/Scripting.h"

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
        double GetTimeSeconds();
        
        inline Layer* AddLayer(Layer* layer) 
        {   
            m_LayerStack.PushLayer(layer); 
            layer->OnAttach(); 

            return layer;
        }
        
        LayerScene* layerScene;
        LayerEditor* layerEditor;
        LayerUI* layerUI;
        LayerGraphics* layerGraphics;
        LayerFysics* layerFysics;

    private:
        static Application* s_Instance;
 
        Window* m_Window;
        LayerStack m_LayerStack;
    };

    Application* CreateApplication();
}
