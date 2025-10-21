#pragma once
#include "LayerSystem/Layers/LayerFysics.h"

namespace IonixEngine
{
    class Collider
    {
        b2World* world;

        Collider()
        {
            world = LayerFysics::GetInstance()->GetWorld();
        }

        virtual bool isTouching(const Collider& other) const = 0;

        struct Rect {
            float x, y;           //Origin point (top-left)
            float width, height;
        };

        bool isTouching(const Rect& a, const Rect& b) //Collision detection between two Rect objects
        {
            return (a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y);
        }

        struct Circle {
            float x, y;           //Centre point
            float radius;
        };

        bool isTouching(const Circle& a, const Circle& b) //Collision detection between two circle objects
        {
            float dx = a.x - b.x;
            float dy = a.y - b.y;
            float distanceSqr = dx * dx + dy * dy;
            float radiusSum = a.radius + b.radius;
            return (distanceSqr <= radiusSum * radiusSum);
        }
    };

}

