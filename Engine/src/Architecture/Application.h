#pragma once
#include "LayerSystem/Layers/LayerTexture.hpp"
#include "Window/Window.h"
#include "Macros.h"
#include "LayerSystem/LayerStack.h"
#include "LayerSystem/Layers/LayerEditor.h"
#include "Maf/MafUtils.h"
#include <chrono>
#include <cstdint>
#include <iostream>
#include "LayerSystem/Layers/LayerUI.h"
#include "LayerSystem/Layers/LayerGraphics.h"
#include "LayerSystem/Layers/SceneLayer.h"
#include "LayerSystem/Layers/LayerFysics.h"
#include "LayerSystem/Layers/LayerInput.h"
#include"Scripting/Scripting.h"
// #include "Scripting/Audio/AudioScripting.h"
#include "LayerSystem/Layers/LayerSound.h"
#include "Audio/AudioPlayer.h"

namespace IonixEngine {
    class Camera;
}


namespace IonixEngine {    
    class ENGINE_API Application {
    public:
        static Application& Get();

        Application();
        virtual ~Application();

        bool m_Running = true;
        float deltaTime = 0.0f;
        float time = 0.0f;

        Camera* currentCam = nullptr;
        
        inline Window& GetWindow() { return *m_Window; }

        void Run();
        void OnEvent(IonixEvent& e);
        
        inline Layer* AddLayer(Layer* layer) {   
            m_LayerStack.PushLayer(layer); 
            layer->OnAttach(); 

            return layer;
        }
        
        inline float GetPhysicsInterpolationAlpha() const //method used to interpolate between physics states
        {
            return m_FixedTimeAccumulator / m_FixedTimeStep;
        }
        
        LayerScene* layerScene;
        LayerEditor* layerEditor;
        LayerUI* layerUI;
        LayerGraphics* layerGraphics;
        LayerTexture* layerTexture;
        LayerFysics* layerFysics;
        LayerInput* layerInput;
        LayerSound* layerSound;


    private:
        uint64_t startTick;
        uint64_t currentTick;
        static Application* s_Instance;
        Window* m_Window;
        LayerStack m_LayerStack;
        
        // fixed update timing
        float m_FixedTimeStep = 1.0f / 60.0f;  // 60 Hz fixed update
        float m_FixedTimeAccumulator = 0.0f;
        Uint64 m_LastFrameTime = 0;
    };

    Application* CreateApplication();
}
