#pragma once
#include "Joints.h"
#include "Architecture/Application.h"
#include "FysicsManager.h"


//-----------------Base Class-----------------
IonixEngine::Joints::Joints() {}

b2Joint* IonixEngine::Joints::getJoint()
{
    return nullptr;
}

void IonixEngine::Joints::destroyJoint(b2Joint* joint)
{
    Application::Get().layerFysics->GetFysicsManager()->GetWorld()->DestroyJoint(joint);
}

b2Body* IonixEngine::Joints::getBodyA(b2Joint* joint)
{
    return joint->GetBodyA();
}

void IonixEngine::Joints::setBodyA(b2Joint* joint, b2Body* newBody)
{
    _bodyA = newBody;
    destroyJoint(joint);
    setJoint();
}

b2Body* IonixEngine::Joints::getBodyB(b2Joint* joint)
{
    return joint->GetBodyB();
}

void IonixEngine::Joints::setBodyB(b2Joint* joint, b2Body* newBody)
{
    _bodyB = newBody;
    Application::Get().layerFysics->GetFysicsManager()->GetWorld()->DestroyJoint(joint);
    setJoint();
}

b2Vec2 IonixEngine::Joints::getAnchorA(b2Joint* joint)
{
    return joint->GetAnchorA();
}

void IonixEngine::Joints::setAnchorA(b2Joint* joint, b2Vec2 newAnchor)
{
    _anchorA = newAnchor;
    Application::Get().layerFysics->GetFysicsManager()->GetWorld()->DestroyJoint(joint);
    setJoint();
}

b2Vec2 IonixEngine::Joints::getAnchorB(b2Joint* joint)
{
    return joint->GetAnchorB();
}

void IonixEngine::Joints::setAnchorB(b2Joint* joint, b2Vec2 newAnchor)
{
    _anchorB = newAnchor;
    Application::Get().layerFysics->GetFysicsManager()->GetWorld()->DestroyJoint(joint);
    setJoint();
}

b2JointUserData IonixEngine::Joints::getUserData(b2Joint* joint)
{
    return joint->GetUserData();
}

bool IonixEngine::Joints::getCollideConntected(b2Joint* joint)
{
    return joint->GetCollideConnected();
}

b2Vec2 IonixEngine::Joints::getReactionForce(b2Joint* joint, float inverseDeltaTime)
{
    return joint->GetReactionForce(inverseDeltaTime);
}

float IonixEngine::Joints::getReactionTorque(b2Joint* joint, float inverseDeltaTime)
{
    return joint->GetReactionTorque(inverseDeltaTime);
}

b2JointType IonixEngine::Joints::getType(b2Joint* joint)
{
    return joint->GetType();
}

bool IonixEngine::Joints::isEnabled(b2Joint* joint)
{
    return joint->IsEnabled();
}

void IonixEngine::Joints::shiftOrigin(b2Joint* joint, b2Vec2 newOrigin)
{
    joint->ShiftOrigin(newOrigin);
}

//-----------------Prismatic Class-----------------

void IonixEngine::PrismaticJoints::setJoint()
{
    b2PrismaticJointDef jointDef;
    _anchorA = _bodyA->GetWorldCenter();
    jointDef.Initialize(_bodyA, _bodyB, _anchorA, _worldAxis);
    jointDef.lowerTranslation = _lowerTranslation;
    jointDef.upperTranslation = _upperTranslation;
    jointDef.enableLimit = _enableLimit;
    jointDef.maxMotorForce = _maxMotorForce;
    jointDef.motorSpeed = _motorSpeed;
    jointDef.enableMotor = _enableMotor;

    joint = (b2PrismaticJoint*)Application::Get().layerFysics->GetFysicsManager()->GetWorld()->CreateJoint(&jointDef);
}

