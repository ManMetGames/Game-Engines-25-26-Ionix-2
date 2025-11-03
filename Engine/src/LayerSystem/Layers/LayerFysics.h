#pragma once
#include "LayerSystem/Layer.h"
#include <Fysics/FysicsManager.h>

namespace IonixEngine
{
    
    class LayerFysics : public Layer
    {
    public:
        LayerFysics() {}

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;

        FysicsManager* GetFysicsManager();
        
    private:
        FysicsManager* fysicsManager;
        static LayerFysics* instance;
        
        float timeStep{ 1.f / 60.f };
        int32 velocityIterations{ 6 };
        int32 positionIterations{ 2 };
        float ppm{ 100.f };
    };
}