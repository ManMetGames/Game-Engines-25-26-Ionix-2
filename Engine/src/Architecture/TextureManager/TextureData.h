#include <string>
#include <SDL.h>
#include <SDL_image.h>
#pragma once


//extern DECLSPEC SDL_Surface* SDLCALL SDL_CreateRGBSurface
//(Uint32 flags, int width, int height, int depth,
//	Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask);

//make texturedata class, store useful data - assetpath
// nicknames
//find out how to do sdl textures, then match
//class owns it's data
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
		inline bool IsValid() { return texture != nullptr; }
	};
}