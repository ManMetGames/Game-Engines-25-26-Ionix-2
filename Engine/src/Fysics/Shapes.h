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
                AddBox(entity, b2Vec2 { width, height }, offset, angle, isTrigger, 0x0001, 0xFFFF);
                break;
            case fysicShapeType::polygon:
                AddPolygon(entity, vertices);
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
        void AddBox(Entity* entity,
        b2Vec2 size,   
        b2Vec2 offset,
        float angle,
        bool isTrigger,
        uint16 categoryBits,
        uint16 maskBits);
        void AddSpriteCollider(Entity* entity, bool isTrigger, float scaleFactor = 1.0f);//This will add a box collider of the correct size and position as long as the entity also has a sprite component
        void AddPolygon(Entity* entity, std::vector<b2Vec2>& vertices);

       
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

