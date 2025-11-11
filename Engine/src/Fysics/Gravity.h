#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class Gravity
    {
        b2World* world{nullptr};
    public:
        explicit Gravity(b2World* w): world(w) {}
        void SetGravity(float x, float y, bool wake=true)
        {
            if (!world) return;
            world->SetGravity(b2Vec2(x, y));
            if (wake) for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) b->SetAwake(true);
        }
        b2Vec2 GetGravity() const
        {
            return world ? world->GetGravity() : b2Vec2(0.f, 0.f);
        }
    };
}

