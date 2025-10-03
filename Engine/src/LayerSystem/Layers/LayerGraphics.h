#pragma once
#include "LayerSystem/Layer.h"
#include "Graphics/SpriteClass.h"

namespace IonixEngine
{
    class LayerGraphics : public Layer
    {
    public:
        LayerGraphics();

        SDL_Renderer* renderer;
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;

    private:
    };
}