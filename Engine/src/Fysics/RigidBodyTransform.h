#pragma once
#include "box2d.h"

namespace IonixEngine
{
    struct RigidBodyTransform
    {
        b2Vec2 previousPosition;
        b2Vec2 currentPosition;
        float previousRotation;
        float currentRotation;

        RigidBodyTransform()
            : previousPosition(0.0f, 0.0f)
            , currentPosition(0.0f, 0.0f)
            , previousRotation(0.0f)
            , currentRotation(0.0f)
        {
        }

        RigidBodyTransform(const b2Vec2& pos, float rot)
            : previousPosition(pos)
            , currentPosition(pos)
            , previousRotation(rot)
            , currentRotation(rot)
        {
        }
    };
}
