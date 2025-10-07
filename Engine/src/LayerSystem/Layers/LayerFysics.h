#pragma once
#include "LayerSystem/Layer.h"
#include "box2d.h"

namespace IonixEngine
{
    class LayerFysics : public Layer
    {
    public:
        LayerFysics() {}

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;
    };
}