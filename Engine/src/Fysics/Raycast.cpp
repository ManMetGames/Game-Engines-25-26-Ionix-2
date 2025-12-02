#include "Raycast.h"
#include "FysicsManager.h"

namespace IonixEngine
{
    Raycast::Raycast()
    {
        world = LayerFysics::GetInstance()->GetWorld();
    }

    bool Raycast::CastFirst(const b2Vec2& p1, const b2Vec2& p2, Hit& outHit)
    {
        if (!world)
        {
            return false;
        }

        b2RayCastInput input;
        input.p1 = p1;
        input.p2 = p2;
        input.maxFraction = 1.0f;

        float closestFraction = 1.0f;
        b2Vec2 intersectionNormal(0.0f, 0.0f);
        b2Body* hitBody = nullptr;

        for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
        {
            for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
            {
                b2RayCastOutput output;
                if (!f->RayCast(&output, input, 0))
                {
                    continue;
                }

                if (output.fraction < closestFraction)
                {
                    closestFraction = output.fraction;
                    intersectionNormal = output.normal;
                    hitBody = b;
                }
            }
        }

        if (!hitBody || closestFraction == 1.0f)
        {
            return false;
        }

        outHit.fraction = closestFraction;
        outHit.normal = intersectionNormal;
        outHit.point = input.p1 + closestFraction * (input.p2 - input.p1);
        outHit.body = hitBody;

        FysicsManager* manager = FysicsManager::GetManager();
        outHit.entity = manager ? manager->GetEntityFromBody(hitBody) : nullptr;

        return true;
    }
}
