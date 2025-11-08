#include "OldSpriteComponent.h"
#include <Graphics/QueueRenderer.h>

//DO NOT USE, will delete in the lab since last time deleting files caused github to S&!% itself

namespace IonixEngine {
	OldSpriteComponent::OldSpriteComponent(Entity* entity, std::string alias, int zedOrder) : Component(entity, false, true, false) {
		texture = IonixEngine::TextureManager::Get().GetTexture(alias).GetTexture(); //adding sprite image file to the texture manager
		zOrder = zedOrder;
		width = 100;
		height = 100;
	}

	void OldSpriteComponent::Render(RenderData* data)
	{
		//create and send render data to the render queue
		data->queue->AddToQueue(RenderCall {
			texture,
			SDL_Rect { (int) (entity->position.x - width / 2), (int) (entity->position.y - height / 2), (int) width, (int) height },
			SDL_Rect { (int)(entity->position.x - width / 2), (int)(entity->position.y - height / 2), (int)width, (int)height }
		});
	}

}