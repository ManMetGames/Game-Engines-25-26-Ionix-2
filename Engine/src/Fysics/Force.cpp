#include "Force.h"
#include "Architecture/Application.h"
#include "FysicsManager.h"

namespace IonixEngine
{	
    void Force::AddForce(Entity* entity, b2Vec2 force, b2Vec2 point)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        if (body)
        {
            body->ApplyForce(force, point, true);
        }
    }

    void Force::AddForceToCenter(Entity* entity, b2Vec2 force)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        if (body)
        {
            body->ApplyForceToCenter(force, true);
        }
    }
    // Add impulse
    void Force::AddImpulse(Entity* entity, b2Vec2 impulse, b2Vec2 point)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        if (body)
        {
            body->ApplyLinearImpulse(impulse, point, true);
        }
    }

    // Add impulse at the center of mass 
    void Force::AddImpulseToCenter(Entity* entity, b2Vec2 impulse)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        // add forces to my fysics manager for the body dictionary
        if (body)
        {
            body->ApplyLinearImpulseToCenter(impulse, true);
        }
    }
    //Add Torque
    void Force::AddTorque(Entity* entity, float torque)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        if (body)
        {
            body->ApplyTorque(torque, true);
        }
    }
    //Add impulse with torque
    void Force::AddAngularImpulse(Entity* entity, float torque)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        if (body) {
            body->ApplyAngularImpulse(torque, true);
        }
    }

    // Clear all forces/velocities on this body
    void Force::ClearForces(Entity* entity)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        if (body)
        {
            body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
            body->SetAngularVelocity(0.0f);
        }
    }
}