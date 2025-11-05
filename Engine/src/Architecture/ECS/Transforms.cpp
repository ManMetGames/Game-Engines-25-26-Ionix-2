#include <algorithm>
#include <cmath>

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
        localScale = Vec2{ 1.0f,1.0f };
        childTransforms = std::vector<Transform*>();
    }

    std::stack<Transform*> Transform::getPathToParent()
    {
        Transform* parent = parentTransform;
        std::stack<Transform*> pathToParent;

        while (parent)
        {
            pathToParent.push(parent);
            parent = parent->parentTransform;
        }
        return pathToParent;
    }

    Vec2 Transform::GetGlobalPosition()
    {
        Vec2 position = { 0.0f,0.0f };

        Mat3 transformMat = Mat3{
            1.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,0.0f,1.0f };

        std::stack<Transform*> pathToParent = getPathToParent();

        while (!pathToParent.empty())
        {
            //Vec2 parentPos = t->localPosition;
            //float parentRot = t->localRotation;

            //parentPos = Vec2Rotate(parentPos, parentRot);

            //position.x += parentPos.x;
            //position.y += parentPos.y;


            Transform* t = pathToParent.top();

            transformMat = transformMat * t->GetLocalTransformMatrix();

            pathToParent.pop();
        }

        transformMat = transformMat * GetLocalTransformMatrix();

        position = { transformMat.c,transformMat.f };

        return position;
    }



    float Transform::GetGlobalRotation()
    {
        float rot = 0.0f;
        std::stack<Transform*> pathToParent = getPathToParent();

        while (!pathToParent.empty())
        {
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

    Vec2 Transform::GetGlobalScale()
    {
        //I don't think order matters for getting scale

        //ordered version
        std::stack<Transform*> pathToParent = getPathToParent();
        Vec2 scale = { 1.0f,1.0f };
        while (!pathToParent.empty())
        {
            Vec2 parentScale = pathToParent.top()->localScale;
            scale = { scale.x * parentScale.x,scale.y * parentScale.y };
            pathToParent.pop();
        }

        scale = { scale.x * localScale.x,scale.y * localScale.y };

        //unordered version
        //Vec2 scale = localScale;
        //Transform* parent = parentTransform;
        //while (parent) 
        //{
        //    scale = { scale.x * parent->localScale.x,scale.y * parent->localScale.y };
        //    parent = parent->parentTransform;
        //}
        return scale;
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
        localRotation = fmod(localRotation, 360.0f);
    }

    Vec2 Transform::GetLocalPosition()
    {
        return localPosition;
    }

    float Transform::GetLocalRotation()
    {
        return localRotation;
    }

    Vec2 Transform::GetLocalScale()
    {
        return localScale;
    }

    Mat3 Transform::GetLocalScaleMatrix()
    {
        return Mat3{
            localScale.x,0.0f,0.0f,
            0.0f,localScale.y,0.0f,
            0.0f,0.0f,1.0f };
    }

    Mat3 Transform::GetLocalRotationMatrix()
    {
        float angle = localRotation * DEG2RAD;

        return Mat3{
            cosf(angle),sinf(angle),0.0f,
            -sinf(angle),cosf(angle),0.0f,
            0.0f,0.0f,1.0f };
    }

    Mat3 Transform::GetLocalTranslationMatrix()
    {
        return Mat3{
            1.0f,0.0f,localPosition.x,
            0.0f,1.0f,localPosition.y,
            0.0f,0.0f,1.0f };
    }

    Mat3 Transform::GetLocalTransformMatrix()
    {
        //if performance is an issue, may want to use a single matrix for combined transforms,
        //rather than creating it everytime from the three constituent matrices
        Mat3 output = { 0 };

        Mat3 scale = GetLocalScaleMatrix();
        Mat3 rot = GetLocalRotationMatrix();
        Mat3 translate = GetLocalTranslationMatrix();

        //multiply matrices backwards!!!
        //scale * rotation * translate

        output = scale * rot;
        output = translate * output;
        //SDL_Log("debug log lol %f", output.b);
        return output;
    }

    Mat3 Transform::GetGlobalScaleMatrix()
    {
        Mat3 output = {
            1.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,0.0f,1.0f
        };
        std::stack<Transform*> pathToParent = getPathToParent();
        while (!pathToParent.empty())
        {
            Transform* currentParent = pathToParent.top();

            output = output * currentParent->GetLocalScaleMatrix();

            pathToParent.pop();
        }
        output = output * GetLocalScaleMatrix();
        return output;
    }


    Mat3 Transform::GetGlobalRotationMatrix()
    {
        Mat3 output = {
            1.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,0.0f,1.0f
        };
        std::stack<Transform*> pathToParent = getPathToParent();
        while (!pathToParent.empty())
        {
            Transform* currentParent = pathToParent.top();
            output = output * currentParent->GetLocalRotationMatrix();
            pathToParent.pop();
        }
        output = output * GetLocalRotationMatrix();
        return output;
    }

    Mat3 Transform::GetGlobalTranslationMatrix()
    {
        Mat3 output = {
            1.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,0.0f,1.0f
        };
        std::stack<Transform*> pathToParent = getPathToParent();
        while (!pathToParent.empty())
        {
            Transform* currentParent = pathToParent.top();
            output = output * currentParent->GetLocalTranslationMatrix();
            pathToParent.pop();
        }
        output = output * GetLocalTranslationMatrix();
        return output;
    }

    Mat3 Transform::GetGlobalTransformMatrix()
    {
        Mat3 output = {
            1.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,
            0.0f,0.0f,1.0f
        };
        std::stack<Transform*> pathToParent = getPathToParent();
        while (!pathToParent.empty())
        {
            Transform* currentParent = pathToParent.top();
            output = output * currentParent->GetLocalTransformMatrix();
            pathToParent.pop();
        }
        output = output * GetLocalTransformMatrix();
        return output;
    }

    void Transform::SetLocalPosition(Vec2 newPosition)
    {
        this->localPosition = newPosition;
    }

    void Transform::SetLocalRotation(float newRotation)
    {
        localRotation = newRotation;
        localRotation = fmod(localRotation, 360.0f);
    }

    void Transform::SetLocalScale(Vec2 newScale)
    {
        localScale = newScale;
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
        parentTransform = nullptr;

        if (maintainLocation)
        {
            localPosition = globalPos;
            localRotation = globalRot;
        }
    }

    void Transform::AddChild(Transform* child)
    {
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