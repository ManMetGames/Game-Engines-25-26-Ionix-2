#pragma once
#include "Fysics/FysicsBody.h"
#include "Fysics/Joints.h"
#include "Fysics/Force.h"
#include <Architecture/ECS/Component.hpp>
#include "Fysics/Shapes.h"
#include <unordered_map>
#include "box2d.h"
#include "ContactLisener.h"

namespace IonixEngine
{
    class FysicsManager
    {
		// This should be the sole instance of b2World*
        b2World* world;
		
		// Part of the collision callback system - separate integration
        ContactListener contactListener;
	
		// All of the new classes should be managed via this class like so - initialised in FysicsManager.cpp
        FysicsShapes* shapes;
        Force* force;

		// This data structure is our best friend - It, when combined with two helper methods below, gives us access to find:
		// a) if we know an entity, we can get the rigidbody associated with it to add force to said entity. That's really powerful when the end Lua user only has access to their entity.
		// b) if we know a rigidbody, we can find the associated entity - good for backend cpp management if needed
		std::unordered_map<b2Body*, Entity*> entityBodyMap;

    public:
        FysicsManager();
        ~FysicsManager()
        {
            delete world;
        }

		// Getters
		FysicsShapes* GetShapes() { return shapes; }
		Force* GetForce() { return force; }
        b2World* GetWorld() { return world; }
		std::unordered_map<b2Body*, Entity*>& GetBodyMap() { return entityBodyMap; }
        
		// Helper declarations
		b2Body* GetBodyFromEntity(Entity* entity);
        Entity* GetEntityFromBody(b2Body* entity);
    };
}