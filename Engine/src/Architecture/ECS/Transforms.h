#pragma once

//#include <cmath>
#include <vector>
#include "Entity.hpp"
#include "Temp_Vec2.hpp"


namespace IonixEngine
{
	class Transform
	{
	private:
		std::vector<Transform*> childTransforms;
		Transform* parentTransform;

	public:
		Vec2 position;
		float rotation;

		Transform();
		Vec2 GetGlobalPosition();
		float GetGlobalRotation();

		void SetParent(Transform* parent);
		void RemoveParent(bool maintainLocation);

		void AddChild(Transform* child);
		bool RemoveChild(Transform* child);
		bool RemoveChild(int index);
	};
}