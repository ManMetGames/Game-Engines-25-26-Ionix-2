#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

#include <sol/sol.hpp>

namespace IonixEngine
{
    class Entity;
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
        std::vector<b2Vec2> vertices ;


        void SelectCorrectShape(Entity* entity, fysicShapeType shapeType)
        {
            switch (shapeType)
            {
            case fysicShapeType::circle:
                AddCircle(entity, radius, offset, isTrigger);
                break;
            case fysicShapeType::box:
                AddBox(entity, b2Vec2 { width, height }, offset, angle, isTrigger);
                break;
            case fysicShapeType::polygon:
                AddPolygon(entity);
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

        void AddCircle(Entity* entity, float radius = 1, b2Vec2 offset = { 0.0f, 0.0f }, bool isTrigger = false);


        // Box -------------------------------------------------------
        void AddBox(Entity* entity, b2Vec2 size = { 1, 1 }, b2Vec2 offset = { 0.0f, 0.0f }, float angle = 0.0f, bool isTrigger = false);
        /*void AddBox(Entity* entity, b2Vec2 size = { 1, 1 }, b2Vec2 offset = {0.0f, 0.0f}, float angle = 0.0f, bool isTrigger = false)
        {
            b2PolygonShape shape;
            shape.SetAsBox(offset.x, offset.y, size, angle);

            b2FixtureDef fixtureDef;

            fixtureDef.shape = &shape;
            fixtureDef.isSensor = isTrigger;
            fixtureDef.density = 1.0f;

            if (fixture != nullptr) {
                body->DestroyFixture(fixture);
            }
            fixture = body->CreateFixture(&fixtureDef);
        }*/


        void AddPolygon(Entity* entity);
        //Add Polygon ---------------------------------------------------------
        void AddPolygon(Entity* entity, std::vector<b2Vec2>& vertices);

        //void AddPolygon(std::vector<b2Vec2>& vertices) {
        //    if (vertices.size() < 3 || vertices.size() > b2_maxPolygonVertices)
        //        return;

        //    b2PolygonShape shape;

        //    //Set the polygon shape with the vertices data and their total count
        //    shape.Set(vertices.data(), static_cast<int32>(vertices.size()));

        //    b2FixtureDef fixtureDef;

        //    fixtureDef.shape = &shape;
        //    fixtureDef.isSensor = false;

        //    //Set the body fixture for the created shape with polygons
        //    if (fixture != nullptr) {
        //        body->DestroyFixture(fixture);
        //    }
        //    fixture = body->CreateFixture(&fixtureDef);
        //}

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

        void SetRadius(Entity* entity, float r, b2Body* bodyToChange, fysicShapeType shapeType)
        {
            /*b2Fixture* tempFix;
            tempFix = &bodyToChange->GetFixtureList()[0];
            tempFix->GetShape()->GetType();*/
            radius = r;

            SelectCorrectShape(entity, shapeType);
        }

        float GetWidth()
        {
            return width;
        }

        void SetWidth(float w, fysicShapeType shapeType)
        {
            width = w;
            SelectCorrectShape(nullptr, shapeType);
        }

        float GetHeight()
        {
            return height;
        }

        void SetHeight(float h, fysicShapeType shapeType)
        {
            height = h;
            SelectCorrectShape(nullptr, shapeType);
        }

        float GetAngle()
        {
            return angle;
        }

        void SetAngle(float a, fysicShapeType shapeType)
        {
            angle = a;
            SelectCorrectShape(nullptr, shapeType);
        }

        b2Vec2 GetOffset()
        {
            return offset;
        }

        void SetOffset(b2Vec2& off, fysicShapeType shapeType)
        {
            offset = off;
            SelectCorrectShape(nullptr, shapeType);
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
            SelectCorrectShape(nullptr, shapeType);
        }



    };


}