void IonixEngine::PrismaticJoints::setJoint(Entity* entityA, Entity* entityB, b2Vec2 worldAxis, float lowerTranslation, float upperTranslation, bool enableLimit, float maxMotorForce, float motorSpeed, bool enableMotor)
{
    b2PrismaticJointDef jointDef;


    _bodyA = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entityA);
    _bodyB = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entityB);
    _worldAxis = worldAxis;
    _anchorA = _bodyA->GetWorldCenter();
    jointDef.Initialize(_bodyA, _bodyB, _anchorA, worldAxis);
    _lowerTranslation = jointDef.lowerTranslation = lowerTranslation;
    _upperTranslation = jointDef.upperTranslation = upperTranslation;
    _enableLimit = jointDef.enableLimit = enableLimit;
    _maxMotorForce = jointDef.maxMotorForce = maxMotorForce;
    _motorSpeed = jointDef.motorSpeed = motorSpeed;
    _enableMotor = jointDef.enableMotor = enableMotor;

    joint = (b2PrismaticJoint*)Application::Get().layerFysics->GetFysicsManager()->GetWorld()->CreateJoint(&jointDef);
}

b2Joint* IonixEngine::PrismaticJoints::getJoint()
{
    return joint;
}

float IonixEngine::PrismaticJoints::getJointTranslation(b2PrismaticJoint* _joint)
{
    return _joint->GetJointTranslation();
}

void IonixEngine::PrismaticJoints::setJointTranslation(b2PrismaticJoint* _joint, float lowerTranslation, float upperTranslation)
{
    if (_joint == nullptr) return;
    _joint->SetLimits(lowerTranslation, upperTranslation);
}

float IonixEngine::PrismaticJoints::getJointSpeed(b2PrismaticJoint* _joint)
{
    return _joint->GetJointSpeed();
}

float IonixEngine::PrismaticJoints::getMotorForce(b2PrismaticJoint* _joint, float inverseDeltaTime)
{
    return _joint->GetMotorForce(inverseDeltaTime);
}

float IonixEngine::PrismaticJoints::getMaxMotorForce()
{
    return _maxMotorForce;
}

void IonixEngine::PrismaticJoints::setMaxMotorForce(b2PrismaticJoint* _joint, float force)
{
    if (_joint == nullptr) return;
    _joint->SetMaxMotorForce(force);
    _maxMotorForce = force;
}

float IonixEngine::PrismaticJoints::getMotorSpeed()
{
    return _motorSpeed;
}

void IonixEngine::PrismaticJoints::setMotorSpeed(b2PrismaticJoint* _joint,float speed)
{
    if (_joint == nullptr) return;
    _joint->SetMotorSpeed(speed);
    _motorSpeed = speed;
}

b2Vec2 IonixEngine::PrismaticJoints::getWorldAxis()
{
    return _worldAxis;
}

void IonixEngine::PrismaticJoints::setWorldAxis(b2Vec2 newWorldAxis)
{
    _worldAxis = newWorldAxis;
    destroyJoint(joint);
    setJoint();
}

float IonixEngine::PrismaticJoints::getLowerTranslation()
{
    return _lowerTranslation;
}

void IonixEngine::PrismaticJoints::setLowerTranslation(float newLowerTranslation)
{
    _lowerTranslation = newLowerTranslation;
    destroyJoint(joint);
    setJoint();
}

float IonixEngine::PrismaticJoints::getUpperTranslation()
{
    return _upperTranslation;
}

void IonixEngine::PrismaticJoints::setUpperTranslation(float newUpperTranslation)
{
    _upperTranslation = newUpperTranslation;
    destroyJoint(joint);
    setJoint();
}

bool IonixEngine::PrismaticJoints::getEnableLimit()
{
    return _enableLimit;
}

void IonixEngine::PrismaticJoints::setEnableLimit(bool newEnableLimit)
{
    _enableLimit = newEnableLimit;
    destroyJoint(joint);
    setJoint();
}

bool IonixEngine::PrismaticJoints::getEnableMotor()
{
    return _enableMotor;
}

