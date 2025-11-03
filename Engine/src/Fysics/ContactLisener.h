#pragma once
#include "box2d.h"
#include <iostream>

namespace IonixEngine
{
	class ContactListener : public b2ContactListener
    {
        // Called when two fixtures start touching
        void BeginContact(b2Contact* contact) override;

        // Called when two fixtures no longer touch
        void EndContact(b2Contact* contact) override {
            std::cout << "Collision ended!" << std::endl;
        }
    };    
}

