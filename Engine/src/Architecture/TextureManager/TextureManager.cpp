#include "TextureManager.h"
#include "Architecture/Application.h"
#include <iostream>

namespace IonixEngine
{
	TextureManager::TextureManager()
	{
		renderer = Application::Get().GetWindow().GetSdlRenderer();
		errorTexture = TextureData(renderer, "../Assets/debug.png");
	}
	size_t TextureManager::StringToHash(std::string alias)
	{
		size_t hash = std::hash<std::string>{}(alias);
		return hash;
	}

	/// <summary>
	/// Adds a texture to the TextureManager class, ideally store textures in the "Assets" directory
	/// </summary>
	/// <param name="filepath">- string, if accessing Assets directory, prepend texture named with "../Assets/"</param>
	/// <param name="alias">- string, the name a texture will go by when being retrieved</param>
	void TextureManager::AddTexture(std::string filepath,std::string alias)
	{
		size_t hashName = StringToHash(alias);
		TextureData texture = TextureData(renderer, filepath);
		if (texture.IsValid())
		{
			textureDict[hashName] = texture;
		}
	}

	/// <summary>
	/// Retrieves a texture from the TextureManager, using a given alias
	/// </summary>
	/// <param name="alias">- string, the name used to store a texture with</param>
	/// <returns></returns>
	TextureData& TextureManager::GetTexture(std::string alias)
	{
		auto texture = textureDict.find(TextureManager::StringToHash(alias));
		if (texture != textureDict.end())
		{
			return texture->second;
		}
		else
		{
			//return error texture if requested texture not found
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
			//return error texture if requested texture not found
			return errorTexture;
		};
	}
}