#pragma once
#include "Fysics/FysicsManager.h"
#include "Architecture/Application.h"

namespace IonixEngine
{
    FysicsManager::FysicsManager()
    {
        b2Vec2 gravity = b2Vec2(0.0f, 9.8f);
        world = new b2World(gravity);
		
		// Collision callback system - not ECS
		world->SetContactListener(&contactListener);

		shapes = new FysicsShapes();
        force = new Force();
    }

	// Searches through the dictionary for an entity.
	// Once entity is found, returns the associated rigidbody.
    b2Body* FysicsManager::GetBodyFromEntity(Entity* entity)
    {
        for(auto& pair : entityBodyMap)
        {
            if(pair.second == entity)
            {
                return pair.first;
            }
		}
    }

	// Searches through the dictionary for a rigidbody.
	// Once rigidbody is found, returns the associated entity. - The opposite of the other method.
    Entity* FysicsManager::GetEntityFromBody(b2Body* body)
    {
        for (auto& pair : entityBodyMap)
        {
            if (pair.first == body)
            {
                return pair.second;
            }
        }
    }
}