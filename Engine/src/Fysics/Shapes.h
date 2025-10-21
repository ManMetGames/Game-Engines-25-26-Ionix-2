#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

#include <sol/sol.hpp>



namespace IonixEngine
{
    enum class fysicShapeType{circle, box, polygon};

    class FysicsShapes
    {
        private:
            //b2World* world;
            b2Fixture* fixture;
            //b2Body* body;

        public:
            
            FysicsShapes()
            {
                //world = LayerFysics::GetInstance()->GetWorld();
                //body = attachedBody;
                b2FixtureDef fixtureDef;
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

            //Add Circle
            void AddCircle(float radius, b2Vec2 offset = { 0.0f, 0.0f }, bool isTrigger = false)
            {
                b2CircleShape shape;

                shape.m_radius = radius;
                shape.m_p = offset;
                
                fixtureDef.shape = &shape;
                fixtureDef.isSensor = isTrigger;
            }


            //Add Box
            void AddBox(float width, float height, b2Vec2 offset = { 0.0f, 0.0f }, float angle = 0.0f, bool isTrigger = false)
            {
                b2PolygonShape shape;
                shape.SetAsBox(width * 0.5f, height * 0.5f, offset, angle); 

                b2FixtureDef fixtureDef;

                fixtureDef.shape = &shape;
                fixtureDef.isSensor = isTrigger;

                fixture = body->CreateFixture(&fixtureDef);
            }


            //Add Polygon
            void AddPolygon(const std::vector<b2Vec2>& vertices, bool isTrigger = false){
                if (vertices.size() < 3 || vertices.size() > b2_maxPolygonVertices)
                    return;

                b2PolygonShape shape;

                //Set the polygon shape with the vertices data and their total count
                shape.Set(vertices.data(), static_cast<int32>(vertices.size()));

                b2FixtureDef fixtureDef;

                fixtureDef.shape = &shape;
                fixtureDef.isSensor = isTrigger;

                //Set the body fixture for the created shape with polygons
                fixture = body->CreateFixture(&fixtureDef);


            }


            void Remove()
            {
                if (body && fixture)
                {
                    body->DestroyFixture(fixture);
                    fixture = nullptr;
                }
            }


            void SetTrigger(bool isTrigger)
            {
                if (fixture)
                {
                    fixture->SetSensor(isTrigger);
                }
            }



            
    };

    
}

