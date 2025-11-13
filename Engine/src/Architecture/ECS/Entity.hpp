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

    // Layer flags for culling masks (extend as needed)
    enum LayerFlags : uint32_t {
        LAYER_DEFAULT = 1u << 0,
        LAYER_UI      = 1u << 1,
        LAYER_PLAYER  = 1u << 2,
        LAYER_ENEMY   = 1u << 3,
        LAYER_ALL     = 0xFFFFFFFFu
    };

    class Entity {
    private:
        bool remove;

    public:
        Vec2 position;
        float rotation;
        int32_t zOrder;
        EntityID id;
        Transform transform;

        // Bitmask of layers this entity belongs to (default = LAYER_DEFAULT)
        uint32_t layerMask = LAYER_DEFAULT;

        Entity(EntityID id);

        std::vector<Component*> components;

        // May want to add a pointer to scene entity belongs to?

        void Init(Scene* scene);
        void Render(RenderData* data);
        void Update(float dt);
        void Collision(Entity* other);
        void Destroy(Scene* scene);

        inline void SetLayerMask(uint32_t mask) { layerMask = mask; }
        inline uint32_t GetLayerMask() const { return layerMask; }

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
