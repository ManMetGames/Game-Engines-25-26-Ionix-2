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
        auto entity = []() -> Entity* {
            EntityID entityID = Application::Get().layerScene->GetScene()->CreateEntity();
            return Application::Get().layerScene->GetScene()->GetEntityFromID(entityID);
            };

        lua["Entity"] = lua.create_table_with(
            "create_entity", entity,
            "get_entity_pos", getEntityPos,
            "set_entity_pos", setEntityPos,
            "add_sprite_component", addSpriteComponent,
            "add_audio_component", addAudioPlayerComponent
        );
        // --- New Camera bindings ---
        auto initRenderTexture = [](Camera* cam, SDL_Renderer* renderer) {
            if (cam) cam->InitRenderTexture(renderer);
            };

        auto renderToTexture = [](Camera* cam, SDL_Renderer* renderer) {
            if (cam) cam->RenderToTexture(renderer);
            };

        lua["Camera"] = lua.create_table_with(
            "init_render_texture", initRenderTexture,
            "render_to_texture", renderToTexture
        );

        lua["Entity"] = lua.create_table_with(
            "create_entity", entity,
            "get_entity_pos", getEntityPos,
            "set_entity_pos", setEntityPos,
            "add_sprite_component", addSpriteComponent,
            "add_audio_component", addAudioPlayerComponent
        );
    }

}