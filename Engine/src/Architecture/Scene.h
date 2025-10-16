#pragma once
#include "Architecture/ECS/Component.hpp"
#include "Architecture/Macros.h"
#include "EventSystem/Event.h"
#include "Architecture/ECS/Entity.hpp"
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace IonixEngine {
    struct SceneHandle {
        Scene* currentScene;
        // Scene* persistentScene
        
        SceneHandle();

        void SetScene(Scene* scene);
    };

    // Base interface for a game Scene (single active scene model)
    class Scene {
        RenderData renderData;
        static SceneHandle& Handle();
    public:
        ~Scene() = default;
        
        // Get current scene
        Scene* CurrentScene();
        // Called when the scene becomes the active scene
        void OnEnter();
        // Called when the scene stops being the active scene
        void OnExit();
        // Called once per frame while the scene is active
        void OnUpdate(float dt);
        // Receives input/window events while the scene is active
        void OnEvent(IonixEvent& e);

        void Reserve(std::size_t count); // Pre-allocate storage for entities 

        EntityID CreateEntity();  // Create a new entity and return its unique ID
        
        bool DestroyEntity(EntityID entityId);
        

        // Get the number of currently stored entities
        inline std::size_t Count() const { return m_Entities.size(); }
        
        Entity* GetEntityFromID(EntityID id);

    private:
        std::vector<Entity> m_Entities;
        std::unordered_map<EntityID, std::size_t> m_IdToIndex;
        EntityID m_NextId{ 0 };
    };
}
