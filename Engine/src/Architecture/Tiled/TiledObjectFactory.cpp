#include "TiledObjectFactory.hpp"
#include "ECS/Temp_Vec2.hpp"
#include "Graphics/SpriteComponent.h"
#include "Tiled/TiledObject.hpp"
#include "b2_math.h"

namespace IonixEngine {

Vec2 Vec2FromB2Vec2(b2Vec2 vector) {
    return Vec2 { vector.x, vector.y };
}

TiledObjectFactory::TiledObjectFactory() : addComponent() {
    addComponent["Sprite Component"] = &AddSpriteComponent;
}

void TiledObjectFactory::AddSpriteComponent(Entity* entity, TiledProperty& property, TiledObject& object) {
    entity->AddComponent(new SpriteComponent(entity, property.name, object.size.x, object.size.y, 0));
}

void TiledObjectFactory::CreateEntityFromObjectID(Scene* scene, TiledObject& object, TiledObjectLayer& layer) {
    Entity* entity = scene->GetEntityFromID(scene->CreateEntity());
    entity->transform.SetGlobalPosition(Vec2FromB2Vec2(object.position));
    entity->transform.SetGlobalRotation(object.rotation);

    for (TiledProperty& property : object.properties) {
        addComponent[property.name](entity, property, object);
    }
}

};
