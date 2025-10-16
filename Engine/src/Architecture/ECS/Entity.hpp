#pragma once

#include "Component.hpp"
#include "Temp_Vec2.hpp"
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

    public:
        Vec2 position;
        float rotation;
        int32_t zOrder;
        EntityID id;

        Entity(EntityID id);

        std::vector<Component*> components;

        // May want to add a pointer to scene entity belongs to?

        /**
         * Called once per frame after Entity::Update() - calls Component::Render() on all components with canRender flag set to true
         * @param data - Context for renderer - should probably contain pointer/reference to render queue later
         */
        void Render(RenderData* data);

        /**
         * Called once per frame - calls Component::Update() on all components without isTag set to true
         * @param dt - Time since last frame
         */
        void Update(float dt);

        /**
         * Called when two physics objects collide/intersect - called multiple times while colliding
         * @param other - Entity this entity collided with
         */
        void Collision(Entity* other);

        /**
         * Sets remove flag to true and calls Component::Destroy on all non tag components
         * @param scene - Scene this entity belongs to
         */
        void Destroy(Scene* scene);

        /**
         * Returns whether entity is flagged for removal
         * @return - remove flag value
         */
        inline bool FlaggedForRemoval() { return remove; }

        /**
         * Get component without error checking
         * @return Component pointer - will be nullptr if not component not present
         * @example `entity->GetComponent<SpriteRenderer>()->someSpriteRendererField`
         */
        template<typename T> T* GetComponent() {
            static_assert(std::is_base_of<Component, T>::value, "Type does not inherit component");
            for (Component* candidate : components) {
                T* component = dynamic_cast<T*>(candidate);
                if (component) {
                    return component;
                }
            }
            return nullptr;
        };

        /**
         * Adds component by pointer
         * @return Added component
         * @param component Component to add to components - recommended to pass component made via new to avoid lifetime issues
         * @example `entity->AddComponent(new SpriteRenderer("./Assets/Image.png"));`
         */
        template<typename T> T* AddComponent(T* component) {
            static_assert(std::is_base_of<Component, T>::value, "Type does not inherit component");
            if (!component) { return nullptr; }
            components.push_back(dynamic_cast<Component*>(component));
            components.back()->Start();
            return dynamic_cast<T*>(components.back());
        };

        /**
         * Get component with nicer error handling
         * @return If component was found
         * @param out Pointer to found component - set to nullptr if returns false
         * @example SpriteRenderer* renderer; if (entity->TryGetComponent(renderer)) { // do renderer things }
         */
        template<typename T> bool TryGetComponent(T* out) {
            static_assert(std::is_base_of<Component, T>::value, "Type does not inherit component");
            for (Component* candidate : components) {
                T* component = dynamic_cast<T*>(candidate);
                if (component) {
                    out = component;
                    return true;
                }
            }
            out = nullptr;
            return false;
        };

        /**
         * Get pointers to all matching components
         * @return Vector of all components that successfully casted to T pointers
         */
        template<typename T> std::vector<T*> GetComponents() {
            static_assert(std::is_base_of<Component, T>::value, "Type does not inherit component");
            std::vector<T*> components;
            for (Component* candidate : this->components) {
                T* component = dynamic_cast<T*>(candidate);
                if (component) {
                    components.push_back(component);
                }
            }
            return components;
        };

        /**
         * Check if component is present in components
         * @return If component successfully casts to a T pointer returns true
         */
        template<typename T> bool HasComponent() {
            static_assert(std::is_base_of<Component, T>::value, "Type does not inherit component");
            for (Component* candidate : components) {
                T* component = dynamic_cast<T*>(candidate);
                if (component) {
                    return true;
                }
            }
            return false;
        };
    };
};
