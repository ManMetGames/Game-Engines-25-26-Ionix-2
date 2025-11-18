#include "SpriteComponent.h"
#include <Graphics/QueueRenderer.h>

namespace IonixEngine {

	//Constructors

	SpriteComponent::SpriteComponent(Entity* entity, std::string alias, int x, int y, int zedOrder) : Component(entity, false, true, false) {
		texture = IonixEngine::TextureManager::Get().GetTexture(alias).GetTexture(); //adding sprite image file to the texture manager
		zOrder = zedOrder;
		width = x;
		height = y;
		isReversing = false;
		playbackMode = playbackOptions::FORWARD;

		rows = 1; //default spritesheet size, can be changed in appropriate setters
		cols = 5;



		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);

		spriteWidth = size.x; //default, can be change in setters
		spriteHeight = size.y;
		calculateTotalFrames();

		initialiseSpritesheet();
	}

	SpriteComponent::SpriteComponent(Entity* entity, uint64_t hash, int x, int y, int zedOrder) : Component(entity, false, true, false) {
		texture = IonixEngine::TextureManager::Get().GetTexture(hash).GetTexture(); //adding sprite image file to the texture manager
		zOrder = zedOrder;
		width = x; //size of the sprite
		height = y;
		isReversing = false;
		playbackMode = playbackOptions::FORWARD;

		rows = 1; //default spritesheet size, can be changed in appropriate setters
		cols = 5;


		spriteWidth = 32; //default, can be change in setters
		spriteHeight = 32;

		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);

		spriteWidth = size.x; //default, can be change in setters
		spriteHeight = size.y;
		calculateTotalFrames();

		initialiseSpritesheet();
	}

	void SpriteComponent::Render(RenderData* data)
	{
		// src is the indivudal frame we're rendering
		src.x = spriteWidth * currentCol;

		if (src.x > size.x) {
			currentRow++;
			currentCol = 0;
		}
		if (src.x < 0) {
			currentCol = cols;
			currentRow--;
		}

		//create and send render data to the render queue
		data->queue->AddToQueue(RenderCall {
			texture,
			SDL_Rect { (int) (entity->position.x), (int) (entity->position.y), (int) width, (int) height },
			SDL_Rect { spriteWidth * currentCol, spriteHeight * currentRow, spriteWidth, spriteHeight },
		});


		//This is just here so we can see the animation play at a normal speed
		//THIS WILL BE REMOVED
		//SDL_Delay(60);


		if ((currentFrame != endFrame) && playbackMode != playbackOptions::ONEFRAME)
		{
			switch (isReversing)
			{
			case true:
				currentFrame--;
				currentCol--;
				break;
			case false:
				currentFrame++;
				currentCol++;
				break;
			}
		}

		else {
			switch (playbackMode) {
			case playbackOptions::FORWARD:
				currentFrame = 0;
				currentCol = 0;
				currentRow = 0;
				break;
			case playbackOptions::BACKWARD:
				currentFrame = totalFrames;
				currentCol = cols - 1;
				currentRow = rows - 1;
				break;
			case playbackOptions::FORWARDANDBACKWARD:
				if (isReversing) {
					isReversing = false;
					currentFrame = 0;
					endFrame = totalFrames;
				}
				else {
					isReversing = true;
					endFrame = 0;
					currentFrame = totalFrames;
				}
				break;
			case playbackOptions::PLAYONCE: case playbackOptions::ONEFRAME:
				break;
			}
		}
	}

	void SpriteComponent::calculateTotalFrames()
	{
		totalFrames = rows * cols;
		endFrame = totalFrames - 1;
	}

	void SpriteComponent::changeTexture(std::string alias)
	{
		texture = IonixEngine::TextureManager::Get().GetTexture(alias).GetTexture();
	}

	void SpriteComponent::initialiseSpritesheet()
	{
		switch (playbackMode) {
		case playbackOptions::FORWARD: case playbackOptions::FORWARDANDBACKWARD: case playbackOptions::PLAYONCE:
			endFrame = totalFrames;
			currentFrame = 0;
			currentRow = 0; //0 indexed
			currentCol = 0; //0 indexed
			break;
		case playbackOptions::BACKWARD:
			isReversing = true;
			endFrame = 0;
			currentFrame = totalFrames;
			currentCol = cols - 1;
			currentRow = rows - 1;
			break;
		case playbackOptions::ONEFRAME:
			currentFrame = 0;
			break;
		}
	}

	//setters
	void SpriteComponent::setEndFrame(int x) { endFrame = x; }
	void SpriteComponent::setPlaybackMode(enum playbackOptions x) { playbackMode = x; }
	void SpriteComponent::setCurrentFrame(int x) { if (!(x > totalFrames)) { currentFrame = x; } }
	void SpriteComponent::setRows(int x) { rows = x; }
	void SpriteComponent::setCols(int x) { cols = x; }
	void SpriteComponent::setSpriteWidth(int x) { spriteWidth = x; }
	void SpriteComponent::setSpriteHeight(int x) { spriteHeight = x; }
	void SpriteComponent::setZedOrder(int x) { zOrder = x; }
	void SpriteComponent::setWidth(int x) { width = x; }
	void SpriteComponent::setHeight(int x) { height = x; }

	//getters
	IonixEngine::playbackOptions SpriteComponent::getPlaybackMode() /*oh lawd he big*/ { return playbackOptions(); }
	int SpriteComponent::getCurrentFrame() { return currentFrame; }
	int SpriteComponent::getEndFrame() { return endFrame; }
	int SpriteComponent::getRows() { return rows; }
	int SpriteComponent::getCols() { return cols; }
	int SpriteComponent::getSpriteWidth() { return spriteWidth; }
	int SpriteComponent::getSpriteHeight() { return spriteHeight; }
	int SpriteComponent::getZedOrder() { return zOrder; }
	int SpriteComponent::getTotalFrames() { return totalFrames; }
	int SpriteComponent::getCurrentCol() { return currentCol; }
	int SpriteComponent::getCurrentRow() { return currentRow; }
	int SpriteComponent::getWidth() { return width; }
	int SpriteComponent::getHeight() { return height; }
}
