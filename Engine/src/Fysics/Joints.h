#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class Joints
    {
        b2World* world;

        Joints()
        {
            world = LayerFysics::GetInstance()->GetWorld();
        }

        /*void SetPrismaticJoint(b2Body *bodyA, b2Body *bodyB) {
            b2PrismaticJointDef jointDef;
            b2Vec2 worldAxis(1.0f, 0.0f);
            jointDef.Initialize(bodyA, bodyB, bodyA->GetWorldCenter(), worldAxis);
            jointDef.lowerTranslation = -5.0f;
            jointDef.upperTranslation = 2.5f;
            jointDef.enableLimit = true;
            jointDef.maxMotorForce = 1.0f;
            jointDef.motorSpeed = 0.0f;
            jointDef.enableMotor = true;

            
        }

        void SetWeldJoint() {

        }

        void SetPulleyJoint() {

        }

        void DestroyJoint() {

        }*/
    };

    class PrismaticJoints {

    };

    class WeldJoints {

    };

    class PulleyJoints {

    };

   
    
}

