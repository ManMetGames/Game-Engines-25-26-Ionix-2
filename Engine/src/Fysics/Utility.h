#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class Utility
    {
        b2World* world;
        b2Vec3 position;
        b2Vec3 scale;
        b2Vec3 rotation;

        Utility()
        {
            world = LayerFysics::GetInstance()->GetWorld();
        }

        void setScale(const b2Vec3& newScale) { scale = newScale; }
    };
    
}

