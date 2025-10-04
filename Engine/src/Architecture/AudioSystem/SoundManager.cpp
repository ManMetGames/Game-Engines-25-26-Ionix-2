#include "SoundManager.h"
#include <iostream>
#include <SDL_mixer.h>  // i think this is right I cant tell.

namespace IonixEngine {

    // you got ur singleton!
    SoundManager& SoundManager::GetInstance()
    {
        static SoundManager instance;
        return instance;
    }

    bool SoundManager::Init(int freq, SDL_AudioFormat format, int channels, int chunksize) 
    {
        if (SDL_Init(SDL_INIT_AUDIO) < 0) 
        {
            std::cerr << "SDL audio init failed: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_AudioSpec desiredSpec{};
        desiredSpec.freq = freq;
        desiredSpec.format = format;
        desiredSpec.channels = channels;
        desiredSpec.samples = chunksize;
        desiredSpec.callback = nullptr; // this is the callback, it might need changing one day.

        m_Device = SDL_OpenAudioDevice(nullptr, 0, &desiredSpec, &m_DeviceSpec, 0);
        if (m_Device == 0) 
        {
            std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_PauseAudioDevice(m_Device, 0); // what do you think it does???
        return true;
    }

    void SoundManager::Shutdown() 
    {
        for (auto& [name, clip] : m_Sounds) 
        {
            // shared_ptr takes care of freeing buffer
        }
        m_Sounds.clear();
        m_Volumes.clear();

        if (m_Device != 0) 
        {
            SDL_CloseAudioDevice(m_Device);
            m_Device = 0;
        }
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }

    bool SoundManager::LoadSound(const std::string& name, const std::string& filePath) 
    {
        auto clip = std::make_shared<AudioData>();
        if (SDL_LoadWAV(filePath.c_str(), &clip->spec, &clip->buffer, &clip->length) == nullptr) 
        {
            std::cerr << "Failed to load sound: " << filePath << " - " << SDL_GetError() << std::endl;
            return false;
        }

        m_Sounds[name] = clip;
        return true;
    }

    void SoundManager::PlaySound(const std::string& name, int loops) 
    {
        auto it = m_Sounds.find(name);
        if (it == m_Sounds.end()) 
        {
            std::cerr << "Sound not found: " << name << std::endl;
            return;
        }

        auto& clip = it->second;
        float volume = m_Volumes.count(name) ? m_Volumes[name] : 1.0f;
        int timesToPlay = (loops == 0) ? 1 : loops + 1;  // 0 = play once; >0 = play loops + 1 times

        for (int i = 0; i < timesToPlay; ++i) 
        {
            // create buffer for volume adjustment
            Uint8* tempBuffer = new Uint8[clip->length]();  
            SDL_MixAudioFormat(tempBuffer, clip->buffer, clip->spec.format,  
                clip->length, static_cast<int>(SDL_MIX_MAXVOLUME * volume));

            if (SDL_QueueAudio(m_Device, tempBuffer, clip->length) < 0) 
            {
                std::cerr << "Failed to queue sound: " << SDL_GetError() << std::endl;
            }
            delete[] tempBuffer;  // this looks useful.
        }
    }

    void SoundManager::SetVolume(const std::string& name, float volume) 
    {
        if (volume < 0.0f) volume = 0.0f;
        if (volume > 1.0f) volume = 1.0f;
        m_Volumes[name] = volume;
        // does not play the sound; volume applies to future 'PlaySound' calls
    }

    SoundManager::~SoundManager() 
    {
        Shutdown();
    }

}  