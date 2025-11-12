
#pragma once
#include <sol/sol.hpp>
#include "Architecture/ECS/Component.hpp"
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    

    class FysicsMaterialComponent : public Component
    {
    private:
        b2Body* body;

    public:
        
    };
}
