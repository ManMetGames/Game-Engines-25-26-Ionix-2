#include "RigidbodyComponent.h"
#include "Architecture/Application.h"

IonixEngine::RigidBodyComponent::RigidBodyComponent(IonixEngine::Entity* entity, std::string alias, int type) : Component(entity, true, false, false)
{
	// Every time we add a rigidbody to some entity, we want a brand new physics body definition.
    b2BodyDef bodyDef;

	// Set the type based on param 3 (type)
    switch (type)
    {
    case 0:
        bodyDef.type = b2_staticBody;
		break;
    case 1:
		bodyDef.type = b2_kinematicBody;
        break;
	case 2:     
        bodyDef.type = b2_dynamicBody;
        break;
    default:
        bodyDef.type = b2_staticBody;
        break;
    }

	// Cache the fysicsManager for streamlining 
    FysicsManager& fysicsManager = *Application::Get().layerFysics->GetFysicsManager();
	
	// Initialise a brand new rigidbody and make sure its initialised within our singular physics world
    b2Body* body = fysicsManager.GetWorld()->CreateBody(&bodyDef);


    // Put the rigidbody in the correct position (where the entity is)
	// We divide by 100 because SDL2 uses pixels and Box2D uses meters.
	// If we're setting the PHYSICS (body->SetTransform) position then we need to divide by 100.
	
    body->SetTransform(b2Vec2(entity->position.x / 100.0f, entity->position.y / 100.0f), entity->rotation);

    // Insert into body map for iteration later - we know what rigidbody belongs to what entity
    // Also update bespoke data sructures in Box2D just in case 

    fysicsManager.GetBodyMap()[body] = entity;


    // .. 
    //body->GetUserData().pointer = (uintptr_t)(entity);
}
