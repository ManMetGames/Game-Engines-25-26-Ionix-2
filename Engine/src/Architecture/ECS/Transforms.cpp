#include "SDL.h"
#include "Transforms.h"
#include "../ECS/Temp_Vec2.hpp"
#include "../Scene.h"
#include "../../LayerSystem/Layers/SceneLayer.h"
#include "Entity.hpp"

#include <algorithm>
#include <iostream>
#include <cmath>

namespace IonixEngine
{
    //TEMPORARY STAND-IN UNTIL ACTUAL NULL ID IS CREATED
    EntityID nullID = 69420;

    Transform::Transform(Entity* localEntity) :
        //position(Vec2{ 0.0f,0.0f }),
        localRotation(0.0f),
        parentTransform(nullptr),
        rootEntity(localEntity),
        rootEntityId(localEntity->id)
    {
        parentEntityId = nullID;
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
        if (parentTransform) {
            Mat3 globalPos = parentTransform->GetGlobalTransformMatrix();
            //SDL_Log("[Transforms] GlobalPosMatrix = %f, %f", globalPos.c, globalPos.f);
            Mat3 diffMatrix = {
                1.0f,0.0f,-newPosition.x,
                0.0f,1.0f,-newPosition.y,
                0.0f,0.0f,1.0f };
            //SDL_Log("[Transforms] DiffMatrix = %f, %f", diffMatrix.c, diffMatrix.f);

            Mat3 newGlobalPos = globalPos * diffMatrix;
            //SDL_Log("[Transforms] newGlobal pos: %f %f %f    %f %f %f    %f %f %f",newGlobalPos.a, newGlobalPos.b, newGlobalPos.c, newGlobalPos.d, newGlobalPos.e, newGlobalPos.f, newGlobalPos.g, newGlobalPos.h, newGlobalPos.i);
            localPosition = { newGlobalPos.c,newGlobalPos.f };
            //SDL_Log("[Transforms] Setting global position to: %f, %f (Local Pos = %f, %f)", newPosition.x, newPosition.y, newGlobalPos.c, newGlobalPos.f);
        }
        else { localPosition = newPosition; }
    }

    void Transform::SetGlobalRotation(float rot)
    {
        //get global matrix, get negative rot matrix, use both to create a difference matrix
        //use difference matrix to get local rotation
        float radRotation = rot * DEG2RAD;

        if (parentTransform)
        {
            Mat3 globalRot = parentTransform->GetGlobalRotationMatrix();
            Mat3 diffRotationMatrix = {
                cosf(-radRotation),-sinf(-radRotation),0.0f,
                sinf(-radRotation),cosf(-radRotation),0.0f,
                0.0f,0.0f,1.0f };

            diffRotationMatrix = globalRot * diffRotationMatrix;
            //use local setter instead of direct setting, because it has a modulo clamp on it
            SetLocalRotation(asinf(diffRotationMatrix.d) / DEG2RAD);
        }
        else
        {
            SetLocalRotation(rot);
        }
    }

    void Transform::SetGlobalScale(Vec2 scale)
    {
        Mat3 globalScale = GetGlobalScaleMatrix();

        float newX = (scale.x / globalScale.a) * localScale.x;
        float newY = (scale.y / globalScale.e) * localScale.y;

        localScale = Vec2{ newX,newY };
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
            cosf(angle),-sinf(angle),0.0f,
            sinf(angle),cosf(angle),0.0f,
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
            output = output * pathToParent.top()->GetLocalScaleMatrix();
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
            output = output * pathToParent.top()->GetLocalRotationMatrix();
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
            output = output * pathToParent.top()->GetLocalTranslationMatrix();
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
            output = output * pathToParent.top()->GetLocalTransformMatrix();
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
        if (parentEntityId != nullID)
        {
            RemoveParent();
        }

        Vec2 oldPos = GetGlobalPosition();
        float oldRot = GetGlobalRotation();

        parentTransform = parent;
        parentEntityId = parent->rootEntityId;
        parent->AddChild(this);

        if (maintainLocation)
        {
            SetGlobalPosition(oldPos);
            SetGlobalRotation(oldRot);
        }
        std::cout << "blahblah" << std::endl;
    }

    //maintainLocation = true will attempt to keep the transforms in the same place
    void Transform::RemoveParent(bool maintainLocation)
    {
        Vec2 globalPos = GetGlobalPosition();
        float globalRot = GetGlobalRotation();

        if (parentEntityId != nullID)
        {
            parentTransform = nullptr;
            parentEntityId = nullID;
        }
        
        if (maintainLocation)
        {
            localPosition = globalPos;
            localRotation = globalRot;
        }
    }

    void Transform::AddChild(Transform* child)
    {
        //child->parentTransform = this;
        //child->parentEntityId = rootEntityId;

        childEntityIds.push_back(child->rootEntityId);
        childTransforms.push_back(child);

        Entity* rootEntityPointer = LayerScene::CurrentScene()->GetEntityFromID(rootEntityId);
        rootEntityPointer->transform = *this;

        std::cout << "blahblah" << std::endl;
    }

