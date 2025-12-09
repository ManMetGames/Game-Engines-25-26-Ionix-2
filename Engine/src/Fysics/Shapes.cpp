#include "Shapes.h"
#include "Architecture/ECS/Entity.hpp"
#include "Fysics/FysicsManager.h"

namespace IonixEngine {

    


    
    //add circle
    void FysicsShapes::AddCircle(Entity* entity, float radius, b2Vec2 offset, bool isTrigger) {
        b2CircleShape circle;
        
        body = FysicsManager::GetManager()->GetBodyFromEntity(entity);
        circle.m_radius = radius;
        circle.m_p = offset;
        b2FixtureDef fixtureDef;

        fixtureDef.shape = &circle;
        fixtureDef.isSensor = isTrigger;
        fixtureDef.density = 1.0f;
        fixture = body->CreateFixture(&fixtureDef);
    }

    //add box
    void FysicsShapes::AddBox(Entity* entity,
        b2Vec2 size,   
        b2Vec2 offset,
        float angle,
        bool isTrigger)
    {
        body = FysicsManager::GetManager()->GetBodyFromEntity(entity);
        b2PolygonShape shape;

        b2Vec2 halfSize(size.x * 0.5f, size.y * 0.5f);

        shape.SetAsBox(halfSize.x, halfSize.y, offset, angle);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.isSensor = isTrigger;
        fixtureDef.density = 1.0f;

        fixture = body->CreateFixture(&fixtureDef);        
    }

    void FysicsShapes::AddSpriteCollider(Entity* entity, bool isTrigger, float scaleFactor)
    {
        SpriteComponent* sprite_component = nullptr;
        if (!entity->TryGetComponent<SpriteComponent>(&sprite_component)){return;}
        
        body = FysicsManager::GetManager()->GetBodyFromEntity(entity);
        b2PolygonShape shape;
        
        float xScale = sprite_component->getWidth() / 100.0f;
        float yScale = sprite_component->getHeight() / 100.0f;
        b2Vec2 size;
        size.x = xScale;
        size.y = yScale;
        b2Vec2 halfSize((size.x * 0.5f) * scaleFactor, (size.y * 0.5f) * scaleFactor);

        b2Vec2 offset;
        offset.x = xScale / 2;
        offset.y = yScale / 2;

        shape.SetAsBox(halfSize.x, halfSize.y, offset, angle);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.isSensor = isTrigger;
        fixtureDef.density = 1.0f;

        fixture = body->CreateFixture(&fixtureDef);        
    }

    //add polygon
    void FysicsShapes::AddPolygon(Entity* entity, float tileSize, std::vector<b2Vec2>& vertices) {
        body = FysicsManager::GetManager()->GetBodyFromEntity(entity);

        if (vertices.size() < 3 || vertices.size() > b2_maxPolygonVertices)
            return;

        b2PolygonShape shape;
        tileSize /= 100.0f;

        //Set the polygon shape with the vertices data and their total count
        for (int i = 0; i < vertices.size(); i++)
        {
            vertices[i] = tileSize * vertices[i];
        }
        shape.Set(vertices.data(), static_cast<int32>(vertices.size()));

        b2FixtureDef fixtureDef;

        fixtureDef.shape = &shape;
        fixtureDef.isSensor = false;

        //Set the body fixture for the created shape with polygons
        if (fixture) {
            body->DestroyFixture(fixture);
        }
        fixture = body->CreateFixture(&fixtureDef);      
    }



}