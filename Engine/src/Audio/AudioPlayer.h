#pragma once
#include "LayerSystem/Layer.h"
#include "UI/UI.h"
#include "../SDL/SDL2_mixer-2.8.0/include/SDL_mixer.h"
#include "Architecture/AudioSystem/SoundManager.h"
#include <iostream>
#include <string>

namespace IonixEngine
{
    class Audio
    {
    public:
        void Audio::PlayAudio(const std::string& soundName, int loops)
        {
            Mix_Chunk* chunk = SoundManager::GetInstance().GetAudio(soundName);
            if (chunk == nullptr)
            {
                SDL_Log("[AudioPlayer] Failed to play sound: %s not found in SoundManager", soundName.c_str());
                return;
            }

            m_Channel = Mix_PlayChannel(-1, chunk, loops);
        }


        float Audio::volume = 128;
        void Audio::ChangeVolume(float volume)
        {
            Mix_MasterVolume(volume);
        }


        // play/pause function
        void Pause()
        {
            if (m_Channel != -1)
            {
                Mix_Pause(m_Channel);
            }
        }

        void Resume()
        {
            if (m_Channel != -1)
            {
                Mix_Resume(m_Channel);
            }
        }

        // Music loop - loops by specified number of times
        void Audio::LoopAudioByTimes(Mix_Music* music, int loops)
        {
            Mix_PlayMusic(music, loops); // loops: 0 = once, 1+ = that many times, -1 = infinite
        }

    private:
        int m_Channel = -1; // SDL_mixer channel this instance is using (-1 = not playing)
    };
}