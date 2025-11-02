#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
     class Joints    
    {
    private:

    protected:
        b2Body* _bodyA = nullptr;
        b2Body* _bodyB = nullptr;
        b2Vec2 _anchorA = b2Vec2(0,0);
        b2Vec2 _anchorB = b2Vec2(0, 0);
        b2JointUserData userData;

    public:
        b2World* world;        

        Joints()
        {
            world = LayerFysics::GetInstance()->GetWorld();
        }

        virtual void setJoint() {};

        virtual b2Joint* getJoint() { b2Joint* joint; return joint; };

        void destroyJoint(b2Joint* joint) {
            world->DestroyJoint(joint);
        }
    
        b2Body* getBodyA(b2Joint* joint) {
            return joint->GetBodyA();
        }

        void setBodyA(b2Joint* joint, b2Body* newBody) {
            _bodyA = newBody;
            destroyJoint(joint);
            setJoint();
        }
     
        b2Body* getBodyB(b2Joint* joint) {
            return joint->GetBodyB();
        }

        void setBodyB(b2Joint* joint, b2Body* newBody) {
            _bodyB = newBody;
            world->DestroyJoint(joint);
            setJoint();
        }

        b2Vec2 getAnchorA(b2Joint* joint) {
            return joint->GetAnchorA();
        }

        void setAnchorA(b2Joint* joint, b2Vec2 newAnchor) {
            _anchorA = newAnchor;
            world->DestroyJoint(joint);
            setJoint();
        }

        b2Vec2 getAnchorB(b2Joint* joint) {
            return joint->GetAnchorB();
        }


        void setAnchorB(b2Joint* joint, b2Vec2 newAnchor) {
            _anchorB = newAnchor;
            world->DestroyJoint(joint);
            setJoint();
        }

        b2JointUserData getUserData(b2Joint* joint) {
           return joint->GetUserData();
        }     

        bool getCollideConntected(b2Joint* joint) {
            return joint->GetCollideConnected();
        }

        b2Vec2 getReactionForce(b2Joint* joint, float inverseDeltaTime) {
            return joint->GetReactionForce(inverseDeltaTime);
        }

        float getReactionTorque(b2Joint* joint, float inverseDeltaTime) {
            return joint->GetReactionTorque(inverseDeltaTime);
        }

        b2JointType getType(b2Joint* joint) {
            return joint->GetType();
        }

        bool isEnabled(b2Joint* joint) {
            return joint->IsEnabled();
        }

        void shiftOrigin(b2Joint* joint, b2Vec2 newOrigin) {
            joint->ShiftOrigin(newOrigin);
        }

    };

    class PrismaticJoints : public Joints{
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

        void setJoint() override{
            b2PrismaticJointDef jointDef;
            _anchorA = _bodyA->GetWorldCenter();
            jointDef.Initialize(_bodyA, _bodyB, _anchorA, _worldAxis);
            jointDef.lowerTranslation = _lowerTranslation;
            jointDef.upperTranslation = _upperTranslation;
            jointDef.enableLimit = _enableLimit;
            jointDef.maxMotorForce = _maxMotorForce;
            jointDef.motorSpeed = _motorSpeed;
            jointDef.enableMotor = _enableMotor;

            joint = (b2PrismaticJoint*)world->CreateJoint(&jointDef);
        }

      void setJoint(b2Body* bodyA, b2Body* bodyB, b2Vec2 worldAxis, float lowerTranslation, float upperTranslation, bool enableLimit, float maxMotorForce, float motorSpeed, bool enableMotor) {
         b2PrismaticJointDef jointDef;
         _bodyA = bodyA;
         _bodyB = bodyB;
         _worldAxis = worldAxis;
         _anchorA = bodyA->GetWorldCenter();
         jointDef.Initialize(bodyA, bodyB, _anchorA, worldAxis);
         _lowerTranslation = jointDef.lowerTranslation = lowerTranslation;
         _upperTranslation = jointDef.upperTranslation = upperTranslation;
         _enableLimit = jointDef.enableLimit = enableLimit;
         _maxMotorForce = jointDef.maxMotorForce = maxMotorForce;
         _motorSpeed = jointDef.motorSpeed = motorSpeed;
         _enableMotor = jointDef.enableMotor = enableMotor;

         joint = (b2PrismaticJoint*)world->CreateJoint(&jointDef);

      }
      b2Joint* getJoint() override {
          return joint;
      }

      float getJointTranslation() {
          return joint->GetJointTranslation();
      }

      void setJointTranslation(float lowerTranslation, float upperTranslation) {
          if (joint == nullptr) return;
          joint->SetLimits(lowerTranslation, upperTranslation);
      }
    
      float getJointSpeed() {
          return joint->GetJointSpeed();
      }

      float getMotorForce(float inverseDeltaTime) {
          return joint->GetMotorForce(inverseDeltaTime);
      }

      float getMaxMotorForce() {
          return _maxMotorForce;
      }

      void setMaxMotorForce(float force) {
          if (joint == nullptr) return;
          joint->SetMaxMotorForce(force);
          _maxMotorForce = force;
      }


      float getMotorSpeed() {
          return _motorSpeed;
      }

      void setMotorSpeed(float speed) {
          if (joint == nullptr) return;
          joint->SetMotorSpeed(speed);
          _motorSpeed = speed;
      }

      b2Vec2 getWorldAxis() {
          return _worldAxis;
      }

      void setWorldAxis(b2Vec2 newWorldAxis){
          _worldAxis = newWorldAxis;
          destroyJoint(joint);
          setJoint();
      }

      float getLowerTranslation() {
          return _lowerTranslation;
      }

      void setLowerTranslation(float newLowerTranslation) {
          _lowerTranslation = newLowerTranslation;
          destroyJoint(joint);
          setJoint();
      }

      float getUpperTranslation() {
          return _upperTranslation;
      }

      void setUpperTranslation(float newUpperTranslation) {
          _upperTranslation = newUpperTranslation;
          destroyJoint(joint);
          setJoint();
      }

      bool getEnableLimit() {
          return _enableLimit;
      }

      void setEnableLimit(bool newEnableLimit) {
          _enableLimit = newEnableLimit;
          destroyJoint(joint);
          setJoint();
      }

      bool getEnableMotor() {
          return _enableMotor;
      }

      void setEnableMotor(bool newEnableMotor) {
          _enableMotor = newEnableMotor;
          destroyJoint(joint);
          setJoint();
      }
    };

    class WeldJoints : public Joints {
    private:
        b2WeldJoint* joint;

    public:

        using Joints::Joints;

        void setJoint() override {
            b2WeldJointDef jointDef;

            _anchorA = _bodyA->GetWorldCenter();

            jointDef.Initialize(_bodyA, _bodyB, _anchorA);

            joint = (b2WeldJoint*)world->CreateJoint(&jointDef);
        }

        void setJoint(b2Body* bodyA, b2Body* bodyB) {
            b2WeldJointDef jointDef;
            _bodyA = bodyA;
            _bodyB = bodyB;
            _anchorA = bodyA->GetWorldCenter();

            jointDef.Initialize(bodyA,bodyB,_anchorA);

            joint = (b2WeldJoint*)world->CreateJoint(&jointDef);

        }

        b2Joint* getJoint() override {
            return joint;
        }

        void setDamping(float damping) {
            joint->SetDamping(damping);
        }

        float getDamping() {
            return joint->GetDamping();
        }

        void setStiffness(float stiffness) {
            joint->SetStiffness(stiffness);
        }

        float getStiffness() {
            return joint->GetStiffness();
        }
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

        void setJoint()override {

             _anchorA = _bodyA->GetWorldCenter();
             _anchorB = _bodyB->GetWorldCenter();

            b2Vec2 groundAnchorA(_position1.x, _position1.y + _lengthA);
            b2Vec2 groundAnchorB(_position2.x, _position2.y + _lengthB);

            b2PulleyJointDef jointDef;
            jointDef.Initialize(_bodyA, _bodyB, groundAnchorA, groundAnchorB, _anchorA, _anchorB, _ratio);

            joint = (b2PulleyJoint*)world->CreateJoint(&jointDef);
        }

        void setJoint(b2Body* bodyA, b2Body* bodyB, b2Vec2 p1, b2Vec2 p2, float ratio, float lengthA, float lengthB) {
            _bodyA = bodyA;
            _bodyB = bodyB;
            _position1 = p1;
            _position2 = p2;
            _ratio = ratio;
            _lengthA = lengthA;
            _lengthB = lengthB;

             _anchorA = bodyA->GetWorldCenter();
             _anchorB = bodyB->GetWorldCenter();

            b2Vec2 groundAnchorA(p1.x, p1.y + lengthA);
            b2Vec2 groundAnchorB(p2.x, p2.y + lengthB);

            b2PulleyJointDef jointDef;
            jointDef.Initialize(bodyA, bodyB, groundAnchorA, groundAnchorB, _anchorA, _anchorB, ratio);

            joint = (b2PulleyJoint*)world->CreateJoint(&jointDef);
        }

        b2Joint* getJoint() override {
            return joint;
        }

        float getLengthA() {
            return joint->GetLengthA();
        }

        void setLengthA(float newLengthA) {
            _lengthA = newLengthA;
            destroyJoint(joint);
            setJoint();
        }

        float getLengthB() {
            return joint->GetLengthA();
        }

        void setLengthB(float newLengthB) {
            _lengthB = newLengthB;
            destroyJoint(joint);
            setJoint();
        }

        float getCurrentLengthA() {
            return joint->GetCurrentLengthA();
        }

        float getCurrentLengthB() {
            return joint->GetCurrentLengthB();
        }

        float getRatio() {
            return joint->GetRatio();
        }

        void setRatio(float newRatio) {
            _ratio = newRatio;
            destroyJoint(joint);
            setJoint();
        }

        b2Vec2 getGroundAnchorA() {
            return joint->GetGroundAnchorA();
        }

        b2Vec2 getGroundAnchorB() {
            return joint->GetGroundAnchorB();
        }

        b2Vec2 getPosition1() {
            return _position1;
        }

        void setPosition1(b2Vec2 newPosition) {
            _position1 = newPosition;
            destroyJoint(joint);
            setJoint();
        }

        b2Vec2 getPosition2() {
            return _position2;
        }

        void setPosition2(b2Vec2 newPosition) {
           _position2 = newPosition;
            destroyJoint(joint);
            setJoint();
        }
    };

    class DistanceJoints : public Joints {
    private:
        b2DistanceJoint* joint;
        float _length = -1.0f;

    public:

        using Joints::Joints;

        void setJoint() override {

            b2DistanceJointDef jointDef;
            jointDef.Initialize(_bodyA, _bodyB, _anchorA, _anchorB);
            if (_length > 0.0f) jointDef.length = _length;

            joint = (b2DistanceJoint*)world->CreateJoint(&jointDef);
        }

        void setJoint(b2Body* bodyA, b2Body* bodyB, const b2Vec2& anchorA, const b2Vec2& anchorB, float length = -1.0f) {
            _bodyA = bodyA;
            _bodyB = bodyB;
            _anchorA = anchorA;
            _anchorB = anchorB;
            _length = length;
            b2DistanceJointDef jointDef;
            jointDef.Initialize(bodyA, bodyB, anchorA, anchorB);
            if (length > 0.0f) jointDef.length = length;

            joint = (b2DistanceJoint*)world->CreateJoint(&jointDef);
        }

        float getLength() { return joint->GetLength(); }
        void setLength(float l) { joint->SetLength(l); _length = l; }

        b2Joint* getJoint() override { return joint; }


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

        void setJoint() override {

            b2RevoluteJointDef def;
            _anchorA = _bodyA->GetWorldCenter();
            def.Initialize(_bodyA, _bodyB, _anchorA);

            def.enableLimit = _enableLimit;
            def.lowerAngle = _lowerAngle;
            def.upperAngle = _upperAngle;

            def.enableMotor = _enableMotor;
            def.motorSpeed = _motorSpeed;
            def.maxMotorTorque = _maxMotorTorque;

            joint = (b2RevoluteJoint*)world->CreateJoint(&def);
        }

        void setJoint(b2Body* bodyA, b2Body* bodyB,  bool enableLimit = false, float lowerAngle = 0.0f, float upperAngle = 0.0f, bool enableMotor = false, float motorSpeed = 0.0f, float maxMotorTorque = 0.0f) {

            _bodyA = bodyA;
            _bodyB = bodyB;
            
            b2RevoluteJointDef def;
            _anchorA = bodyA->GetWorldCenter();
            def.Initialize(bodyA, bodyB, _anchorA);

            _enableLimit = def.enableLimit = enableLimit;
            _lowerAngle = def.lowerAngle = lowerAngle;
            _upperAngle = def.upperAngle = upperAngle;

            _enableMotor = def.enableMotor = enableMotor;
            _motorSpeed = def.motorSpeed = motorSpeed;
            _maxMotorTorque = def.maxMotorTorque = maxMotorTorque;

            joint = (b2RevoluteJoint*)world->CreateJoint(&def);
        }

        float getJointAngle() { return joint->GetJointAngle(); }
        float getJointSpeed() { return joint->GetJointSpeed(); }

        float getMotorSpeed() {
            return _motorSpeed;
        }
        void setMotorSpeed(float speed) { 
            _motorSpeed = speed;
            joint->SetMotorSpeed(speed); 
        }

        float getMotorTorque(float inv_dt) { return joint->GetMotorTorque(inv_dt); }
        void setMaxMotorTorque(float torque) { joint->SetMaxMotorTorque(torque);  _maxMotorTorque = torque; }

        bool getEnableLimit() {
            return _enableLimit;
        }

        void setEnableLimit(bool newEnableLimit) {
            _enableLimit = newEnableLimit;
            destroyJoint(joint);
            setJoint();
        }

        bool getEnableMotor() {
            return _enableMotor;
        }

        void setEnableMotor(bool newEnableMotor) {
            _enableMotor = newEnableMotor;
            destroyJoint(joint);
            setJoint();
        }

        float getLowerAngle() {
            return _lowerAngle;
        }

        void setLowerAngle(float newLowerAngle) {
            _lowerAngle = newLowerAngle;
            destroyJoint(joint);
            setJoint();
        }

        float getUpperAngle() {
            return _upperAngle;
        }

        void setUpperAngle(float newUpperAngle) {
            _upperAngle = newUpperAngle;
            destroyJoint(joint);
            setJoint();
        }

        b2Joint* getJoint() override { return joint; }
    };
}

