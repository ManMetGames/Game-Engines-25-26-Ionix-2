#pragma once
#include "Entity.hpp"
#include "Component.hpp"
#include "SDL_render.h"
#include <cmath>
#include <string>

namespace IonixEngine {

    class SpriteRenderer : public Component {
        SDL_Texture* image;
        Vec2 size;
    public:
        SpriteRenderer(Entity* entity, const std::string& textureThing, Vec2 rectSize = Vec2{100,100});

        virtual void Render(RenderData* data) override;
    };

    class EntityMover : public Component {
        float speed = 0.0f;
    public:
        EntityMover(Entity* entity, float speed);
        virtual void Update(float dt) override;
    };
};
