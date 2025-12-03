#include "Raycast.h"
#include "Architecture/ECS/Entity.hpp"
#include "Fysics/FysicsManager.h"

namespace IonixEngine {
    
    void Raycast::MakeRayCast(Entity* entity)
    {
        Raycast callback;
        b2Vec2 point1(entity->position.x, entity->position.y);
        b2Vec2 point2(entity->position.x, entity->position.y - 5);
        Application::Get().layerFysics->GetWorld()->RayCast(&callback, point1, point2);
        std::cout << point1.x << ", " << point1.y << std::endl;
    }

}