#pragma once

#include "Scene.h"
#include "Tiled/TiledMap.hpp"
#include "Tiled/TiledObject.hpp"
#include <unordered_map>

namespace IonixEngine {

typedef void (*AddComponent)(Entity* entity, std::unordered_map<std::string, TiledProperty&>& map, TiledObject& object);

class TiledObjectFactory {
    std::unordered_map<std::string, AddComponent> addComponent;
    static void  AddSpriteComponent(Entity* entity, std::unordered_map<std::string, TiledProperty&>& properties, TiledObject& object);
    static void  AddAnimatorComponent(Entity* entity, std::unordered_map<std::string, TiledProperty&>& properties, TiledObject& object);
    static void AddFysicsBody(Entity* entity, std::unordered_map<std::string, TiledProperty&>& properties, TiledObject& object);
    static void AddBoxCollider(Entity* entity, std::unordered_map<std::string, TiledProperty&>& properties, TiledObject& object);
    static void AddAudioComponent(Entity* entity, std::unordered_map<std::string, TiledProperty&>& properties, TiledObject& object);
public:
    TiledObjectFactory();
    void CreateEntityFromObjectID(Scene* scene, TiledObject& object, TiledObjectLayer& layer);
    void CreateTilemapFromLayer(Scene* scene, TiledTileLayer& layer, TiledTileset& tileset);
};

};
