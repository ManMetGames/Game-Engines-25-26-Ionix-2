#pragma once

#include "Temp_Vec2.hpp"

//#include <cmath>
#include <vector>
#include <stack>

namespace IonixEngine
{
	typedef uint64_t EntityID;
	class Entity;
	class Transform
	{
	private:
		std::vector<Transform*> childTransforms;
		std::vector<EntityID> childEntityIds;

		Transform* parentTransform;
		EntityID parentEntityId;

		Entity* rootEntity;
		EntityID rootEntityId;

		Vec2 localPosition;
		Vec2 localScale;
		float localRotation;

		std::stack<Transform*> getPathToParent();

	public:
		Transform(Entity* localEntity);
		Vec2 GetGlobalPosition();
		float GetGlobalRotation();
		Vec2 GetGlobalScale();

		void SetGlobalPosition(Vec2 transform);
		void SetGlobalRotation(float rot);
		void SetGlobalScale(Vec2 scale);

		Vec2 GetLocalPosition();
		float GetLocalRotation();
		Vec2 GetLocalScale();

		Mat3 GetLocalScaleMatrix();
		Mat3 GetLocalRotationMatrix();
		Mat3 GetLocalTranslationMatrix();
		Mat3 GetLocalTransformMatrix();

		Mat3 GetGlobalScaleMatrix();
		Mat3 GetGlobalRotationMatrix();
		Mat3 GetGlobalTranslationMatrix();
		Mat3 GetGlobalTransformMatrix();

		void SetLocalPosition(Vec2 newPos);
		void SetLocalRotation(float newRot);
		void SetLocalScale(Vec2 newScale);

		void SetParent(Transform* parent, bool maintainLocation = true);
		void RemoveParent(bool maintainLocation = true);

		void AddChild(Transform* child);
		bool RemoveChild(Transform* removalChild);
		bool RemoveChild(int index);

		void RecalculatePointers();
	};
}