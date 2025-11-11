#include "LayerSystem/Layers/LayerGraphics.h"
#include "Architecture/TextureManager/TextureManager.h"
#include "EventSystem/Event.h"
#include <iostream>
#include "Architecture/Application.h"
#include "Graphics/QueueRenderer.h"
#include "SDL_render.h"

namespace IonixEngine
{

    LayerGraphics::LayerGraphics() {}

    void LayerGraphics::OnAttach() {
        TextureManager::Get().Init();
        TextureManager::Get().AddTexture("./Assets/Test.png", "Test");
        TextureManager::Get().AddTexture("./Assets/ball.png", "Ball");
        TextureManager::Get().AddTexture("./Assets/ball2.png", "Ball 2");
        TextureManager::Get().AddTexture("./Assets/aur naur.jpg", "pim");
        SDL_Log("Added debug texture");
    }

    void LayerGraphics::OnDetach() {
        TextureManager::Get().Shutdown();
    }

    void LayerGraphics::OnUpdate() {
        //ball.drawFrame(0,0,32,32);
        queue.RenderFromQueue();
    }

    void LayerGraphics::OnEvent(IonixEvent& e) {}
    QueueRenderer* LayerGraphics::GetQueue()
    {
        return &queue;
    }
}
