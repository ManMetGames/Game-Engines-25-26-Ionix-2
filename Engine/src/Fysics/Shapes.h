#pragma once
#include <box2d.h>
#include <vector>
#include <sol/sol.hpp>

namespace IonixEngine
{
    enum class fysicShapeType{circle, box, polygon};

    class FysicsShapes
    {
        private:
            b2Fixture* fixture;
            b2Body* body;

        public:
            
            FysicsShapes()
            {
            }

            ~FysicsShapes()
            {
                if (body && fixture)
                {
                    body->DestroyFixture(fixture);
                    fixture = nullptr;
                }
            }

            void AttatchBody(b2Body* attachedBody)
            {
                body = attachedBody;
            }

            //Add Circle
            void AddCircle(float radius, b2Vec2 offset = { 0.0f, 0.0f }, bool isTrigger = false)
            {
                b2CircleShape shape;

                shape.m_radius = 5;
                shape.m_p = offset;
                b2FixtureDef fixtureDef;

                fixtureDef.shape = &shape;
                fixtureDef.isSensor = isTrigger;
                fixture = body->CreateFixture(&fixtureDef);
                
            }


            //Add Box
            void AddBox()
            {
                b2PolygonShape shape;
                shape.SetAsBox(0.5f, 0.5f, b2Vec2_zero, 0.0f);

                b2FixtureDef fixtureDef;

                fixtureDef.shape = &shape;
                fixtureDef.isSensor = true;
                fixtureDef.density = 1.0f;

                fixture = body->CreateFixture(&fixtureDef);
            }

            void AddBox(float width, float height, b2Vec2 offset = { 0.0f, 0.0f }, float angle = 0.0f, bool isTrigger = false)
            {
                b2PolygonShape shape;
                shape.SetAsBox(width * 0.5f, height * 0.5f, offset, angle); 

                b2FixtureDef fixtureDef;

                fixtureDef.shape = &shape;
                fixtureDef.isSensor = isTrigger;
                fixtureDef.density = 1.0f;

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

            b2Fixture* GetFixture()
            {
                return fixture;
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

