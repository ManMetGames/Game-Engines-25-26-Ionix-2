#pragma once
#include <sol/sol.hpp>

#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    enum class fysicBodyType {staticBody, dynamicBody, kinematicBody};
    
    class FysicBody
    {
    private:
        b2World* world;
        b2Body* body;
    public:
        FysicBody()
        {
            world = LayerFysics::GetInstance()->GetWorld();
            b2BodyDef bodyDef;
            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(0, 0);
            bodyDef.awake = true;
            bodyDef.fixedRotation = false;
            body = world->CreateBody(&bodyDef);
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
    };
}

