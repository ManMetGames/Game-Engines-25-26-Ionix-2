#pragma once
#include "Fysics/FysicsManager.h"
//#include "Debug/DebugDraw.h"
#include "Architecture/Application.h"

namespace IonixEngine
{
    FysicsManager::FysicsManager()
    {
        b2Vec2 gravity = b2Vec2(0.0f, 9.8f);
        world = new b2World(gravity);

        // Collision callback system - not ECS
        world->SetContactListener(&contactListener);

        shape = new FysicsShapes();
        force = new Force();
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
    }

    Entity* FysicsManager::GetEntityFromBody(b2Body* body)
    {
        for (auto& pair : entityBodyMap)
        {
            return pair.second;
        }
    }

    Collider* FysicsManager::GetColliderForEntity(Entity* entity)
    {
        auto it = entityToColliderMap.find(entity);
        if (it != entityToColliderMap.end())
        {
            return it->second;
        }
        return nullptr;
    }
}