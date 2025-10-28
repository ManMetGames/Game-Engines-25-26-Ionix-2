#include "Scene.h"
#include "Architecture/Application.h"
#include "Architecture/ECS/ECS_Test.hpp"

namespace IonixEngine {
    void Scene::OnEnter() {
        SDL_Log("[Scene] Started Scene");
        Reserve(50);
        renderData.renderer = Application::Get().GetWindow().GetSdlRenderer();

        EntityID first = CreateEntity();
        Entity* firstEntity = GetEntityFromID(first);
        if (!firstEntity)
        {
            return;
        }
        firstEntity->transform.SetLocalPosition(Vec2 { 100, 100 });
        firstEntity->AddComponent(new SpriteRenderer(firstEntity));
        firstEntity->AddComponent(new EntityMover(firstEntity, 60));

        EntityID second = CreateEntity();
        Entity* secondEntity = GetEntityFromID(second);
        if (!secondEntity)
        {
            return;
        }
        secondEntity->transform.SetLocalPosition(Vec2{ 0, 100 });
        secondEntity->transform.SetParent(&firstEntity->transform, false);
        secondEntity->AddComponent(new SpriteRenderer(secondEntity));
        secondEntity->AddComponent(new EntityMover(secondEntity, -60));


        EntityID third = CreateEntity();
        Entity* thirdEntity = GetEntityFromID(third);
        if (!thirdEntity)
        {
            return;
        }
        thirdEntity->transform.SetLocalPosition(Vec2{ 0, 100 });
        thirdEntity->transform.SetParent(&secondEntity->transform, false);
        thirdEntity->AddComponent(new SpriteRenderer(thirdEntity));

    }

    void Scene::OnUpdate(float dt) {
        for (size_t i = 0; i < m_Entities.size(); i++) {
            Entity* entity = &m_Entities[i];
            entity->Update(dt);
            entity->Render(&renderData);
        }
    }

    void Scene::OnEvent(IonixEvent& event) {}

    void Scene::OnExit() {
        for (Entity& entity : m_Entities) {
            entity.Destroy(this);
        }
        m_Entities.clear();
    }

    void Scene::Reserve(std::size_t count) {
        m_Entities.reserve(count);
        m_IdToIndex.reserve(count * 2);
    }

    EntityID Scene::CreateEntity() {
        const EntityID entityId = m_NextId++;
        const std::size_t index = m_Entities.size();
        m_Entities.push_back(Entity{ entityId });
        m_IdToIndex[entityId] = index;
        return entityId;
    }

    bool Scene::DestroyEntity(EntityID entityId) {
        auto mapIt = m_IdToIndex.find(entityId); // find id in the map
        if (mapIt == m_IdToIndex.end())
            return false; // if id does not exist in the map, return false

        const std::size_t index = mapIt->second; // get the vector index of the entity
        const std::size_t lastIndex = m_Entities.size() - 1; // get index of last element in the vector

        if (index != lastIndex) // if entity deleted is not already the last one
        {
            // Move last entity into the removed slot and fix mapping
            Entity& movedEntity = m_Entities[lastIndex]; // reference to the last entity in the vector
            m_Entities[index] = movedEntity; // move last entity into the removed slot
            m_IdToIndex[movedEntity.id] = index; // update the map
        }

        m_Entities.pop_back(); // remove last element from the vector
        m_IdToIndex.erase(mapIt); // remove id from the map
        return true;
    }

    Entity* Scene::GetEntityFromID(EntityID id) {
        auto find = m_IdToIndex.find(id);
        if (find != m_IdToIndex.end()) {
            return &m_Entities[find->second];
        }
        return nullptr;
    }
}
