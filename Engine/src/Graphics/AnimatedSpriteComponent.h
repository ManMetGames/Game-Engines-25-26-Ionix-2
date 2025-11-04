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
		int endFrame;
		bool isReversing;
		int rows, cols;
		int spriteWidth, spriteHeight;
		int currentRow, currentCol;

		enum playbackOptions {
			FORWARD,
			BACKWARD,
			FORWARDANDBACKWARD,
			PLAYONCE,
			ONEFRAME
		};

		enum playbackOptions playbackMode;

		bool reverseOnEnd;

	public:
		AnimatedSpriteComponent(Entity* entity, std::string alias, int zedOrder);
		virtual void Render(RenderData* data) override;

		void calculateTotalFrames();

		void getFrame();
		void setEndFrame(int x);

		void setReverseOnEnd(bool x);
		void setIsLooping(bool x);
		void setPlaybackMode(enum playbackOptions x);
		void setCurrentFrame(int x);
		void setRows(int x);
		void setCols(int x);
		void setSpriteWidth(int x);
		void setSpriteHeight(int x);
	};
}