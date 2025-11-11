#pragma once
#include "Fysics/FysicsBody.h"
#include "Fysics/Shapes.h"
#include "Fysics/Force.h"
#include "Fysics/RigidBodyTransform.h"
#include "Architecture/ECS/Entity.hpp"
#include <unordered_map>
#include "box2d.h"
#include "Fysics/Collider.h"
#include <vector>
#include <functional>
#include "Contactlistener.h"

using Entity = int;

namespace IonixEngine
{
    class LayerFysics;

    class FysicsManager
    {
    private:
 
        b2World* world;
        FysicsShapes* shapes;
        Force* force;
        std::unordered_map<b2Body*, Entity*> entityBodyMap;
        std::unordered_map<b2Body*, RigidBodyTransform> transformMap; //used for interpolation

    public:
        FysicsManager();
        ~FysicsManager();

        FysicsShapes* GetShapes() { return shapes; }
        Force* GetForce() { return force; }
        b2World* GetWorld() { return world; }
        std::unordered_map<b2Body*, Entity*>& GetBodyMap() { return entityBodyMap; }
        std::unordered_map<b2Body*, RigidBodyTransform>& GetTransformMap() { return transformMap; } //used for interpolation
        
        b2Body* GetBodyFromEntity(Entity* entity);
        Entity* GetEntityFromBody(b2Body* body);

        // ---------- Forces ----------

        void FB_AddForce(int dicIndex, b2Vec2 forceVec, b2Vec2 point)
        {
            if (!force) force = new Force();
            force->SetBody(BodyDic[dicIndex]->GetBody());
            force->AddForce(forceVec, point);
        }

        void FB_AddForceToCenter(int dicIndex, b2Vec2 forceVec)
        {
            if (!force) force = new Force();
            force->SetBody(BodyDic[dicIndex]->GetBody());
            force->AddForceToCenter(forceVec);
        }

        void FB_AddImpulse(int dicIndex, b2Vec2 impulse, b2Vec2 point)
        {
            if (!force) force = new Force();
            force->SetBody(BodyDic[dicIndex]->GetBody());
            force->AddImpulse(impulse, point);
        }

        void FB_AddImpulseToCenter(int dicIndex, b2Vec2 impulse)
        {
            if (!force) force = new Force();
            force->SetBody(BodyDic[dicIndex]->GetBody());
            force->AddImpulseToCenter(impulse);
        }

        void FB_AddTorque(int dicIndex, float torque)
        {
            if (!force) force = new Force();
            force->SetBody(BodyDic[dicIndex]->GetBody());
            force->AddTorque(torque);
        }

        void FB_AddAngularImpulse(int dicIndex, float torque)
        {
            if (!force) force = new Force();
            force->SetBody(BodyDic[dicIndex]->GetBody());
            force->AddAngularImpulse(torque);
        }

        void FB_ClearForces(int dicIndex)
        {
            if (!force) force = new Force();
            force->SetBody(BodyDic[dicIndex]->GetBody());
            force->ClearForces();
        }

        void AddJointToFysicsBody()
        {
            //...
        }
    };
}