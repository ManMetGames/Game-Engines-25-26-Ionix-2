#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Architecture/Application.h"

namespace IonixEngine
{
    class Force
    {
    private:
        b2World* world;
        b2Body* body;

    public:

        void SetBody(b2Body* b)
        {
            body = b;
        }

        void AddForce(Entity* entity, b2Vec2 force, b2Vec2 point);
        void AddForceToCenter(Entity* entity, b2Vec2 force);

        void AddImpulse(Entity* entity, float impulseX, float impulseY, int forcePosX, int forcePosY);
        void AddImpulseToCenter(Entity* entity, b2Vec2 impulse);
        void AddAngularImpulse(Entity* entity, float torque);

        void AddTorque(Entity* entity, float torque);

        void ClearForces(Entity* entity);
        
    };
    
}