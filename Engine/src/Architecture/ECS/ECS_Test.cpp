#include "ECS_Test.hpp"
#include "Architecture/TextureManager/TextureManager.h"

namespace IonixEngine {
    SpriteRenderer::SpriteRenderer(Entity* entity) : Component(entity, false, true, false) {
        image = IonixEngine::TextureManager::Get().GetTexture("Test").GetTexture();
    }

    void SpriteRenderer::Render(RenderData* data) {
        Vec2 position = entity->transform.GetGlobalPosition();
        Vec2 scale = entity->transform.GetGlobalScale();
        SDL_Rect draw = { static_cast<int>(position.x - (50 * scale.x)), static_cast<int>(position.y - (50 * scale.y)), static_cast<int>(100 * scale.x), static_cast<int>(100 * scale.y) };

        if (SDL_RenderCopy(data->renderer, image, nullptr, &draw) < 0) {
            SDL_Log("Could not draw texture: %s", SDL_GetError());
        }
    }


    EntityMover::EntityMover(Entity* entity, float speed) : Component(entity, false, false, false), speed(speed) {}

    void EntityMover::Update(float dt) {
        float localRot = entity->transform.GetLocalRotation();
        float newRotation = localRot + (speed * dt);
        entity->transform.SetLocalRotation(newRotation);
    }
}
