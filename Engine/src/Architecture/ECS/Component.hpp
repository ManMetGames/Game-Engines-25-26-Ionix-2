#pragma once

#include "SDL_render.h"

namespace IonixEngine {

    class Entity;

    class QueueRenderer;

    class RenderData {
    public:
        SDL_Renderer* renderer;
        QueueRenderer* queue;
    };

    class Component {
    protected:
        Entity* entity;
        bool hasPhysics;
        bool canRender;
        bool isTag;

    public:
        // You should be able to hardcode most of the booleans in your component
        // Ex: A rigidbody would likely be Rigidbody(...) : Component(entity, true, false, false)
        Component(Entity* entity, bool hasPhysics, bool canRender, bool tag);
        virtual ~Component() = default;

        virtual void Start();
        virtual void Update(float dt);
        virtual void Render(RenderData* renderData);
        virtual void Collide(Entity* other);
        virtual void Destroy();

        // Update the entity pointer (used when entity is moved in memory)
        void SetEntity(Entity* newEntity) { entity = newEntity; }

        inline bool HasPhysics() { return hasPhysics; }
        inline bool CanRender() { return canRender; }
        inline bool IsTag() { return isTag; }
    };
};
