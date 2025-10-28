#pragma once

//#include <cmath>
#include <vector>
#include <stack>
#include <string>
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

		Vec2 position;
		float rotation;

		std::stack<Transform*> getPathToParent();

	public:
		Transform(Entity* parentEntity);
		Vec2 GetGlobalPosition();
		float GetGlobalRotation();

		void SetGlobalPosition(Vec2 transform);
		void SetGlobalRotation(float rot);

		Vec2 GetLocalPosition();
		float GetLocalRotation();

		void SetLocalPosition(Vec2 transform);
		void SetLocalRotation(float rot);


		void SetParent(Transform* parent, bool maintainLocation = true);
		void RemoveParent(bool maintainLocation = true);

		void AddChild(Transform* child);
		bool RemoveChild(Transform* child);
		bool RemoveChild(int index);

		void GetRotationsAll();
	};
}
