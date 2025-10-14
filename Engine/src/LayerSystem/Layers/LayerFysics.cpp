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
    void LayerFysics::OnDetach() 
    {
        delete world; world = nullptr;
    }
    void LayerFysics::OnUpdate()
    {
        if (!world) return; //cheeky early return to see if world is attatched
        world->Step(timeStep, velocityIterations, positionIterations); // world step on update
    } 
    void LayerFysics::OnEvent(IonixEvent& e)
    {
        // Switch statement routes the event and invokes the relevant event handler

        switch (e.Type)
        {
            // Add more cases as needed.... (Note: Most engine features don't require events, they
            //                              can just be callable functions.
        }
    }

    void LayerFysics::SetGravity(float x, float y, bool wake)
    {
        if (!world) return;
        world->SetGravity(b2Vec2(x, y));
        if (wake) for (b2Body* b = world->GetBodyList();b;b = b->GetNext()) b->SetAwake(true);
    }

    b2Vec2 LayerFysics::GetGravity() const
    {
        return world ? world->GetGravity() : b2Vec2(0.f, 0.f);
    }
}