#pragma once

#include "Scene.h"
#include "Tiled/TiledMap.hpp"
#include "Tiled/TiledObject.hpp"
#include <unordered_map>

namespace IonixEngine {

typedef void (*AddComponent)(Entity*, TiledProperty&, TiledObject&);

class TiledObjectFactory {
    std::unordered_map<std::string, AddComponent> addComponent;
    static void AddSpriteComponent(Entity* entity, TiledProperty& property, TiledObject& object);
public:
    TiledObjectFactory();
    void CreateEntityFromObjectID(Scene* scene, TiledObject& object, TiledObjectLayer& layer);
};

};
