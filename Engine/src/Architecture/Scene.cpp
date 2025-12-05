#include "Scene.h"
#include "Architecture/Application.h"
#include "Architecture/JSON/JSONDeserialize.hpp"
#include "Architecture/Tiled/TiledMap.hpp"
#include "Architecture/Tiled/TiledObject.hpp"
#include "Architecture/Tiled/TiledObjectFactory.hpp"
#include "SDL_log.h"
#include <cstdio>
#include <fstream>

namespace IonixEngine {
    void Scene::OnEnter() {
        SDL_Log("[Scene] Started Scene");
        //m_Entities.reserve(50);
        Reserve(1024);
        renderData.renderer = Application::Get().GetWindow().GetSdlRenderer();
        renderData.queue = Application::Get().layerGraphics->GetQueue();

        SDL_Log("[Scene] Beginning Tiled Loading...");
        std::ifstream jsonFile = std::ifstream("./First.json");
        std::stringstream jsonData;
        jsonData << jsonFile.rdbuf();
        JSONDeserialize json = JSONDeserialize(jsonData.str());
        TiledMap map = TiledMap(&json);

        TiledObjectFactory factory;

        for (TiledLayer& layer : map.layers) {
            if (layer.isTile) {
                size_t tilesetIdx = map.GetTilemapIdx(layer.tileLayer);
                factory.CreateTilemapFromLayer(this, layer.tileLayer, map.tilesets[tilesetIdx]);
            } else {
                for (TiledObject& object : layer.objectLayer.objects) {
                    factory.CreateEntityFromObjectID(this, object, layer.objectLayer);
                }
            }
        }
    }

    void Scene::OnUpdate(float dt) {
        // SDL_Log("[DEBUG TEST] Scene OnUpdate running...");
        for (size_t i = 0; i < m_Entities.size(); i++) {
            Entity* entity = &m_Entities[i];
            // SDL_Log("[DEBUG TEST] Updating entity %zu",i);
            entity->Update(dt);
            // SDL_Log("[DEBUG TEST] Rendering entity %zu", i);
            //SDL_Log("[DEBUG] entity #%zu pos at: X: %f, Y: %f", i, entity->transform.GetGlobalPosition().x, entity->transform.GetGlobalPosition().y);
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

    EntityID Scene::CreateEntity(int renderLayer) {
        const EntityID entityId = m_NextId++;
        const std::size_t index = m_Entities.size();
        m_Entities.push_back(Entity{ entityId , renderLayer});
        m_IdToIndex[entityId] = index;
        return entityId;
    }

    Entity* Scene::CreateAndGetEntity(int renderLayer) {
        return GetEntityFromID(CreateEntity(renderLayer));
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
