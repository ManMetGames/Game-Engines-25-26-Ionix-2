#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "Architecture/ECS/Temp_Vec2.hpp"
#pragma once

//texturedata class, owns it's data
namespace IonixEngine
{
	class TextureData
	{
	private:
		int w = 0;
		int h = 0;
		std::string assetPath;
		SDL_Texture* texture = nullptr;

	public:
		TextureData(SDL_Renderer* renderer,std::string assetPath);
		TextureData();
		SDL_Texture* GetTexture();
		Vec2 GetDimensions();
		inline bool IsValid() { return texture != nullptr; }
	};
}