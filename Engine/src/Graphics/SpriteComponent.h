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
	class SpriteComponent : public Component {
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
		SpriteComponent(Entity* entity, std::string alias, int width, int height, int zedOrder);
		SpriteComponent(Entity* entity, uint32_t hash, int width, int height, int zedOrder);
		virtual void Render(RenderData* data) override;

		void calculateTotalFrames();

		//utility functions

		void changeTexture(std::string alias);
		void initialiseSpritesheet();

		//Setters
		void setEndFrame(int x);
		void setPlaybackMode(enum playbackOptions x);
		void setCurrentFrame(int x);
		void setRows(int x);
		void setCols(int x);
		void setSpriteWidth(int x);
		void setSpriteHeight(int x);
		void setZedOrder(int x);
		void setWidth(int x);
		void setHeight(int x);

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
		int getWidth();
		int getHeight();
	};
}