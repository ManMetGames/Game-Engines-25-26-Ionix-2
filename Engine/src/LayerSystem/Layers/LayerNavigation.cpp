#include "LayerNavigation.h"

#include "Architecture/Application.h"

namespace IonixEngine
{
    LayerNavigation* LayerNavigation::instance = nullptr;

    LayerNavigation* LayerNavigation::GetInstance()
    {
        return Application::Get().layerNavigation;
    }
    void LayerNavigation::OnAttach()
    {
        instance = this;
        nav = new NavMef();
    }

    void LayerNavigation::OnDetach()
    {
        instance = nullptr;
        agents.clear();
    }

    void LayerNavigation::OnUpdate()
    {
        //draw navmesh, debug visualisation etc.        
        for (auto& agent : agents) {
            agent->Update(1.0f);
        }
    }


    
    
    std::vector<int> LayerNavigation::RequestPath(int startCell, int goalCell)
    {
        return nav->FindPath(startCell, goalCell);
    }
    NavAgent* LayerNavigation::CreateAgent(const b2Vec2 endPosition)
    {
        auto agent = std::make_unique<NavAgent>(nav, ent, 3.0f);
        NavAgent* agentPtr = agent.get();
        agent->PlaceAgent(endPosition);
        agents.push_back(std::move(agent));

        return agentPtr;
    }
}
