#pragma once
#include "LayerSystem/Layer.h"
#include "Architecture/TextureManager/TextureData.h"
#include "Architecture/TextureManager/TextureManager.h"

namespace IonixEngine
{
    class LayerTexture : public Layer
    {
    public:
        LayerTexture() {}

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;
    };
}