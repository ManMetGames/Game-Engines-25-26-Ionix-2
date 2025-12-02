#pragma once
#include "Fysics/NavMef.h"
#include "Fysics/NavAgent.h"
#include "LayerSystem/Layer.h"
#include <vector>

namespace IonixEngine
{
    class LayerNavigation : public Layer
    {
    private:
        NavMef nav;
        std::vector<NavAgent> agents;
        int m_nextId;

    public:
        static LayerNavigation* instance;

        NavMef& GetNavMef() { return nav; }
        std::vector<int> RequestPath(int startCell, int goalCell);
        NavAgent* CreateAgent(const b2Vec2 endPosition);

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
    };
}