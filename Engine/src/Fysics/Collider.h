#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Maf/MafUtils.h"

namespace IonixEngine
{
    class Collider
    {
        b2World* world;

        Collider()
        {
            world = LayerFysics::GetInstance()->GetWorld();
        }

        
        struct Contact {         //Contact struct
            bool isTouching;
            float overlapX;
            float overlapY;
            float normalX;
            float normalY;
        };

        struct Rect {             //Box struct
            float x, y;           //Origin point (top-left)
            float width, height;
        };

        bool isTouching(const Rect& a, const Rect& b) //Collision detection between two Rect objects
        {
            return (a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y);
        }

        Contact getContact(const Rect& a, const Rect& b)
        {
            //Creates an empty contact and gives variables values
            Contact c{};
            float aCentreX = a.x + a.width / 2;
            float aCentreY = a.y + a.height / 2;
            float bCentreX = b.x + b.width / 2;
            float bCentreY = b.y + b.height / 2;

            //Checks for no overlap case
            float dx = bCentreX - aCentreX;
            float overlapX = (a.width / 2 + b.width / 2) - Maf::mafAbs(dx);
            if (overlapX <= 0) return c;


            float dy = bCentreY - aCentreY;
            float overlapY = (a.height / 2 + b.height / 2) - Maf::mafAbs(dy);
            if (overlapY <= 0) return c;

            //An overlap is detected.
            c.isTouching = true;

            //Picking the smallest overlap
            if (overlapX < overlapY)
            {
                //Determins the direction of the normal to apply force in the appropriate direction
                c.overlapX = (dx < 0 ? -overlapX : overlapX);
                c.overlapY = 0;
                c.normalX = (dx < 0 ? -1 : 1);
                c.normalY = 0;
            }
            else
            {
                //Determins the direction of the normal to apply force in the appropriate direction
                c.overlapX = 0;
                c.overlapY = (dy < 0 ? -overlapY : overlapY);
                c.normalX = 0;
                c.normalY = (dy < 0 ? -1 : 1);
            }
            //Returns the Contact point and the direction of the normal
            return c;
        }

        struct Circle {           //Circle struct
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

        Contact getContact(const Circle& a, const Circle& b)
        {
            //Creates an empty contact and gives variables values
            Contact c{};
            float dx = a.x - b.x;
            float dy = a.y - b.y;
            float distanceSqr = dx * dx + dy * dy;
            float radiusSum = a.radius + b.radius;

            //Checks for no overlap case
            if (distanceSqr >= radiusSum * radiusSum) return c;

            //An overlap is detected.
            c.isTouching = true;

            //Determins the true distance between the two circles
            float dist = Maf::mafSqrt(distanceSqr);
            //Computes the depth
            float overlap = radiusSum - dist;
            //Creates a unit vector for the collision normal
            c.normalX = dx / dist;
            c.normalY = dy / dist;
            //Works out the required distance to move a circle to seperate them
            c.overlapX = c.normalX * overlap;
            c.overlapY = c.normalY * overlap;

            //Returns the Contact point and the direction of the normal
            return c;
        }
    };
}