void IonixEngine::PrismaticJoints::setEnableMotor(bool newEnableMotor)
{
    _enableMotor = newEnableMotor;
    destroyJoint(joint);
    setJoint();
}

//-----------------Weld Class-----------------

void IonixEngine::WeldJoints::setJoint()
{
    b2WeldJointDef jointDef;

    _anchorA = _bodyA->GetWorldCenter();

    jointDef.Initialize(_bodyA, _bodyB, _anchorA);

    joint = (b2WeldJoint*)Application::Get().layerFysics->GetFysicsManager()->GetWorld()->CreateJoint(&jointDef);
}

void IonixEngine::WeldJoints::setJoint(Entity* entityA, Entity* entityB)
{
    b2WeldJointDef jointDef;
    _bodyA = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entityA);
    _bodyB = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entityB);
    _anchorA = _bodyA->GetWorldCenter();

    jointDef.Initialize(_bodyA, _bodyB, _anchorA);

    joint = (b2WeldJoint*)Application::Get().layerFysics->GetFysicsManager()->GetWorld()->CreateJoint(&jointDef);

}

b2Joint* IonixEngine::WeldJoints::getJoint()
{
    return joint;
}

void IonixEngine::WeldJoints::setDamping(b2WeldJoint* _joint, float damping)
{
    _joint->SetDamping(damping);
}

float IonixEngine::WeldJoints::getDamping(b2WeldJoint* _joint)
{
    return _joint->GetDamping();
}

void IonixEngine::WeldJoints::setStiffness(b2WeldJoint* _joint, float stiffness)
{
    _joint->SetStiffness(stiffness);
}

float IonixEngine::WeldJoints::getStiffness(b2WeldJoint* _joint)
{
    return _joint->GetStiffness();
}

//-----------------Pulley Class-----------------

void IonixEngine::PulleyJoints::setJoint()
{
    _anchorA = _bodyA->GetWorldCenter();
    _anchorB = _bodyB->GetWorldCenter();

    b2Vec2 groundAnchorA(_position1.x, _position1.y + _lengthA);
    b2Vec2 groundAnchorB(_position2.x, _position2.y + _lengthB);

    b2PulleyJointDef jointDef;
    jointDef.Initialize(_bodyA, _bodyB, groundAnchorA, groundAnchorB, _anchorA, _anchorB, _ratio);

    joint = (b2PulleyJoint*)Application::Get().layerFysics->GetFysicsManager()->GetWorld()->CreateJoint(&jointDef);
}

void IonixEngine::PulleyJoints::setJoint(Entity* entityA, Entity* entityB, b2Vec2 p1, b2Vec2 p2, float ratio, float lengthA, float lengthB)
{
    _bodyA = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entityA);
    _bodyB = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entityB);
    _position1 = p1;
    _position2 = p2;
    _ratio = ratio;
    _lengthA = lengthA;
    _lengthB = lengthB;

    _anchorA = _bodyA->GetWorldCenter();
    _anchorB = _bodyB->GetWorldCenter();

    b2Vec2 groundAnchorA(p1.x, p1.y + lengthA);
    b2Vec2 groundAnchorB(p2.x, p2.y + lengthB);

    b2PulleyJointDef jointDef;
    jointDef.Initialize(_bodyA, _bodyB, groundAnchorA, groundAnchorB, _anchorA, _anchorB, ratio);

    joint = (b2PulleyJoint*)Application::Get().layerFysics->GetFysicsManager()->GetWorld()->CreateJoint(&jointDef);
}

b2Joint* IonixEngine::PulleyJoints::getJoint()
{
    return joint;
}

float IonixEngine::PulleyJoints::getLengthA(b2PulleyJoint* _joint)
{
    return _joint->GetLengthA();
}

void IonixEngine::PulleyJoints::setLengthA(float newLengthA)
{
    _lengthA = newLengthA;
    destroyJoint(joint);
    setJoint();
}

