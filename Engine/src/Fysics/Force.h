#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class Force
    {
    private:
        b2World* world;
        b2Body* body;

        public:

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
        // Add impulse
        void AddImpulse(b2Vec2 impulse, b2Vec2 point)
        {
            if (body)
            {
                body->ApplyLinearImpulse(impulse, point, true);
            }
        }

        // Add impulse at the center of mass 
        void AddImpulseToCenter(b2Vec2 impulse)
        {
            if (body)
            {
                body->ApplyLinearImpulseToCenter(impulse, true);
            }
        }
        //Add Torque
        void AddTorque(float torque)
        {
            if (body)
            {
                body->ApplyTorque(torque, true);
            }
        }
        //Add impulse with torque
        void AddAngularImpulse(float torque) {
            if (body) {
                body->ApplyAngularImpulse(torque, true);
            }
        }
    };
    
}

