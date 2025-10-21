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
        SDL_Log("Added debug texture");
    }

    void LayerGraphics::OnDetach() {
        TextureManager::Get().Shutdown();
    }

    void LayerGraphics::OnUpdate() {
    }

    void LayerGraphics::OnEvent(IonixEvent& e) {}
}
