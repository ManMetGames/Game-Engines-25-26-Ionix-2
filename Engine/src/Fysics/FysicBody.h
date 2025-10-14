#pragma once
#include <sol/sol.hpp>

#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    enum class fysicBodyType {staticBody, dynamicBody, kinematicBody  };
    
    class FysicBody
    {
        b2World* world;

        fysicBodyType type;
        switch (type)
        {
        case fysicBodyType::staticBody:
            
        }
        
        FysicBody(float xPos, float yPos, enum fysicBodyType)
        {
            world = LayerFysics::GetInstance()->GetWorld();

            b2BodyDef bodyDef;
            bodyDef.type = b2;
            bodyDef.position.Set(xPos, yPos);
            b2Body* body = world.CreateBody(&bodyDef);
        }
    };
    
}

