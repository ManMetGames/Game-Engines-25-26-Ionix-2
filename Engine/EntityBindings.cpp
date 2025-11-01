#include "Scripting/Bindings/EntityBindings.h"
#include "Architecture/Application.h"
#include "Architecture/ECS/Entity.h"
#include "Architecture/ECS/EntityID.h"
#include "Architecture/ECS/Components/AnimatedSpriteComponent.h"

namespace IonixEngine {

    void RegisterEntityBindings(sol::state& lua) {
        auto entity = []() -> Entity* {
            EntityID entityID = Application::Get().layerScene->GetScene()->CreateEntity();
            return Application::Get().layerScene->GetScene()->GetEntityFromID(entityID);
            };

        auto getEntityPos = [](Entity* entity) -> Vec2 {
            return entity->position;
            };

        auto setEntityPos = [](Entity* entity, float x, float y) {
            if (entity == nullptr) return;
            entity->position = Vec2{ x, y };
            };

        auto addSpriteComponent = [](Entity* entity, std::string alias, int zedOrder) {
            entity->AddComponent(new AnimatedSpriteComponent(entity, alias, zedOrder));
            };

        lua["Entity"] = lua.create_table_with(
            "create_entity", entity,
            "get_entity_pos", getEntityPos,
            "set_entity_pos", setEntityPos,
            "add_sprite_component", addSpriteComponent
        );
    }

}