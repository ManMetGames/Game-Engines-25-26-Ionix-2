#include "FysicsBody.h"
#include "Architecture/Application.h"
#include "FysicsManager.h"

namespace IonixEngine
{
    FysicsBody::FysicsBody(Entity* entity, std::string alias, b2World* world) : Component(entity, true, true, false)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(0, 10);
        bodyDef.awake = true;
        bodyDef.fixedRotation = false;

        //Get fysics manager
        FysicsManager& fysics_manager = *Application::Get().layerFysics->GetFysicsManager();
        b2Body* body = fysics_manager.GetWorld()->CreateBody(&bodyDef);
        body->SetTransform(b2Vec2(entity->position.x / 100, entity->position.y / 100), entity->rotation);
        body->GetUserData().pointer = (uintptr_t)(entity);
        fysics_manager.GetBodyMap()[body] = entity;
    }

    /*FysicsBody::FysicsBody(Entity* entity, std::string alias, b2World* world, float xPos, float yPos, fysicsBodyType b_type, bool rotationLocked) : Component(entity, true, true, false)
    {
        if (!world) world = nullptr;
        b2BodyDef bodyDef;

        switch (b_type)
        {
            case fysicsBodyType::staticBody: bodyDef.type = b2_staticBody; break;
            case fysicsBodyType::dynamicBody: bodyDef.type = b2_dynamicBody; break;
            case fysicsBodyType::kinematicBody: bodyDef.type = b2_kinematicBody; break;
        }

        bodyDef.position.Set(xPos, yPos);
        bodyDef.awake = true;
        bodyDef.fixedRotation = rotationLocked;
        body = world->CreateBody(&bodyDef);
    }

    FysicsBody::FysicsBody(Entity* entity, std::string alias, b2World* world, float xPos, float yPos, fysicsBodyType b_type, bool rotationLocked, float gravityScale) : Component(entity, true, true, false)
    {
        if (!world) world = nullptr;
        b2BodyDef bodyDef;

        switch (b_type)
        {
            case fysicsBodyType::staticBody: bodyDef.type = b2_staticBody; break;
            case fysicsBodyType::dynamicBody: bodyDef.type = b2_dynamicBody; break;
            case fysicsBodyType::kinematicBody: bodyDef.type = b2_kinematicBody; break;
        }

        bodyDef.position.Set(xPos, yPos);
        bodyDef.awake = true;
        bodyDef.fixedRotation = rotationLocked;
        bodyDef.gravityScale = gravityScale;
        body = world->CreateBody(&bodyDef);
    }*/

    b2Body* FysicsBody::GetBody() { return body; }

    b2Vec2 FysicsBody::GetPosition() const
    {
        return body ? body->GetPosition() : b2Vec2(0.0f, 0.0f);
    }

    void FysicsBody::SetPosition(float x, float y)
    {
        if (body)
        {
            b2Vec2 newPos(x, y);
            body->SetTransform(newPos, body->GetAngle());
        }
    }

    float FysicsBody::GetAngle() const
    {
        return body ? body->GetAngle() : 0.0f;
    }

    void FysicsBody::SetAngle(float angleInRadians)
    {
        if (body)
        {
            b2Vec2 currentPos = body->GetPosition();
            body->SetTransform(currentPos, angleInRadians);
        }
    }

    b2Vec2 FysicsBody::GetLinearVelocity() const
    {
        return body ? body->GetLinearVelocity() : b2Vec2(0.0f, 0.0f);
    }

    void FysicsBody::SetLinearVelocity(float x, float y)
    {
        if (body)
        {
            b2Vec2 newVelocity(x, y);
            body->SetLinearVelocity(newVelocity);
        }
    }

    float FysicsBody::GetAngularVelocity() const
    {
        return body ? body->GetAngularVelocity() : 0.0f;
    }

    void FysicsBody::SetAngularVelocity(float x)
    {
        if (body) body->SetAngularVelocity(x);
    }

    void FysicsBody::SetAwake(bool flag)
    {
        if (body) body->SetAwake(flag);
    }

    bool FysicsBody::GetAwake()
    {
        return body ? body->IsAwake() : false;
    }

    void FysicsBody::RotatePosition(float angle)
    {
        if (body)
        {
            float currentAngle = body->GetAngle();
            float newAngle = currentAngle + angle;
            body->SetTransform(body->GetPosition(), newAngle);
        }
    }

    float FysicsBody::GetLinearDamping() { return body->GetLinearDamping(); }

    void FysicsBody::SetLinearDamping(float linearDamping)
    {
        body->SetLinearDamping(linearDamping);
    }

    float FysicsBody::GetAngularDamping() { return body->GetAngularDamping(); }

    void FysicsBody::SetAngularDamping(float angularDamping)
    {
        body->SetAngularDamping(angularDamping);
    }

    bool FysicsBody::GetAllowSleep() { return body->IsSleepingAllowed(); }

    void FysicsBody::SetAllowSleep(bool flag)
    {
        body->SetSleepingAllowed(flag);
    }

    bool FysicsBody::GetFixedRotation() { return body->IsFixedRotation(); }

    void FysicsBody::SetFixedRotation(bool flag)
    {
        body->SetFixedRotation(flag);
    }

    bool FysicsBody::GetIsBullet() { return body->IsBullet(); }

    void FysicsBody::SetIsBullet(bool flag)
    {
        body->SetBullet(flag);
    }

    float FysicsBody::GetGravityScale() { return body->GetGravityScale(); }

    void FysicsBody::SetGravityScale(float gravityScale)
    {
        body->SetGravityScale(gravityScale);
    }
}