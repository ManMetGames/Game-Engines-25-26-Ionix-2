#pragma once
#include "Architecture/ECS/Entity.hpp"
#include "Architecture/ECS/Component.hpp"
#include "SDL.h"
#include "Architecture/TextureManager/TextureManager.h"
#include "UI/UIManager.h"
#include <iostream>

namespace IonixEngine {

	class AnimatedSpriteComponent : public Component {
		// for now we will assume all spritesheets are 1 row	and of uniform size (32x32, 64x64 etc.)
		// will improve later on	
		SDL_Texture* texture;
		SDL_Point size;
		SDL_Rect src;
		SDL_Surface* image;
		int zOrder;
		float width, height;
		int totalFrames;
		int currentFrame;

		bool reverseOnEnd;
		bool isReversing;

		Uint32 pixels;
		Uint8 RED;
		Uint8 GREEN;
		Uint8 BLUE;
		Uint8 ALPHA;
		
	public:
		AnimatedSpriteComponent(Entity* entity, std::string alias, int zedOrder, int width, int height);
		virtual void Render(RenderData* data) override;
		void getFrame();
		void SpriteSize(int x, int y);
		void SpriteRotation(float angleInDegrees);
		void SetColours(int red, int green, int blue, int alpha);
	};
}