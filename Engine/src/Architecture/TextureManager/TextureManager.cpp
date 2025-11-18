#include "TextureManager.h"
#include "Architecture/Application.h"
#include "SDL_render.h"
#include "Architecture/Assets.hpp"
#include "Architecture/SHA256.hpp"
#include <cstdint>
#include <iostream>
#include <string>

namespace IonixEngine {

    TextureManager::TextureManager() {
        renderer = Application::Get().GetWindow().GetSdlRenderer();
        errorTexture = TextureData();
    }

    /// <summary>
    /// Adds a texture to the TextureManager class, ideally store textures in the "Assets" directory
    /// </summary>
    /// <param name="filepath">- string, if accessing Assets directory, prepend texture named with "../Assets/"</param>
    /// <param name="alias">- string, the name a texture will go by when being retrieved</param>
    void TextureManager::AddTexture(std::string filepath,std::string alias) {
        uint32_t hashName = Get32BitHash(alias);
        if (textureDict.find(hashName) != textureDict.end()) { return; }
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
        for (const std::pair<std::string, std::string>& pair : Assets::Get().textures.GetTextures()) {
            AddTexture(pair.second, pair.first);
        }
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
    TextureData& TextureManager::GetTexture(std::string alias) {
        auto texture = textureDict.find(Get32BitHash(alias));
        if (texture != textureDict.end()) {
            return texture->second;
        } else {
            //return error texture if requested texture not found
            //Temporary removing logs so I can see what's going on
           // SDL_Log("Failed to find texture: %s", alias.c_str());
            return errorTexture;
        }
    }

    TextureData& TextureManager::GetTexture(uint32_t hash) {
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

    SDL_Texture* TextureManager::GetRawTexture(uint32_t hash) {
        auto texture = textureDict.find(hash);
        if (texture != textureDict.end()) {
            return texture->second.GetTexture();
        } else {
            //return error texture if requested texture not found
            //Temporary removing logs so I can see what's going on
            //SDL_Log("Failed to find texture: %lu", hash);
            return errorTexture.GetTexture();
        }
    }
}
