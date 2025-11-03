#pragma once
#include "Fysics/Force.h"
#include "Architecture/Application.h"

void IonixEngine::Force::AddForce(Entity* entity, float xForce, float yForce, float xForceOrigin, float yForceOrigin)
{
	// Look how I use the entity (will be passed by the end user in Lua).
	// I pass it to one of the two helper functions to extract the physics body that is associated with the entity the end user wants to add force to.
	b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);

	// The rest is as you would expect...
    if (body)
    {
        body->ApplyForce(b2Vec2(xForce, yForce), b2Vec2(xForceOrigin, yForceOrigin), true);
    }
}

// Look how the identical workflow applies - be mindful of the scaling.
void IonixEngine::Force::AddImpulseForce(Entity* entity, float xForce, float yForce, float xForceOrigin, float yForceOrigin)
{
    b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);

    if (body)
    {
        // Impulses are in Newton-seconds or kg m/s
        b2Vec2 impulse(xForce, yForce);
        b2Vec2 point = body->GetWorldCenter();  // Apply at center of mass for now

        // If specific point is given (not 0,0), convert from pixels to meters (Box2D - meters, SDL2 - pixels)
        if (xForceOrigin != 0 || yForceOrigin != 0)
        {
            point.x /= 100.0f;
            point.y /= 100.0f;
        }

        body->ApplyLinearImpulse(impulse, point, true);
    }
}
