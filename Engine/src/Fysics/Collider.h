#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Maf/MafUtils.h"
#include "Fysics/Shapes.h"

#include <iostream>

namespace IonixEngine
{
    class Collider
    {
        /*b2World* world;
        FysicsShapes shape;

        using CollisionCallback = std::function<void(Collider* other)>;
        std::vector<CollisionCallback> collisionCallbacks_;*/

    public:

        Collider()
        {
            //world = LayerFysics::GetInstance()->GetWorld();
        }

        /*Collider(b2Body* body)
        {
            world = LayerFysics::GetInstance()->GetWorld();
            shape.AttatchBody(body);
        }

        FysicsShapes& GetShape() { return shape; }

        void RegisterCollisionCallback(const CollisionCallback& callback)
        {
            collisionCallbacks_.push_back(callback);
        }

        void EmitCollision(Collider* other)
        {
            for (auto& cb : collisionCallbacks_)
                cb(other);
        }

        void SetWorld(b2World* newWorld) { world = newWorld; }
        b2World* GetWorld() const { return world; }
        */
    };
}

