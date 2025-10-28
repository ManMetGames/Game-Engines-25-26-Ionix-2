#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
     class Joints    
    {

    public:
        b2World* world;    


        Joints()
        {
            world = LayerFysics::GetInstance()->GetWorld();          
        }              

        void destroyJoint(b2Joint* joint) {
            world->DestroyJoint(joint);
        }
    
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
    public:

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

      void setMotorSpeed(float speed) {
          if (joint == nullptr) return;
          joint->SetMotorSpeed(speed);
      }

      void setMotorForce(float force) {
          if (joint == nullptr) return;
          joint->SetMaxMotorForce(force);
      }
    };

    class WeldJoints : Joints {
    private:
        b2WeldJoint* joint;

    public:

        void setJoint(b2Body* bodyA, b2Body* bodyB) {
            b2WeldJointDef jointDef;

            b2Vec2 anchor = bodyA->GetWorldCenter();

            jointDef.Initialize(bodyA,bodyB,anchor);

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

    public:

        void setJoint(b2Body* bodyA, b2Body* bodyB, b2Vec2 p1, b2Vec2 p2, float ratio, float lengthA, float lengthB) {

            b2Vec2 anchorA = bodyA->GetWorldCenter();
            b2Vec2 anchorB = bodyB->GetWorldCenter();

            b2Vec2 groundAnchorA(p1.x, p1.y + lengthA);
            b2Vec2 groundAnchorB(p1.x, p1.y + lengthB);

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

        float getLengthB() {
            return joint->GetLengthA();
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

        b2Vec2 getGroundAnchorA() {
            return joint->GetGroundAnchorA();
        }

        b2Vec2 getGroundAnchorB() {
            return joint->GetGroundAnchorB();
        }
    };
}