float IonixEngine::PulleyJoints::getLengthB(b2PulleyJoint* _joint)
{
    return _joint->GetLengthB();
}

void IonixEngine::PulleyJoints::setLengthB(float newLengthB)
{
    _lengthB = newLengthB;
    destroyJoint(joint);
    setJoint();
}

float IonixEngine::PulleyJoints::getCurrentLengthA(b2PulleyJoint* _joint)
{
    return _joint->GetCurrentLengthA();
}

float IonixEngine::PulleyJoints::getCurrentLengthB(b2PulleyJoint* _joint)
{
    return _joint->GetCurrentLengthB();
}

float IonixEngine::PulleyJoints::getRatio(b2PulleyJoint* _joint)
{
    return _joint->GetRatio();
}

void IonixEngine::PulleyJoints::setRatio(float newRatio)
{
    _ratio = newRatio;
    destroyJoint(joint);
    setJoint();
}

b2Vec2 IonixEngine::PulleyJoints::getGroundAnchorA(b2PulleyJoint* _joint)
{
    return _joint->GetGroundAnchorA();
}

b2Vec2 IonixEngine::PulleyJoints::getGroundAnchorB(b2PulleyJoint* _joint)
{
    return _joint->GetGroundAnchorB();
}

b2Vec2 IonixEngine::PulleyJoints::getPosition1()
{
    return _position1;
}

void IonixEngine::PulleyJoints::setPosition1(b2Vec2 newPosition)
{
    _position1 = newPosition;
    destroyJoint(joint);
    setJoint();
}

b2Vec2 IonixEngine::PulleyJoints::getPosition2()
{
    return _position2;
}

void IonixEngine::PulleyJoints::setPosition2(b2Vec2 newPosition)
{
    _position2 = newPosition;
    destroyJoint(joint);
    setJoint();
}

//-----------------Distance Class-----------------

void IonixEngine::DistanceJoints::setJoint()
{
    b2DistanceJointDef jointDef;
    jointDef.Initialize(_bodyA, _bodyB, _anchorA, _anchorB);
    if (_length > 0.0f) jointDef.length = _length;

    joint = (b2DistanceJoint*)Application::Get().layerFysics->GetFysicsManager()->GetWorld()->CreateJoint(&jointDef);
}

void IonixEngine::DistanceJoints::setJoint(Entity* entityA, Entity* entityB, float length)
{
    _bodyA = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entityA);
    _bodyB = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entityB);
    _anchorA = _bodyA->GetWorldCenter();
    _anchorB = _bodyB->GetWorldCenter();
    _length = length;
    b2DistanceJointDef jointDef;
    jointDef.Initialize(_bodyA, _bodyB, _anchorA, _anchorB);
    if (length > 0.0f) jointDef.length = length;

    joint = (b2DistanceJoint*)Application::Get().layerFysics->GetFysicsManager()->GetWorld()->CreateJoint(&jointDef);
}

float IonixEngine::DistanceJoints::getLength(b2DistanceJoint* _joint)
{
    return _joint->GetLength();
}

void IonixEngine::DistanceJoints::setLength(b2DistanceJoint* _joint, float l)
{
    _joint->SetLength(l);
    _length = l;
}

void IonixEngine::DistanceJoints::setLinearStiffness(b2DistanceJoint* _joint, float frequencyHz, float dampingRatio)
{
    float stiffness = _joint->GetStiffness();
    float damping = _joint->GetDamping();
    b2LinearStiffness(stiffness, damping, frequencyHz, dampingRatio, _joint->GetBodyA(), _joint->GetBodyB());
}

b2Joint* IonixEngine::DistanceJoints::getJoint()
{
    return joint;
}

//-----------------Revolute Class-----------------

