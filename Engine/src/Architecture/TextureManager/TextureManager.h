#pragma once
#include <string>
#include <map>
#include "TextureData.h"

//singleton
namespace IonixEngine
{
	class TextureManager
	{
	private:
		std::map<size_t, TextureData> textureDict;
		TextureData errorTexture;
		SDL_Renderer* renderer;
	public:
		TextureManager();
		static size_t StringToHash(std::string alias);
		void AddTexture(std::string filepath,std::string alias);
		TextureData& GetTexture(std::string alias);
		TextureData& GetTexture(size_t hash);
		static TextureManager& Get()
		{
			static TextureManager instance;
			return instance;
		};
	};
}