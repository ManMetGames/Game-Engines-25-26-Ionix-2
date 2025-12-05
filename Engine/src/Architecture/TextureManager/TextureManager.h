#pragma once
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <string>
#include <map>
#include "SHA256.hpp"
#include "TextureData.h"
#include "Architecture/StringUtils.hpp"

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
		void AddTexture(std::string filepath,std::string alias);
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

		static uint32_t HashFromPath(const std::string& str) {
			uint32_t hash = uint32_t(-1);
		
			std::string base = std::filesystem::path(str).stem().string();

			if (isdigit(base[0])) { base = "_" + base; }

			ReplaceAll(base, "-", "_");
			ReplaceAll(base, " ", "_");
			ReplaceAll(base, ",", "_");
			ReplaceAll(base, "(", "_");
			ReplaceAll(base, ")", "_");

			hash = Get32BitHash(base);

			return hash;
		}
	};
}
