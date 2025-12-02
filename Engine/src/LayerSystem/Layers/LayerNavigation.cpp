#include "LayerNavigation.h"

namespace IonixEngine
{
    LayerNavigation* LayerNavigation::instance = nullptr;

    void LayerNavigation::OnAttach()
    {
        instance = this;
    }

    void LayerNavigation::OnDetach()
    {
        instance = nullptr;
    }

    void LayerNavigation::OnUpdate()
    {
        //draw navmesh, debug visualisation etc.        
        for (NavAgent agent : agents) {
            //agent.Update(1);
        }
    }

    std::vector<int> LayerNavigation::RequestPath(int startCell, int goalCell)
    {
        return nav->FindPath(startCell, goalCell);
    }
    NavAgent* LayerNavigation::CreateAgent(const b2Vec2 endPosition)
    {
        NavAgent m_agent(nav, ent);
        agents.emplace_back(m_agent);
        m_agent.PlaceAgent(endPosition);

        return nullptr;
    }
}
