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
        Entity* m_entity;
        float m_speed;

        std::vector<b2Vec2> m_path; // this needs the funnel alg
        int m_pathIndex = 0;

    public:
        NavAgent(NavMef* nav, Entity* ent, float speed);
        void PlaceAgent(b2Vec2 endPosition);  //kkkkkk
        void Update(float dt); // to move agent

    };
}

