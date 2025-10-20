#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class Force
    {
        b2World* world;

        Force()
        {
            world = LayerFysics::GetInstance()->GetWorld();
        }
    };
    
}

