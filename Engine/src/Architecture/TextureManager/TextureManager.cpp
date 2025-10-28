#include "TextureManager.h"
#include "Architecture/Application.h"
#include "SDL_render.h"
#include <iostream>

namespace IonixEngine
{
    TextureManager::TextureManager()
    {
        renderer = Application::Get().GetWindow().GetSdlRenderer();
        errorTexture = TextureData();
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
        SDL_Texture* texture = IMG_LoadTexture(renderer, filepath.c_str());
        if (texture) {
            textureDict[hashName] = TextureData();
            textureDict[hashName].SetData(texture, filepath);
            SDL_Log("[Texture] Loaded texture at %s with alias: %s", filepath.c_str(), alias.c_str());
        } else {
            SDL_Log("Failed to load texture: %s", IMG_GetError());
        }
    }

    void TextureManager::Init() {
        std::string filepath = "./Assets/Debug.png";
        SDL_Texture* debug = IMG_LoadTexture(renderer, filepath.c_str());
        errorTexture.SetData(debug, filepath);
    }

    void TextureManager::Shutdown() {
        textureDict.clear();
        errorTexture.Free();
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
            SDL_Log("Failed to find texture: %s", alias.c_str());
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
