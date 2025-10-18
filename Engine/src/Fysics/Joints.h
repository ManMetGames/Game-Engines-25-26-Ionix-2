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

   
        virtual void setJoint(b2Body* bodyA, b2Body* bodyB);       

        virtual void deleteJoint();
    
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

      void setJoint(b2Body* bodyA, b2Body* bodyB) override {
         b2PrismaticJointDef jointDef;
         b2Vec2 worldAxis(1.0f, 0.0f);
         jointDef.Initialize(bodyA, bodyB, bodyA->GetWorldCenter(), worldAxis);
         jointDef.lowerTranslation = -5.0f;
         jointDef.upperTranslation = 2.5f;
         jointDef.enableLimit = true;
         jointDef.maxMotorForce = 1.0f;
         jointDef.motorSpeed = 0.0f;
         jointDef.enableMotor = true;

         joint = (b2PrismaticJoint*)world->CreateJoint(&jointDef);

      }
      b2Joint* getJoint() {
          return joint;
      }
      void deleteJoint() override {
          world->DestroyJoint(joint);
      }

      float getJointTranslation() {
          return joint->GetJointTranslation();
      }
    
      float getJointSpeed() {
          return joint->GetJointSpeed();
      }

      float getMotorForce() {
          return joint->GetMotorForce(60.0f); //requires inverse delta time to be made
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

    class PulleyJoints {

    };
    
}

