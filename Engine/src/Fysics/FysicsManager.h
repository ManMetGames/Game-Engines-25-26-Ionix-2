#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicsBody.h"
#include "Fysics/Shapes.h"
#include "Fysics/Joints.h"
#include "Fysics/Force.h"
#include "Fysics/Collider.h"

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
        b2ContactListener* contactListener;

        using GlobalCollisionCallback = std::function<void(Collider*, Collider*)>;
        std::vector<GlobalCollisionCallback> globalCollisionCallbacks_;

    public:
        static LayerFysics* s_instance;
        static void SetInstance(LayerFysics* instance) {
            s_instance = instance;
        }

        void RegisterCollisionCallback(const GlobalCollisionCallback& callback)
        {
            globalCollisionCallbacks_.push_back(callback);
        }

        void EmitCollision(Collider* a, Collider* b)
        {
            std::cout << "[FysicsManager] Collision emitted\n";

            for (auto& cb : globalCollisionCallbacks_)
                cb(a, b);

            if (a) a->EmitCollision(b);
            if (b) b->EmitCollision(a);
        }

        void BeginContact(b2Contact* contact)
        {
            auto* colA = (Collider*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
            auto* colB = (Collider*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

            if (colA && colB)
            {
                colA->EmitCollision(colB);
                colB->EmitCollision(colA);
            }
        }

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

//class ContactListener : public b2ContactListener
//{
//public:
//    void BeginContact(b2Contact* contact) override
//    {
//        auto colA = (Collider*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
//        auto colB = (Collider*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
//
//        if (colA && colB)
//        {
//            colA->EmitCollision(colB);
//            colB->EmitCollision(colA);
//        }
//    }
//};