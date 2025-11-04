#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicsBody.h"
#include "Fysics/Shapes.h"
#include "Fysics/Joints.h"
#include "Fysics/Force.h"
#include "Fysics/Collider.h"
#include <vector>
#include <functional>
#include "Contactlistener.h"
#include <unordered_map>

using Entity = int;

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
        std::unordered_map<Entity*, Collider*> entityToColliderMap;


        static LayerFysics* s_instance;
    public:
        static void SetInstance(LayerFysics* instance) {
            s_instance = instance;
        }

        b2Body* GetBodyFromEntity(Entity* entity);
        Entity* GetEntityFromBody(b2Body* entity);
        Collider* GetColliderForEntity(Entity* entity);

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