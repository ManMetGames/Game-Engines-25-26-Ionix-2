#include "Scene.h"
#include "Architecture/Application.h"
#include "Architecture/JSON/JSONDeserialize.hpp"
#include "Architecture/Tiled/TiledMap.hpp"
#include "Architecture/Tiled/TiledObjectFactory.hpp"
#include "SDL_log.h"
#include <cstdio>
#include <fstream>
#include <sstream>

namespace IonixEngine {
    void Scene::OnEnter() {
        SDL_Log("[Scene] Started Scene");
        //m_Entities.reserve(50);
        Reserve(1024);
        renderData.renderer = Application::Get().GetWindow().GetSdlRenderer();
        renderData.queue = Application::Get().layerGraphics->GetQueue();

        //EntityID first = CreateEntity();
        //Entity* firstEntity = GetEntityFromID(first);
        //if (!firstEntity)
        //{
        //   SDL_Log("[DEBUG TEST] First entity failed, returning...");
        //    return;
        //}
        //firstEntity->transform.SetLocalPosition(Vec2 { 500, 300 });
        //firstEntity->AddComponent(new SpriteRenderer(firstEntity));
        //firstEntity->AddComponent(new EntityMover(firstEntity, 60));
        //// firstEntity->transform.SetLocalScale(Vec2{ 0.5,1.5 });

        //EntityID second = CreateEntity();
        //Entity* secondEntity = GetEntityFromID(second);
        //if (!secondEntity)
        //{
        //    SDL_Log("[DEBUG TEST] Second entity failed, returning...");
        //    return;
        //}
        //secondEntity->transform.SetLocalPosition(Vec2{ 0, 100 });
        //secondEntity->transform.SetParent(&firstEntity->transform, false);
        //secondEntity->transform.SetLocalScale(Vec2{ 1.3,1.25 });
        //secondEntity->AddComponent(new SpriteRenderer(secondEntity));
        //secondEntity->AddComponent(new EntityMover(secondEntity, -60));

        //EntityID third = CreateEntity();
        //Entity* thirdEntity = GetEntityFromID(third);
        //if (!thirdEntity)
        //{
        //    SDL_Log("[DEBUG TEST] Third entity failed, returning...");
        //    return;
        //}
        //thirdEntity->transform.SetLocalPosition(Vec2{ 0, -100 });
        //thirdEntity->transform.SetParent(&secondEntity->transform, false);
        //thirdEntity->AddComponent(new SpriteComponent(thirdEntity, "ball",100,100, 0));
        

        SDL_Log("[Scene] Beginning Tiled Loading...");
        std::ifstream jsonFile = std::ifstream("./First.json");
        std::stringstream jsonData;
        jsonData << jsonFile.rdbuf();
        JSONDeserialize json = JSONDeserialize(jsonData.str());
        TiledMap map = TiledMap(&json);

        SDL_Log("[Tiled] Loaded map data:\n%s", map.ToString().c_str());

        TiledObjectFactory factory;

        size_t capacity = 0;
        for (TiledLayer& layer : map.layers) {
            if (layer.isTile) { continue; }
            capacity += layer.objectLayer.objects.size();
        }
        std::vector<std::pair<TiledObject, TiledObjectLayer>> orderedObjects;
        orderedObjects.reserve(capacity);

        std::vector<std::pair<TiledTileLayer, size_t>> tileLayers;

        for (TiledLayer& layer : map.layers) {
            if (layer.isTile) {
                tileLayers.push_back(std::make_pair(layer.tileLayer, map.GetTilemapIdx(layer.tileLayer)));
            } else {
                for (TiledObject& object : layer.objectLayer.objects) {
                    for (TiledProperty& property : object.properties) {
                        orderedObjects.push_back(std::make_pair(object, layer.objectLayer));
                    }
                }
            }
        }

        for (std::pair<TiledObject, TiledObjectLayer>& object : orderedObjects) {
            factory.CreateEntityFromObjectID(this, object.first, object.second);
        }

        for (std::pair<TiledTileLayer, size_t>& tile : tileLayers) {
            factory.CreateTilemapFromLayer(this, tile.first, map.tilesets[tile.second]);
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
