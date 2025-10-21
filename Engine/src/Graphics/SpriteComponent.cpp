#include "SpriteComponent.h"

namespace IonixEngine {
	SpriteComponent::SpriteComponent(Entity* entity, std::string filePath, std::string alias, int zedOrder) : Component(entity, false, true, false) {
		IonixEngine::TextureManager::Get().AddTexture(filePath, alias); //adding sprite image file to the texture manager
		zOrder = zedOrder;
	}

	void SpriteComponent::SetTexture(std::string newFilePath, std::string newAlias) {

		TextureData x = IonixEngine::TextureManager::Get().GetTexture(newAlias);

		IonixEngine::TextureManager::Get().AddTexture(newFilePath, newAlias); 
	}

	std::string SpriteComponent::GetAlias()
	{
		return spriteAlias;
	}

}