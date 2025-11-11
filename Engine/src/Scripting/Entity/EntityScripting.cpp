#include "Scripting/Entity/EntityScripting.h"
#include "Architecture/Application.h"
#include "Architecture/ECS/Entity.hpp"

namespace IonixEngine {

    EntityScripting* EntityScripting::s_Instance = nullptr;

    EntityScripting& EntityScripting::Get() {
        if (!s_Instance)
            s_Instance = new EntityScripting();
        return *s_Instance;
    }

    void EntityScripting::Init(sol::state& lua)
    {
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

        auto addAudioPlayerComponent = [](Entity* entity, std::string clip = "", bool playOnAwake = false) {
            entity->AddComponent(new AudioPlayer(entity, clip, playOnAwake));
            };

        auto getSpriteComponent = [](Entity* entity) {
            return entity->GetComponent<AnimatedSpriteComponent>();
            };

        auto getAudioPlayerComponent = [](Entity* entity) {
            return entity->GetComponent<AudioPlayer>();
            };

        /*auto tryGetSpriteComponent = [](Entity* entity) -> auto {
            AnimatedSpriteComponent** comp;
            struct result {bool isOK; AnimatedSpriteComponent* comp; };
            return result{ entity->TryGetComponent<AnimatedSpriteComponent>(comp), *comp };
            };
         
        auto tryGetAudioComponent = [](Entity* entity) -> std::tuple<bool, AudioPlayer*> {
            AudioPlayer** comp;
            return std::make_tuple(entity->TryGetComponent<AudioPlayer>(comp), *comp);  
            };*/

        auto hasSpriteComponent = [](Entity* entity) -> bool {
            return entity->HasComponent<AnimatedSpriteComponent>();
            };

        auto hasAudioComponent = [](Entity* entity) -> bool {
            return entity->HasComponent<AudioPlayer>();
            };

        

        lua["Entity"] = lua.create_table_with(
            "create_entity", entity,
            "get_entity_pos", getEntityPos,
            "set_entity_pos", setEntityPos,
            "add_sprite_component", addSpriteComponent,
            "add_audio_component", addAudioPlayerComponent,
            "get_sprite_component", getSpriteComponent,
            "get_audio_component", getAudioPlayerComponent,
            "try_get_sprite_component", tryGetSpriteComponent,
            "try_get_audio_component", tryGetAudioComponent,
            "has_sprite_component", hasSpriteComponent,
            "has_audio_component", hasAudioComponent
        );
    }

}