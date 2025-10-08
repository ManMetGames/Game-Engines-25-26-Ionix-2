#include "TextureData.h"
#include "SDL_surface.h"
#include <string>
#include <IOStream>

//extern DECLSPEC SDL_Surface* SDLCALL SDL_CreateRGBSurface
//(Uint32 flags, int width, int height, int depth,
//	Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);

namespace IonixEngine
{
	TextureData::TextureData(SDL_Renderer* renderer,std::string assetPath)
	{
		this->assetPath = assetPath;
		this->texture = IMG_LoadTexture(renderer,assetPath.c_str());
		if (texture)
		{
			std::cout << "Texture file loaded" << std::endl;
		}
		else
		{
			std::cout << "Texture file failed to load: " << IMG_GetError() << "\n";
		}
	}

	SDL_Texture* TextureData::getTexture()
	{
		return texture;
	}

	TextureData::TextureData()
	{
		assetPath = std::string();
		texture = nullptr;
	}




}