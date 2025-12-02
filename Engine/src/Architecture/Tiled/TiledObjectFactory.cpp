#include "TiledObjectFactory.hpp"
#include "ECS/Temp_Vec2.hpp"
#include "Graphics/SpriteComponent.h"
#include "Tiled/TiledObject.hpp"
#include "Fysics/FysicsBody.h"
#include "b2_math.h"
#include <string>
#include <unordered_map>

namespace IonixEngine {

Vec2 Vec2FromB2Vec2(b2Vec2 vector) {
    return Vec2 { vector.x, vector.y };
}

TiledObjectFactory::TiledObjectFactory() : addComponent() {
    addComponent["Sprite Component"] = &AddSpriteComponent;
    addComponent["Rigidbody"] = &AddPolygonComponent;
}

void TiledObjectFactory::AddSpriteComponent(Entity* entity, std::unordered_map<std::string, TiledProperty&>& properties, TiledObject& object) {
    entity->AddComponent(new SpriteComponent(entity, properties["SpriteName"].name, object.size.x, object.size.y, 0));
}

void TiledObjectFactory::AddPolygonComponent(Entity* entity, std::unordered_map<std::string, TiledProperty&>& properties, TiledObject& object) {
    entity->AddComponent(new FysicsBody(entity, (int) fysicsBodyType::dynamicBody, false));
}

void TiledObjectFactory::CreateEntityFromObjectID(Scene* scene, TiledObject& object, TiledObjectLayer& layer) {
    Entity* entity = scene->GetEntityFromID(scene->CreateEntity());
    entity->transform.SetGlobalPosition(Vec2FromB2Vec2(object.position));
    entity->transform.SetGlobalRotation(object.rotation);

    std::unordered_map<std::string, std::unordered_map<std::string, TiledProperty&>> propertiesMap;

    for (TiledProperty& property : object.properties) {
        size_t start = 0;
        size_t end = property.name.find('.');
        if (end == property.name.npos) { continue; }
        std::string component = property.name.substr(start, end - 1);
        std::string componentField = property.name.substr(end + 1);
        propertiesMap[component][componentField] = property;
    }

    for (std::pair<std::string, std::unordered_map<std::string, TiledProperty&>> properties : propertiesMap) {
        auto addFunc = addComponent.find(properties.first);
        if (addFunc != addComponent.end()) {
            addFunc->second(entity, properties.second, object);
        }
    }
}

};
