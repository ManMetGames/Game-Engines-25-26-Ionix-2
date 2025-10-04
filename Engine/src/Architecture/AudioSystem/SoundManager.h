#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <SDL.h>
#include "AudioData.h"

namespace IonixEngine
{
    class SoundManager 
    {
    public:
        
        static SoundManager& GetInstance();

        // just for testing for now maybe
        bool Init(int freq = 44100, SDL_AudioFormat format = AUDIO_F32SYS,
            int channels = 2, int chunksize = 4096);
        void Shutdown();

        bool LoadSound(const std::string& name, const std::string& filePath);
        void PlaySound(const std::string& name, int loops = 0);
        void SetVolume(const std::string& name, float volume); // 0.0f to 1.0f

    private:
        SoundManager() = default;
        ~SoundManager();

        SoundManager(const SoundManager&) = delete;
        SoundManager& operator=(const SoundManager&) = delete;

        std::unordered_map<std::string, std::shared_ptr<AudioData>> m_Sounds;
        std::unordered_map<std::string, float> m_Volumes;

        SDL_AudioDeviceID m_Device = 0;
        SDL_AudioSpec m_DeviceSpec{};
    };
}