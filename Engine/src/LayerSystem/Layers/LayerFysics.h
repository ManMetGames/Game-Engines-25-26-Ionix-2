#pragma once
#include "LayerSystem/Layer.h"
#include "box2d.h"
#include "Fysics/FysicsManager.h"

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

        b2Body* CreateGroundBox(float x = 0.f, float y = 0.f, float hx = 50.f, float hy = 1.f, float angle = 0.f, float friction = 0.6f, float restitution = 0.f);  

        b2World* GetWorld() const { return world; }

        
        static LayerFysics* GetInstance();
        
    private:
        static LayerFysics* instance;
        b2World* world{ nullptr };
        
        float timeStep{ 1.f / 60.f };
        int32 velocityIterations{ 6 };
        int32 positionIterations{ 2 };
        float ppm{ 100.f };

    };
}