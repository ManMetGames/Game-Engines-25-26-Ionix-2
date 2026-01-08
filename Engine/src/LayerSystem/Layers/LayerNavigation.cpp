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
    NavAgent* LayerNavigation::CreateAgent(Entity* ent, float speed)
    {
        auto agent = std::make_unique<NavAgent>(nav, ent, speed);
        NavAgent* agentPtr = agent.get();
        agents.push_back(std::move(agent));
        return agentPtr;
    }
}
