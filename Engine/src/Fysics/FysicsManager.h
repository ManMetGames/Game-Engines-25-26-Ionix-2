#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

#include <vector>
#include <functional>

using EntityID = int;

namespace IonixEngine
{
    class FysicsManager
    {
    public:
        static LayerFysics* s_instance;
        static void SetInstance(LayerFysics* instance) {
            s_instance = instance;
        }
        using CollisionCallback = std::function<void(EntityID, EntityID)>;

        void RegisterCollisionCallback(CollisionCallback callback);
        void EmitCollision(EntityID a, EntityID b);
        void Update();

    private:
        std::vector<CollisionCallback> collisionCallbacks_;
    };
}

