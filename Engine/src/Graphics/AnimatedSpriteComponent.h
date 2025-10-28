#pragma once
#include "Architecture/ECS/Entity.hpp"
#include "Architecture/ECS/Component.hpp"
#include "SDL.h"
#include "Architecture/TextureManager/TextureManager.h"
#include <iostream>

namespace IonixEngine {

	class AnimatedSpriteComponent : public Component {
		// for now we will assume all spritesheets are 1 row	and of uniform size (32x32, 64x64 etc.)
		// will improve later on	
		SDL_Texture* texture;
		SDL_Point size;
		SDL_Rect src;
		int zOrder;
		float width, height;
		int totalFrames;
		int currentFrame;

		bool reverseOnEnd;
		bool isReversing;
	public:
		AnimatedSpriteComponent(Entity* entity, std::string alias, int zedOrder);
		virtual void Render(RenderData* data) override;
		void getFrame();
	};
}