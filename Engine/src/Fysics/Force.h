#pragma once
#include "box2d.h"

namespace IonixEngine
{
    class Entity;
    class Force
    {
    private:
	
		// Not the best idea with this integration - world is singular to FysicsManager and body is accessed via the new dictionary (see Force.cpp for example of how).
        //b2World* world;
        //b2Body* body;

        public:

        void AddForce(Entity* entity, float xForce, float yForce, float xForceOrigin, float yForceOrigin);
        void AddImpulseForce(Entity* entity, float xForce, float yForce, float xForceOrigin, float yForceOrigin);

		/*
        Nothing wrong with the below, they just need the same treatment as above.
			 This needs tasking up as 'refactoring x to work with ECS system'
			
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
        */
		
    };
    
}

