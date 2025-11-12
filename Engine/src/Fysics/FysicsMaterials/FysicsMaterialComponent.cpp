#include "FysicsMaterialComponent.h"
#include "Architecture/Application.h"
#include "Fysics/FysicsManager.h"

namespace IonixEngine
{
    FysicsMaterialComponent::FysicsMaterialComponent(Entity* entity, std::string alias, b2World* world, FysicsMaterialType m_type) : Component(entity, true, true, false)
    {
        FysicsManager& fysics_manager = *Application::Get().layerFysics->GetFysicsManager();
        b2Body body* = fysics_manager.GetBodyFromEntity(entity);
    }
    
}
