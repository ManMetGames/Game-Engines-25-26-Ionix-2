#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include <box2d.h>
#include <iostream>

namespace IonixEngine
{
    class ContactListener : public b2ContactListener
    {
    public:

        void BeginContact(b2Contact* contact) override;

        void EndContact(b2Contact* contact) override
        {
            std::cout << "Collision ended!" << std::endl;
        }
    };
    
}

