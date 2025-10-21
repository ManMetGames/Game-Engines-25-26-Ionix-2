#pragma once
#include "Architecture/ECS/Entity.hpp"
#include "Architecture/ECS/Component.hpp"
#include "SDL.h"
#include "Architecture/TextureManager/TextureManager.h"
#include <iostream>

namespace IonixEngine {

	class SpriteComponent : public Component {
		std::string filePath;
		std::string spriteName;
		int zOrder;
	public:
		SpriteComponent(Entity* entity, std::string filePath, std::string name, int zedOrder);
		void SetTexture(std::string newName, std::string newAlias);
	};
}