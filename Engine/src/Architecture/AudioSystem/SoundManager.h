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
        bool Init(int freq = 44100, SDL_AudioFormat format = AUDIO_F32SYS, int channels = 2, int chunksize = 4096);
        void Shutdown();
        float volume = 128.0f;

        bool LoadSound(const std::string& name, const std::string& filePath);
        Mix_Chunk* GetAudio(const std::string& name);
        void SetVolume(const std::string& name, float volume); // 0.0f to 1.0f
        float GetPlayTime(const std::string& alias);
        //void PlaySound(const std::string& name, int loops);

        std::unordered_map<std::string, AudioData> m_Sounds;
        std::unordered_map<std::string, float> m_Volumes;
    private:
        SoundManager() = default;
        ~SoundManager();

        SoundManager(const SoundManager&) = delete;
        SoundManager& operator=(const SoundManager&) = delete;


        SDL_AudioDeviceID m_Device = 0;
        SDL_AudioSpec m_DeviceSpec{};
    };
}
