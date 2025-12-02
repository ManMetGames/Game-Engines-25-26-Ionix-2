#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "NavMef.h"

//#include <vector>
//#include <array>
//#include <iostream>
//#include <queue>

namespace IonixEngine
{

    class NavAgent {
    private:

        //need a size for collisions are we using cicles or tile for agent
        NavMef* m_NavMef;
        //b2Body* m_Body; //maybe needed idk 
        Entity* entity;
        float speed;

        std::vector<int> m_path;
        int m_pathIndex = 0;

    public:
        void PlaceAgent(b2Vec2 endPosition);
        void Update(float dt); // to move agent

    };
}

