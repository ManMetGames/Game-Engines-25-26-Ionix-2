#pragma once
#include "box2d.h"
#include <string>
#include <Architecture/ECS/Component.hpp>

namespace IonixEngine
{
    class RigidBodyComponent : public Component
    {
    public:
        RigidBodyComponent(IonixEngine::Entity* entity, std::string alias, int type = 0);            
    };
}

