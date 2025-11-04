#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicsManager.h"
#include "Architecture/Application.h"

#include <iostream>
#include <ostream>

#include "EventSystem/Event.h"

namespace IonixEngine
{
    LayerFysics* LayerFysics::instance = nullptr;
    LayerFysics* FysicsManager::s_instance = nullptr;

    LayerFysics* LayerFysics::GetInstance() {
        return instance;
    }

    void LayerFysics::OnAttach() // call first says archie or brok
    {
        FysicsManager::SetInstance(this);
        //Set Gravity
        b2Vec2 gravity = b2Vec2(0.0f, -9.8f);
        //Create the world
        world = new b2World(gravity);

        //create default ground box
        b2BodyDef groundDef; groundDef.position.Set(0.f, -1.f);
        b2Body* ground = world->CreateBody(&groundDef);
        b2PolygonShape g; g.SetAsBox(50.f, 1.f);
        ground->CreateFixture(&g, 0.f);
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

   

    b2Body* LayerFysics::CreateGroundBox(float x, float y, float hx, float hy, float angle, float friction, float restitution)
    {
        if (!world) return nullptr;
        b2BodyDef def; def.position.Set(x, y);
        b2Body* body = world->CreateBody(&def);
        b2PolygonShape shape; shape.SetAsBox(hx, hy, b2Vec2(0.f, 0.f), angle);
        b2FixtureDef fix; fix.shape = &shape; fix.density = 0.f; fix.friction = friction; fix.restitution = restitution;
        body->CreateFixture(&fix);
        return body;
    }

    FysicsManager* LayerFysics::GetFysicsManager()
    {
        return fysicsManager;
    }

}