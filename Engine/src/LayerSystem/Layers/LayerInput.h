#pragma once
#include "LayerSystem/Layer.h"
#include "Input/Input.h"
#include <unordered_map>


namespace IonixEngine
{
    class ControllerManager;

    class LayerInput : public Layer
    {
    public:
        LayerInput() {}

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;

        bool IsControllerButtonDown(int instanceId, Uint8 button);

        Input* m_Input;  
    
        std::unordered_map<int, ControllerManager*>& GetControllerManager();

    private:
        void OnControllerButtonDown(ControllerButtonDownEvent& e);      
        std::unordered_map<int, ControllerManager*> controllerManagers;
    };
}