#pragma once
#include "LayerSystem/Layer.h"
#include "Input/Input.h"
namespace IonixEngine
{
    class LayerInput : public Layer
    {
    public:
        LayerInput() {}

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;


        Input* m_Input;
   
    };
}