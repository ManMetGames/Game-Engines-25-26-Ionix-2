#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicsManager.h"

#include <iostream>
#include <ostream>

#include "EventSystem/Event.h"

namespace IonixEngine
{
    LayerFysics* LayerFysics::instance = nullptr;

    LayerFysics* LayerFysics::GetInstance() {
        return instance;
    }

    b2World* LayerFysics::GetWorld() {
        return fysicsManager ? fysicsManager->GetWorld() : nullptr;
    }

    void LayerFysics::OnAttach() // call first says archie or brok
    {
        instance = this;
        fysicsManager = new FysicsManager();

        //create default ground box
        b2World* world = fysicsManager->GetWorld();
        b2BodyDef groundDef; groundDef.position.Set(0.f, -1.f);
        b2Body* ground = world->CreateBody(&groundDef);
        b2PolygonShape g; g.SetAsBox(50.f, 1.f);
        ground->CreateFixture(&g, 0.f);
    }
    
    void LayerFysics::OnDetach() 
    {
        instance = nullptr;
        delete fysicsManager;
        fysicsManager = nullptr;
    }
    
    void LayerFysics::OnUpdate()
    {
        if (!fysicsManager) return;
        
        b2World* world = fysicsManager->GetWorld();
        if (!world) return;
        
        world->Step(timeStep, velocityIterations, positionIterations);
        
        // Sync physics transforms to entities
        auto& bodyMap = fysicsManager->GetBodyMap();
        for (auto& [body, entity] : bodyMap)
        {
            if (!body->IsAwake()) continue;
            
            b2Vec2 pos = body->GetPosition();
            entity->position.x = pos.x * ppm;
            entity->position.y = pos.y * ppm;
            entity->rotation = body->GetAngle();
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
        if (!fysicsManager) return nullptr;
        b2World* world = fysicsManager->GetWorld();
        if (!world) return nullptr;
        
        b2BodyDef def; def.position.Set(x, y);
        b2Body* body = world->CreateBody(&def);
        b2PolygonShape shape; shape.SetAsBox(hx, hy, b2Vec2(0.f, 0.f), angle);
        b2FixtureDef fix; fix.shape = &shape; fix.density = 0.f; fix.friction = friction; fix.restitution = restitution;
        body->CreateFixture(&fix);
        return body;
    }
}