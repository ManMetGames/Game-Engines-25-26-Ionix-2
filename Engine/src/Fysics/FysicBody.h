#pragma once
#include <sol/sol.hpp>

#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/Shapes.h"

namespace IonixEngine
{
    enum class fysicBodyType {staticBody, dynamicBody, kinematicBody};
    
    class FysicBody
    {
    private:
        b2World* world;
        b2Body* body;
        FysicsShapes* shape;
    public:
        FysicBody()
        {
            world = Application::Get().layerFysics->GetWorld();
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(0, 10);
            bodyDef.awake = true;
            bodyDef.fixedRotation = false;
            body = world->CreateBody(&bodyDef);

            shape = new FysicsShapes(body);
            shape->AddBox(5.0f, 5.0f);

            
        }
        
        FysicBody(float xPos, float yPos, fysicBodyType b_type, bool rotationLocked)
        {
            world = LayerFysics::GetInstance()->GetWorld();
            b2BodyDef bodyDef;
            switch (b_type)
            {
                case fysicBodyType::staticBody:
                    bodyDef.type = b2_staticBody;
                     break;
                case fysicBodyType::dynamicBody:
                    bodyDef.type = b2_dynamicBody;
                    break;
                case fysicBodyType::kinematicBody:
                    bodyDef.type = b2_kinematicBody;
                    break;
            }
            bodyDef.position.Set(xPos, yPos);
            bodyDef.awake = true;
            bodyDef.fixedRotation = rotationLocked;
            body = world->CreateBody(&bodyDef);
        }

        FysicBody(float xPos, float yPos, fysicBodyType b_type, bool rotationLocked, float gravityScale)
        {
            world = LayerFysics::GetInstance()->GetWorld();
            b2BodyDef bodyDef;
            switch (b_type)
            {
            case fysicBodyType::staticBody:
                bodyDef.type = b2_staticBody;
                break;
            case fysicBodyType::dynamicBody:
                bodyDef.type = b2_dynamicBody;
                break;
            case fysicBodyType::kinematicBody:
                bodyDef.type = b2_kinematicBody;
                break;
            }
            bodyDef.position.Set(xPos, yPos);
            bodyDef.awake = true;
            bodyDef.fixedRotation = rotationLocked;
            bodyDef.gravityScale = gravityScale;
            body = world->CreateBody(&bodyDef);
        }

        ~FysicBody()
        {
            if (world && body)
            {
                world->DestroyBody(body);
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

        void SetPosition(float x, float y)
        {
            if (body)
            {
                b2Vec2 newPos(x, y);
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
        void SetLinearVelocity(float x, float y)
        {
            if (body)
            {
                b2Vec2 newVelocity(x, y);
                body->SetLinearVelocity(newVelocity);
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
        
        //Add Force
        
       
        //Set Active
        void SetActive(bool flag) {
            if (body) {
                body->SetAwake(flag);
            }
        }
        //Get Active 
        bool GetActive() {
            return body->IsAwake();
        }
        
        //Rotate Position or Body - doesnt set the angle to be something like Set Angle adds rotation to current body
        void RotatePosition(float angle) {
            float currentAngle = body->GetAngle(); 
            float newAngle = currentAngle + angle; // adding rotation to current rotation
            body->SetTransform(body->GetPosition(), newAngle);

        }
        
    };
}

