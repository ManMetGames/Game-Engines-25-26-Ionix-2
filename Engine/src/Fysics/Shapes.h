#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

#include <sol/sol.hpp>



namespace IonixEngine
{
    enum class fysicShapeType { circle, box, polygon, none };

    class FysicsShapes
    {
    private:
        /*b2Fixture* fixture;
        b2Body* body;*/
        b2Fixture* fixture = nullptr;
        b2Body* body = nullptr;

        fysicShapeType currentShapeType = fysicShapeType::none;
        float radius = 0.5f;
        float width = 1.0f;
        float height = 1.0f;
        float angle = 0.0f;
        bool isTrigger = false;
        b2Vec2 offset = { 0.0f, 0.0f };
        std::vector<b2Vec2> vertices;


        void SelectCorrectShape(fysicShapeType shapeType)
        {
            switch (shapeType)
            {
            case fysicShapeType::circle:
                AddCircle(radius, offset, isTrigger);
                break;
            case fysicShapeType::box:
                AddBox(width, height, offset, angle, isTrigger);
                break;
            case fysicShapeType::polygon:
                AddPolygon(vertices);
                break;
            default:
                break;
            }
        }

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



        // Body ------------------------------------------------------

        void AttatchBody(b2Body* attachedBody)
        {
            body = attachedBody;
        }

        b2Body* GetBody()
        {
            return body;
        }

        void SetBody(b2Body* newBody)
        {
            body = newBody;
        }

        b2Fixture* GetFixture()
        {
            return fixture;
        }

        void SetFixture(b2Fixture* newFixture)
        {
            fixture = newFixture;
        }


        // Circle ----------------------------------------------------


    //DEFAULT CONSTRUCTOR FOR REFERENCE
        void AddCircle()
        {
            b2CircleShape shape;

            shape.m_radius = radius;
            shape.m_p = offset;
            b2FixtureDef fixtureDef;

            fixtureDef.shape = &shape;
            fixtureDef.isSensor = isTrigger;

            if (fixture != nullptr) {
                body->DestroyFixture(fixture);
            }
            fixture = body->CreateFixture(&fixtureDef);
        }

        void AddCircle(float radius, b2Vec2 offset = { 0.0f, 0.0f }, bool isTrigger = false)
        {
            b2CircleShape shape;

            shape.m_radius = radius;
            shape.m_p = offset;
            b2FixtureDef fixtureDef;

            fixtureDef.shape = &shape;
            fixtureDef.isSensor = isTrigger;

            if (fixture != nullptr) {
                body->DestroyFixture(fixture);
            }
            fixture = body->CreateFixture(&fixtureDef);


        }


        // Box -------------------------------------------------------
        void AddBox()
        {
            b2PolygonShape shape;
            shape.SetAsBox(0.5f, 0.5f, b2Vec2_zero, 0.0f);

            b2FixtureDef fixtureDef;

            fixtureDef.shape = &shape;
            fixtureDef.isSensor = true;
            fixtureDef.density = 1.0f;

            if (fixture != nullptr) {
                body->DestroyFixture(fixture);
            }
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

            if (fixture != nullptr) {
                body->DestroyFixture(fixture);
            }
            fixture = body->CreateFixture(&fixtureDef);
        }


        //Add Polygon ---------------------------------------------------------

        void AddPolygon(std::vector<b2Vec2>& vertices) {
            if (vertices.size() < 3 || vertices.size() > b2_maxPolygonVertices)
                return;

            b2PolygonShape shape;

            //Set the polygon shape with the vertices data and their total count
            shape.Set(vertices.data(), static_cast<int32>(vertices.size()));

            b2FixtureDef fixtureDef;

            fixtureDef.shape = &shape;
            fixtureDef.isSensor = false;

            //Set the body fixture for the created shape with polygons
            if (fixture != nullptr) {
                body->DestroyFixture(fixture);
            }
            fixture = body->CreateFixture(&fixtureDef);
        }

        /*b2Fixture* GetFixture()
        {
            return fixture;
        }*/
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

        // Getters and Setters ----------------------------------------------------------------

        float GetRadius()
        {
            return radius;
        }

        void SetRadius(float r, b2Body* bodyToChange, fysicShapeType shapeType)
        {
            /*b2Fixture* tempFix;
            tempFix = &bodyToChange->GetFixtureList()[0];
            tempFix->GetShape()->GetType();*/
            radius = r;

            SelectCorrectShape(shapeType);
        }

        float GetWidth()
        {
            return width;
        }

        void SetWidth(float w, fysicShapeType shapeType)
        {
            width = w;
            SelectCorrectShape(shapeType);
        }

        float GetHeight()
        {
            return height;
        }

        void SetHeight(float h, fysicShapeType shapeType)
        {
            height = h;
            SelectCorrectShape(shapeType);
        }

        float GetAngle()
        {
            return angle;
        }

        void SetAngle(float a, fysicShapeType shapeType)
        {
            angle = a;
            SelectCorrectShape(shapeType);
        }

        b2Vec2 GetOffset()
        {
            return offset;
        }

        void SetOffset(b2Vec2& off, fysicShapeType shapeType)
        {
            offset = off;
            SelectCorrectShape(shapeType);
        }

        bool IsShapeTrigger()
        {
            return isTrigger;
        }

        void SetShapeTrigger(bool trigger)
        {
            isTrigger = trigger;

            if (fixture)
                fixture->SetSensor(trigger);
        }

        fysicShapeType GetShapeType()
        {
            return currentShapeType;
        }

        std::vector<b2Vec2> GetVertices()
        {
            return vertices;
        }

        void SetVertices(std::vector<b2Vec2>& verts, fysicShapeType shapeType)
        {
            vertices = verts;
            SelectCorrectShape(shapeType);
        }



    };


}

