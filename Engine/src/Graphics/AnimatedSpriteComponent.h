#pragma once
#include "Architecture/ECS/Entity.hpp"
#include "Architecture/ECS/Component.hpp"
#include "SDL.h"
#include "Architecture/TextureManager/TextureManager.h"
#include <iostream>


namespace IonixEngine {

	enum playbackOptions {
		FORWARD,
		BACKWARD,
		FORWARDANDBACKWARD,
		PLAYONCE,
		ONEFRAME
	};

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

		enum playbackOptions playbackMode;

	public:
		AnimatedSpriteComponent(Entity* entity, std::string alias, int zedOrder);
		virtual void Render(RenderData* data) override;

		void calculateTotalFrames();

		//Setters
		void setEndFrame(int x);
		void setPlaybackMode(enum playbackOptions x);
		void setCurrentFrame(int x);
		void setRows(int x);
		void setCols(int x);
		void setSpriteWidth(int x);
		void setSpriteHeight(int x);
		void setZedOrder(int x);

		//Getters
		playbackOptions getPlaybackMode();
		int getCurrentFrame();
		int getEndFrame();
		int getRows();
		int getCols();
		int getSpriteWidth();
		int getSpriteHeight();
		int getZedOrder();
		int getTotalFrames();
		int getCurrentCol();
		int getCurrentRow();
	};
}