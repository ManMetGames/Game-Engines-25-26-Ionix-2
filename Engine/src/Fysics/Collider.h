#pragma once
#include "LayerSystem/Layers/LayerFysics.h"
#include "Maf/MafUtils.h"

#include <iostream>

namespace IonixEngine
{
    class Collider
    {
        b2World* world;

        using CollisionCallback = std::function<void(Collider* other)>;
        std::vector<CollisionCallback> collisionCallbacks_;

    public:

        Collider()
        {
            world = LayerFysics::GetInstance()->GetWorld();
        }

        void RegisterCollisionCallback(const CollisionCallback& callback)
        {
            collisionCallbacks_.push_back(callback);
        }

        void EmitCollision(Collider* other)
        {
            for (auto& cb : collisionCallbacks_)
                cb(other);
        }

        struct Contact {         //Contact struct
            bool isTouching;
            float overlapX;
            float overlapY;
            float normalX;
            float normalY;
        };

        struct Rect {             //Box struct
        private:
            float x, y;           //Origin point (top-left)
            float width, height;
        public:
            Rect(float x_, float y_, float width_, float height_)
				: x(x_), y(y_), width(width_), height(height_) {}

			float getX() const { return x; }
			float getY() const { return y; }
			float getWidth() const { return width; }
			float getHeight() const { return height; }

            void SetPos(float newX, float newY) { x = newX; y = newY; }
            void SetSize(float newWidth, float newHeight) { width = newWidth; height = newHeight; }
        };

        bool isTouching(const Rect& a, const Rect& b) //Collision detection between two Rect objects
        {
            float leftA = std::min(a.getX(), a.getX() + a.getWidth());
            float rightA = std::max(a.getX(), a.getX() + a.getWidth());
            float topA = std::min(a.getY(), a.getY() + a.getHeight());
            float bottomA = std::max(a.getY(), a.getY() + a.getHeight());

            float leftB = std::min(b.getX(), b.getX() + b.getWidth());
            float rightB = std::max(b.getX(), b.getX() + b.getWidth());
            float topB = std::min(b.getY(), b.getY() + b.getHeight());
            float bottomB = std::max(b.getY(), b.getY() + b.getHeight());

            bool touching = (leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB);

            if (touching)
            {
                //Collision detected
                std::cout << "[Collider::isTouching] Collision detected between rectangles!\n";
            }

            return touching;
        }

        Contact getContact(const Rect& a, const Rect& b)
        {
            //Creates an empty contact and gives variables values
            Contact contact{};
            float aCentreX = a.getX() + a.getWidth() / 2;
            float aCentreY = a.getY() + a.getHeight() / 2;
            float bCentreX = b.getX() + b.getWidth() / 2;
            float bCentreY = b.getY() + b.getHeight() / 2;

            //Checks for no overlap case
            float dx = bCentreX - aCentreX;
            float overlapX = (a.getWidth() / 2 + b.getWidth() / 2) - Maf::mafAbs(dx);
            if (overlapX <= 0) return contact;


            float dy = bCentreY - aCentreY;
            float overlapY = (a.getHeight() / 2 + b.getHeight() / 2) - Maf::mafAbs(dy);
            if (overlapY <= 0) return contact;

            if (a.getWidth() <= 0 || a.getHeight() <= 0 || b.getWidth() <= 0 || b.getHeight() <= 0)
            {
                contact.isTouching = false;
                return contact;
            }

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
        private:
            float x, y;           //Centre point
            float radius;
        public:
            Circle(float x_, float y_, float radius_) : x(x_), y(y_), radius(radius_) {}
            float getX() const { return x; }
            float getY() const { return y; }
            float getRadius() const { return radius; }
            void SetPos(float newX, float newY) { x = newX; y = newY; }
            void SetRadius(float Radius) { radius = Radius; }
        };

        bool isTouching(const Circle& a, const Circle& b) //Collision detection between two circle objects
        {
            float dx = a.getX() - b.getX();
            float dy = a.getY() - b.getY();
            float distanceSqr = dx * dx + dy * dy;
            float radiusSum = a.getRadius() + b.getRadius();
            return (distanceSqr <= radiusSum * radiusSum);
        }

        Contact getContact(const Circle& a, const Circle& b)
        {
            //Creates an empty contact and gives variables values
            Contact contact{};
            float dx = a.getX() - b.getX();
            float dy = a.getY() - b.getY();
            float distanceSqr = dx * dx + dy * dy;
            float radiusSum = a.getRadius() + b.getRadius();

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

        void SetWorld(b2World* newWorld) { world = newWorld; }

        b2World* GetWorld() const { return world; }
    };
}

