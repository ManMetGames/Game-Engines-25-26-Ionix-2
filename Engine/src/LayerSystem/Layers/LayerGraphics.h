#pragma once
#include "LayerSystem/Layer.h"

namespace IonixEngine
{
    class LayerGraphics : public Layer
    {
    public:
        LayerGraphics() {}

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;

    private:
    };
}