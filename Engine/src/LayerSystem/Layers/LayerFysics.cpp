#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicsManager.h"
#include "Architecture/Application.h"
#include "Maf/MafUtils.h"

#include <iostream>
#include <ostream>

#include "EventSystem/Event.h"

namespace IonixEngine
{
    LayerFysics* LayerFysics::instance = nullptr;

    LayerFysics* LayerFysics::GetInstance() {
        return Application::Get().layerFysics;
    }

    b2World* LayerFysics::GetWorld() {
        return fysicsManager ? fysicsManager->GetWorld() : nullptr;
    }

    void LayerFysics::OnAttach() // call first says archie or brok
    {
        instance = this;
        fysicsManager = new FysicsManager();
        //create default ground box
        /*b2World* world = fysicsManager->GetWorld();
        b2BodyDef groundDef; groundDef.position.Set(0.f, -2.f); // roughly 600 pixels down from the top
        b2Body* ground = world->CreateBody(&groundDef);
        b2PolygonShape g; g.SetAsBox(50.f, 1.f);
        ground->CreateFixture(&g, 0.f);*/
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

        
        // get interpolation alpha (0.0 to 1.0) representing progress between physics frames
        float alpha = Application::Get().GetPhysicsInterpolationAlpha();
        
        auto& bodyMap = fysicsManager->GetBodyMap();
        auto& transformMap = fysicsManager->GetTransformMap();
        
        auto& entityBodies = fysicsManager->GetCollisionListener()->entityBodiesToDestroy;
        if (!entityBodies.empty())
        {
            for (b2Body* body : entityBodies)
            {                                   
                fysicsManager->GetWorld()->DestroyBody(body);
            }

            entityBodies.clear();
        }
        


        // interpolate visual positions for all physics bodies
        /*for (auto& [body, entity] : bodyMap)
        {
            // skip if no transform data exists yet
            if (transformMap.find(body) == transformMap.end()) continue;
            
            auto& transform = transformMap[body];
            
            // lerp position between previous and current physics states
            float lerpedX = Maf::mafLerp(transform.previousPosition.x, transform.currentPosition.x, alpha);
            float lerpedY = Maf::mafLerp(transform.previousPosition.y, transform.currentPosition.y, alpha);
            
            // lerp rotation
            float lerpedRotation = Maf::mafLerp(transform.previousRotation, transform.currentRotation, alpha);
            
            // apply interpolated values to entity (convert from meters to pixels)
            entity->position.x = lerpedX * ppm;
            entity->position.y = lerpedY * ppm;
            entity->rotation = lerpedRotation;
        }*/


    }
    
    void LayerFysics::OnFixedUpdate()
    {
        if (!fysicsManager) return;
        
        b2World* world = fysicsManager->GetWorld();
        if (!world) return;
        
        auto& bodyMap = fysicsManager->GetBodyMap();
        auto& transformMap = fysicsManager->GetTransformMap();

        fysicsManager->GetWorld()->Step(timeStep, velocityIterations, positionIterations);
        //fysicsManager->GetWorld()->DebugDraw();

        

        // AFTER physics step, update current visual state
        for (auto& val : bodyMap)
        {
            Vec2 pos;
            pos.x = val.first->GetPosition().x * ppm;
            pos.y = val.first->GetPosition().y * ppm;

            val.second->transform.SetLocalPosition(pos);
            //val.second->position.x = pos.x;
            //val.second->position.y = pos.y;
        }

        // before physics step, save current state as previous
        // BEFORE physics step, save current state as previous
        for (auto& [body, entity] : bodyMap)
        {
            if (transformMap.find(body) == transformMap.end())
            {
                // first time seeing this body - initialize with current state
                transformMap[body] = RigidBodyTransform(body->GetPosition(), body->GetAngle());
            }
            else
            {
                // move current to previous
                auto& transform = transformMap[body];
                transform.previousPosition = transform.currentPosition;
                transform.previousRotation = transform.currentRotation;
            }
        }
        
        // step physics simulation at fixed timestep
        //world->Step(timeStep, velocityIterations, positionIterations);
        //
        //// AFTER physics step, update current state
        for (auto& val : bodyMap)
        {
           Vec2 pos;
           pos.x = val.first->GetPosition().x * ppm;
           pos.y = val.first->GetPosition().y * ppm;

           val.second->transform.SetLocalPosition(pos);
            //val.second->position.x = pos.x;
            //val.second->position.y = pos.y;
            
            // update current transform state for interpolation
            if (transformMap.find(val.first) != transformMap.end())
            {
                auto& transform = transformMap[val.first];
                transform.currentPosition = val.first->GetPosition();
                transform.currentRotation = val.first->GetAngle();
            }

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