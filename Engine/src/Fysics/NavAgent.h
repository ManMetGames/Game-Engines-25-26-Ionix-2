#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

//#include <vector>
//#include <array>
//#include <iostream>
//#include <queue>

namespace IonixEngine
{

    class NavAgent {
    private:

        //need a size for collisions are we using cicles or tile for agent
        b2Vec2 size;
        b2Vec2 offset;
        float speed;
        float acceleration;
    public:
        NavMef PlaceAgent();

    };
}

