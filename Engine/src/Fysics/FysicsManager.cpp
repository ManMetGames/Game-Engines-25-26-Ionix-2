#include "FysicsManager.h"
#include "Architecture/Application.h"
#include <Testing/Box2D/DebugDraw.h>

namespace IonixEngine
{
	FysicsManager* FysicsManager::GetManager() {
		return Application::Get().layerFysics->GetFysicsManager();
	}

	FysicsManager::FysicsManager()
	{
		b2Vec2 gravity(0.0f, 9.8f);
		world = new b2World(gravity);

		DebugDraw* debugDraw = new DebugDraw(Application::Get().GetWindow().GetSdlRenderer(), 30.0f);
		debugDraw->SetFlags(
			b2Draw::e_shapeBit
			| b2Draw::e_jointBit
			| b2Draw::e_centerOfMassBit
		);

		world->SetDebugDraw(debugDraw);

		// create and configure collision listener
		collisionListener = new CollisionListener(this);
		collisionListener->SetEventCallback([](IonixEvent& e) {
			Application::Get().OnEvent(e);
		});
		world->SetContactListener(collisionListener);

		shapes = new FysicsShapes();
		force = new Force();
		prismaticJoint = new PrismaticJoints();
		weldJoint = new WeldJoints();
		pulleyJoint = new PulleyJoints();
		revoluteJoint = new RevoluteJoints();
		distanceJoint = new DistanceJoints();
	}

	FysicsManager::~FysicsManager()
	{
		// helpers
		delete shapes;
		delete force;

		// cleans up all bodies in the map
		bodyEntityMap.clear();

		// (this also destroys all bodies/fixtures/joints)
		delete world;
	}

	b2Body* FysicsManager::GetBodyFromEntity(Entity* entity)
	{
		for (auto& pair : bodyEntityMap)
		{
			if (pair.second == entity)
			{
				return pair.first;
			}
		}
		return nullptr;
	}

	Entity* FysicsManager::GetEntityFromBody(b2Body* body)
	{
		auto it = bodyEntityMap.find(body);
		if (it != bodyEntityMap.end())
		{
			return it->second;
		}
		return nullptr;
	}

	void FysicsManager::AddEntityBodyPair(Entity* entity, b2Body* body)
	{
		//entityBodyMap[entity] = body; // Not needed as we have helper methods which get what we need from 1 data structure.
		bodyEntityMap[body] = entity;
	}
}