void IonixEngine::RevoluteJoints::setJoint()
{
    b2RevoluteJointDef def;
    _anchorA = _bodyA->GetWorldCenter();
    def.Initialize(_bodyA, _bodyB, _anchorA);

    def.enableLimit = _enableLimit;
    def.lowerAngle = _lowerAngle;
    def.upperAngle = _upperAngle;

    def.enableMotor = _enableMotor;
    def.motorSpeed = _motorSpeed;
    def.maxMotorTorque = _maxMotorTorque;

    joint = (b2RevoluteJoint*)Application::Get().layerFysics->GetFysicsManager()->GetWorld()->CreateJoint(&def);
}

void IonixEngine::RevoluteJoints::setJoint(Entity* entityA, Entity* entityB, bool enableLimit, float lowerAngle, float upperAngle, bool enableMotor, float motorSpeed, float maxMotorTorque)
{
    _bodyA = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entityA);
    _bodyB = Application::Get().layerFysics->GetFysicsManager()->GetBodyFromEntity(entityB);

    b2RevoluteJointDef def;
    _anchorA = _bodyA->GetWorldCenter();
    def.Initialize(_bodyA, _bodyB, _anchorA);

    _enableLimit = def.enableLimit = enableLimit;
    _lowerAngle = def.lowerAngle = lowerAngle;
    _upperAngle = def.upperAngle = upperAngle;

    _enableMotor = def.enableMotor = enableMotor;
    _motorSpeed = def.motorSpeed = motorSpeed;
    _maxMotorTorque = def.maxMotorTorque = maxMotorTorque;

    joint = (b2RevoluteJoint*)Application::Get().layerFysics->GetFysicsManager()->GetWorld()->CreateJoint(&def);
}

float IonixEngine::RevoluteJoints::getJointAngle(b2RevoluteJoint* _joint)
{
    return joint->GetJointAngle();
}

float IonixEngine::RevoluteJoints::getJointSpeed(b2RevoluteJoint* _joint)
{
    return joint->GetJointSpeed();
}

float IonixEngine::RevoluteJoints::getMotorSpeed(b2RevoluteJoint* _joint)
{
    return _motorSpeed;
}

void IonixEngine::RevoluteJoints::setMotorSpeed(b2RevoluteJoint* _joint, float speed)
{
    _motorSpeed = speed;
    joint->SetMotorSpeed(speed);
}

float IonixEngine::RevoluteJoints::getMotorTorque(b2RevoluteJoint* _joint, float inv_dt)
{
    return joint->GetMotorTorque(inv_dt);
}

void IonixEngine::RevoluteJoints::setMaxMotorTorque(b2RevoluteJoint* _joint, float torque)
{
    joint->SetMaxMotorTorque(torque);
    _maxMotorTorque = torque;
}

bool IonixEngine::RevoluteJoints::getEnableLimit()
{
    return _enableLimit;
}

void IonixEngine::RevoluteJoints::setEnableLimit(bool newEnableLimit)
{
    _enableLimit = newEnableLimit;
    destroyJoint(joint);
    setJoint();
}

bool IonixEngine::RevoluteJoints::getEnableMotor()
{
    return _enableMotor;
}

void IonixEngine::RevoluteJoints::setEnableMotor(bool newEnableMotor)
{
    _enableMotor = newEnableMotor;
    destroyJoint(joint);
    setJoint();
}

float IonixEngine::RevoluteJoints::getLowerAngle()
{
    return _lowerAngle;
}

void IonixEngine::RevoluteJoints::setLowerAngle(float newLowerAngle)
{
    _lowerAngle = newLowerAngle;
    destroyJoint(joint);
    setJoint();
}

float IonixEngine::RevoluteJoints::getUpperAngle()
{
    return _upperAngle;
}

void IonixEngine::RevoluteJoints::setUpperAngle(float newUpperAngle)
{
    _upperAngle = newUpperAngle;
    destroyJoint(joint);
    setJoint();
}

b2Joint* IonixEngine::RevoluteJoints::getJoint()
{
    return joint;
}