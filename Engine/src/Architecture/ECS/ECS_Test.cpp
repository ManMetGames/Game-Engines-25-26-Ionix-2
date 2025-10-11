#include "Entity.hpp"
#include "Component.hpp"
#include "SDL_render.h"
#include "Architecture/TextureManager/TextureManager.h"
#include <cmath>

class SpriteRenderer : public Component {
    SDL_Texture* image;
public:
    SpriteRenderer(Entity* entity) : Component(entity, false, true, false) {
        image = IonixEngine::TextureManager::Get().GetTexture("Debug").GetTexture();
    }

    virtual void Render(RenderData* data) override;
};

class EntityMover : public Component { 
    float time = 0.0f;
public:
    EntityMover(Entity* entity) : Component(entity, false, false, false) {}
    virtual void Update(float dt) override;
};

void SpriteRenderer::Render(RenderData* data) {
    SDL_Rect draw = { static_cast<int>(entity->position.x - 50), static_cast<int>(entity->position.y - 50), 100, 100 };

    SDL_RenderCopy(data->renderer, image, nullptr, &draw);
}

void EntityMover::Update(float dt) {
    time += dt * 3.14;
    entity->position = Vec2 { 500 + 100 * cosf(time), 500 + sinf(time) * 100 };
}
