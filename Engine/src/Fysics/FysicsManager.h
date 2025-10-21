#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicBody.h"
#include "Fysics/Shapes.h"
#include "Fysics/Joints.h"
#include "Fysics/Force.h"

namespace IonixEngine
{
    class FysicsManager
    {
    private:
        FysicBody* body;
        FysicsShapes* shape;
        Joints* joint;
        Force* force;
        b2World* world;

        static LayerFysics* s_instance;
    public:
        static void SetInstance(LayerFysics* instance) {
            s_instance = instance;
        }

        void Create() 
        {   
            body = new FysicBody(world);
            shape = new FysicsShapes(body->GetBody());
            
        }
    };
}