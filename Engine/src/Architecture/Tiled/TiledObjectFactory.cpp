#include "TiledObjectFactory.hpp"
#include "Audio/AudioPlayer.h"
#include "AudioSystem/SoundManager.h"
#include "ECS/Temp_Vec2.hpp"
#include "Graphics/SpriteComponent.h"
#include "LayerSystem/Layers/LayerFysics.h"
#include "SDL_log.h"
#include "StringUtils.hpp"
#include "TextureManager/TextureManager.h"
#include "Tiled/TiledObject.hpp"
#include "Fysics/FysicsBody.h"
#include "Fysics/FysicsManager.h"
#include "Architecture/Tiled/TileMapRenderer.hpp"
#include "b2_math.h"
#include "b2_shape.h"
#include <filesystem>
#include <string>
#include <unordered_map>

namespace IonixEngine {

Vec2 Vec2FromB2Vec2(b2Vec2 vector) {
    return Vec2 { vector.x, vector.y };
}

TiledObjectFactory::TiledObjectFactory() : addComponent() {
    addComponent["SpriteComponent"] = &AddSpriteComponent;
    addComponent["Rigidbody"] = &AddFysicsBody;
    addComponent["AudioComponent"] = &AddAudioComponent;
}

void TiledObjectFactory::AddSpriteComponent(Entity* entity, std::unordered_map<std::string, TiledProperty&>& properties, TiledObject& object) {
    uint32_t hash = TextureManager::HashFromPath(properties["ImagePath"].stringValue);
    entity->AddComponent(new SpriteComponent(entity, hash, properties["Width"].intValue, properties["Height"].intValue, properties["ZOrder"].intValue));
}

void TiledObjectFactory::AddBoxCollider(Entity* entity, std::unordered_map<std::string, TiledProperty&>& properties, TiledObject& object) {
    FysicsBody* fb = entity->GetComponent<FysicsBody>();
    if (!fb) {
        fb = entity->AddComponent(new FysicsBody(entity, (int) fysicsBodyType::staticBody, false));
    }
    FysicsManager::GetManager()->GetShapes()->AddBox(entity, { properties["Size X"].floatValue, properties["Size Y"].floatValue }, { properties["Offset X"].floatValue, properties["Offset Y"].floatValue }, properties["Angle"].floatValue, properties["IsTrigger"].boolValue);
}

void TiledObjectFactory::AddAudioComponent(Entity* entity, std::unordered_map<std::string, TiledProperty&>& properties, TiledObject& object) {
    entity->AddComponent(new AudioPlayer(entity, SoundManager::HashFromPath(properties["AudioFilePath"].stringValue), properties["PlayOnAwake"].boolValue));
}

void TiledObjectFactory::AddFysicsBody(Entity* entity, std::unordered_map<std::string, TiledProperty&>& properties, TiledObject& object) {
    FysicsBody* fb = entity->AddComponent(new FysicsBody(entity, properties["BodyType"].intValue, false));
    fb->SetGravityScale(entity, properties["Gravity"].floatValue);
    fb->SetLinearDamping(entity, properties["Friction"].floatValue);

    if (object.polygon.size() > 0) {
        FysicsManager::GetManager()->GetShapes()->AddPolygon(entity, 16, object.polygon);
    }

    // TODO: Density seem to require a shape also colliders seem to set whether they are triggers?
    // fb->SetIsBullet(entity, properties["IsTrigger"].boolValue);
}

void TiledObjectFactory::CreateEntityFromObjectID(Scene* scene, TiledObject& object, TiledObjectLayer& layer) {
    Entity* entity = scene->CreateAndGetEntity(0);
    entity->transform.SetGlobalPosition(Vec2FromB2Vec2(object.position));
    entity->transform.SetGlobalRotation(object.rotation);

    SDL_Log("[Object Factory] Created entity: %s, with id %lu", object.name.c_str(), entity->id);

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
            SDL_Log("[Object Factory] Added component: %s to entity: %s, with id %lu", properties.first.c_str(), object.name.c_str(), entity->id);
        }
    }

}

void TiledObjectFactory::CreateTilemapFromLayer(Scene* scene, TiledTileLayer& layer, TiledTileset& tileset) {
    Entity* entity = scene->CreateAndGetEntity(0);
    entity->transform.SetGlobalPosition(Vec2FromB2Vec2(layer.position));
    entity->AddComponent(new TileMapRenderer(entity, layer, tileset));
}

};
