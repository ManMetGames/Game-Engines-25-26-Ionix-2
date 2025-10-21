#pragma once
#include "Architecture/ECS/Entity.hpp"
#include "Architecture/ECS/Component.hpp"
#include "SDL.h"
#include "Architecture/TextureManager/TextureManager.h"
#include <iostream>

namespace IonixEngine {

	class SpriteComponent : public Component {
		SDL_Texture* texture;
		int zOrder;
		float width, height;
	public:
		SpriteComponent(Entity* entity, std::string alias, int zedOrder);
		virtual void Render(RenderData* data) override;
	};
}