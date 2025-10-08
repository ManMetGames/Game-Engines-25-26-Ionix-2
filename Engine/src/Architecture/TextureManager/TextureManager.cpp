#include "TextureManager.h"
#include <Architecture/Application.h>
#include <iostream>


namespace IonixEngine
{
	TextureManager::TextureManager()
	{
		renderer = Application::Get().GetWindow().GetSdlRenderer();
		errorTexture = TextureData(renderer, "debug.png");
	}
	size_t TextureManager::stringToHash(std::string filepath)
	{
		size_t hash = std::hash<std::string>{}(filepath);
		return hash;
	}

	void TextureManager::AddTexture(std::string filepath,std::string alias)
	{
		size_t hashName = stringToHash(alias);
		TextureData texture = TextureData(renderer, filepath);
		if (texture.IsValid()) {
			textureDict[hashName] = texture;
		}
	}
	TextureData& TextureManager::GetTexture(std::string alias)
	{
		auto texture = textureDict.find(TextureManager::stringToHash(alias));
		if (texture != textureDict.end())
		{
			return texture->second;
		}
		else
		{
			//return error texture
			return errorTexture;
		};
	}
	TextureData& TextureManager::GetTexture(size_t hash)
	{
		auto texture = textureDict.find(hash);
		if (texture != textureDict.end())
		{
			return texture->second;
		}
		else
		{
			//return error texture
			return errorTexture;
		};
	}
}