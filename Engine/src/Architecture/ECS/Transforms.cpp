#include <algorithm>
#include <cmath>

#include "Transforms.h"
#include "ECS/Temp_Vec2.hpp"
#include "Entity.hpp"

namespace IonixEngine
{
	Transform::Transform(Entity* parentEntity) :
		position(Vec2{ 0.0f,0.0f }),
		rotation(0.0f),
		parentTransform(nullptr),
		entity(parentEntity)
	{
		childTransforms = std::vector<Transform*>();
	}

	Vec2 Transform::GetGlobalPosition()
	{
		Vec2 position;
		Transform* parent = parentTransform;
		while (parent) {
			Vec2 parentPos = parentTransform->position;
			float parentRot = parentTransform->rotation;

			parentPos = Vec2Rotate(parentPos, parentRot);

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
		while (parent)
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
		float accumulator = 0.0f;
		Transform* parent = parentTransform;
		while (parent)
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

	void Transform::SetLocalPosition(Vec2 localPosition)
	{
		position = localPosition;
	}

	void Transform::SetLocalRotation(float localRotation)
	{
		rotation = localRotation;
	}

	//maintainLocation = true will attempt to keep the transforms in the same place
	void Transform::SetParent(Transform* parent, bool maintainLocation)
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
	void Transform::RemoveParent(bool maintainLocation)
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
