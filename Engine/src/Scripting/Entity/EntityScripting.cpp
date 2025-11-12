#include "Scripting/Entity/EntityScripting.h"
#include "Architecture/Application.h"
#include "Architecture/ECS/Entity.hpp"
#include "Graphics/Camera.h"
#include <SDL.h>
#include <SDL_render.h>
#include "sol/sol.hpp"

// Extern declaration of global camera (defined in Application.cpp)
extern IonixEngine::Camera* cam;

namespace IonixEngine {

    EntityScripting* EntityScripting::s_Instance = nullptr;

    EntityScripting& EntityScripting::Get() {
        if (!s_Instance)
            s_Instance = new EntityScripting();
        return *s_Instance;
    }

    void EntityScripting::Init(sol::state& lua)
    {
        // --- Entity bindings ---
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

        // --- Camera bindings (no SDL_Renderer exposed to Lua) ---
        auto initRenderTexture = [](Camera* cam) {
            if (cam) cam->InitRenderTexture(Application::Get().GetWindow().GetSdlRenderer());
            };

        auto renderToTexture = [](Camera* cam) {
            if (cam) cam->RenderToTexture(Application::Get().GetWindow().GetSdlRenderer());
            };

        lua["Camera"] = lua.create_table_with(
            "init_render_texture", initRenderTexture,
            "render_to_texture", renderToTexture
        );

        // Expose global camera pointer
        lua["cam"] = cam;

        // Expose Entity functions
        lua["Entity"] = lua.create_table_with(
            "create_entity", entity,
            "get_entity_pos", getEntityPos,
            "set_entity_pos", setEntityPos,
            "add_sprite_component", addSpriteComponent,
            "add_audio_component", addAudioPlayerComponent
        );
    }
}