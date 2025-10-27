#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Maf/MafUtils.h"
#include "Fysics/FysicsManager.h"

#include <iostream>

namespace IonixEngine
{
    class Collider
    {
        b2World* world;
        FysicsManager* fysicsManager;

    public:

        Collider(FysicsManager* manager) : fysicsManager(manager)
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
            if (a.width <= 0 || a.height <= 0 || b.width <= 0 || b.height <= 0)
                return false;

            float leftA = std::min(a.x, a.x + a.width);
            float rightA = std::max(a.x, a.x + a.width);
            float topA = std::min(a.y, a.y + a.height);
            float bottomA = std::max(a.y, a.y + a.height);

            float leftB = std::min(b.x, b.x + b.width);
            float rightB = std::max(b.x, b.x + b.width);
            float topB = std::min(b.y, b.y + b.height);
            float bottomB = std::max(b.y, b.y + b.height);

            bool touching = (leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB);

            if (touching && fysicsManager)
            {
                //Collision detected
                std::cout << "[Collider::isTouching] Collision detected between rectangles!\n";
                fysicsManager->EmitCollision(1, 2); // Placeholder EntityIDs
            }

            return touching;
        }

        Contact getContact(const Rect& a, const Rect& b)
        {
            //Creates an empty contact and gives variables values
            Contact contact{};
            float aCentreX = a.x + a.width / 2;
            float aCentreY = a.y + a.height / 2;
            float bCentreX = b.x + b.width / 2;
            float bCentreY = b.y + b.height / 2;

            //Checks for no overlap case
            float dx = bCentreX - aCentreX;
            float overlapX = (a.width / 2 + b.width / 2) - Maf::mafAbs(dx);
            if (overlapX <= 0) return contact;


            float dy = bCentreY - aCentreY;
            float overlapY = (a.height / 2 + b.height / 2) - Maf::mafAbs(dy);
            if (overlapY <= 0) return contact;

            //An overlap is detected.
            contact.isTouching = true;

            //Picking the smallest overlap
            if (overlapX < overlapY)
            {
                //Determins the direction of the normal to apply force in the appropriate direction
                contact.overlapX = (dx < 0 ? -overlapX : overlapX);
                contact.overlapY = 0;
                contact.normalX = (dx < 0 ? -1 : 1);
                contact.normalY = 0;
            }
            else
            {
                //Determins the direction of the normal to apply force in the appropriate direction
                contact.overlapX = 0;
                contact.overlapY = (dy < 0 ? -overlapY : overlapY);
                contact.normalX = 0;
                contact.normalY = (dy < 0 ? -1 : 1);
            }
            //Returns the Contact point and the direction of the normal
            return contact;
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
            Contact contact{};
            float dx = a.x - b.x;
            float dy = a.y - b.y;
            float distanceSqr = dx * dx + dy * dy;
            float radiusSum = a.radius + b.radius;

            //Checks for no overlap case
            if (distanceSqr >= radiusSum * radiusSum) return contact;

            //An overlap is detected.
            contact.isTouching = true;

            //Determins the true distance between the two circles
            float dist = Maf::mafSqrt(distanceSqr);
            //Computes the depth
            float overlap = radiusSum - dist;

            //Considering edge case of circles being exactly on top of each other
            if (dist == 0.0f)
            {
                contact.isTouching = true;
                contact.normalX = 1.0f;
                contact.normalY = 0.0f;
                contact.overlapX = 0.0f;
                contact.overlapY = radiusSum;
                return contact;
            }
            //Creates a unit vector for the collision normal
            contact.normalX = dx / dist;
            contact.normalY = dy / dist;
            //Works out the required distance to move a circle to seperate them
            contact.overlapX = contact.normalX * overlap;
            contact.overlapY = contact.normalY * overlap;

            //Returns the Contact point and the direction of the normal
            return contact;
        }
    };
}

