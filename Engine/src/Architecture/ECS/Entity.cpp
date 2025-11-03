#include "Entity.hpp"
#include "Component.hpp"

//For some reason Visual Studio cannot find Scene.h
//#include "Scene.h"
#include "./Architecture/Scene.h"

#include <vector>
#include <cmath>

#include "SDL_log.h"

namespace IonixEngine {

    Entity::Entity(EntityID id) :
        id(id),
        position(Vec2{ 0, 0 }),
        zOrder(0),
        rotation(0),
        remove(false),
        transform(this)
    {
        components = std::vector<Component*>();
        SDL_Log("Entity capacity = %zu",components.capacity());
        components.reserve(10);
        SDL_Log("Entity capacity = %zu", components.capacity());
    }

    void Entity::Init(Scene* scene) {}

    void Entity::Render(RenderData* data) {
        for (Component* component : components) {
            if (!component->CanRender()) { continue; }
            component->Render(data);
        }
    }

    void Entity::Update(float dt) {
        for (Component* component : components) {
            if (component->IsTag()) { continue; }
            component->Update(dt);
        }
    }

    void Entity::Collision(Entity* other) {
        for (Component* component : components) {
            if (component->IsTag()) { continue; }
            component->Collide(other);
        }
    }

    void Entity::Destroy(Scene* scene) {
        for (Component* component : components) {
            if (component->IsTag()) { continue; }
            component->Destroy();
        }
        remove = true; // Mark for removal
    }
}
