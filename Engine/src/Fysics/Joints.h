#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class Joints
    {
        b2World* world;

        Joints()
        {
            world = LayerFysics::GetInstance()->GetWorld();
        }
    };
    
}

