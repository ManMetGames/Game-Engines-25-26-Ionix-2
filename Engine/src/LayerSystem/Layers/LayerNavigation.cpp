#include "LayerNavigation.h"

namespace IonixEngine
{
    LayerNavigation* LayerNavigation::instance = nullptr;

    void LayerNavigation::OnAttach()
    {
        instance = this;

        // Load navmesh from a file / generated data
        // nav.Load(cornerList, indexList);
    }

    void LayerNavigation::OnDetach()
    {
        instance = nullptr;
    }

    void LayerNavigation::OnUpdate()
    {
        //draw navmesh, debug visualisation etc.
    }

    std::vector<int> LayerNavigation::RequestPath(int startCell, int goalCell)
    {
        return nav.FindPath(startCell, goalCell);
    }
}
