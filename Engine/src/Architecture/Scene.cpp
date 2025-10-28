#include "Scene.h"
#include "Architecture/Application.h"
#include "Architecture/ECS/ECS_Test.hpp"
#include "SDL_render.h"

namespace IonixEngine {
    void Scene::OnEnter() {
        SDL_Log("[Scene] Started Scene");
        renderData.renderer = Application::Get().GetWindow().GetSdlRenderer();
        renderData.queue = Application::Get().layerGraphics->GetQueue();

        EntityID en = CreateEntity();
        Entity* entity = GetEntityFromID(en);
        if (!entity) { return; }
        //entity->AddComponent(new SpriteComponent(entity, "pim", 0));
        entity->AddComponent(new AnimatedSpriteComponent(entity, "Ball", 0));
        entity->position.x = 500;
        entity->position.y = 500;
        //entity->AddComponent(new EntityMover(entity));
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
