#include <algorithm>

#include "Transforms.h"

namespace IonixEngine
{
	Transform::Transform(Entity& parentEntity) :
		position(Vec2{ 0.0f,0.0f }),
		rotation(0.0f),
		parentTransform(nullptr),
		parentEntity(parentEntity)
	{
		childTransforms = std::vector<Transform*>();
	}

	Vec2 Transform::GetGlobalPosition()
	{
		Vec2 position;
		Transform* parent = parentTransform;
		while (parent != nullptr)
		{
			Vec2 parentPos = parentTransform->position;
			float parentRot = parentTransform->rotation;

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

			position.x += parentPos.x;
			position.y += parentPos.y;
			parent = parent->parentTransform;
		}
		Vec2 local = GetLocalPosition();
		position.x += local.x;
		position.y += local.y;

		return position;
	}

	float Transform::GetGlobalRotation()
	{
		Transform* parent = parentTransform;
		float rot = 0.0f;
		while (parent != nullptr)
		{
			rot += parent->rotation;
			parent = parent->parentTransform;
		}
		rot += rotation;
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

	void Transform::SetGlobalPosition(Vec2 transform)
	{
		Vec2 parentPos = parentTransform->GetGlobalPosition();
		float parentRot = parentTransform->GetGlobalRotation();

		float xOffset = transform.x - parentPos.x;
		float yOffset = transform.y - parentPos.y;

		float mag = sqrt(pow(xOffset, 2) + pow(yOffset, 2));

		float xAngle = asin(xOffset / mag);
		//float yAngle = acos(yOffset / mag);

		xAngle -= parentRot;

		float x = mag * sin(xAngle);
		float y = mag * cos(xAngle);

		position = { x,y };
	}

	void Transform::SetGlobalRotation(float rot)
	{
		float accumulator = 0.0f;
		Transform* parent = parentTransform;
		while (parent != nullptr)
		{
			accumulator += parent->rotation;
			parent = parent->parentTransform;
		}
		rotation = rot - accumulator;
	}

	Vec2 Transform::GetLocalPosition()
	{
		return position;
	}

	float Transform::GetLocalRotation()
	{
		return rotation;
	}

	//maintainLocation = true will attempt to keep the transforms in the same place
	void Transform::SetParent(Transform* parent, bool maintainLocation = true)
	{
		if (parentTransform != nullptr) { RemoveParent(); }

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
	void Transform::RemoveParent(bool maintainLocation = true)
	{
		Vec2 globalPos = GetGlobalPosition();
		float globalRot = GetGlobalRotation();

		parentTransform->RemoveChild(this);
		parentTransform = nullptr;

		if (maintainLocation)
		{
			position = globalPos;
			rotation = globalRot;
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