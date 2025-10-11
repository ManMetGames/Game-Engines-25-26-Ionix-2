#include "TextureData.h"
#include "SDL_surface.h"
#include "SDL_render.h"
#include "Architecture/ECS/Temp_Vec2.hpp"
#include <string>
#include <IOStream>

namespace IonixEngine
{
	TextureData::TextureData(SDL_Renderer* renderer,std::string assetPath)
	{
		this->assetPath = assetPath;
		texture = IMG_LoadTexture(renderer,assetPath.c_str());

		if (texture)
		{
			std::cout << "Texture file loaded from: "<< assetPath << std::endl;

			if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0)
			{
				w = 0;
				h = 0;
				std::cout << "Error retrieving texture width/height." << std::endl;
			}

			//std::cout << "Set width to: " << w << "\nSet height to: " << h << std::endl;
		}
		else
		{
			std::cout << "Texture file failed to load: " << IMG_GetError() << std::endl;
		}


	}

	SDL_Texture* TextureData::GetTexture()
	{
		return texture;
	}

	Vec2 TextureData::GetDimensions()
	{
		struct Vec2 output = {w,h};
		return output;
	}

	TextureData::TextureData()
	{
		assetPath = std::string();
		texture = nullptr;
	}
}