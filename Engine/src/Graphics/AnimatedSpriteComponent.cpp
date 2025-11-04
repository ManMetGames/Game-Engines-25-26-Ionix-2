#include "AnimatedSpriteComponent.h"
#include <Graphics/QueueRenderer.h>

namespace IonixEngine {
	AnimatedSpriteComponent::AnimatedSpriteComponent(Entity* entity, std::string alias, int zedOrder) : Component(entity, false, true, false) {
		texture = IonixEngine::TextureManager::Get().GetTexture(alias).GetTexture(); //adding sprite image file to the texture manager
		zOrder = zedOrder;
		width = 200;
		height = 200;
<<<<<<< Updated upstream
=======
		isReversing = false;

		playbackMode = PLAYONCE; //the default playback mode, can be changed via the appropriate setter

		rows = 1; //default values, user can set the rows and columns via the appropriate setters
		cols = 1;

		spriteWidth = 32; //default sizes, user can set the width and height of sprites via the appropriate setters
		spriteHeight = 32;
>>>>>>> Stashed changes
		
		//calculating the total frame count
		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
<<<<<<< Updated upstream
		totalFrames =  size.x / size.y;
		currentFrame = 0;
=======
		calculateTotalFrames();

		switch (playbackMode) { //setting the initial start frame values
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
>>>>>>> Stashed changes
	}

	void AnimatedSpriteComponent::Render(RenderData* data)
	{
		if (currentFrame > totalFrames - 1)
		{
			currentFrame = 0;
		}

<<<<<<< Updated upstream
		// src is the indivudal frame we're rendering
		src.x = size.y * currentFrame;
		src.y = 0;
		src.w = size.y;
		src.h = size.y;
=======
		if (src.x > size.x) { //if this is true, the next frame is off the texture, so we handle appropriately
			currentRow++;
			currentCol = 0;
		}
		if (src.x < 0) { //if the src.x is less han zero, we can assume the image is reversing, therefore we know to move up to the previous row
			currentCol = cols;
			currentRow--;
		}
>>>>>>> Stashed changes

		//create and send render data to the render queue
		data->queue->AddToQueue(RenderCall {
			texture,
<<<<<<< Updated upstream
			SDL_Rect { (int) (entity->position.x - width / 2), (int) (entity->position.y - height / 2), (int) width, (int) height },
			SDL_Rect { src.x, src.y, src.w, src.h },

=======
			SDL_Rect { (int) (entity->position.x), (int) (entity->position.y), (int) width, (int) height },
			SDL_Rect { spriteWidth * currentCol, spriteHeight * currentRow, spriteWidth, spriteHeight },
>>>>>>> Stashed changes
		});


		//This is just here so we can see the animation play at a normal speed
		//THIS WILL BE REMOVED
		//SDL_Delay(60);

		currentFrame++;
	}

<<<<<<< Updated upstream
=======
	void AnimatedSpriteComponent::calculateTotalFrames()
	{
		totalFrames = rows * cols;
	}

	//setters
	void AnimatedSpriteComponent::setEndFrame(int x) { endFrame = x; }
	void AnimatedSpriteComponent::setPlaybackMode(enum playbackOptions x) { playbackMode = x; }
	void AnimatedSpriteComponent::setCurrentFrame(int x) { if (!(x > totalFrames)) { currentFrame = x; } }
	void AnimatedSpriteComponent::setRows(int x) { rows = x; }
	void AnimatedSpriteComponent::setCols(int x) { cols = x; }
	void AnimatedSpriteComponent::setSpriteWidth(int x) { spriteWidth = x; }
	void AnimatedSpriteComponent::setSpriteHeight(int x) { spriteHeight = x; }
	void AnimatedSpriteComponent::setZedOrder(int x) { zOrder = x; }

	//getters
	IonixEngine::AnimatedSpriteComponent::playbackOptions AnimatedSpriteComponent::getPlaybackMode() /*good googly moogly*/ { return playbackOptions(); }
	int AnimatedSpriteComponent::getCurrentFrame(){ return currentFrame; }
	int AnimatedSpriteComponent::getEndFrame() { return endFrame; }
	int AnimatedSpriteComponent::getRows() { return rows; }
	int AnimatedSpriteComponent::getCols() { return cols; }
	int AnimatedSpriteComponent::getSpriteWidth() { return spriteWidth; }
	int AnimatedSpriteComponent::getSpriteHeight() { return spriteHeight; }
	int AnimatedSpriteComponent::getZedOrder() { return zOrder; }
	int AnimatedSpriteComponent::getTotalFrames() { return totalFrames; }
	int AnimatedSpriteComponent::getCurrentCol() { return currentCol; }
	int AnimatedSpriteComponent::getCurrentRow() { return currentRow; }
>>>>>>> Stashed changes
}

/*
TODO
- UNDO COMMENTED FUNCTION IN UI
- multiple rows
- overall make more versatile/usable

*/
