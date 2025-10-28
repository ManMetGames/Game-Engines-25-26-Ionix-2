#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicsBody.h"
#include "Fysics/Shapes.h"
#include "Fysics/Joints.h"
#include "Fysics/Force.h"

#include <vector>
#include <functional>

using EntityID = int;

namespace IonixEngine
{
    class FysicsManager
    {
    private:
        FysicsShapes* shape;
        PrismaticJoints* joint;
        Force* force;
        b2World* world;
        using CollisionCallback = std::function<void(EntityID, EntityID)>;
        std::vector<CollisionCallback> collisionCallbacks_;

    public:
        static LayerFysics* s_instance;
        static void SetInstance(LayerFysics* instance) {
            s_instance = instance;
        }

        void RegisterCollisionCallback(CollisionCallback callback);
        void EmitCollision(EntityID a, EntityID b);
        void Update();

        void Create() 
        {   
            FysicsBody* body = new FysicsBody(world);
            FysicsBody* bodyb = new FysicsBody(world);
            shape = new FysicsShapes();
            shape->AttatchBody(body->GetBody());
            shape->AddBox();
            shape->AttatchBody(bodyb->GetBody());
            shape->AddBox();

            joint = new PrismaticJoints();
            joint->setJoint(body->GetBody(), bodyb->GetBody(), b2Vec2_zero, 1.0f, 1.0f, true, 1.0f, 1.0f, true);

        }
    };
}