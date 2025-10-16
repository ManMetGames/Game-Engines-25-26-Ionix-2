#include "Entity.hpp"
#include "Component.hpp"
#include <type_traits>
#include <vector>

namespace IonixEngine {

Entity::Entity(EntityID id) : 
    id(id),
    position(Vec2 { 0, 0 }), 
    zOrder(0), 
    rotation(0), 
    remove(false)
{
    components = std::vector<Component*>();
}

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
