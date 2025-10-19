#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
     class Joints    
    {
    public:
        b2World* world;    

     

        b2JointUserData userData;
        Joints()
        {
            world = LayerFysics::GetInstance()->GetWorld();          
        }              

        virtual void destroyJoint();
    
        b2Body* getBodyA(b2Joint* joint) {
            return joint->GetBodyA();
        }
     
        b2Body* getBodyB(b2Joint* joint) {
            return joint->GetBodyB();
        }

        b2Vec2 getAnchorA(b2Joint* joint) {
            return joint->GetAnchorA();
        }

        b2Vec2 getAnchorB(b2Joint* joint) {
            return joint->GetAnchorB();
        }

        void getUserData(b2Joint* joint) {
           userData = joint->GetUserData();
        }

       
    };

    class PrismaticJoints : public Joints{
    public:

        PrismaticJoints() : Joints() {}
        b2PrismaticJoint* joint;

      void setJoint(b2Body* bodyA, b2Body* bodyB, b2Vec2 worldAxis, float lowerTranslation, float upperTranslation, bool enableLimit, float maxMotorForce, float motorSpeed, bool enableMotor) {
         b2PrismaticJointDef jointDef;
         jointDef.Initialize(bodyA, bodyB, bodyA->GetWorldCenter(), worldAxis);
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
      void destroyJoint() override {
          world->DestroyJoint(joint);
      }

      float getJointTranslation() {
          return joint->GetJointTranslation();
      }
    
      float getJointSpeed() {
          return joint->GetJointSpeed();
      }

      float getMotorForce(float inverseDeltaTime) {
          return joint->GetMotorForce(inverseDeltaTime);
      }

      void SetMotorSpeed(float speed) {
          joint->SetMotorSpeed(speed);
      }

      void SetMotorForce(float force) {
          joint->SetMaxMotorForce(force);
      }
    };

    class WeldJoints {

    };

    class PulleyJoints : public Joints {
    public:
        PulleyJoints() : Joints() {}

        b2PulleyJoint* joint;

        void setJoint(b2Body* bodyA, b2Body* bodyB, b2Vec2 p1, b2Vec2 p2, float ratio, float lengthA, float lengthB) {

            b2Vec2 anchorA = bodyA->GetWorldCenter();
            b2Vec2 anchorB = bodyB->GetWorldCenter();

            b2Vec2 groundAnchorA(p1.x, p1.y + lengthA);
            b2Vec2 groundAnchorB(p1.x, p1.y + lengthB);

            b2PulleyJointDef jointDef;
            jointDef.Initialize(bodyA, bodyB, groundAnchorA, groundAnchorB, anchorA, anchorB, ratio);

            joint = (b2PulleyJoint*)world->CreateJoint(&jointDef);
        }

        float getLengthA() {
            return joint->GetLengthA();
        }

        float getLengthB() {
            return joint->GetLengthA();
        }
    };
    
}

