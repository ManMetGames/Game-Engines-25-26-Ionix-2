#include "LayerSystem/Layers/LayerFysics.h"

#include <iostream>
#include <ostream>

#include "EventSystem/Event.h"

namespace IonixEngine
{
    void LayerFysics::OnAttach()
    {
        //Set Gravity
        b2Vec2 gravity = b2Vec2(0.0f, -9.8f);
        //Create the world
        b2World world(gravity);
    }
    void LayerFysics::OnDetach() {}
    void LayerFysics::OnUpdate(){} 
    void LayerFysics::OnEvent(IonixEvent& e)
    {
        // Switch statement routes the event and invokes the relevant event handler

        switch (e.Type)
        {
            // Add more cases as needed.... (Note: Most engine features don't require events, they
            //                              can just be callable functions.
        }
    }
}