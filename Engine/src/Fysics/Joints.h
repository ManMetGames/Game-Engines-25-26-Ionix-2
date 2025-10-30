#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
     class Joints    
    {
    private:

    protected:
        b2Body* bodyA = nullptr;
        b2Body* bodyB = nullptr;
        b2Vec2 anchorA;
        b2Vec2 anchorB;
        b2JointUserData userData;

    public:
        b2World* world;        

        Joints()
        {
            world = LayerFysics::GetInstance()->GetWorld();          
        }              

        virtual void setJoint() {};

        void destroyJoint(b2Joint* joint) {
            world->DestroyJoint(joint);
        }
    
        b2Body* getBodyA(b2Joint* joint) {
            return joint->GetBodyA();
        }

        void setBodyA(b2Joint* joint, b2Body* newBody) {
            bodyA = newBody;
            destroyJoint(joint);
            setJoint();
        }
     
        b2Body* getBodyB(b2Joint* joint) {
            return joint->GetBodyB();
        }

        void setBodyB(b2Joint* joint, b2Body* newBody) {
            bodyA = newBody;
            world->DestroyJoint(joint);
        }

        b2Vec2 getAnchorA(b2Joint* joint) {
            return joint->GetAnchorA();
        }

        b2Vec2 getAnchorB(b2Joint* joint) {
            return joint->GetAnchorB();
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

        b2JointType getType(b2Joint* joint, float inverseDeltaTime) {
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

        b2Vec2 worldAxis;
        float lowerTranslation;
        float upperTranslation;
        bool enableLimit;
        float maxMotorForce;
        float motorSpeed;
        bool enableMotor;

    public:

        void setJoint() override{
            b2PrismaticJointDef jointDef;
            anchorA = bodyA->GetWorldCenter();
            jointDef.Initialize(bodyA, bodyB, anchorA, worldAxis);
            jointDef.lowerTranslation = lowerTranslation;
            jointDef.upperTranslation = upperTranslation;
            jointDef.enableLimit = enableLimit;
            jointDef.maxMotorForce = maxMotorForce;
            jointDef.motorSpeed = motorSpeed;
            jointDef.enableMotor = enableMotor;

            joint = (b2PrismaticJoint*)world->CreateJoint(&jointDef);
        }

      void setJoint(b2Body* bodyA, b2Body* bodyB, b2Vec2 worldAxis, float lowerTranslation, float upperTranslation, bool enableLimit, float maxMotorForce, float motorSpeed, bool enableMotor) {
         b2PrismaticJointDef jointDef;
         anchorA = bodyA->GetWorldCenter();
         jointDef.Initialize(bodyA, bodyB, anchorA, worldAxis);
         jointDef.lowerTranslation = lowerTranslation;
         jointDef.upperTranslation = upperTranslation;
         jointDef.enableLimit = enableLimit;
         jointDef.maxMotorForce = maxMotorForce;
         jointDef.motorSpeed = motorSpeed;
         jointDef.enableMotor = enableMotor;

         joint = (b2PrismaticJoint*)world->CreateJoint(&jointDef);

      }
      b2Joint* getJoint() {
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

      void setMotorForce(float force) {
          if (joint == nullptr) return;
          joint->SetMaxMotorForce(force);
      }


      float getMotorSpeed() {
          return motorSpeed;
      }

      void setMotorSpeed(float speed) {
          if (joint == nullptr) return;
          joint->SetMotorSpeed(speed);
          motorSpeed = speed;
      }

      b2Vec2 getWorldAxis() {
          return worldAxis;
      }

      void setWorldAxis(b2Vec2 newWorldAxis){
          worldAxis = newWorldAxis;
          destroyJoint(joint);
          setJoint();
      }

      float getLowerTranslation() {
          return lowerTranslation;
      }

      void setLowerTranslation(float newLowerTranslation) {
          lowerTranslation = newLowerTranslation;
          destroyJoint(joint);
          setJoint();
      }

      float getUpperTranslation() {
          return upperTranslation;
      }

      void setUpperTranslation(float newUpperTranslation) {
          upperTranslation = newUpperTranslation;
          destroyJoint(joint);
          setJoint();
      }

      bool getEnableLimit() {
          return enableLimit;
      }

      void setEnableLimit(bool newEnableLimit) {
          enableLimit = newEnableLimit;
          destroyJoint(joint);
          setJoint();
      }

      bool getEnableMotor() {
          return enableMotor;
      }

      void setEnableMotor(bool newEnableMotor) {
          enableMotor = newEnableMotor;
          destroyJoint(joint);
          setJoint();
      }
    };

    class WeldJoints : Joints {
    private:
        b2WeldJoint* joint;

    public:

        void setJoint(b2Body* bodyA, b2Body* bodyB) {
            b2WeldJointDef jointDef;

            anchorA = bodyA->GetWorldCenter();

            jointDef.Initialize(bodyA,bodyB,anchorA);

            joint = (b2WeldJoint*)world->CreateJoint(&jointDef);

        }

        b2Joint* getJoint() {
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
        b2Vec2 position1;
        b2Vec2 position2;
        float ratio;
        float lengthA;
        float lengthB;

    public:

        void setJoint() {

             anchorA = bodyA->GetWorldCenter();
             anchorB = bodyB->GetWorldCenter();

            b2Vec2 groundAnchorA(position1.x, position1.y + lengthA);
            b2Vec2 groundAnchorB(position2.x, position2.y + lengthB);

            b2PulleyJointDef jointDef;
            jointDef.Initialize(bodyA, bodyB, groundAnchorA, groundAnchorB, anchorA, anchorB, ratio);

            joint = (b2PulleyJoint*)world->CreateJoint(&jointDef);
        }

        void setJoint(b2Body* bodyA, b2Body* bodyB, b2Vec2 p1, b2Vec2 p2, float ratio, float lengthA, float lengthB) {

             anchorA = bodyA->GetWorldCenter();
             anchorB = bodyB->GetWorldCenter();

            b2Vec2 groundAnchorA(p1.x, p1.y + lengthA);
            b2Vec2 groundAnchorB(p2.x, p2.y + lengthB);

            b2PulleyJointDef jointDef;
            jointDef.Initialize(bodyA, bodyB, groundAnchorA, groundAnchorB, anchorA, anchorB, ratio);

            joint = (b2PulleyJoint*)world->CreateJoint(&jointDef);
        }

        b2Joint* getJoint() {
            return joint;
        }

        float getLengthA() {
            return joint->GetLengthA();
        }

        void setLengthA(float newLengthA) {
            lengthA = newLengthA;
            destroyJoint(joint);
            setJoint();
        }

        float getLengthB() {
            return joint->GetLengthA();
        }

        void setLengthB(float newLengthB) {
            lengthB = newLengthB;
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
            ratio = newRatio;
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
            return position1;
        }

        void setPosition1(b2Vec2 newPosition1) {
            position1 = newPosition1;
            destroyJoint(joint);
            setJoint();
        }

        b2Vec2 getPosition2() {
            return position2;
        }

        void setPosition2(b2Vec2 newPosition2) {
            position2 = newPosition2;
            destroyJoint(joint);
            setJoint();
        }
    };

    class DistanceJoints : public Joints {
    private:
        b2DistanceJoint* joint;
        float length = -1.0f;

    public:

        void setJoint() {

            b2DistanceJointDef jointDef;
            jointDef.Initialize(bodyA, bodyB, anchorA, anchorB);
            if (length > 0.0f) jointDef.length = length;

            joint = (b2DistanceJoint*)world->CreateJoint(&jointDef);
        }

        void setJoint(b2Body* bodyA, b2Body* bodyB, const b2Vec2& anchorA, const b2Vec2& anchorB, float length = -1.0f) {

            b2DistanceJointDef jointDef;
            jointDef.Initialize(bodyA, bodyB, anchorA, anchorB);
            if (length > 0.0f) jointDef.length = length;

            joint = (b2DistanceJoint*)world->CreateJoint(&jointDef);
        }

        float getLength() { return joint->GetLength(); }
        void setLength(float l) { joint->SetLength(l); }

        b2Joint* getJoint() { return joint; }


    };

    class RevoluteJoints : public Joints {
    private:
        b2RevoluteJoint* joint;
        bool enableLimit;
        float lowerAngle;
        float upperAngle;
        bool enableMotor;
        float motorSpeed;
        float maxMotorTorque;

    public:

        void setJoint() {

            b2RevoluteJointDef def;
            def.Initialize(bodyA, bodyB, bodyA->GetWorldCenter());

            def.enableLimit = enableLimit;
            def.lowerAngle = lowerAngle;
            def.upperAngle = upperAngle;

            def.enableMotor = enableMotor;
            def.motorSpeed = motorSpeed;
            def.maxMotorTorque = maxMotorTorque;

            joint = (b2RevoluteJoint*)world->CreateJoint(&def);
        }

        void setJoint() {

            b2RevoluteJointDef def;
            anchorA = bodyA->GetWorldCenter();
            def.Initialize(bodyA, bodyB, anchorA);

            def.enableLimit = enableLimit;
            def.lowerAngle = lowerAngle;
            def.upperAngle = upperAngle;

            def.enableMotor = enableMotor;
            def.motorSpeed = motorSpeed;
            def.maxMotorTorque = maxMotorTorque;

            joint = (b2RevoluteJoint*)world->CreateJoint(&def);
        }

        void setJoint(b2Body* bodyA, b2Body* bodyB,  bool enableLimit = false, float lowerAngle = 0.0f, float upperAngle = 0.0f, bool enableMotor = false, float motorSpeed = 0.0f, float maxMotorTorque = 0.0f) {

            b2RevoluteJointDef def;
            anchorA = bodyA->GetWorldCenter();
            def.Initialize(bodyA, bodyB, anchorA);

            def.enableLimit = enableLimit;
            def.lowerAngle = lowerAngle;
            def.upperAngle = upperAngle;

            def.enableMotor = enableMotor;
            def.motorSpeed = motorSpeed;
            def.maxMotorTorque = maxMotorTorque;

            joint = (b2RevoluteJoint*)world->CreateJoint(&def);
        }

        float getJointAngle() { return joint->GetJointAngle(); }
        float getJointSpeed() { return joint->GetJointSpeed(); }

        float getMotorSpeed() {
            return motorSpeed;
        }
        void setMotorSpeed(float speed) { 
            motorSpeed = speed;
            joint->SetMotorSpeed(speed); 
        }

        float getMotorTorque(float inv_dt) { return joint->GetMotorTorque(inv_dt); }
        void setMaxMotorTorque(float torque) { joint->SetMaxMotorTorque(torque); }     

        bool getEnableLimit() {
            return enableLimit;
        }

        void setEnableLimit(bool newEnableLimit) {
            enableLimit = newEnableLimit;
            destroyJoint(joint);
            setJoint();
        }

        bool getEnableMotor() {
            return enableMotor;
        }

        void setEnableMotor(bool newEnableMotor) {
            enableMotor = newEnableMotor;
            destroyJoint(joint);
            setJoint();
        }

        float getLowerAngle() {
            return lowerAngle;
        }

        void setLowerAngle(float newLowerAngle) {
            lowerAngle = newLowerAngle;
            destroyJoint(joint);
            setJoint();
        }

        float getUpperAngle() {
            return upperAngle;
        }

        void setUpperAngle(float newUpperAngle) {
            upperAngle = newUpperAngle;
            destroyJoint(joint);
            setJoint();
        }



        b2Joint* getJoint() { return joint; }
    };
}

