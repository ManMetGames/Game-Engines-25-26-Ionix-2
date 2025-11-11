#pragma once

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
            // add forces to my fysics manager for the body dictionary
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

        // Clear all forces/velocities on this body
        void ClearForces()
        {
            if (body)
            {
                body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                body->SetAngularVelocity(0.0f);
            }
        }
        
    };
    
}