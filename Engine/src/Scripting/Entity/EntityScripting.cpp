#include "Scripting/Entity/EntityScripting.h"
#include "Architecture/Application.h"
#include "Architecture/ECS/Entity.hpp"
#include "Fysics/FysicsBody.h"
#include "Fysics/FysicsManager.h"
#include "Audio/MusicComponent.h"
namespace IonixEngine {

    EntityScripting* EntityScripting::s_Instance = nullptr;

    EntityScripting& EntityScripting::Get() {
        if (!s_Instance)
            s_Instance = new EntityScripting();
        return *s_Instance;
    }

    void EntityScripting::Init(sol::state& lua)
    {
        auto entity = [](sol::optional<int> renderLayer) -> Entity* {
            EntityID entityID;
            if (!renderLayer)
            {
                entityID = Application::Get().layerScene->GetScene()->CreateEntity(0);
            }
            else
            {
                entityID = Application::Get().layerScene->GetScene()->CreateEntity(*renderLayer);
            }
            return Application::Get().layerScene->GetScene()->GetEntityFromID(entityID);
            };

        auto destroy = [](Entity* entityToDestroy) -> bool {
            if (entityToDestroy == nullptr) return false;
            FysicsBody* bodyToDestroy = nullptr;
            if (entityToDestroy->TryGetComponent<FysicsBody>(&bodyToDestroy))
            {
           
                Application::Get().layerFysics->GetFysicsManager()->GetCollisionListener()->AddEntityBodiesToDestroy(bodyToDestroy->GetBody());

            }
            
            return Application::Get().layerScene->GetScene()->DestroyEntity(entityToDestroy->id);
            };

        //----------Transforms-----------
        auto getGlobalPos = [](Entity* entity) -> b2Vec2 {
            Vec2 pos = entity->transform.GetGlobalPosition();
            b2Vec2 returnPos = b2Vec2{ pos.x, pos.y };
            return returnPos;
            };

        auto getCenterPos = [](Entity* entity) -> b2Vec2 {
            SpriteComponent* comp = nullptr;
            if (!entity->TryGetComponent<SpriteComponent>(&comp))
            {
                std::cout << "Entity : " << entity->id << " has no sprite component, returning default position."<< std::endl;
                Vec2 pos = entity->transform.GetGlobalPosition();
                b2Vec2 nonCenterPos = b2Vec2{ pos.x, pos.y };
                return nonCenterPos;
            }
            Vec2 pos = entity->transform.GetGlobalPosition();            
            int entW = entity->GetComponent<SpriteComponent>()->getWidth() / 2;
            int entH = entity->GetComponent<SpriteComponent>()->getHeight() / 2;
            b2Vec2 returnPos = b2Vec2{ pos.x + entW, pos.y + entH};
            return returnPos;
        };

        auto getGlobalRot = [](Entity* entity) -> float {
            return entity->transform.GetGlobalRotation();
            };

        auto getGlobalScale = [](Entity* entity) -> b2Vec2 {
            Vec2 pos = entity->transform.GetGlobalScale();
            b2Vec2 returnPos = b2Vec2{ pos.x, pos.y };
            return returnPos;
            };

        auto setGlobalPos = [](Entity* entity, float x, float y) {
            if (entity == nullptr) return;
            FysicsBody* body = nullptr;
            if (entity->TryGetComponent(&body))
            {
                body->SetPosition(entity, x / 100, y / 100);

            }
            else {
                entity->transform.SetGlobalPosition(Vec2{ x, y });
            }
            };

        auto setGlobalRot = [](Entity* entity, float rot) {
            if (entity == nullptr) return;
            FysicsBody* body = nullptr;
            if (entity->TryGetComponent(&body))
            {
                body->SetAngle(entity, rot);
            }
            entity->transform.SetGlobalRotation(rot);
            };

        auto setGlobalScale = [](Entity* entity, float x, float y) {
            if (entity == nullptr) return;
            entity->transform.SetGlobalScale(Vec2{ x, y });
            };

        auto getLocalPos = [](Entity* entity) -> b2Vec2 {
            Vec2 pos = entity->transform.GetLocalPosition();
            b2Vec2 returnPos = b2Vec2{ pos.x, pos.y };
            return returnPos;
            };

        auto getLocalRot = [](Entity* entity) -> float {
            return entity->transform.GetLocalRotation();
            };

        auto getLocalScale = [](Entity* entity) -> b2Vec2 {
            Vec2 pos = entity->transform.GetLocalScale();
            b2Vec2 returnPos = b2Vec2{ pos.x, pos.y };
            return returnPos;
            };

        auto setLocalPos = [](Entity* entity, float x, float y) {
            if (entity == nullptr) return;
			entity->transform.SetLocalPosition(Vec2{ x, y });
            };

        auto setLocalRot = [](Entity* entity, float rot) {
            if (entity == nullptr) return;
            entity->transform.SetLocalRotation(rot);
            };

        auto setLocalScale = [](Entity* entity, float x, float y) {
            if (entity == nullptr) return;
            entity->transform.SetLocalScale(Vec2{ x, y });
            };

        auto setParent = [](Entity* entity, Entity* newParent, sol::optional<bool> maintainLocation) {
            if (entity == nullptr || newParent == nullptr) return;
            if (!maintainLocation)
            {
                entity->transform.SetParent(&newParent->transform);
            }
            else
            {
                entity->transform.SetParent(&newParent->transform, *maintainLocation);
            }

            };

        auto removeParent = [](Entity* entity, sol::optional<bool> maintainLocation) {
            if (entity == nullptr) return;
            if (!maintainLocation)
            {
                entity->transform.RemoveParent();
            }
            else
            {
                entity->transform.RemoveParent(*maintainLocation);
            }

            };

        auto addChild = [](Entity* entity, Entity* newChild) {
            if (entity == nullptr || newChild == nullptr) return;
            entity->transform.AddChild(&newChild->transform);
            };

        auto removeChild = [](Entity* entity, Entity* newChild) {
            entity->transform.RemoveChild(&newChild->transform);
            };

        auto removeChildWithIndex = [](Entity* entity, int index) {
                entity->transform.RemoveChild(index);
            };

        //----------Components--------------

        auto addSpriteComponent = [](Entity* entity, uint32_t alias, int width, int height, int zedOrder) -> const SpriteComponent*{
            //entity->AddComponent(new SpriteComponent(entity, alias, width, height, zedOrder));
            return entity->AddComponent(new SpriteComponent(entity, alias, width, height, zedOrder));
            };

        auto addAudioPlayerComponent = [](Entity* entity, std::string clip = "", bool playOnAwake = false) {
            entity->AddComponent(new AudioPlayer(entity, clip, playOnAwake));
            };

        auto addMusicComponent = [](Entity* entity, std::string musicTrack = "", bool playOnAwake = false) {
            entity->AddComponent(new MusicComponent(entity, musicTrack, playOnAwake));
            };
        
        auto addFysicsBodyComponent = [](Entity* entity) {
            entity->AddComponent(new FysicsBody(entity, "", Application::Get().layerFysics->GetWorld()));
            };

        auto addFysicsBodyComponentWithType = [](Entity* entity, int b_type, bool rotationLocked) {
            entity->AddComponent(new FysicsBody(entity, b_type, rotationLocked));
            };

        auto getSpriteComponent = [](Entity* entity) {
            return entity->GetComponent<SpriteComponent>();
            };

        auto getAudioPlayerComponent = [](Entity* entity) {
            return entity->GetComponent<AudioPlayer>();
            };

        auto getMusicComponent = [](Entity* entity) {
            return entity->GetComponent<MusicComponent>();
            };

        auto getFysicsBodyComponent = [](Entity* entity) {
            return entity->GetComponent<FysicsBody>();
            };

        auto tryGetSpriteComponent = [](Entity* entity) -> auto {
            SpriteComponent* comp = nullptr;
            std::pair<bool, SpriteComponent*> result;
            bool hasComp = entity->TryGetComponent<SpriteComponent>(&comp);

            result = std::make_pair(hasComp, comp);

            return result;
            };

        auto tryGetAudioComponent = [](Entity* entity) -> auto {
            AudioPlayer* comp = nullptr;
            std::pair<bool, AudioPlayer*> result;
            bool hasComp = entity->TryGetComponent<AudioPlayer>(&comp);

            result = std::make_pair(hasComp, comp);

            return result;
            };

        auto tryGetMusicComponent = [](Entity* entity) -> auto {
            MusicComponent* comp = nullptr;
            std::pair<bool, MusicComponent*> result;
            bool hasComp = entity->TryGetComponent<MusicComponent>(&comp);

            result = std::make_pair(hasComp, comp);

            return result;
            };

        auto tryGetFysicsBodyComponent = [](Entity* entity) -> auto {
            FysicsBody* comp = nullptr;
            std::pair<bool, FysicsBody*> result;
            bool hasComp = entity->TryGetComponent<FysicsBody>(&comp);

            result = std::make_pair(hasComp, comp);

            return result;
            };

        auto hasSpriteComponent = [](Entity* entity) -> bool {
            return entity->HasComponent<SpriteComponent>();
            };

        auto hasAudioComponent = [](Entity* entity) -> bool {
            return entity->HasComponent<AudioPlayer>();
            };

        auto hasMusicComponent = [](Entity* entity) -> bool {
            return entity->HasComponent<MusicComponent>();
            };

        auto hasFysicsBodyComponent = [](Entity* entity) -> bool {
            return entity->HasComponent<FysicsBody>();
            };

        

        lua["Entity"] = lua.create_table_with(
            "create_entity", entity,
            "destroy_entity", destroy,
            "get_global_pos", getGlobalPos,
            "get_center_pos", getCenterPos,
            "set_global_pos", setGlobalPos,
            "get_global_rot", getGlobalRot,
            "set_global_rot", setGlobalRot,
            "get_global_scale", getGlobalScale,
            "set_global_scale", setGlobalScale,
            "get_local_pos", getLocalPos,
            "set_local_pos", setLocalPos,
            "get_local_rot", getLocalRot,
            "set_local_rot", setLocalRot,
            "get_local_scale", getLocalScale,
            "set_local_scale", setLocalScale,
            "set_parent", setParent,
            "remove_parent", removeParent,
            "add_child", addChild,
            "remove_child", removeChild,
            "remove_child_index", removeChildWithIndex,
            "add_sprite_component", addSpriteComponent,
            "add_audio_component", addAudioPlayerComponent,
            "add_music_component", addMusicComponent,
            "add_fysics_component", addFysicsBodyComponent,
            "add_fysics_component", addFysicsBodyComponentWithType,
            "get_sprite_component", getSpriteComponent,
            "get_audio_component", getAudioPlayerComponent,
            "get_music_component", getMusicComponent,
            "get_fysics_component", getFysicsBodyComponent,
            "try_get_sprite_component", tryGetSpriteComponent,
            "try_get_audio_component", tryGetAudioComponent,
            "try_get_music_component", tryGetMusicComponent,
            "try_get_fysics_component", tryGetFysicsBodyComponent,
            "has_sprite_component", hasSpriteComponent,
            "has_audio_component", hasAudioComponent,
            "has_music_component", hasMusicComponent,
            "has_fysics_component", hasFysicsBodyComponent

        );
    }

}