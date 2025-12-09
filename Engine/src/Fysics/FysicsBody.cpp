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
        //bodyDef.position.Set(entity->transform.GetLocalPosition().x, entity->transform.GetLocalPosition().y);
        bodyDef.awake = true;
        bodyDef.fixedRotation = false;

        //Get fysics manager
        FysicsManager& fysics_manager = *Application::Get().layerFysics->GetFysicsManager();
        b2Body* body = fysics_manager.GetWorld()->CreateBody(&bodyDef);
        Vec2 pos = entity->transform.GetGlobalPosition();
        body->SetTransform(b2Vec2(pos.x / 100, pos.y / 100), entity->transform.GetGlobalRotation());
        body->GetUserData().pointer = (uintptr_t)(entity);
        fysics_manager.GetBodyMap()[body] = entity;

        // Important that we keep reference to the bodies that are created via this data structure
        Application::Get().layerFysics->GetFysicsManager()->AddEntityBodyPair(entity, body);
    }

    FysicsBody::FysicsBody(Entity* entity, int bodyType, bool rotationLocked) : Component(entity, true, true, false)
    {
        b2BodyDef bodyDef;

        switch (bodyType)
        {
            case 0: bodyDef.type = b2_staticBody; break;
            case 1: bodyDef.type = b2_kinematicBody; break;
            case 2: bodyDef.type = b2_dynamicBody; break;
        }

        bodyDef.awake = true;
        bodyDef.fixedRotation = rotationLocked;
        //bodyDef.position.x = entity->position.x / 100;
        //bodyDef.position.y = entity->position.y / 100;
        bodyDef.position = b2Vec2(entity->transform.GetLocalPosition().x / 100, entity->transform.GetLocalPosition().y / 100);

        body = Application::Get().layerFysics->GetFysicsManager()->GetWorld()->CreateBody(&bodyDef);


        // Important that we keep reference to the bodies that are created via this data structure
        Application::Get().layerFysics->GetFysicsManager()->AddEntityBodyPair(entity, body);
    }
    /*

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

    b2Vec2 FysicsBody::GetPosition(Entity* entity) const
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        b2Vec2 scaledPos = b2Vec2(body->GetPosition().x * 100, body->GetPosition().y * 100);
        return body ? scaledPos : b2Vec2(0.0f, 0.0f);
    }

    void FysicsBody::SetPosition(Entity* entity, float x, float y)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        if (body)
        {
            b2Vec2 newPos(x, y);
            body->SetTransform(newPos, body->GetAngle());
        }
    }

    float FysicsBody::GetAngle(Entity* entity) const
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        return body ? body->GetAngle() : 0.0f;
    }

    void FysicsBody::SetAngle(Entity* entity, float angleInRadians)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        if (body)
        {
            b2Vec2 currentPos = body->GetPosition();
            body->SetTransform(currentPos, angleInRadians);
        }
    }

    b2Vec2 FysicsBody::GetLinearVelocity(Entity* entity) const
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        return body ? body->GetLinearVelocity() : b2Vec2(0.0f, 0.0f);
    }

    void FysicsBody::SetLinearVelocity(Entity* entity, float x, float y)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        if (body)
        {
            b2Vec2 newVelocity(x, y);
            body->SetLinearVelocity(newVelocity);
        }
    }

    float FysicsBody::GetAngularVelocity(Entity* entity) const
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        return body ? body->GetAngularVelocity() : 0.0f;
    }

    void FysicsBody::SetAngularVelocity(Entity* entity, float x)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        if (body) body->SetAngularVelocity(x);
    }

    void FysicsBody::SetAwake(Entity* entity, bool flag)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        if (body) body->SetAwake(flag);
    }

    bool FysicsBody::GetAwake(Entity* entity)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        return body ? body->IsAwake() : false;
    }

    void FysicsBody::RotatePosition(Entity* entity, float angle)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        if (body)
        {
            float currentAngle = body->GetAngle();
            float newAngle = currentAngle + angle;
            body->SetTransform(body->GetPosition(), newAngle);
        }
    }

    float FysicsBody::GetLinearDamping(Entity* entity)
    { 
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        return body->GetLinearDamping(); 
    }

    void FysicsBody::SetLinearDamping(Entity* entity, float linearDamping)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        body->SetLinearDamping(linearDamping);
    }

    float FysicsBody::GetAngularDamping(Entity* entity) 
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        return body->GetAngularDamping(); 
    }

    void FysicsBody::SetAngularDamping(Entity* entity, float angularDamping)
    {
        body->SetAngularDamping(angularDamping);
    }

    bool FysicsBody::GetAllowSleep(Entity* entity) 
    { 
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        return body->IsSleepingAllowed();
    }

    void FysicsBody::SetAllowSleep(Entity* entity, bool flag)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        body->SetSleepingAllowed(flag);
    }

    bool FysicsBody::GetFixedRotation(Entity* entity) 
    { 
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        return body->IsFixedRotation(); 
    }

    void FysicsBody::SetFixedRotation(Entity* entity, bool flag)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        body->SetFixedRotation(flag);
    }

    bool FysicsBody::GetIsBullet(Entity* entity)
    { 
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        return body->IsBullet();
    }

    void FysicsBody::SetIsBullet(Entity* entity, bool flag)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        body->SetBullet(flag);
    }

    float FysicsBody::GetGravityScale(Entity* entity)
    { 
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        return body->GetGravityScale(); 
    }

    void FysicsBody::SetGravityScale(Entity* entity, float gravityScale)
    {
        b2Body* body = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entity);
        body->SetGravityScale(gravityScale);
    }

    void FysicsBody::Update(float deltaTime)
    {
        b2Vec2 pos = body->GetPosition();
        float rot = body->GetAngle();
        entity->transform.SetLocalPosition(Vec2{pos.x * 100, pos.y * 100});
        entity->transform.SetLocalRotation(rot);
    }
}
