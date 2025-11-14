#include "FysicsManager.h"
#include "Architecture/Application.h"

namespace IonixEngine
{
	FysicsManager* FysicsManager::GetManager() {
		return Application::Get().layerFysics->GetFysicsManager();
	}

	FysicsManager::FysicsManager()
	{
		b2Vec2 gravity(0.0f, 9.8f);
		world = new b2World(gravity);

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
		entityBodyMap.clear();

		// (this also destroys all bodies/fixtures/joints)
		delete world;
	}

	b2Body* FysicsManager::GetBodyFromEntity(Entity* entity)
	{
		for (auto& pair : entityBodyMap)
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
		auto it = entityBodyMap.find(body);
		if (it != entityBodyMap.end())
		{
			return it->second;
		}
		return nullptr;
	}
}