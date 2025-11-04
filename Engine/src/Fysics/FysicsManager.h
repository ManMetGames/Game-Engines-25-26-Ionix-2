#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicsBody.h"
#include "Fysics/Shapes.h"
#include "Fysics/Joints.h"
#include "Fysics/Force.h"
#include "Fysics/Collider.h"
#include <vector>
#include <functional>
#include "../../contactlistener.h"
#include <unordered_map>

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
        ContactListener contactListener;

        using GlobalCollisionCallback = std::function<void(Collider*, Collider*)>;
        std::vector<GlobalCollisionCallback> globalCollisionCallbacks_;

        std::unordered_map<b2Body*, Entity*> entityBodyMap;

    public:
        static LayerFysics* s_instance;
        static void SetInstance(LayerFysics* instance) {
            s_instance = instance;
        }

        b2Body* GetBodyFromEntity(Entity* entity);
        Entity* GetEntityFromBody(b2Body* entity);

        void RegisterCollisionCallback(const GlobalCollisionCallback& callback)
        {
            globalCollisionCallbacks_.push_back(callback);
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