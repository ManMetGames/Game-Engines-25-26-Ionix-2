
#pragma once
#include <sol/sol.hpp>
#include "Architecture/ECS/Component.hpp"
#include "LayerSystem/Layers/LayerFysics.h"
#include "Fysics/Shapes.h"

namespace IonixEngine
{
    enum class fysicsBodyType { staticBody, dynamicBody, kinematicBody };

    class FysicsBody : public Component
    {
    private:
        b2Body* body;

    public:
        // Constructors
        FysicsBody(Entity* entity, std::string alias, b2World* world);
        FysicsBody(Entity* entity, int b_type, bool rotationLocked);

        //FysicsBody(Entity* entity, std::string alias, b2World* world, float xPos, float yPos, fysicsBodyType b_type, bool rotationLocked, float gravityScale);

        // Body access
        b2Body* GetBody();

        // Position
        b2Vec2 GetPosition(Entity* entity) const;
        void SetPosition(Entity* entity, float x, float y);

        // Angle
        float GetAngle(Entity* entity) const;
        void SetAngle(Entity* entity, float angleInRadians);

        // Velocity
        b2Vec2 GetLinearVelocity(Entity* entity) const;
        void SetLinearVelocity(Entity* entity, float x, float y);
        float GetAngularVelocity(Entity* entity) const;
        void SetAngularVelocity(Entity* entity, float x);

        // Awake
        void SetAwake(Entity* entity, bool flag);
        bool GetAwake(Entity* entity);

        // Rotation
        void RotatePosition(Entity* entity, float angle);

        // Damping
        float GetLinearDamping(Entity* entity);
        void SetLinearDamping(Entity* entity, float linearDamping);
        float GetAngularDamping(Entity* entity);
        void SetAngularDamping(Entity* entity, float angularDamping);

        // Sleep
        bool GetAllowSleep(Entity* entity);
        void SetAllowSleep(Entity* entity, bool flag);

        // Fixed rotation
        bool GetFixedRotation(Entity* entity);
        void SetFixedRotation(Entity* entity, bool flag);

        // Bullet
        bool GetIsBullet(Entity* entity);
        void SetIsBullet(Entity* entity, bool flag);

        // Gravity
        float GetGravityScale(Entity* entity);
        void SetGravityScale(Entity* entity, float gravityScale);
        //update
        virtual void Update(float deltaTime) override;
    };
}
