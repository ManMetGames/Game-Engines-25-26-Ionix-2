
#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class Entity;
    class Joints
    {
    private:
        b2Joint* joint;
    protected:
        b2Body* _bodyA = nullptr;
        b2Body* _bodyB = nullptr;
        b2Vec2 _anchorA = b2Vec2(0, 0);
        b2Vec2 _anchorB = b2Vec2(0, 0);
        b2JointUserData userData;

    public:

        Joints();

        virtual void setJoint() {};

        virtual b2Joint* getJoint();

        void destroyJoint(b2Joint* joint);

        b2Body* getBodyA(b2Joint* joint);

        void setBodyA(b2Joint* joint, b2Body* newBody);

        b2Body* getBodyB(b2Joint* joint);

        void setBodyB(b2Joint* joint, b2Body* newBody);

        b2Vec2 getAnchorA(b2Joint* joint);

        void setAnchorA(b2Joint* joint, b2Vec2 newAnchor);

        b2Vec2 getAnchorB(b2Joint* joint);


        void setAnchorB(b2Joint* joint, b2Vec2 newAnchor);

        b2JointUserData getUserData(b2Joint* joint);

        bool getCollideConntected(b2Joint* joint);

        b2Vec2 getReactionForce(b2Joint* joint, float inverseDeltaTime);

        float getReactionTorque(b2Joint* joint, float inverseDeltaTime);

        b2JointType getType(b2Joint* joint);

        bool isEnabled(b2Joint* joint);

        void shiftOrigin(b2Joint* joint, b2Vec2 newOrigin);

    };

    class PrismaticJoints : public Joints {
    private:
        b2PrismaticJoint* joint;

        b2Vec2 _worldAxis;
        float _lowerTranslation;
        float _upperTranslation;
        bool _enableLimit;
        float _maxMotorForce;
        float _motorSpeed;
        bool _enableMotor;

    public:

        using Joints::Joints;

        void setJoint() override;

        void setJoint(Entity* entityA, Entity* entityB, b2Vec2 worldAxis, float lowerTranslation, float upperTranslation, bool enableLimit, float maxMotorForce, float motorSpeed, bool enableMotor);

        b2Joint* getJoint() override;

        float getJointTranslation();

        void setJointTranslation(float lowerTranslation, float upperTranslation);

        float getJointSpeed();

        float getMotorForce(float inverseDeltaTime);

        float getMaxMotorForce();

        void setMaxMotorForce(float force);

        float getMotorSpeed();

        void setMotorSpeed(float speed);

        b2Vec2 getWorldAxis();

        void setWorldAxis(b2Vec2 newWorldAxis);

        float getLowerTranslation();

        void setLowerTranslation(float newLowerTranslation);

        float getUpperTranslation();

        void setUpperTranslation(float newUpperTranslation);

        bool getEnableLimit();

        void setEnableLimit(bool newEnableLimit);

        bool getEnableMotor();

        void setEnableMotor(bool newEnableMotor);
    };

    class WeldJoints : public Joints {
    private:
        b2WeldJoint* joint;

    public:

        using Joints::Joints;

        void setJoint() override;

        void setJoint(Entity* entityA, Entity* entityB);

        b2Joint* getJoint() override;

        void setDamping(float damping);

        float getDamping();

        void setStiffness(float stiffness);

        float getStiffness();
    };

    class PulleyJoints : public Joints {
    private:
        b2PulleyJoint* joint;
        b2Vec2 _position1;
        b2Vec2 _position2;
        float _ratio;
        float _lengthA;
        float _lengthB;

    public:

        using Joints::Joints;

        void setJoint()override;

        void setJoint(Entity* entityA, Entity* entityB, b2Vec2 p1, b2Vec2 p2, float ratio, float lengthA, float lengthB);

        b2Joint* getJoint() override;

        float getLengthA();

        void setLengthA(float newLengthA);

        float getLengthB();

        void setLengthB(float newLengthB);

        float getCurrentLengthA();

        float getCurrentLengthB();

        float getRatio();

        void setRatio(float newRatio);

        b2Vec2 getGroundAnchorA();

        b2Vec2 getGroundAnchorB();

        b2Vec2 getPosition1();

        void setPosition1(b2Vec2 newPosition);

        b2Vec2 getPosition2();

        void setPosition2(b2Vec2 newPosition);
    };

    class DistanceJoints : public Joints {
    private:
        b2DistanceJoint* joint;
        float _length = -1.0f;

    public:

        using Joints::Joints;

        void setJoint() override;

        void setJoint(Entity* entityA, Entity* entityB, const b2Vec2& anchorA, const b2Vec2& anchorB, float length = -1.0f);

        float getLength();

        void setLength(float l);

        b2Joint* getJoint() override;


    };

    class RevoluteJoints : public Joints {
    private:
        b2RevoluteJoint* joint;
        bool _enableLimit;
        float _lowerAngle;
        float _upperAngle;
        bool _enableMotor;
        float _motorSpeed;
        float _maxMotorTorque;

    public:

        using Joints::Joints;

        void setJoint() override;

        void setJoint(Entity* entityA, Entity* entityB, bool enableLimit = false, float lowerAngle = 0.0f, float upperAngle = 0.0f, bool enableMotor = false, float motorSpeed = 0.0f, float maxMotorTorque = 0.0f);

        float getJointAngle();

        float getJointSpeed();

        float getMotorSpeed();

        void setMotorSpeed(float speed);

        float getMotorTorque(float inv_dt);

        void setMaxMotorTorque(float torque);

        bool getEnableLimit();

        void setEnableLimit(bool newEnableLimit);

        bool getEnableMotor();

        void setEnableMotor(bool newEnableMotor);

        float getLowerAngle();

        void setLowerAngle(float newLowerAngle);

        float getUpperAngle();

        void setUpperAngle(float newUpperAngle);

        b2Joint* getJoint() override;
    };


}