#include "LayerSystem/Layers/LayerGraphics.h"
#include "EventSystem/Event.h"
#include <iostream>
#include "Architecture/Application.h"

namespace IonixEngine
{

    LayerGraphics::LayerGraphics(){}

    void LayerGraphics::OnAttach() {}

    void LayerGraphics::OnDetach() {}

    void LayerGraphics::OnUpdate() 
    {
        Sprite sprite1 = Sprite("aur naur.jpg");
        sprite1.draw(150,0,250,200);
    }

    void LayerGraphics::OnEvent(IonixEvent& e) {}
 }

