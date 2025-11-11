
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
        /*FysicsBody(Entity* entity, std::string alias, b2World* world, float xPos, float yPos, fysicsBodyType b_type, bool rotationLocked);
        FysicsBody(Entity* entity, std::string alias, b2World* world, float xPos, float yPos, fysicsBodyType b_type, bool rotationLocked, float gravityScale);*/

        // Body access
        b2Body* GetBody();

        // Position
        b2Vec2 GetPosition() const;
        void SetPosition(float x, float y);

        // Angle
        float GetAngle() const;
        void SetAngle(float angleInRadians);

        // Velocity
        b2Vec2 GetLinearVelocity() const;
        void SetLinearVelocity(float x, float y);
        float GetAngularVelocity() const;
        void SetAngularVelocity(float x);

        // Awake
        void SetAwake(bool flag);
        bool GetAwake();

        // Rotation
        void RotatePosition(float angle);

        // Damping
        float GetLinearDamping();
        void SetLinearDamping(float linearDamping);
        float GetAngularDamping();
        void SetAngularDamping(float angularDamping);

        // Sleep
        bool GetAllowSleep();
        void SetAllowSleep(bool flag);

        // Fixed rotation
        bool GetFixedRotation();
        void SetFixedRotation(bool flag);

        // Bullet
        bool GetIsBullet();
        void SetIsBullet(bool flag);

        // Gravity
        float GetGravityScale();
        void SetGravityScale(float gravityScale);
    };
}
