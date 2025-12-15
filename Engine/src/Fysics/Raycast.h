#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    struct RayHit
    {
        
        b2Vec2 point;
        b2Vec2 normal;
        float fraction = 0.0f;
        b2Body* body;
        Entity* entity;
    };
    class Entity;

    class Raycast
    {
    public:
        b2World* world = nullptr;


        Raycast();

        bool CastFirst(const b2Vec2& p1, const b2Vec2& p2, RayHit& outHit);

    private:
    };
}
