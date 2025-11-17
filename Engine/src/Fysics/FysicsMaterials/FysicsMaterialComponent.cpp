#include "FysicsMaterialComponent.h"
#include "Architecture/Application.h"
#include "Fysics/FysicsManager.h"

namespace IonixEngine
{
    FysicsMaterialComponent::FysicsMaterialComponent(Entity* entity, std::string alias, b2World* world, float friction, float restitution) : Component(entity, true, true, false)
    {
        FysicsManager& fysics_manager = *Application::Get().layerFysics->GetFysicsManager();
        body = fysics_manager.GetBodyFromEntity(entity);
        if (!body){return;}
        b2Fixture* fixture = body->GetFixtureList();
        if (!fixture){return;}
        fixture->SetFriction(friction);
        fixture->SetRestitution(restitution);
    }

    void FysicsMaterialComponent::UpdateMaterial(Entity* entity, float friction, float restitution)
    {
        FysicsManager& fysics_manager = *Application::Get().layerFysics->GetFysicsManager();
        b2Body* body = fysics_manager.GetBodyFromEntity(entity);
        if (!body){return;}
        b2Fixture* fixture = body->GetFixtureList();
        if (!fixture){return;}
        fixture->SetFriction(friction);
        fixture->SetRestitution(restitution);
    }

    float FysicsMaterialComponent::GetFriction(Entity* entity)
    {
        FysicsManager& fysics_manager = *Application::Get().layerFysics->GetFysicsManager();
        body = fysics_manager.GetBodyFromEntity(entity);
        b2Fixture* fixture = body->GetFixtureList();
        return fixture->GetFriction();
    }

    float FysicsMaterialComponent::GetRestitution(Entity* entity)
    {
        FysicsManager& fysics_manager = *Application::Get().layerFysics->GetFysicsManager();
        body = fysics_manager.GetBodyFromEntity(entity);
        b2Fixture* fixture = body->GetFixtureList();
        return fixture->GetFriction();
    }
}
