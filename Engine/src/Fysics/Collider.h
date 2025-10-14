#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class Collider
    {
        b2World* world;

        Collider()
        {
            world = LayerFysics::GetInstance()->GetWorld();
        }
    };
}

