#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class FysicBody
    {
        b2World* world;

        FysicBody()
        {
            world = LayerFysics::GetInstance()->GetWorld();
        }
    };
    
}