    bool Transform::RemoveChild(Transform* removalChild)
    {
        //auto iterator = std::find(childTransforms.begin(), childTransforms.end(), removalChild);
        //if (iterator != childTransforms.end())
        //{
        //    childTransforms.erase(iterator);
        //    return true;
        //}

        removalChild->parentTransform = nullptr;
        removalChild->parentEntityId = nullID;

        std::vector<Transform*> newChildTransforms;
        std::vector<EntityID> newChildIDs;
        bool output = false;
        for (size_t i = 0; i < childTransforms.size(); i++)
        {
            Transform* currentChild = childTransforms[i];
            if (currentChild != removalChild)
            {
                newChildTransforms.push_back(currentChild);
                newChildIDs.push_back(currentChild->rootEntityId);
            }
            else { output = true; }
        }
        if (output)
        {
            childTransforms = newChildTransforms;
            childEntityIds = newChildIDs;
        }
        return output;
    }

    bool Transform::RemoveChild(int index)
    {
        //size should be largest index +1, therfore if index is smaller, it is valid
        
        if (index < childTransforms.size())
        {
            childTransforms[index]->parentTransform = nullptr;
            childTransforms[index]->parentEntityId = 0;

            childEntityIds.erase(childEntityIds.begin() + index);
            childTransforms.erase(childTransforms.begin() + index);
            return true;
        }
        else { return false; }
    }

    void Transform::RecalculatePointers()
    {
        std::cout << "Recalculating pointers..." << std::endl;

        Entity* rootEntityPointer = LayerScene::CurrentScene()->GetEntityFromID(rootEntityId);
        std::cout << "Got root entity pointer..." << std::endl;

        rootEntity = rootEntityPointer;
        std::cout << "set rootentity to new pointer..." << std::endl;

        rootEntityPointer->transform = *this;
        std::cout << "set rootentity transform to this..." << std::endl;

        //attempt 2, reset own pointers, do not consider pointers to self
        if (parentEntityId != nullID)
        {
            std::cout << "Recalculating parent pointer..." << std::endl;
            Entity* parentEntityPointer = LayerScene::CurrentScene()->GetEntityFromID(parentEntityId);
            parentTransform = &parentEntityPointer->transform;
            parentEntityPointer->transform = *parentTransform;
        }

        for (size_t i = 0; i < childTransforms.size(); i++)
        {
            std::cout << "Recalculating child pointer "<< i << "..." << std::endl;
            Entity* childEntityPointer = LayerScene::CurrentScene()->GetEntityFromID(childEntityIds[i]);
            childTransforms[i] = &childEntityPointer->transform;
            childEntityPointer->transform = *childTransforms[i];
        }
        
        //branch for re-setting parent's pointers
        //go to parent transform, identify pointer to this transform, then perform reassignment
        //if (parentEntityId != nullID)
        //{
        //    //need pointers to the real data, or edits will be ineffective
        //    Entity* parentEntityPointer = LayerScene::CurrentScene()->GetEntityFromID(parentEntityId);
        //    std::vector<Transform*>* parentChildPtrs = &parentEntityPointer->transform.childTransforms;
        //    std::vector<EntityID> parentChildIds = parentEntityPointer->transform.childEntityIds;
        //    for (size_t i = 0; i < parentChildIds.size(); i++)
        //    {
        //        EntityID id = parentChildIds.at(i);
        //        if (id == parentEntityId)
        //        {
        //            parentChildPtrs->at(i) = this;
        //        }
        //    }
        //}
        //
        //for (size_t i = 0; i < childEntityIds.size(); i++)
        //{
        //    EntityID childEntityId = childEntityIds.at(i);
        //    Entity* childEntityPtr = LayerScene::CurrentScene()->GetEntityFromID(childEntityId);
        //    childEntityPtr->transform.parentTransform = this;
        //}
        
        //rootEntity->transform = *this;
        //parentTransform = &rootEntity->transform;

        //if (parentEntityId != 69420)
        //{
        //    Entity* parentEntity = LayerScene::CurrentScene()->GetEntityFromID(parentEntityId);
        //    //parentEntity->transform = *this;
        //    parentTransform = &parentEntity->transform;
        //    for (int i = 0; i < parentTransform->childTransforms.size(); i++)
        //    {
        //        if (parentTransform->childEntityIds[i]== rootEntityId)
        //        {
        //            parentTransform->childTransforms[i] = this;
        //        }

        //    }
        //}
        //else
        //{
        //    parentTransform = nullptr;
        //    
        //}

        //std::cout << "Reassigning transforms of child entities..." << std::endl;
        //for (int i = 0; i < childTransforms.size(); i++)
        //{
        //    std::cout << i << std::endl;
        //    Entity* childEntity = LayerScene::CurrentScene()->GetEntityFromID(childEntityIds[i]);
        //    //childEntity->transform = *this;
        //    childTransforms[i] = &childEntity->transform;
        //    childTransforms[i]->parentTransform = this;
        //}
        std::cout << "Transform pointers recalculated..." << std::endl;
    }

}