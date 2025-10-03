#pragma once
#include "LayerSystem/Layer.h"
#include "UI/UI.h"

namespace IonixEngine
{
    class LayerUI : public Layer
    {
    public:
        LayerUI() {}

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;
    };
}