#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicsBody.h"
#include "Fysics/Shapes.h"
#include "Fysics/Joints.h"
#include "Fysics/Force.h"

#include <unordered_map>
#include <vector>
#include <functional>

#include "Maf/MafUtils.h"

using EntityID = int;

namespace IonixEngine
{
    class FysicsManager
    {
    private:
        FysicsShapes* shape = new FysicsShapes();
        PrismaticJoints* joint;
        Force* force;
        
    public:
        static LayerFysics* s_instance;
        std::unordered_map<int, FysicsBody*> BodyDic;
        int BodyIncrement = 0;

        static void SetInstance(LayerFysics* instance) {
            s_instance = instance;
        }

        void FB_Create()
        {
            FysicsBody* body = new FysicsBody(s_instance->GetWorld());
            BodyDic[BodyIncrement] = body;
            std::cout << "Body Created at Index: " << BodyIncrement << std::endl;
            BodyIncrement++;
        }

        void FB_AddShape(int dicIndex, fysicShapeType shapeType)
        {
            shape->SetBody(BodyDic[dicIndex]->GetBody());
            switch (shapeType)
            {
            case fysicShapeType::circle:
                shape->AddCircle();
                break;
            case fysicShapeType::box:
                shape->AddBox();
                break;
            }
        }

        void FB_AddPolygon(int dicIndex, std::vector<b2Vec2>& vertices)
        {
            shape->SetBody(BodyDic[dicIndex]->GetBody());
            shape->AddPolygon(vertices);
        }
        
        void FB_Destroy(int dicIndex)
        {
            BodyDic.erase(dicIndex);
        }

        b2Vec2 FB_GetPos(int dicIndex)
        {                       
            return BodyDic[dicIndex]->GetPosition();
        }

        void FB_SetPos(int dicIndex, b2Vec2 bodyPos)
        {
            BodyDic[dicIndex]->SetPosition(bodyPos);
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