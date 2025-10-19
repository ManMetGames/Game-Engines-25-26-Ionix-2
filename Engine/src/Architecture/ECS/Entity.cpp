#include "Entity.hpp"
#include "Component.hpp"
#include <type_traits>
#include <vector>
#include <cmath>

namespace IonixEngine {

    Entity::Entity(EntityID id) :
        id(id),
        position(Vec2{ 0, 0 }),
        zOrder(0),
        rotation(0),
        remove(false),
        parentID(0)
    {
        components = std::vector<Component*>();
        childID = std::vector<EntityID>();
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

    float Entity::GetGlobalRotation(Scene* scene)
    {
        //Unsure how to have parenting system yet sooo...
        if (parentID != 0)
        {
            Entity* parent = scene->GetEntityFromID(parentID);
            return rotation + parent->GetGlobalRotation(scene);
        }
        else
        {
            return rotation;
        }
    }

    Vec2 Entity::GetGlobalPosition(Scene* scene)
    {
        if (parentID != 0)
        {
            Entity* parent = scene->GetEntityFromID(parentID);
            float parentRot = parent->GetGlobalRotation(scene);
            Vec2 parentPos = parent->GetGlobalPosition(scene);

            float angle = parentRot;

            if (position.x != 0.0f && position.y != 0.0f)
            {
                angle += (atan((position.x / position.y)) - 180.0f);
            }

            float mag = sqrt(pow(position.x, 2) + pow(position.y, 2));

            float x = mag * sin(angle);
            float y = mag * cos(angle);

            x += parentPos.x;
            y += parentPos.y;

            return Vec2{ x,y };
        }
        else
        {
            return position;
        }
    }
}