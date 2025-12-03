#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class Raycast : public b2RayCastCallback
    {
        b2RayCastCallback* raycast;

    public:
        Raycast()
        {
            m_fixture = nullptr;
        }
        float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
        {
            m_fixture = fixture;
            m_point = point;
            m_normal = normal;
            m_fraction = fraction;
            return fraction;
        }

        void MakeRayCast(Entity* entity);
 
        b2Fixture* m_fixture;
        b2Vec2 m_point;
        b2Vec2 m_normal;
        float m_fraction;
    };
};

