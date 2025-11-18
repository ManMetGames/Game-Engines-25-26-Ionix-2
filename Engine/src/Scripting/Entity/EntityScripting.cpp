#include "Scripting/Entity/EntityScripting.h"
#include "Architecture/Application.h"
#include "Architecture/ECS/Entity.hpp"
#include "Fysics/FysicsBody.h"
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

        auto addSpriteComponent = [](Entity* entity, uint32_t hash, int width, int height, int zedOrder) {
            std::cout << hash << std::endl;
            entity->AddComponent(new SpriteComponent(entity, hash, width, height, zedOrder));
            };

        auto addAudioPlayerComponent = [](Entity* entity, std::string clip = "", bool playOnAwake = false) {
            entity->AddComponent(new AudioPlayer(entity, clip, playOnAwake));
            };
        
        auto addFysicsBodyComponent = [](Entity* entity) {
            entity->AddComponent(new FysicsBody(entity, "", Application::Get().layerFysics->GetWorld()));
            };

        auto getSpriteComponent = [](Entity* entity) {
            return entity->GetComponent<SpriteComponent>();
            };

        auto getAudioPlayerComponent = [](Entity* entity) {
            return entity->GetComponent<AudioPlayer>();
            };

        auto getFysicsBodyComponent = [](Entity* entity) {
            entity->GetComponent<FysicsBody>();
            };

        auto tryGetSpriteComponent = [](Entity* entity) -> auto {
            SpriteComponent* comp = new SpriteComponent();
            SpriteComponent** comp2 = &comp;
            std::pair<bool, SpriteComponent*> result;
            bool hasComp = entity->TryGetComponent<SpriteComponent>(comp2);

            result = std::make_pair(hasComp, *comp2);
            
            return result;
            };
         
        auto tryGetAudioComponent = [](Entity* entity) -> auto {
            AudioPlayer* comp = new AudioPlayer();
            AudioPlayer** comp2 = &comp;
            std::pair<bool, AudioPlayer*> result;
            bool hasComp = entity->TryGetComponent<AudioPlayer>(comp2);

            result = std::make_pair(hasComp, *comp2);

            return result;
            };

        auto tryGetFysicsBodyComponent = [](Entity* entity) -> auto {
            FysicsBody* comp = new FysicsBody();
            FysicsBody** comp2 = &comp;
            std::pair<bool, FysicsBody*> result;
            bool hasComp = entity->TryGetComponent<FysicsBody>(comp2);

            result = std::make_pair(hasComp, *comp2);

            return result;
            };

        auto hasSpriteComponent = [](Entity* entity) -> bool {
            return entity->HasComponent<SpriteComponent>();
            };

        auto hasAudioComponent = [](Entity* entity) -> bool {
            return entity->HasComponent<AudioPlayer>();
            };

        auto hasFysicsBodyComponent = [](Entity* entity) -> bool {
            return entity->HasComponent<FysicsBody>();
            };

        

        lua["Entity"] = lua.create_table_with(
            "create_entity", entity,
            "get_entity_pos", getEntityPos,
            "set_entity_pos", setEntityPos,
            "add_sprite_component", addSpriteComponent,
            "add_audio_component", addAudioPlayerComponent,
            "add_fysics_component", addFysicsBodyComponent,
            "get_sprite_component", getSpriteComponent,
            "get_audio_component", getAudioPlayerComponent,
            "get_fysics_component", getFysicsBodyComponent,
            "try_get_sprite_component", tryGetSpriteComponent,
            //"try_get_audio_component", tryGetAudioComponent,
            "has_sprite_component", hasSpriteComponent,
            "has_audio_component", hasAudioComponent,
            "has_fysics_component", hasFysicsBodyComponent

        );
    }

}