#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicsBody.h"
#include "Fysics/Shapes.h"
#include "Fysics/Joints.h"
#include "Fysics/Force.h"

#include <unordered_map>
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

    public:
        static LayerFysics* s_instance;
        std::unordered_map<int, FysicsBody*> BodyDic;
        int BodyCount = 0;


        static void SetInstance(LayerFysics* instance) {
            s_instance = instance;
        }

        void FB_Create()
        {
            FysicsBody* body = new FysicsBody(s_instance->GetWorld());
            BodyDic[BodyCount] = body;
            BodyCount++;
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