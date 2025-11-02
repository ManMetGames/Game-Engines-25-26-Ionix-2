#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class RaycastCallback : public b2RayCastCallback // single hit callback
    {
    public:
        RaycastCallback() : m_hit(false), m_fixture(nullptr), m_point(0.0f, 0.0f), m_normal(0.0f, 0.0f), m_fraction(1.0f) {}

        float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
            const b2Vec2& normal, float fraction) override
        {
            m_hit = true;
            m_fixture = fixture;
            m_point = point;
            m_normal = normal;
            m_fraction = fraction;
            return fraction;
        }

        bool m_hit;
        b2Fixture* m_fixture;
        b2Vec2 m_point;
        b2Vec2 m_normal;
        float m_fraction;
    };

    class Raycast
    {
        b2World* world;

        Raycast()
        {
            world = LayerFysics::GetInstance()->GetWorld();
        }

        //simple ray cast  - returns true if hit
        bool Cast(const b2Vec2& start, const b2Vec2& end,
            b2Vec2& hitPoint, b2Vec2& hitNormal)
        {
            RaycastCallback callback;
            world->RayCast(&callback, start, end);

            if (callback.m_hit)
            {
                hitPoint = callback.m_point;
                hitNormal = callback.m_normal;
                return true;
            }
            return false;
        }

        // raycast returning fixture
        bool CastGetFixture(const b2Vec2& start, const b2Vec2& end,
            b2Fixture** outFixture)
        {
            RaycastCallback callback;
            world->RayCast(&callback, start, end);

            if (callback.m_hit && outFixture)
            {
                *outFixture = callback.m_fixture;
                return true;
            }
            return false;
        }

        // raycast in direction with length
        bool CastDirection(const b2Vec2& origin, const b2Vec2& direction, float length,
            b2Vec2& hitPoint, b2Vec2& hitNormal)
        {
            b2Vec2 end;
            end.x = origin.x + direction.x * length;
            end.y = origin.y + direction.y * length;
            return Cast(origin, end, hitPoint, hitNormal);
        }

        // Get distance to nearest object in direction
        float GetDistanceToNearest(const b2Vec2& origin, const b2Vec2& direction, float maxDistance)
        {
            b2Vec2 end;
            end.x = origin.x + direction.x * maxDistance;
            end.y = origin.y + direction.y * maxDistance;

            RaycastCallback callback;
            world->RayCast(&callback, origin, end);

            if (callback.m_hit)
                return callback.m_fraction * maxDistance;

            return maxDistance;
        }

        // line of sight check
        bool HasLineOfSight(const b2Vec2& from, const b2Vec2& to)
        {
            RaycastCallback callback;
            world->RayCast(&callback, from, to);
            return !callback.m_hit;
        }
    };
    
}

