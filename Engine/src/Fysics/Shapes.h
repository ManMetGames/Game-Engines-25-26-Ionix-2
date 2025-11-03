#pragma once
#include "box2d.h"
#include <vector> 
#include "Architecture/ECS/Entity.hpp"

// Some great work done in the initial Shapes.h class - lots of inspiration taken.
// You can keep as was and integrate ECS in a similar method to this example or you can run with this example (one method with a switch statement).
namespace IonixEngine
{
    class Application;
    class FysicsShapes
    {
        public:
            
        FysicsShapes() {}

        //Add Box - Note the Entity* param
        void AddCollider(Entity* entity, int colliderType, float density, bool isTrigger);
    };    
}

