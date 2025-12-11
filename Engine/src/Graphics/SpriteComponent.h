#pragma once
#include "Architecture/ECS/Entity.hpp"
#include "Architecture/ECS/Component.hpp"
#include "SDL.h"
#include "Architecture/TextureManager/TextureManager.h"
#include <iostream>
#include <box2d.h>

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
		bool isReversing; // kai is smelly (its true)
		int rows, cols;
		int spriteWidth, spriteHeight;
		int currentRow, currentCol;
		float spriteAngle;
		int renderLayer;
		float timer = 0.0f;
		float tickRate;
		b2Vec2 boxColliderSize;
		
		// Color tint (default white = no tint)
		Uint8 colorR = 255;
		Uint8 colorG = 255;
		Uint8 colorB = 255;

		enum playbackOptions playbackMode;

	public:
		SpriteComponent(Entity* entity, std::string alias, int width, int height, int zedOrder);
		SpriteComponent(Entity* entity, uint32_t hash, int width, int height, int zedOrder);
		virtual void Render(RenderData* data) override;
		virtual void Update(float deltaTime) override;

		void calculateTotalFrames();

		//utility functions

		void changeTexture(std::string alias, int rows, int cols, int spriteWidth, int spriteHeight);
		void initialiseSpritesheet();
		void setAnimation(int rows, int cols, int spriteWidth, int spriteHeight);

		//Setters
		void setEndFrame(int x);
		void setPlaybackMode(enum playbackOptions x);
		void setCurrentFrame(int x);
		void setRows(int x);
		void setCols(int x);
		void setRowsAndCols(int Rows, int Cols);
		void setSpriteWidth(int x);
		void setSpriteHeight(int x);
		void setZedOrder(int x);
		void setWidth(int x);
		void setHeight(int x);
		void setAngle(float angle);
		void setTickRate(float x);
		void setBoxColliderSize(b2Vec2 newSize);
		void setColor(Uint8 r, Uint8 g, Uint8 b);

		//Getters
		IonixEngine::playbackOptions getPlaybackMode();
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
		float getAngle();
		int getTickRate();
		b2Vec2 getBoxColliderSize();
	};
}