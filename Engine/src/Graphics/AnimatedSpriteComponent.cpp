#include "AnimatedSpriteComponent.h"
#include <Graphics/QueueRenderer.h>

namespace IonixEngine {
	AnimatedSpriteComponent::AnimatedSpriteComponent(Entity* entity, std::string alias, int zedOrder, int w, int h) : Component(entity, false, true, false) {
		texture = IonixEngine::TextureManager::Get().GetTexture(alias).GetTexture(); //adding sprite image file to the texture manager
		zOrder = zedOrder;
		width = w;
		height = h;
		
		//calculating the total frame count
		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
		totalFrames =  size.x / size.y;
		
		currentFrame = 0;
	}

	void AnimatedSpriteComponent::Render(RenderData* data)
	{
		if (currentFrame > totalFrames - 1)
		{
			currentFrame = 0;
		}

		// src is the indivudal frame we're rendering
		src.x = size.y * currentFrame;
		src.y = 0;
		src.w = size.y;
		src.h = size.y;

		//create and send render data to the render queue
		data->queue->AddToQueue(RenderCall {
			texture,
			SDL_Rect { (int) (entity->position.x - width / 2), (int) (entity->position.y - height / 2), (int) width, (int) height },
			SDL_Rect { src.x, src.y, src.w, src.h },
			zOrder,
			entity->rotation,
			entity
		});


		//This is just here so we can see the animation play at a normal speed
		//THIS WILL BE REMOVED
		//SDL_Delay(60);

		currentFrame++;
	}

}

/*
TODO
- UNDO COMMENTED FUNCTION IN UI
- multiple rows
- overall make more versatile/usable

*/
