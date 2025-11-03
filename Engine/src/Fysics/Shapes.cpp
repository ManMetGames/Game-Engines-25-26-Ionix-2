#pragma once
#include "Fysics/Shapes.h"
#include "Architecture/Application.h"
#include <unordered_map>

namespace IonixEngine
{
    void FysicsShapes::AddCollider(Entity* entity, int colliderType, float density, bool isTrigger)
    {
		// Brand new fixture def (rigidbodies can support multiple if needed)
        b2FixtureDef fixtureDef;
		
		// Create shape 
        b2PolygonShape shape;

		// What collider type - again, extend for circle/polygon or go your initial method of having inidividual methods - both completely fine.
        switch (colliderType)
        {
        case 0: // Box Collider
            shape.SetAsBox(0.15, 0.15, b2Vec2_zero, 0.0f);
            fixtureDef.shape = &shape;
            break;
        }
    
		// Making sure to use the data passed into the method.
        fixtureDef.isSensor = isTrigger;
        fixtureDef.density = density;

		// Access the dictionary
		std::unordered_map<b2Body*, Entity*>& bodyMap = Application::Get().layerFysics->GetFysicsManager()->GetBodyMap();

		// Get the rigidbody associated with the entity we want to add a collider to.
		b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);

		// Add the fixture to the rigidbody. Done.
        body->CreateFixture(&fixtureDef);
    }
}

