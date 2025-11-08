#pragma once
#include "Fysics/FysicsBody.h"
#include "Fysics/Shapes.h"
#include "Fysics/Force.h"
#include "Fysics/RigidBodyTransform.h"
#include "Architecture/ECS/Entity.hpp"

#include <unordered_map>
#include "box2d.h"

namespace IonixEngine
{
    class FysicsManager
    {
    private:
 
        b2World* world;
        FysicsShapes* shapes;
        Force* force;
        std::unordered_map<b2Body*, Entity*> entityBodyMap;
        std::unordered_map<b2Body*, RigidBodyTransform> transformMap;

    public:
        FysicsManager();
        ~FysicsManager();

        FysicsShapes* GetShapes() { return shapes; }
        Force* GetForce() { return force; }
        b2World* GetWorld() { return world; }
        std::unordered_map<b2Body*, Entity*>& GetBodyMap() { return entityBodyMap; }
        std::unordered_map<b2Body*, RigidBodyTransform>& GetTransformMap() { return transformMap; }
        
        b2Body* GetBodyFromEntity(Entity* entity);
        Entity* GetEntityFromBody(b2Body* body);

        // these relied on static s_instance and BodyDic which are now gone.
        // you'll need to refactor them to use the instance approach.

        /*
        // Body management
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

        b2Vec2 FB_GetWorldGravity()
        {
            if (!gravity && s_instance) gravity = new Gravity(s_instance->GetWorld());
            return gravity ? gravity->GetGravity() : b2Vec2(0.f, 0.f);
        }

        void FB_SetGravity(b2Vec2 bodyGravity)
        {
            if (!gravity && s_instance) gravity = new Gravity(s_instance->GetWorld());
            if (gravity) gravity->SetGravity(bodyGravity.x, bodyGravity.y, true);
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
            return false;
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

        }
        */
    };
}