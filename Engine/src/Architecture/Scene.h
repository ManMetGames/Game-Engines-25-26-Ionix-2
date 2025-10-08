#pragma once
#include "Architecture/Macros.h"
#include "EventSystem/Event.h"
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace IonixEngine
{
    using EntityID = std::uint64_t;

    struct Entity
    {
        EntityID Id{ 0 };
    };

    // Base interface for a game Scene (single active scene model)
    class ENGINE_API Scene
    {
    public:
        virtual ~Scene() = default;

        // Called when the scene becomes the active scene
        virtual void OnEnter() {}
        // Called when the scene stops being the active scene
        virtual void OnExit() {}
        // Called once per frame while the scene is active
        virtual void OnUpdate() {}
        // Receives input/window events while the scene is active
        virtual void OnEvent(IonixEvent& e) {}

        void Reserve(std::size_t count) // Pre-allocate storage for entities 
        {
            m_Entities.reserve(count);
            m_IdToIndex.reserve(count * 2);
        }

        EntityID CreateEntity()  // Create a new entity and return its unique ID
        {
            const EntityID entityId = m_NextId++;
            const std::size_t index = m_Entities.size();
            m_Entities.push_back(Entity{ entityId });
            m_IdToIndex[entityId] = index;
            return entityId;
        }

       
        bool DestroyEntity(EntityID entityId)
        {
            auto mapIt = m_IdToIndex.find(entityId); // find id in the map
            if (mapIt == m_IdToIndex.end())
                return false; // if id does not exist in the map, return false

            const std::size_t index = mapIt->second; // get the vector index of the entity
            const std::size_t lastIndex = m_Entities.size() - 1; // get index of last element in the vector

            if (index != lastIndex) // if entity deleted is not already the last one
            {
                // Move last entity into the removed slot and fix mapping
                Entity& movedEntity = m_Entities[lastIndex]; // reference to the last entity in the vector
                m_Entities[index] = movedEntity; // move last entity into the removed slot
                m_IdToIndex[movedEntity.Id] = index; // update the map
            }

            m_Entities.pop_back(); // remove last element from the vector
            m_IdToIndex.erase(mapIt); // remove id from the map
            return true;
        }

        // Get the number of currently stored entities
        std::size_t Count() const { return m_Entities.size(); }

    private:
        std::vector<Entity> m_Entities;
        std::unordered_map<EntityID, std::size_t> m_IdToIndex;
        EntityID m_NextId{ 0 };
    };
}
