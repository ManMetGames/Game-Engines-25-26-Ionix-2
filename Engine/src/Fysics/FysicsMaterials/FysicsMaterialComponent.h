
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
        FysicsMaterialComponent(Entity* entity, std::string alias, b2World* world, float friction, float restitution);
        void UpdateMaterial(Entity* entity, float friction, float restitution);
        float GetFriction(Entity* entity);
        float GetRestitution(Entity* entity);
    };
}
