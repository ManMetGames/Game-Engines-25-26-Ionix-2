#include "SpriteComponent.h"

namespace IonixEngine {
	SpriteComponent::SpriteComponent(Entity* entity, std::string filePath, std::string name, int zedOrder) : Component(entity, false, true, false) {
		IonixEngine::TextureManager::Get().AddTexture(filePath, name); //adding sprite image file to the texture manager
		zOrder = zedOrder;
	}

	void SpriteComponent::SetTexture(std::string newFilePath, std::string newAlias) {
		//check if alias already exists, if so delete the entry

		IonixEngine::TextureManager::Get().AddTexture(newFilePath, newAlias); 
	}

}