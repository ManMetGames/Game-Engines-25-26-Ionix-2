#pragma once

#include "Component.hpp"
#include "Temp_Vec2.hpp"
#include "Transforms.h"
#include <cstdint>
#include <vector>
#include <type_traits>

namespace IonixEngine {

    typedef uint64_t EntityID;

    class Component;

    class Scene;

    class RenderData;

    class Entity {
    private:
        bool remove;
        bool m_Active;  // Entity is active and should be updated/rendered

    public:
        Vec2 position;
        float rotation;
        int32_t zOrder;
        EntityID id;
        Transform transform;
        int renderLayer;
        
        Entity(EntityID id, int renderLayer);
        
        // move constructor for ownership transfer
        Entity(Entity&& other) noexcept;
        
        // move assignment for proper ownership transfer
        Entity& operator=(Entity&& other) noexcept;
        
        // disable copy operations to prevent accidental component pointer duplication
        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;
        
        std::vector<Component*> components;

        // May want to add a pointer to scene entity belongs to?

        void Init(Scene* scene);
        void Render(RenderData* data);
        void Update(float dt);
        void Collision(Entity* other);
        void Destroy(Scene* scene);
        
        // Active state management
        bool IsActive() const { return m_Active; }
        void SetActive(bool active) { m_Active = active; }

        template<typename T> T* GetComponent() {
            static_assert(std::is_base_of<Component, T>::value, "Type does not inherit component");
            for (Component* candidate : components) {
                T* component = dynamic_cast<T*>(candidate);
                if (component) {
                    return component;
                }
            }
            return nullptr;
        }

        template<typename T> T* AddComponent(T* component) {
            static_assert(std::is_base_of<Component, T>::value, "Type does not inherit component");
            if (!component) { return nullptr; }
            components.push_back(dynamic_cast<Component*>(component));
            components.back()->Start();
            return dynamic_cast<T*>(components.back());
        }

        template<typename T> bool TryGetComponent(T** out) {
            static_assert(std::is_base_of<Component, T>::value, "Type does not inherit component");
            for (Component* candidate : components) {
                T* component = dynamic_cast<T*>(candidate);
                if (component) {
                    *out = component;
                    return true;
                }
            }
            *out = nullptr;
            return false;
        }

        template<typename T> bool HasComponent() {
            static_assert(std::is_base_of<Component, T>::value, "Type does not inherit component");
            for (Component* candidate : components) {
                T* component = dynamic_cast<T*>(candidate);
                if (component) {
                    return true;
                }
            }
            return false;
        }
    };
};
