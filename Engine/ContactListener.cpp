#pragma once
#include "box2d.h"
#include <iostream>
#include "ContactListener.h"
#include "Fysics/Collider.h"

namespace IonixEngine
{
	void ContactListener::BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();

		Entity* colObject1 = Application::Get().layerFysics->GetFysicsManager()->GetEntityFromBody(bodyA);
		Entity* colObject2 = Application::Get().layerFysics->GetFysicsManager()->GetEntityFromBody(bodyB);

		std::cout << "Collision started!" << std::endl;

		if (colObject1 && colObject2)
		{
			colObject1->Collider::EmitCollision(colObject2);
			colObject2->Collider::EmitCollision(colObject1);
		}
	}
}
