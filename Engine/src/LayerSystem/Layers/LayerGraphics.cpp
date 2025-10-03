#include "LayerSystem/Layers/LayerGraphics.h"
#include "EventSystem/Event.h"
#include "Architecture/Application.h"
#include <iostream>

namespace IonixEngine
{
    LayerGraphics::LayerGraphics() {

        renderer = (Application::Get().GetWindow().m_Renderer);

        //test sprite
        Sprite newSprite = Sprite("Graphics/aur naur.jpg", renderer);
    }

    void LayerGraphics::OnAttach() {}

    void LayerGraphics::OnDetach() {}

    void LayerGraphics::OnUpdate() {
        //todo get sprites to draw here
        // current idea
        //create stack/store any and all sprites somewhere
        //increment through and draw them
        // :)
    }

    void LayerGraphics::OnEvent(IonixEvent& e) {}
 }