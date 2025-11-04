#include "AnimatedSpriteComponent.h"
#include <Graphics/QueueRenderer.h>

namespace IonixEngine {
	AnimatedSpriteComponent::AnimatedSpriteComponent(Entity* entity, std::string alias, int zedOrder) : Component(entity, false, true, false) {
		texture = IonixEngine::TextureManager::Get().GetTexture(alias).GetTexture(); //adding sprite image file to the texture manager
		zOrder = zedOrder;
		width = 200; //size of the sprite
		height = 200;
		isReversing = false;
		reverseOnEnd = false;
		playbackMode = FORWARDANDBACKWARD;

		rows = 2;
		cols = 5;


		spriteWidth = 32;
		spriteHeight = 32;
		
		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);

		//totalFrames =  size.x / size.y;
		calculateTotalFrames();

		switch (playbackMode) {
		case FORWARD: case FORWARDANDBACKWARD: case PLAYONCE:
			endFrame = totalFrames;
			currentFrame = 0;
			currentRow = 0; //0 indexed
			currentCol = 0; //0 indexed
			break;
		case BACKWARD:
			isReversing = true;
			endFrame = 0;
			currentFrame = totalFrames;
			currentCol = cols - 1;
			currentRow = rows - 1;
			break;
		case ONEFRAME:
			currentFrame = 0;
			break;
		}
	}

	void AnimatedSpriteComponent::Render(RenderData* data)
	{
		// src is the indivudal frame we're rendering
		src.x = spriteWidth * currentCol;

		if (src.x > size.x) {
			currentRow++;
			currentCol = 0;
		}
		if (src.x < 0) {
			currentCol = cols - 1;
			currentRow--;
		}

		//create and send render data to the render queue
		data->queue->AddToQueue(RenderCall {
			texture,
			//SDL_Rect { (int) (entity->position.x - width / 2), (int) (entity->position.y - height / 2), (int) width, (int) height },
			SDL_Rect { (int) (entity->position.x), (int) (entity->position.y), (int) width, (int) height },
			SDL_Rect { spriteWidth * currentCol, spriteHeight * currentRow, spriteWidth, spriteHeight },
		});


		//This is just here so we can see the animation play at a normal speed
		//THIS WILL BE REMOVED
		SDL_Delay(60);


		if ((currentFrame != endFrame) && playbackMode != ONEFRAME)
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
			case FORWARD:
				currentFrame = 0;
				currentCol = 0;
				currentRow = 0;
				break;
			case BACKWARD:
				currentFrame = totalFrames;
				currentCol = cols - 1;
				currentRow = rows - 1;
				break;
			case FORWARDANDBACKWARD:
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
			case PLAYONCE: case ONEFRAME:
				break;
			}
		}
	}

	void AnimatedSpriteComponent::calculateTotalFrames()
	{
		totalFrames = rows * cols;
	}

	//setters
	void AnimatedSpriteComponent::setEndFrame(int x) { endFrame = x; }
	void AnimatedSpriteComponent::setIsLooping(bool x) { looping = x; }
	void AnimatedSpriteComponent::setPlaybackMode(enum playbackOptions x) { playbackMode = x; }
	void AnimatedSpriteComponent::setCurrentFrame(int x) { if (!(x > totalFrames)) { currentFrame = x; } }
	void AnimatedSpriteComponent::setReverseOnEnd(bool x) { reverseOnEnd = x; }
	void AnimatedSpriteComponent::setRows(int x) { rows = x; }
	void AnimatedSpriteComponent::setCols(int x) { cols = x; }
	void AnimatedSpriteComponent::setSpriteWidth(int x) { spriteWidth = x; }
	void AnimatedSpriteComponent::setSpriteHeight(int x) { spriteHeight = x; }
}
