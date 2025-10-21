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
		Entity& parentEntity;

		Vec2 position;
		float rotation;

	public:


		Transform(Entity& parentEntity);
		Vec2 GetGlobalPosition();
		float GetGlobalRotation();

		void SetGlobalPosition(Vec2 transform);
		void SetGlobalRotation(float rot);

		Vec2 GetLocalPosition();
		float GetLocalRotation();

		void SetParent(Transform* parent, bool maintainLocation);
		void RemoveParent(bool maintainLocation);

		void AddChild(Transform* child);
		bool RemoveChild(Transform* child);
		bool RemoveChild(int index);
	};
}