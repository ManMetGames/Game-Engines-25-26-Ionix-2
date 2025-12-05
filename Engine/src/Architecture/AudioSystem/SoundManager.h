#pragma once
#include <cstdint>
#include <filesystem>
#include <unordered_map>
#include <string>
#include <memory>
#include <SDL.h>
#include "AudioData.h"
#include "Architecture/SHA256.hpp"

#include "Architecture/StringUtils.hpp"
namespace IonixEngine
{
    class SoundManager 
    {
    public:
        static SoundManager& GetInstance();

        // just for testing for now maybe
        bool Init(int freq = 44100, SDL_AudioFormat format = AUDIO_F32SYS, int channels = 2, int chunksize = 4096);
        void Shutdown();

        bool LoadSound(const std::string& name, const std::string& filePath);
        Mix_Chunk* GetAudio(const std::string& name);
        Mix_Chunk* GetAudio(uint32_t hash);
        void SetVolume(const std::string& name, float volume); // 0.0f to 1.0f
        void SetVolume(uint32_t hash, float volume); // 0.0f to 1.0f
        float GetPlayTime(const std::string& alias);
        float GetPlayTime(uint32_t hash);


		static uint32_t HashFromPath(const std::string& str) {
			uint32_t hash = uint32_t(-1);
		
			std::string base = std::filesystem::path(str).stem().string();

			ReplaceAll(base, "-", "_");
			ReplaceAll(base, " ", "_");
			ReplaceAll(base, ",", "_");

			hash = Get32BitHash(base);

			return hash;
		}

    private:
        SoundManager() = default;
        ~SoundManager();

        SoundManager(const SoundManager&) = delete;
        SoundManager& operator=(const SoundManager&) = delete;

        std::unordered_map<uint32_t, AudioData> m_Sounds;
        std::unordered_map<uint32_t, float> m_Volumes;

        SDL_AudioDeviceID m_Device = 0;
        SDL_AudioSpec m_DeviceSpec{};
    };
}
