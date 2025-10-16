#pragma once
#include "LayerSystem/Layer.h"
#include "UI/UI.h"
#include "../SDL/SDL2_mixer-2.8.0/include/SDL_mixer.h"
#include "Architecture/AudioSystem/SoundManager.h"
#include <iostream>
#include <string>

namespace IonixEngine
{
    class AudioPlayer
    {
    public:
        // Per-instance properties (more should be added, leaving it to the audio team members)
        float volume = 128.0f;
        bool mute = false;
        std::string clip = "";

        void PlayAudio(const std::string& soundName, int loops)
        {
            Mix_Chunk* chunk = SoundManager::GetInstance().GetAudio(soundName);
            if (chunk == nullptr)
            {
                SDL_Log("[AudioPlayer] Failed to play sound: %s not found in SoundManager", soundName.c_str());
                return;
            }

            m_Channel = Mix_PlayChannel(-1, chunk, loops);
            
            // Apply volume (unless muted)
            if (m_Channel != -1)
            {
                int vol = mute ? 0 : static_cast<int>(volume);
                Mix_Volume(m_Channel, vol);
            }
        }

        void Play()
        {
            if (clip.empty())
            {
                SDL_Log("[AudioPlayer] Cannot play: no clip assigned");
                return;
            }
            
            PlayAudio(clip, 0);
            // example:
            // gunshotSound.clip = "gunshot";
            // gunshotSound.volume = 64;
            // gunshotSound.Play();
        }

        void ChangeVolume(float vol)
        {
            volume = vol;
            if (m_Channel != -1 && !mute)
            {
                Mix_Volume(m_Channel, static_cast<int>(volume));
            }
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

        // Status query
        bool IsPlaying() const
        {
            if (m_Channel == -1)
            {
                return false;
            }
            
            return Mix_Playing(m_Channel) == 1;
        }

        // Music loop - loops by specified number of times
        void LoopAudioByTimes(Mix_Music* music, int loops)
        {
            Mix_PlayMusic(music, loops); // loops: 0 = once, 1+ = that many times, -1 = infinite
        }

    private:
        int m_Channel = -1; // SDL_mixer channel this instance is using (-1 = not playing)
    };
}