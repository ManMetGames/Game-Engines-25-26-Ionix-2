#include "ECS_Test.hpp"
#include "Architecture/TextureManager/TextureManager.h"

namespace IonixEngine {
    SpriteRenderer::SpriteRenderer(Entity* entity, const std::string& textureThing, Vec2 rectSize) : Component(entity, false, true, false) {
        image = IonixEngine::TextureManager::Get().GetTexture(textureThing).GetTexture();
        size = rectSize;
    }

    void SpriteRenderer::Render(RenderData* data) {
        Vec2 position = entity->transform.GetGlobalPosition();
        SDL_Rect draw = { static_cast<int>(position.x - (size.x/2.0)), static_cast<int>(position.y - (size.y/2.0)), size.x, size.y };

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
