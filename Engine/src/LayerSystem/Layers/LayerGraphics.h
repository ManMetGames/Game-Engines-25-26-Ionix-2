#pragma once
#include "LayerSystem/Layer.h"
#include "Graphics/SpriteClass.h"

namespace IonixEngine
{
    class LayerGraphics : public Layer
    {

        //SDL_Window win;
        //SDL_Renderer* renderer;
        SDL_Texture* img;
        SDL_Rect texr;

    public:
        LayerGraphics();
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;

    private:
    };
}