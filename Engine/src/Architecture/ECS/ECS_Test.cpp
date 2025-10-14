#include "ECS_Test.hpp"
namespace IonixEngine {
    SpriteRenderer::SpriteRenderer(Entity* entity) : Component(entity, false, true, false) {
		image = IonixEngine::TextureManager::Get().GetTexture("Test").GetTexture();
	}

    void SpriteRenderer::Render(RenderData* data) {
    SDL_Rect draw = { static_cast<int>(entity->position.x - 50), static_cast<int>(entity->position.y - 50), 100, 100 };

    if (SDL_RenderCopy(data->renderer, image, nullptr, &draw) < 0) {
        SDL_Log("Could not draw texture: %s", SDL_GetError());
    }
}

void EntityMover::Update(float dt) {
    time += dt * 3.14;
    entity->position = Vec2 { 500 + 100 * cosf(time), 300 + sinf(time) * 100 };
}
}
