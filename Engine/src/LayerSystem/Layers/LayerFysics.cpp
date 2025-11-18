#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/FysicsManager.h"
#include "Architecture/Application.h"
#include "Maf/MafUtils.h"
#include "EventSystem/Event.h"

#include <iostream>

namespace IonixEngine
{
    LayerFysics* LayerFysics::instance = nullptr;

    LayerFysics* LayerFysics::GetInstance() {
        return Application::Get().layerFysics;
    }

    b2World* LayerFysics::GetWorld() {
        return fysicsManager ? fysicsManager->GetWorld() : nullptr;
    }

    //---------------------------------------------------------
    // Helper: Check if a body pointer is valid and alive
    //---------------------------------------------------------
    static bool IsBodyInvalid(b2Body* body)
    {
        if (!body) return true;
        if (body->GetWorld() == nullptr) return true;                  // detached or destroyed
        if (body->GetFixtureList() == nullptr) return true;            // Box2D pruned it
        return false;
    }

    //---------------------------------------------------------
    // Attach
    //---------------------------------------------------------
    void LayerFysics::OnAttach()
    {
        instance = this;
        fysicsManager = new FysicsManager();
    }

    //---------------------------------------------------------
    // Detach
    //---------------------------------------------------------
    void LayerFysics::OnDetach() 
    {
        instance = nullptr;
        delete fysicsManager;
        fysicsManager = nullptr;
    }

    //---------------------------------------------------------
    // Interpolated Render-Update
    //---------------------------------------------------------
    void LayerFysics::OnUpdate()
    {
        if (!fysicsManager) return;

        float alpha = Application::Get().GetPhysicsInterpolationAlpha();

        auto& bodyMap = fysicsManager->GetBodyMap();
        auto& transformMap = fysicsManager->GetTransformMap();

        for (auto it = bodyMap.begin(); it != bodyMap.end();)
        {
            b2Body* body = it->first;
            Entity* entity = it->second;

            // Body is destroyed → remove safely
            if (IsBodyInvalid(body))
            {
                transformMap.erase(body);
                it = bodyMap.erase(it);
                continue;
            }

            // No transform data yet
            if (transformMap.find(body) == transformMap.end())
            {
                ++it;
                continue;
            }

            auto& transform = transformMap[body];

            // Interpolate physics position
            float lx = Maf::mafLerp(transform.previousPosition.x, transform.currentPosition.x, alpha);
            float ly = Maf::mafLerp(transform.previousPosition.y, transform.currentPosition.y, alpha);
            float lr = Maf::mafLerp(transform.previousRotation,  transform.currentRotation,  alpha);

            entity->position.x = lx * ppm;
            entity->position.y = ly * ppm;
            entity->rotation   = lr;

            ++it;
        }
    }

    //---------------------------------------------------------
    // Fixed Physics Update
    //---------------------------------------------------------
    void LayerFysics::OnFixedUpdate()
    {
        if (!fysicsManager) return;

        b2World* world = fysicsManager->GetWorld();
        if (!world) return;

        auto& bodyMap = fysicsManager->GetBodyMap();
        auto& transformMap = fysicsManager->GetTransformMap();

        //-----------------------------------------------------
        // 1. Prepare previous transforms
        //-----------------------------------------------------
        for (auto it = bodyMap.begin(); it != bodyMap.end();)
        {
            b2Body* body = it->first;

            // Remove invalid bodies
            if (IsBodyInvalid(body))
            {
                transformMap.erase(body);
                it = bodyMap.erase(it);
                continue;
            }

            if (transformMap.find(body) == transformMap.end())
            {
                transformMap.emplace(body, 
                    RigidBodyTransform(body->GetPosition(), body->GetAngle()));
            }
            else
            {
                auto& t = transformMap[body];
                t.previousPosition = t.currentPosition;
                t.previousRotation = t.currentRotation;
            }

            ++it;
        }

        //-----------------------------------------------------
        // 2. Step the physics world
        //-----------------------------------------------------
        world->Step(timeStep, velocityIterations, positionIterations);

        //-----------------------------------------------------
        // 3. Update current transforms + reposition entities
        //-----------------------------------------------------
        for (auto it = bodyMap.begin(); it != bodyMap.end();)
        {
            b2Body* body = it->first;
            Entity* entity = it->second;

            // Remove invalid bodies
            if (IsBodyInvalid(body))
            {
                transformMap.erase(body);
                it = bodyMap.erase(it);
                continue;
            }

            // Convert meters → pixels
            float px = body->GetPosition().x * ppm;
            float py = body->GetPosition().y * ppm;

            entity->position.x = px;
            entity->position.y = py;
            entity->rotation   = body->GetAngle();

            // Update interpolation state
            if (transformMap.find(body) != transformMap.end())
            {
                auto& t = transformMap[body];
                t.currentPosition = body->GetPosition();
                t.currentRotation = body->GetAngle();
            }

            ++it;
        }
    }

    //---------------------------------------------------------
    // Events (unused here)
    //---------------------------------------------------------
    void LayerFysics::OnEvent(IonixEvent& e)
    {
        // nothing here yet
    }

    //---------------------------------------------------------
    // Create Ground Helper
    //---------------------------------------------------------
    b2Body* LayerFysics::CreateGroundBox(float x, float y, float hx, float hy, 
                                         float angle, float friction, float restitution)
    {
        if (!fysicsManager) return nullptr;

        b2World* world = fysicsManager->GetWorld();
        if (!world) return nullptr;

        b2BodyDef def; 
        def.position.Set(x, y);

        b2Body* body = world->CreateBody(&def);

        b2PolygonShape shape;
        shape.SetAsBox(hx, hy, b2Vec2(0.f, 0.f), angle);

        b2FixtureDef fix;
        fix.shape      = &shape;
        fix.density    = 0.f;
        fix.friction   = friction;
        fix.restitution = restitution;

        body->CreateFixture(&fix);
        return body;
    }
}
