#pragma once
#include "Fysics/NavMef.h"
#include "LayerSystem/Layer.h"
#include <vector>

namespace IonixEngine
{
    class LayerNavigation : public Layer
    {
    private:
        NavMef nav;

    public:
        static LayerNavigation* instance;

        NavMef& GetNavMef() { return nav; }
        std::vector<int> RequestPath(int startCell, int goalCell);

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
    };
}