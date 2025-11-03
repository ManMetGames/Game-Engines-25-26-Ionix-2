#pragma once
#include "box2d.h"
#include <iostream>
#include "Architecture/Application.h"

namespace IonixEngine
{
    // Called when two fixtures begin to touch
    void ContactListener::BeginContact(b2Contact* contact) 
    {
        // Get the fixtures that are colliding
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        // Get the bodies
        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        Entity* colObject1 = Application::Get().layerFysics->GetFysicsManager()->GetEntityFromBody(bodyA);
        Entity* colObject2 = Application::Get().layerFysics->GetFysicsManager()->GetEntityFromBody(bodyB);

        std::cout << "Collision started!" << std::endl;
		
        // Go ahead and raise an event with the two Entity* objects 
    }  
}

