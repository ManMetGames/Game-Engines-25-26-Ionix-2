#include "Entity.hpp"
#include "Component.hpp"
#include "Architecture/Scene.h"
#include "Architecture/Application.h"
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
    }

    void Entity::Init(Scene* scene) {}

    void Entity::Render(RenderData* data) {
        // Determine active camera (focused). If none, use first camera.
        Camera* activeCam = nullptr;
        if (Application::Get().layerGraphics) {
            auto& cams = Application::Get().layerGraphics->m_Cameras;
            for (auto cam : cams) {
                if (cam && cam->isFocused) {
                    activeCam = cam;
                    break;
                }
            }
            if (!activeCam && !cams.empty()) activeCam = cams.front();
        }

        // If we have an active camera, cull this entity if masks don't overlap.
        if (activeCam) {
            if ((activeCam->GetCullingMask() & layerMask) == 0u) {
                // Culled for this camera; skip rendering components.
                return;
            }
        }

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
