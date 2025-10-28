#pragma once
#include "LayerSystem/Layer.h"
#include "Graphics/SpriteClass.h"
#include "Graphics/QueueRenderer.h"

namespace IonixEngine
{
    class LayerGraphics : public Layer
    {

        //SDL_Window win;
        //SDL_Renderer* renderer;
        SDL_Texture* img;
        SDL_Renderer* renderer;

        //this sprite is just for testing purposes while we work on the sprite component xoxo
        //Sprite sprite1 = Sprite("./Assets/ball sprite sheet.png");
        QueueRenderer queue;

    public:
        LayerGraphics();
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;
        QueueRenderer* GetQueue();
    private:
    };
}
