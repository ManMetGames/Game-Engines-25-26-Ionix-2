#include "ECS_Test.hpp"
#include "Architecture/TextureManager/TextureManager.h"
#include "Architecture/ECS/Component.hpp"
#include <Graphics/QueueRenderer.h>

namespace IonixEngine {
    SpriteRenderer::SpriteRenderer(Entity* entity) : Component(entity, false, true, false) {
        image = IonixEngine::TextureManager::Get().GetTexture("Test").GetTexture();
    }

    void SpriteRenderer::Render(RenderData* data) {
        Vec2 position = entity->transform.GetGlobalPosition();
        Vec2 scale = entity->transform.GetGlobalScale();
        SDL_Rect draw = { static_cast<int>(position.x - (50 * scale.x)), static_cast<int>(position.y - (50 * scale.y)), static_cast<int>(100 * scale.x), static_cast<int>(100 * scale.y) };

        data->queue->AddToQueue(RenderCall{
            image,
            SDL_Rect { (int)(position.x), (int)(position.y), (int)100, (int)100},
            SDL_Rect { 0, 0, 298, 325 },
        });
    }


    EntityMover::EntityMover(Entity* entity, float speed) : Component(entity, false, false, false), speed(speed) {}

    void EntityMover::Update(float dt) {
        float localRot = entity->transform.GetLocalRotation();
        float newRotation = localRot + (speed * dt);
        entity->transform.SetLocalRotation(newRotation);
    }
}
