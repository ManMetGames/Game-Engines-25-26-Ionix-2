#pragma once
//#include "LayerSystem/Layers/LayerFysics.h"

// Now managed via FysicsManager directly. Set gravity is still a nice method to have, just needs updating for new system

namespace IonixEngine
{
    class Gravity
    {
        //b2World* world;

        Gravity() 
        {
            //world = LayerFysics::GetInstance()->GetWorld();
        }

        void Gravity::SetGravity(float x, float y, bool wake)
        {
            //if (!world) return;
            //world->SetGravity(b2Vec2(x, y));
            //if (wake) for (b2Body* b = world->GetBodyList();b;b = b->GetNext()) b->SetAwake(true);
        }

        //b2Vec2 Gravity::GetGravity() const
        //{
            //return world ? world->GetGravity() : b2Vec2(0.f, 0.f);
        //}
    };
}

