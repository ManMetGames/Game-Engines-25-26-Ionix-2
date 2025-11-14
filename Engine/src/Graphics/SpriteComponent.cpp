#include "SpriteComponent.h"
#include <Graphics/QueueRenderer.h>

namespace IonixEngine {

	//Constructors

	SpriteComponent::SpriteComponent(Entity* entity, std::string alias, int width, int height, int zedOrder) : Component(entity, false, true, false) {
		texture = IonixEngine::TextureManager::Get().GetTexture(alias).GetTexture(); //adding sprite image file to the texture manager
		zOrder = zedOrder;
		this->width = width; //size of the sprite
		this->height = height;
		
		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
	}

	SpriteComponent::SpriteComponent(Entity* entity, uint64_t hash, int width, int height, int zedOrder) : Component(entity, false, true, false) {
		texture = IonixEngine::TextureManager::Get().GetTexture(hash).GetTexture(); //adding sprite image file to the texture manager
		zOrder = zedOrder;

		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
	}

	void SpriteComponent::Render(RenderData* data)
	{
		data->queue->AddToQueue(RenderCall {
			texture,
			SDL_Rect { (int) (entity->position.x), (int) (entity->position.y), (int)width, (int)height },
			SDL_Rect { 0, 0, (int)width, (int)height },
		});
	}
}
