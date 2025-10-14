#pragma once
#include "LayerSystem/Layer.h"
#include "box2d.h"

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
        virtual void SetGravity(float x, float y, bool wake = true);
        virtual b2Vec2 GetGravity() const;

        b2World* GetWorld() const { return world; }
    private:
        b2World* world{ nullptr };
        float timeStep{ 1.f / 60.f };
        int32 velocityIterations{ 6 };
        int32 positionIterations{ 2 };
        float ppm{ 100.f };
    };
}