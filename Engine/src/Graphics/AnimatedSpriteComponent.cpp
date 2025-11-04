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
		playbackMode = ONEFRAME;
		
		//calculating the total frame count
		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
		totalFrames =  size.x / size.y;

		switch (playbackMode) {
		case FORWARD: case FORWARDANDBACKWARD: case PLAYONCE:
			endFrame = totalFrames - 1;
			currentFrame = 0;
			break;
		case BACKWARD:
			endFrame = 0;
			currentFrame = totalFrames - 1;
			break;
		case ONEFRAME:
			currentFrame = 0;
			break;
		}

		//setCurrentFrame(2);
	}

	void AnimatedSpriteComponent::Render(RenderData* data)
	{
		// src is the indivudal frame we're rendering
		src.x = size.y * currentFrame;
		src.y = 0;
		src.w = size.y;
		src.h = size.y;

		//create and send render data to the render queue
		data->queue->AddToQueue(RenderCall {
			texture,
			//SDL_Rect { (int) (entity->position.x - width / 2), (int) (entity->position.y - height / 2), (int) width, (int) height },
			SDL_Rect { (int) (entity->position.x), (int) (entity->position.y), (int) width, (int) height },
			SDL_Rect { src.x, src.y, src.w, src.h },
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
				break;
			case false:
				currentFrame++;
				break;
			}
		}

		else {
			switch (playbackMode) {
			case FORWARD:
				currentFrame = 0;
				break;
			case BACKWARD:
				currentFrame = totalFrames - 1;
				break;
			case FORWARDANDBACKWARD:
				if (isReversing) {

					endFrame = totalFrames - 1;
				}
				else {
					endFrame = 0;
				}
				isReversing = !isReversing;
				break;
			case PLAYONCE: case ONEFRAME:
				break;
			}
		}
	}

	void AnimatedSpriteComponent::setEndFrame(int x) { endFrame = x; }

	void AnimatedSpriteComponent::setIsLooping(bool x) { looping = x; }
	void AnimatedSpriteComponent::setPlaybackMode(enum playbackOptions x) { playbackMode = x; }
	void AnimatedSpriteComponent::setCurrentFrame(int x) { if (!(x > totalFrames)) { currentFrame = x; } }
	void AnimatedSpriteComponent::setReverseOnEnd(bool x) { reverseOnEnd = x; }
	void AnimatedSpriteComponent::setRows(int x) { rows = x; }
	void AnimatedSpriteComponent::setCols(int x) { cols = x; }
}
