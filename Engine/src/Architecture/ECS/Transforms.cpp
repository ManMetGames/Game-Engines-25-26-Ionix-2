#include <algorithm>
#include <cmath>
#include <stack>

#include "SDL.h"
#include "Transforms.h"
#include "../ECS/Temp_Vec2.hpp"
#include "Entity.hpp"

namespace IonixEngine
{
    Transform::Transform(Entity* parentEntity) :
        //position(Vec2{ 0.0f,0.0f }),
        localRotation(0.0f),
        parentTransform(nullptr),
        entity(parentEntity)
    {
        localPosition = Vec2{ 0.0f,0.0f };
        childTransforms = std::vector<Transform*>();
    }

    Vec2 Transform::GetGlobalPosition()
    {
        Vec2 position = { 0.0f,0.0f };
        Transform* parent = parentTransform;

        std::stack<Transform*> pathToParent;
        while (parent) {
            pathToParent.push(parent);
            parent = parent->parentTransform;
        }

        while (!pathToParent.empty())
        {
            Transform* t = pathToParent.top();
            Vec2 parentPos = t->localPosition;
            float parentRot = t->localRotation;

            SDL_Log("parentPos = %f, %f", parentPos.x, parentPos.y);
            SDL_Log("parentRot = %f", parentRot);

            parentPos = Vec2Rotate(parentPos, parentRot);

            position.x += parentPos.x;
            position.y += parentPos.y;

            pathToParent.pop();
        }

        Vec2 local = Vec2Rotate(localPosition, 0.0f);
        position.x += local.x;
        position.y += local.y;

        SDL_Log("[Transforms] Returning Global Pos: %f, %f", position.x, position.y);
        return position;


    }
        


    float Transform::GetGlobalRotation()
    {
        Transform* parent = parentTransform;
        float rot = 0.0f;
        std::stack<Transform*> pathToParent;
        while (parent)
        {
            pathToParent.push(parent);
            parent = parent->parentTransform;
        }

        while (!pathToParent.empty()) {
            rot += pathToParent.top()->localRotation;
            pathToParent.pop();
        }
        rot += localRotation;
        return rot;
        /*if (parentTransform != nullptr)
        {
            return parentTransform->GetGlobalRotation() + rotation;
        }
        else
        {
            return rotation;
        }*/
    }

    void Transform::SetGlobalPosition(Vec2 newPosition)
    {
        Vec2 parentPos = parentTransform->GetGlobalPosition();
        float parentRot = parentTransform->GetLocalRotation();

        Vec2 difference = { newPosition.x - parentPos.x, newPosition.y - parentPos.y };
        difference = Vec2Rotate(difference, -parentRot);

        localPosition = difference;
    }

    void Transform::SetGlobalRotation(float rot)
    {
        float accumulator = 0.0f;
        Transform* parent = parentTransform;
        while (parent)
        {
            accumulator += parent->localRotation;
            parent = parent->parentTransform;
        }
        localRotation = rot - accumulator;
        localRotation = fmod(localRotation,360.0f);
    }

    Vec2 Transform::GetLocalPosition()
    {
        return localPosition;
    }

    float Transform::GetLocalRotation()
    {
        return localRotation;
    }

    void Transform::SetLocalPosition(Vec2 localPosition)
    {
        localPosition = localPosition;
    }

    void Transform::SetLocalRotation(float localRotation)
    {
        localRotation = localRotation;
        localRotation = fmod(localRotation, 360.0f);
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
            localPosition = globalPos;
            localRotation = globalRot;
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
