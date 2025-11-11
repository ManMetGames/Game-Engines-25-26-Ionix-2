#pragma once
#include "Architecture/ECS/Entity.hpp"
#include "Architecture/ECS/Component.hpp"
#include "SDL.h"
#include "Architecture/TextureManager/TextureManager.h"
#include <iostream>

//DO NOT USE, will delete in the lab since last time deleting files caused github to S&!% itself

namespace IonixEngine {

	class OldSpriteComponent : public Component {
		SDL_Texture* texture;
		int zOrder;
		float width, height;
	public:
		OldSpriteComponent(Entity* entity, std::string alias, int zedOrder);
		virtual void Render(RenderData* data) override;
	};
}