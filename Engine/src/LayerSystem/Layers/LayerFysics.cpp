#include "LayerSystem/Layers/LayerFysics.h"
#include <iostream>
#include <ostream>
#include "EventSystem/Event.h"
#include "Architecture/ECS/Entity.hpp"

namespace IonixEngine
{
    void LayerFysics::OnAttach() // call first says archie or brok
    {
        fysicsManager = new FysicsManager();
    }
    void LayerFysics::OnDetach()
    {
    }
    void LayerFysics::OnUpdate()
    {
        if (!fysicsManager->GetWorld()) return; //cheeky early return to see if world is attatched

        fysicsManager->GetWorld()->Step(timeStep, velocityIterations, positionIterations); // world step on update

        std::unordered_map<b2Body*, Entity*> bodyMap = fysicsManager->GetBodyMap();

        // Code move this to a two-pass system if needed later
        for (auto b : bodyMap)
        {
            if (!b.first->IsAwake()) continue;  // Skip sleeping bodies

			// Update entity position based on physics body position	
            Entity* ent = b.second;
            b2Vec2 pos = b.first->GetPosition();

            ent->position.x = pos.x * 100.0f;
            ent->position.y = pos.y * 100.0f;

			float angle = b.first->GetAngle();
			//std::cout << angle << std::endl;
            ent->rotation = b.first->GetAngle() * (180.0f / M_PI);
        }
    } 
    
    void LayerFysics::OnEvent(IonixEvent& e)
    {
        // Switch statement routes the event and invokes the relevant event handler

        switch (e.Type)
        {
            // Add more cases as needed.... (Note: Most engine features don't require events, they
            //                              can just be callable functions.
        }
    }

    FysicsManager* LayerFysics::GetFysicsManager()
    {
        return fysicsManager;
    }
}