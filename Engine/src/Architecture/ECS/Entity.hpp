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

    enum RenderLayer {
        LAYER_BACKGROUND = 1 << 0, // 0001
        LAYER_MIDGROUND = 1 << 1, // 0010
        LAYER_UI = 1 << 2, // 0100
        LAYER_DEBUG = 1 << 3, // 1000
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
        uint32_t renderLayer;
      
        Entity(EntityID id);

        
        std::vector<Component*> components;

        // May want to add a pointer to scene entity belongs to?

        void Init(Scene* scene);
        void Render(RenderData* data);
        void Update(float dt);
        void Collision(Entity* other);
        void Destroy(Scene* scene);

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
