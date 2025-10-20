#include <algorithm>

#include "Transforms.h"

namespace IonixEngine
{
	Transform::Transform() :
		position(Vec2{ 0.0f,0.0f }),
		rotation(0.0f),
		parentTransform(nullptr)
	{
		childTransforms = std::vector<Transform*>();
	}

	Vec2 Transform::GetGlobalPosition()
	{
        if (parentTransform != nullptr)
        {
            float parentRot = parentTransform->GetGlobalRotation();
            Vec2 parentPos = parentTransform->GetGlobalPosition();

            float angle = parentRot;

            //turns out there's a lot of edge cases here
            //angle additions if x & y are non-zero
            if (position.x != 0.0f && position.y != 0.0f)
            {
                if (position.y > 0.0f)
                {
                    angle += atan(position.x / position.y);
                }
                else
                {
                    angle += (atan((position.x / position.y)) + 180.0f);
                }
            }
            //angle additions if either x or y is zero
            else if (position.y == 0.0f)
            {
                if (position.x > 0.0f)
                {
                    angle += 90.0f;
                }
                else
                {
                    angle -= 90.0f;
                }
            }
            else if (position.x == 0.0f && position.y < 0.0f)
            {
                angle += 180.0f;
            }
            //apparently, there's no action taken if x = 0 and y > 0


            float mag = sqrt(pow(position.x, 2) + pow(position.y, 2));

            float x = mag * sin(angle);
            float y = mag * cos(angle);

            x += parentPos.x;
            y += parentPos.y;

            return Vec2{ x,y };
        }
        else
        {
            return position;
        }
	}

	float Transform::GetGlobalRotation()
	{
		if (parentTransform != nullptr)
		{
			return parentTransform->GetGlobalRotation() + rotation;
		}
		else
		{
			return rotation;
		}
	}

    //Current transform will be displaced by this operation!
    void Transform::SetParent(Transform* parent)
    {
        if (parentTransform != nullptr) { RemoveParent(); }

        /*Vec2 parentPos = parent->GetGlobalPosition();
        float parentRot = parent->GetGlobalRotation();*/

        parent->AddChild(this);
        parentTransform = parent;
    }

    //maintainLocation = true will attempt to keep the transforms in the same place
    void Transform::RemoveParent(bool maintainLocation = true)
    {
        if (maintainLocation)
        {
            position = GetGlobalPosition();
            rotation = GetGlobalRotation();
        }

        parentTransform->RemoveChild(this);
        parentTransform = nullptr;
    }

    void Transform::AddChild(Transform* child)
    {
        childTransforms.push_back(child);
    }

    bool Transform::RemoveChild(Transform* child)
    {
        auto thing = std::find(childTransforms.begin(), childTransforms.end(), child);
        if (thing != childTransforms.end())
        {
            childTransforms.erase(thing);
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