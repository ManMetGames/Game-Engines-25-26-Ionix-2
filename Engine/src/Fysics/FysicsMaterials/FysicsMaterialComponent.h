
#pragma once
#include <sol/sol.hpp>
#include "Architecture/ECS/Component.hpp"
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
     enum class FysicsMaterialType { firctionMaterial, bouncyMaterial };

    class FysicsMaterialComponent : public Component
    {
    private:
        b2Body* body;

    public:
        FysicsMaterialComponent(Entity* entity, std::string alias, b2World* world, FysicsMaterialType m_type);
    };
}
