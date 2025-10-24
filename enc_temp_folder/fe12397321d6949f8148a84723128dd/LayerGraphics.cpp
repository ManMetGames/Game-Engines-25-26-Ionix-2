#include "LayerSystem/Layers/LayerGraphics.h"
#include "Architecture/TextureManager/TextureManager.h"
#include "EventSystem/Event.h"
#include <iostream>
#include "Architecture/Application.h"
#include "SDL_render.h"

namespace IonixEngine
{

    LayerGraphics::LayerGraphics() {}

    void LayerGraphics::OnAttach() {
        TextureManager::Get().Init();
        TextureManager::Get().AddTexture("./Assets/Test.png", "Test");
        TextureManager::Get().AddTexture("./Assets/ball sprite sheet.png", "Ball");
        SDL_Log("Added debug texture");
    }

    void LayerGraphics::OnDetach() {
        TextureManager::Get().Shutdown();
    }

    void LayerGraphics::OnUpdate() {
        // no longer causes an EVIL memory leak <3
        ball.drawFrame(0,0,32,32);
    }

    void LayerGraphics::OnEvent(IonixEvent& e) {}
}
