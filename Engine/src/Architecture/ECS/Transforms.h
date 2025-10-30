#pragma once

//#include <cmath>
#include <vector>
#include "Temp_Vec2.hpp"

namespace IonixEngine
{

	class Entity;
	class Transform
	{
	private:
		std::vector<Transform*> childTransforms;
		Transform* parentTransform;
		Entity* entity;

		Vec2 localPosition;
		Vec2 localScale;
		float localRotation;

	public:
		Transform(Entity* parentEntity);
		Vec2 GetGlobalPosition();
		float GetGlobalRotation();

		void SetGlobalPosition(Vec2 transform);
		void SetGlobalRotation(float rot);

		Vec2 GetLocalPosition();
		float GetLocalRotation();

		Mat2 GetRotationMatrix();
		Mat2 GetScaleMatrix();
		Mat2 GetTransformMatrix();

		void SetLocalPosition(Vec2 newPos);
		void SetLocalRotation(float newRot);
		void SetLocalScale(Vec2 newScale);

		void SetParent(Transform* parent, bool maintainLocation = true);
		void RemoveParent(bool maintainLocation = true);

		void AddChild(Transform* child);
		bool RemoveChild(Transform* child);
		bool RemoveChild(int index);
	};
}
