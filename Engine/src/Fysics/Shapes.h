#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

#include <sol/sol.hpp>



namespace IonixEngine
{
    enum class fysicShapeType{circle, box, polygon};

    class FysicsShapes
    {
        private:
            b2World* world;
            b2Fixture* fixture;
            b2Body* body;

        public:
            //world = LayerFysics::GetInstance()->GetWorld();
            FysicsShapes(b2Body* attachedBody)
            {
                body = attachedBody;
                fixture = nullptr;
            }

            ~FysicsShapes()
            {
                if (body && fixture)
                {
                    body->DestroyFixture(fixture);
                    fixture = nullptr;
                }
            }

            void AddCircle(float radius, b2Vec2 offset = { 0.0f, 0.0f }, bool isTrigger = false)
            {
                b2CircleShape shape;

                shape.m_radius = radius;
                shape.m_p = offset;

                b2FixtureDef fixtureDef;

                fixtureDef.shape = &shape;
                fixtureDef.isSensor = isTrigger;

                fixture = body->CreateFixture(&fixtureDef);
            }
    };

    
}

