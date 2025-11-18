#include "FysicsMaterialComponent.h"
#include "Architecture/Application.h"
#include "Fysics/FysicsManager.h"

namespace IonixEngine
{
    void FysicsMaterialComponent::UpdateMaterial(Entity* entity, float friction, float restitution)
    {
        GetFixture(entity)->SetFriction(friction);
        GetFixture(entity)->SetRestitution(restitution);
    }

    float FysicsMaterialComponent::GetFriction(Entity* entity)
    {
        b2Fixture* fixture = GetFixture(entity);
        return fixture->GetFriction();
    }

    float FysicsMaterialComponent::GetRestitution(Entity* entity)
    {
        b2Fixture* fixture = GetFixture(entity);
        return fixture->GetFriction();
    }

    b2Fixture* FysicsMaterialComponent::GetFixture(Entity* entity)
    {
        FysicsManager& fysics_manager = *Application::Get().layerFysics->GetFysicsManager();
        b2Body* body = fysics_manager.GetBodyFromEntity(entity);
       // fixture = fysics_manager.GetBodyFromEntity(entity)->GetFixtureList();
        return body->GetFixtureList();
    }
}
