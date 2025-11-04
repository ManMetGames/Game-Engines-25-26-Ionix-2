#include "TextureData.h"
#include "SDL_render.h"
#include "Architecture/ECS/Temp_Vec2.hpp"
#include <string>
#include <IOStream>

namespace IonixEngine
{	
	SDL_Texture* TextureData::GetTexture()
	{
		return texture;
	}

	Vec2 TextureData::GetDimensions()
	{
		return Vec2 { static_cast<float>(w), static_cast<float>(h) };
	}

	TextureData::TextureData()
	{
		assetPath = std::string();
		texture = nullptr;
	}

	TextureData::~TextureData() {
		if (texture) {
			SDL_Log("[Texture Data] Texture: %s has been destroyed", assetPath.c_str());
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}
	}
	void TextureData::SetData(SDL_Texture* texture, std::string assetPath) {
		this->texture = texture;
		this->assetPath = assetPath;
	}
}
