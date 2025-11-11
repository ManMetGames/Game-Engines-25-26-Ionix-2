#pragma once
#include "Entity.hpp"
#include "Component.hpp"
#include "SDL_render.h"
#include <cmath>

namespace IonixEngine {

    class SpriteRenderer : public Component {
        SDL_Texture* image;
    public:
        SpriteRenderer(Entity* entity);

        virtual void Render(RenderData* data) override;
    };

    class EntityMover : public Component {
        float speed = 0.0f;
    public:
        EntityMover(Entity* entity, float speed);
        virtual void Update(float dt) override;
    };
};
