#include "FysicsMaterialComponent.h"
#include "Architecture/Application.h"
#include "Fysics/FysicsManager.h"

namespace IonixEngine
{
    void FysicsMaterialComponent::UpdateMaterial(Entity* entity, float friction, float restitution)
    {
        b2Fixture* fixture = GetFixture(entity);
        if (!fixture)
        {
            // No fixture yet; nothing to update
            return;
        }

        // Directly set material properties on the fixture
        fixture->SetFriction(friction);
        fixture->SetRestitution(restitution);
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
        if (!body)
        {
            std::cerr << "FysicsMaterialComponent::GetFixture: entity has no body\n";
            return nullptr;
        }

        b2Fixture* fixture = body->GetFixtureList();
        if (!fixture)
        {
            std::cerr << "FysicsMaterialComponent::GetFixture: body has no fixtures\n";
            return nullptr;
        }

        return fixture;
    }
}
