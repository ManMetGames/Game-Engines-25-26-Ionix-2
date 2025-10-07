#include "Entity.hpp"
#include "Component.hpp"
#include <type_traits>
#include <vector>

Entity::Entity(Vec2 position) : 
    position(position), 
    zOrder(0), 
    rotation(0), 
    remove(false)
{
    components = std::vector<Component*>();
}

void Entity::Init(Scene* scene) { }

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
