#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class Entity;

    class Raycast
    {
    public:
        struct Hit
        {
            b2Vec2 point;
            b2Vec2 normal;
            float fraction;
            b2Body* body;
            Entity* entity;
        };

        Raycast();

        bool CastFirst(const b2Vec2& p1, const b2Vec2& p2, Hit& outHit);

    private:
        b2World* world;
    };
}
