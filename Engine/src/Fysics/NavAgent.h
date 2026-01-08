#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "NavMef.h"

//#include <vector>
//#include <array>
//#include <iostream>
//#include <queue>

namespace IonixEngine
{
    struct BezierSegment
    {
        b2Vec2 p0, p1, p2, p3;  
    };

    class NavAgent {
    private:

        //need a size for collisions are we using cicles or tile for agent
        NavMef* m_NavMef;
        Entity* m_entity;
        float m_speed;
        std::vector<BezierSegment> m_segments;
        int m_segmentIndex = 0;
        float m_t = 0.0f; 


        std::vector<b2Vec2> m_path; // this needs the funnel alg
        int m_pathIndex = 0;

    public:
        NavAgent(NavMef* nav, Entity* ent, float speed);
        void PlaceAgent(b2Vec2 endPosition);  //kkkkkk
        void Update(float dt); // to move agent
        void MoveTo(const b2Vec2& target);
        std::vector<BezierSegment> SmoothPath(const std::vector<b2Vec2>& path, float smoothFactor = 0.3f);

        

    };
}

