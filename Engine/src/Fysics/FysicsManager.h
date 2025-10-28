#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class FysicsManager
    {
        static LayerFysics* s_instance;
    public:
        static void SetInstance(LayerFysics* instance) {
            s_instance = instance;
        }
    public:
    };
}

