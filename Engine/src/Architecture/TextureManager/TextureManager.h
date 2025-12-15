#pragma once
#include <cstdint>
#include <string>
#include <map>
#include "TextureData.h"

//singleton
namespace IonixEngine
{
	class TextureManager
	{
	private:
		std::map<uint32_t, TextureData> textureDict;
		TextureData errorTexture;
		SDL_Renderer* renderer;
	public:
		TextureManager();
		void AddTexture(std::string filepath, std::string alias);
		TextureData& GetTexture(std::string alias);
		TextureData& GetTexture(uint32_t hash);

		SDL_Texture* GetRawTexture(uint32_t hash);

		void Init();
		void Shutdown();
		static TextureManager& Get()
		{
			static TextureManager instance;
			return instance;
		};
	};
}