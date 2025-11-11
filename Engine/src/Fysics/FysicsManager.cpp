#include "FysicsManager.h"

namespace IonixEngine
{
	FysicsManager::FysicsManager()
	{
		b2Vec2 gravity(0.0f, 9.8f);
		world = new b2World(gravity);

		shapes = new FysicsShapes();
		force = new Force();
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