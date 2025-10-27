#include "Fysics/FysicsManager.h"
#include "Fysics/Collider.h"
#include <iostream>

void IonixEngine::FysicsManager::RegisterCollisionCallback(CollisionCallback callback)
{
	collisionCallbacks_.push_back(std::move(callback));
}

void IonixEngine::FysicsManager::EmitCollision(EntityID a, EntityID b)
{
	for (auto& callback : collisionCallbacks_)
	{
		callback(a, b);
	}
}

void IonixEngine::FysicsManager::Update()
{
	static bool callbackRegistered = false;
	if (!callbackRegistered)
	{
		RegisterCollisionCallback([](EntityID a, EntityID b) 
		{
				std::cout << "[Callback Fired] Entity " << a << " collided with Entity " << b << "\n";
		});
		callbackRegistered = true;
	}

	Collider collider(this);

	Collider::Rect rectA{ 0.0f, 0.0f, 2.0f, 2.0f };
	Collider::Rect rectB{ 1.0f, 1.0f, 2.0f, 2.0f };

	if (collider.isTouching(rectA, rectB))
	{
		std::cout << "[FysicsManager::Update] Collision detected!\n";
	}
}