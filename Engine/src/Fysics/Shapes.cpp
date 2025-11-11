#include "Shapes.h"
#include "Architecture/ECS/Entity.hpp"
#include "Fysics/FysicsManager.h"

namespace IonixEngine {
    void FysicsShapes::AddCircle(Entity* entity, float radius, b2Vec2 offset, bool isTrigger) {
        b2CircleShape shape;

        shape.m_radius = radius;
        shape.m_p = offset;
        b2FixtureDef fixtureDef;

        fixtureDef.shape = &shape;
        fixtureDef.isSensor = isTrigger;

        if (fixture != nullptr) {
            body->DestroyFixture(fixture);
        }
        fixture = FysicsManager::GetManager()->GetBodyFromEntity(entity)->CreateFixture(&fixtureDef);
    }
}