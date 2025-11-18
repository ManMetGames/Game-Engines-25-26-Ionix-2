
#pragma once
#include <sol/sol.hpp>
#include "Architecture/ECS/Component.hpp"
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{

    class FysicsMaterialComponent : public Component
    {
    private:
        b2Fixture* fixture;

    public:
        b2Fixture* GetFixture(Entity* entity);
        void UpdateMaterial(Entity* entity, float friction, float restitution);
        float GetFriction(Entity* entity);
        float GetRestitution(Entity* entity);
    };
}
