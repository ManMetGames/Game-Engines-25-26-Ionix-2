#pragma once
#include <box2d.h>
#include <iostream>
#include "ContactListener.h"
#include "Fysics/Collider.h"
#include "Fysics/FysicsManager.h"
#include "Architecture/Application.h"
#include <EventSystem/EventSDL.h>

namespace IonixEngine
{
	void ContactListener::BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();

		std::cout << "=== COLLISION BEGIN ===" << std::endl;

		Entity* colObject1 = Application::Get().layerFysics->GetFysicsManager()->GetEntityFromBody(bodyA); 
		Entity* colObject2 = Application::Get().layerFysics->GetFysicsManager()->GetEntityFromBody(bodyB); 

		std::cout << "Collision started!" << std::endl;

		if (colObject1 && colObject2)
		{
			std::cout << "Entities found for collision!" << std::endl;
			//Collider* col1 = Application::Get().layerFysics->GetFysicsManager()->GetColliderForEntity(colObject1);
			//Collider* col2 = Application::Get().layerFysics->GetFysicsManager()->GetColliderForEntity(colObject2);

			//if (col1 && col2)
			//{
				EntityCollisionEvent event(fixtureA, fixtureB);
				IonixEngine::EventSDL::EventCallback m_EventCallback;
				m_EventCallback(event);

				//col1->EmitCollision(col2);
				//col2->EmitCollision(col1);
			//}
		}
		std::cout << "=======================" << std::endl;
	}

	void ContactListener::EndContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();

		Entity* colObject1 = Application::Get().layerFysics->GetFysicsManager()->GetEntityFromBody(bodyA);
		Entity* colObject2 = Application::Get().layerFysics->GetFysicsManager()->GetEntityFromBody(bodyB);

		std::cout << "Collision ended!" << std::endl;

		if (colObject1 && colObject2)
		{
			EntityCollisionEvent event(fixtureA, fixtureB);
			IonixEngine::EventSDL::EventCallback m_EventCallback;
			m_EventCallback(event);
		}
	}
}