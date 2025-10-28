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

        void FB_Create()
        {
            FysicsBody* body = new FysicsBody(world);
        }

        void FB_Destroy()
        {

        }

        void FB_GetPos()
        {

        }

        void FB_SetPos()
        {

        }

        void FB_GetAngle()
        {

        }

        void FB_SetAngle()
        {

        }

        void FB_GetVel()
        {

        }

        void FB_SetVel()
        {

        }

        void FB_GetAngularVel()
        {

        }


        void FB_SetAngularVel()
        {

        }

        bool FB_IsActive()
        {

        }

        void FB_SetActive()
        {

        }

        void FB_GetLinearDampaning()
        {

        }

        void FB_SetLinearDampaning()
        {

        }

        void FB_GetAngularDampaning()
        {

        }

        void FB_SetAngularDampaning()
        {

        }

        void FB_IsSleepAllowed()
        {

        }

        void FB_SetSleepAllowed()
        {

        }
        
        void FB_GetFixedRotation()
        {

        }

        void FB_SetFixedRotation()
        {

        }

        void FB_IsBullet()
        {

        }

        void FB_SetIsBullet()
        {

        }

        void FB_GetGravityScale()
        {

        }

        void FB_SetGravityScale()
        {

        }
       






        //-------------------------
        void AddJointToFysicsBody()
        {

        }

    };
}