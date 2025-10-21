#pragma once
#include <sol/sol.hpp>

#include "Architecture/Application.h"
#include "Architecture/ECS/Component.hpp"
#include "Architecture/ECS/Entity.hpp"
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    enum class fysicBodyType {staticBody, dynamicBody, kinematicBody};
    
    class FysicBody : public Component
    {
    private:
        b2World* world;
        b2Body* body;
    public:
        FysicBody(Entity* entity) : Component(entity, false, true, false)
        {
            world = Application::Get().layerFysics->GetWorld();
            //world = LayerFysics::GetInstance()->GetWorld();
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(0, 10);
            bodyDef.awake = true;
            bodyDef.fixedRotation = false;
            
            body = world->CreateBody(&bodyDef);
            Application::Get().layerFysics->entities[body] = entity->id;
        }
        
        FysicBody(Entity* entity, float xPos, float yPos, fysicBodyType b_type, bool rotationLocked) : Component(entity, false, true, false)
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

        FysicBody(Entity* entity, float xPos, float yPos, fysicBodyType b_type, bool rotationLocked, float gravityScale) : Component(entity, false, true, false)
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
        
        void AddForce(b2Vec2 force, b2Vec2 point)
        {
            if (body)
            {
                body->ApplyForce(force, point, true);
            }
        }

        void AddForceToCenter(b2Vec2 force)
        {
            if (body)
            {
                body->ApplyForceToCenter(force, true);
            }
        }

        virtual void Update(float dt) override;

    };

    void FysicBody::Update(float dt) {
        b2ContactEdge* contacts = body->GetContactList();
        if (contacts->contact) {
            Entity* other = entity->scene->GetEntityFromID(Application::Get().layerFysics->entities[contacts->other]);
            if (!other) { return; }
            entity->Collision(other);
        }
    }
}

