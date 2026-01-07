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
    }

    void LayerGraphics::OnDetach() {
        TextureManager::Get().Shutdown();
    }

    void LayerGraphics::OnUpdate() {
        //ball.drawFrame(0,0,32,32);

        queue.Sort();
        queue.RenderFromQueue();
    }

    void LayerGraphics::OnEvent(IonixEvent& e) {}
    QueueRenderer* LayerGraphics::GetQueue()
    {
        return &queue;
    }
}
