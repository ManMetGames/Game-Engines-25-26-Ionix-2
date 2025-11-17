#include "Shapes.h"
#include "Architecture/ECS/Entity.hpp"
#include "Fysics/FysicsManager.h"

namespace IonixEngine {
    //add circle
    void FysicsShapes::AddCircle(Entity* entity, float radius, b2Vec2 offset, bool isTrigger) {
        b2CircleShape shape;

        shape.m_radius = radius;
        shape.m_p = offset;
        b2FixtureDef fixtureDef;

        fixtureDef.shape = &shape;
        fixtureDef.isSensor = isTrigger;

        if (fixture) {
            body->DestroyFixture(fixture);
        }
        fixture = FysicsManager::GetManager()->GetBodyFromEntity(entity)->CreateFixture(&fixtureDef);
    }

    //add box
    void FysicsShapes::AddBox(Entity* entity, b2Vec2 size, b2Vec2 offset, float angle, bool isTrigger)
    {
        b2PolygonShape shape;
        shape.SetAsBox(offset.x, offset.y, size, angle);

        b2FixtureDef fixtureDef;

        fixtureDef.shape = &shape;
        fixtureDef.isSensor = isTrigger;
        fixtureDef.density = 1.0f;

        if (fixture) {
            body->DestroyFixture(fixture);
        }
        fixture = FysicsManager::GetManager()->GetBodyFromEntity(entity)->CreateFixture(&fixtureDef);
    }


    void FysicsShapes::AddPolygon(Entity* entity)
    {
        static std::vector<b2Vec2> defaultVertices = { {0, 1}, { 0.6, 0.6 }, { 0.3, -0.5f }, { -0.3, -0.5f}, {-0.6, 0.6f } };
        AddPolygon(entity, defaultVertices);
    }

    //add polygon
    void FysicsShapes::AddPolygon(Entity* entity, std::vector<b2Vec2>& vertices) {
        if (vertices.size() < 3 || vertices.size() > b2_maxPolygonVertices)
            return;

        b2PolygonShape shape;

        //Set the polygon shape with the vertices data and their total count
        shape.Set(vertices.data(), static_cast<int32>(vertices.size()));

        b2FixtureDef fixtureDef;

        fixtureDef.shape = &shape;
        fixtureDef.isSensor = false;

        //Set the body fixture for the created shape with polygons
        if (fixture) {
            body->DestroyFixture(fixture);
        }
        fixture = FysicsManager::GetManager()->GetBodyFromEntity(entity)->CreateFixture(&fixtureDef);
    }



}