#pragma once
#include <sol/sol.hpp>

#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/Shapes.h"

namespace IonixEngine
{
    enum class fysicsBodyType {staticBody, dynamicBody, kinematicBody};
    
    class FysicsBody
    {
    private:
        b2Body* body;
    public:
        FysicsBody(b2World* world)
        {
            if (!world) world = nullptr;
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(0, 10);
            bodyDef.awake = true;
            bodyDef.fixedRotation = false;
            body = world->CreateBody(&bodyDef);
            
        }
        
        FysicsBody(b2World* world, float xPos, float yPos, fysicsBodyType b_type, bool rotationLocked)
        {
            if (!world) world = nullptr;
            b2BodyDef bodyDef;
            switch (b_type)
            {
                case fysicsBodyType::staticBody:
                    bodyDef.type = b2_staticBody;
                     break;
                case fysicsBodyType::dynamicBody:
                    bodyDef.type = b2_dynamicBody;
                    break;
                case fysicsBodyType::kinematicBody:
                    bodyDef.type = b2_kinematicBody;
                    break;
            }
            bodyDef.position.Set(xPos, yPos);
            bodyDef.awake = true;
            bodyDef.fixedRotation = rotationLocked;
            body = world->CreateBody(&bodyDef);
        }

        FysicsBody(b2World* world, float xPos, float yPos, fysicsBodyType b_type, bool rotationLocked, float gravityScale)
        {
            if (!world) world = nullptr;
            b2BodyDef bodyDef;
            switch (b_type)
            {
            case fysicsBodyType::staticBody:
                bodyDef.type = b2_staticBody;
                break;
            case fysicsBodyType::dynamicBody:
                bodyDef.type = b2_dynamicBody;
                break;
            case fysicsBodyType::kinematicBody:
                bodyDef.type = b2_kinematicBody;
                break;
            }
            bodyDef.position.Set(xPos, yPos);
            bodyDef.awake = true;
            bodyDef.fixedRotation = rotationLocked;
            bodyDef.gravityScale = gravityScale;
            body = world->CreateBody(&bodyDef);
        }
        
        //get body def
        b2Body* GetBody()
        {
            return body;
        }

        ~FysicsBody()
        {
            if (body->GetWorld() && body)
            {
                body->GetWorld()->DestroyBody(body);
                body = nullptr;
            }
        }

        
        //Get & Set Position
        b2Vec2 GetPosition() const
        {
            if (body)
            {
                return body->GetPosition();
            }
            return b2Vec2(0.0f, 0.0f);            
        }

        void SetPosition(b2Vec2 newPos)
        {
            if (body)
            {
                body->SetTransform(newPos, body->GetAngle());//Sets the position, and makes sure the angle doesnt change
            }
        }

        //Get & Set Angle

        float GetAngle() const
        {
            if (body)
            {
                return body->GetAngle();
            }
            return 0.0f;
        }

        void SetAngle(float angleInRadians)
        {
            if (body)
            {
                b2Vec2 currentPos = body->GetPosition();
                body->SetTransform(currentPos, angleInRadians);
            }
        }

        //Get&Set Linear Velocity
        b2Vec2 GetLinearVelocity() const
        {
            if (body)
            {
                return body->GetLinearVelocity();
            }
            return b2Vec2(0.0f, 0.0f);
        }
        void SetLinearVelocity(b2Vec2 newLinVel)
        {
            if (body)
            {
                body->SetLinearVelocity(newLinVel);
            }
        }

        //Get&Set Angular Velocity
        float GetAngularVelocity() const
        {
            if (body)
            {
                return body->GetAngularVelocity();
            }
            return 0.0f;
        }
        void SetAngularVelocity(float x)
        {
            if (body)
            {
                body->SetAngularVelocity(x);
            }
        }
        
        //Set Active
        void SetAwake(bool flag) {
            if (body) {
                body->SetAwake(flag);
            }
        }
        //Get Active 
        bool GetAwake() {
            return body->IsAwake();
        }
        
        //Rotate Position or Body - doesnt set the angle to be something like Set Angle adds rotation to current body
        void RotatePosition(float angle) {
            float currentAngle = body->GetAngle(); 
            float newAngle = currentAngle + angle; // adding rotation to current rotation
            body->SetTransform(body->GetPosition(), newAngle);
        }

        //Linear Damping
        float GetLinearDamping()
        {
                return body->GetLinearDamping();
        }

        void SetLinearDamping(float linearDamping)
        {
                body->SetLinearDamping(linearDamping);
        }

        //Angular Damping
        float GetAngularDamping()
        {
            return body->GetAngularDamping();
        }

        void SetAngularDamping(float angularDamping)
        {
            body->SetAngularDamping(angularDamping);
        }

        //Allow Sleep
        bool GetAllowSleep()
        {
            return body->IsSleepingAllowed();
        }

        void SetAllowSleep(bool flag)
        {
            body->SetSleepingAllowed(flag);
        }

        //Fixed Rotation
        bool GetFixedRotation()
        {
            return body->IsFixedRotation();
        }

        void SetFixedRotation(bool flag)
        {
            body->SetFixedRotation(flag);
        }

        //Bullet
        bool GetIsBullet()
        {
            return body->IsBullet();
        }

        void SetIsBullet(bool flag)
        {
            body->SetBullet(flag);
        }

        //Gravity Scale
        float GetGravityScale()
        {
            return body->GetGravityScale();
        }

        void SetGravityScale(float gravityScale)
        {
            body->SetGravityScale(gravityScale);
        }
        
    };
}

