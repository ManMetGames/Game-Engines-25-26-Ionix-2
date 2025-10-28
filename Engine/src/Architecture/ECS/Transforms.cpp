#include <algorithm>
#include <cmath>
#include <stack>

#include "SDL.h"
#include "Transforms.h"
#include "../ECS/Temp_Vec2.hpp"
#include "Entity.hpp"

namespace IonixEngine
{
    //private methods
    std::stack<Transform*> Transform::getPathToParent()
    {
        std::stack<Transform*> pathToParent = std::stack<Transform*>();
        if (!parentTransform) { return pathToParent; }
        Transform* parent = parentTransform;
        while (parent)
        {
            pathToParent.push(parent);
            parent = parent->parentTransform;
        }
        return pathToParent;
    }

    //public methods
    Transform::Transform(Entity* parentEntity) :
        rotation(0.0f),
        parentTransform(nullptr),
        entity(parentEntity)
    {
        position = Vec2{ 0.0f,0.0f };
        childTransforms = std::vector<Transform*>();
    }

    Vec2 Transform::GetGlobalPosition()
    {
        if (!parentTransform)
        {
            //SDL_Log("[Transforms] Returning Global Pos: %f, %f", position.x, position.y);
            return position;
        }

        std::stack<Transform*> pathToParent = getPathToParent();
        int stackSize = pathToParent.size();
        Vec2 globalPos = { 0.0f,0.0f };
        float globalRot = 0.0f;

        while (!pathToParent.empty())
        {
            Transform* currentParent = pathToParent.top();
            float pRotation = 0.0f;
            if (currentParent->parentTransform)
            {
                pRotation += currentParent->parentTransform->rotation;
            }
            //globalRot += currentParent->rotation;
            Vec2 parentLocalPos = Vec2Rotate(currentParent->position, pRotation);

            globalPos.x += parentLocalPos.x;
            globalPos.y += parentLocalPos.y;

            pathToParent.pop();
        }
        Vec2 displacedLocal = position;
        if (parentTransform) {
            displacedLocal = Vec2Rotate(position, parentTransform->rotation);
        }

        globalPos.x += displacedLocal.x;
        globalPos.y += displacedLocal.y;

        //SDL_Log("[Transforms] Returning Global Pos: %f, %f", globalPos.x, globalPos.y);
        return globalPos;
    }
        


    float Transform::GetGlobalRotation()
    {
        if (!parentTransform) { return rotation; }
        std::stack<Transform*> pathToParent = getPathToParent();
        float rot = 0.0f;
        while (!pathToParent.empty())
        {
            Transform* currentParent = pathToParent.top();
            rot += currentParent->rotation;
            pathToParent.pop();
        }
        rot += rotation;
        return rot;
    }

    void Transform::SetGlobalPosition(Vec2 newPosition)
    {
        Vec2 parentPos = parentTransform->GetGlobalPosition();
        float parentRot = parentTransform->GetLocalRotation();

        Vec2 difference = { newPosition.x - parentPos.x, newPosition.y - parentPos.y };
        difference = Vec2Rotate(difference, -parentRot);

        position = difference;
    }

    void Transform::SetGlobalRotation(float rot)
    {
        std::stack<Transform*> pathToParent = getPathToParent();
        float accumulator = 0.0f;
        while (!pathToParent.empty())
        {
            Transform* currentParent = pathToParent.top();
            accumulator += currentParent->rotation;
            pathToParent.pop();

        }
        rotation = rot - accumulator;
        rotation = fmod(rotation,360.0f);
    }

    Vec2 Transform::GetLocalPosition()
    {
        return position;
    }

    float Transform::GetLocalRotation()
    {
        return rotation;
    }

    void Transform::SetLocalPosition(Vec2 localPosition)
    {
        position = localPosition;
    }

    void Transform::SetLocalRotation(float localRotation)
    {
        rotation = localRotation;
        rotation = fmod(rotation, 360.0f);
    }

    //maintainLocation = true will attempt to keep the transforms in the same place
    void Transform::SetParent(Transform* parent, bool maintainLocation)
    {
        if (parentTransform)
        {
            RemoveParent();
        }

        Vec2 oldPos = GetGlobalPosition();
        float oldRot = GetGlobalRotation();

        parent->AddChild(this);
        SDL_Log("[Transform] New parent set");
        parentTransform = parent;

        if (maintainLocation)
        {
            SetGlobalPosition(oldPos);
            SetGlobalRotation(oldRot);
        }
    }

    //maintainLocation = true will attempt to keep the transforms in the same place
    void Transform::RemoveParent(bool maintainLocation)
    {
        Vec2 globalPos = GetGlobalPosition();
        float globalRot = GetGlobalRotation();

        parentTransform->RemoveChild(this);
        SDL_Log("[Transform] Removing parent transform");
        parentTransform = nullptr;

        if (maintainLocation)
        {
            position = globalPos;
            rotation = globalRot;
        }
    }

    void Transform::AddChild(Transform* child)
    {
        SDL_Log("[Transform] Adding new child transform");
        childTransforms.push_back(child);
    }

    bool Transform::RemoveChild(Transform* child)
    {
        auto iterator = std::find(childTransforms.begin(), childTransforms.end(), child);
        if (iterator != childTransforms.end())
        {
            childTransforms.erase(iterator);
            return true;
        }
        else { return false; }
    }

    bool Transform::RemoveChild(int index)
    {
        //size should be largest index +1, therfore if index is smaller, it is valid
        if (index < childTransforms.size())
        {
            childTransforms.erase(childTransforms.begin() + index);
            return true;
        }
        else { return false; }
    }

}
