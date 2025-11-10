#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicsManager.h"
#include <iostream>
#include <ostream>
#include "EventSystem/Event.h"
#include "Architecture/ECS/Entity.hpp"

namespace IonixEngine
{
    void LayerFysics::OnAttach()
    {
        fysicsManager = new FysicsManager();
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
        delete fysicsManager; fysicsManager = nullptr;
    }

    LayerFysics* LayerFysics::instance = nullptr;

    LayerFysics* LayerFysics::GetInstance() {
        return instance;
    }
  
    void LayerFysics::OnUpdate()
    {
        if (!fysicsManager->GetWorld()) return; //cheeky early return to see if world is attatched
        fysicsManager->GetWorld()->Step(timeStep, velocityIterations, positionIterations); // world step on update

        std::unordered_map<b2Body*, Entity*> bodyEntityMap = fysicsManager->GetBodyMap();

        for (auto b : bodyEntityMap)
        {
            if (!b.first->IsAwake()) continue; // Skip bodies that are not awake
            Entity * ent = b.second;
            b2Vec2 pos = b.first->GetPosition();

            ent->position.x = pos.x * 100.0f;
            ent->position.y = pos.y * 100.0f;

            float angle = b.first->GetAngle();
            ent->rotation = b.first->GetAngle() * (180.0f / M_PI);
        }
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