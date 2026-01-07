#include "Entity.hpp"
#include "Component.hpp"
#include "Architecture/Scene.h"
#include <vector>
#include <cmath>
#include <utility>

#include "SDL_log.h"

namespace IonixEngine {

    Entity::Entity(EntityID id, int renderLayer) :
        id(id),
        position(Vec2{ 0, 0 }),
        zOrder(0),
        rotation(0),
        remove(false),
        m_Active(true),
        transform(this),
        renderLayer(renderLayer)
    {
        components = std::vector<Component*>();
    }

    // Move constructor
    Entity::Entity(Entity&& other) noexcept :
        id(other.id),
        position(other.position),
        zOrder(other.zOrder),
        rotation(other.rotation),
        remove(other.remove),
        m_Active(other.m_Active),
        transform(this),  // Initialize transform with THIS entity pointer
        renderLayer(other.renderLayer),
        components(std::move(other.components))
    {
        // Copy transform state from the moved entity
        transform.SetLocalPosition(other.transform.GetLocalPosition());
        transform.SetLocalRotation(other.transform.GetLocalRotation());
        transform.SetLocalScale(other.transform.GetLocalScale());
        
        // Update all component entity pointers to point to this new entity
        for (Component* component : components) {
            if (component) {
                component->SetEntity(this);
            }
        }
        
        // Clear the moved-from entity's components and mark inactive
        other.components.clear();
        other.m_Active = false;
    }

    // Move assignment operator
    Entity& Entity::operator=(Entity&& other) noexcept {
        if (this != &other) {
            // Transfer simple data
            id = other.id;
            position = other.position;
            zOrder = other.zOrder;
            rotation = other.rotation;
            remove = other.remove;
            m_Active = other.m_Active;
            renderLayer = other.renderLayer;
            
            // Move component ownership
            components = std::move(other.components);
            other.components.clear();
            other.m_Active = false;
            
            // CRITICAL: Update all component entity pointers to point to this entity
            for (Component* component : components) {
                if (component) {
                    component->SetEntity(this);
                }
            }
            
            // Copy transform state (transform already points to this entity)
            transform.SetLocalPosition(other.transform.GetLocalPosition());
            transform.SetLocalRotation(other.transform.GetLocalRotation());
            transform.SetLocalScale(other.transform.GetLocalScale());
        }
        return *this;
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
