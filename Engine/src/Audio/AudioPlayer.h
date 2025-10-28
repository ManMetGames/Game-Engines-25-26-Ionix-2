#pragma once
#include "LayerSystem/Layer.h"
#include "UI/UI.h"
#include "../SDL/SDL2_mixer-2.8.0/include/SDL_mixer.h"
#include "Architecture/AudioSystem/SoundManager.h"
#include "Architecture/ECS/Component.hpp"
#include <iostream>
#include <string>

namespace IonixEngine
{
    class AudioPlayer : public Component
    {
    public:
        // Per-instance properties (more should be added, leaving it to the audio team members)
        float volume = 128.0f;
        bool mute = false;
        std::string clip = "";
        bool playOnAwake = false;

        // Constructor
        AudioPlayer(Entity* entity, const std::string& audioClip = "", bool playOnAwake = false);

        // Component lifecycle overrides
        void Start() override;
        void Update(float deltaTime) override;
        void Destroy() override;

        void Play(int fadeMilliseconds = 0, int numOfLoops = 0)
        {
            if (clip.empty())
            {
                SDL_Log("[AudioPlayer] Cannot play: no clip assigned");
                return;
            }

            Mix_Chunk* chunk = SoundManager::GetInstance().GetAudio(clip);
            if (chunk == nullptr)
            {
                SDL_Log("[AudioPlayer] Failed to play sound: %s not found", clip.c_str());
                return;
            }

            // Choose fade in or instant start
            if (fadeMilliseconds > 0)
            {
                m_Channel = Mix_FadeInChannel(-1, chunk, numOfLoops, fadeMilliseconds);
            }
            else
            {
                m_Channel = Mix_PlayChannel(-1, chunk, numOfLoops);
            }

            // Apply volume/mute
            if (m_Channel != -1)
            {
                int vol = mute ? 0 : static_cast<int>(volume);
                Mix_Volume(m_Channel, vol);
            }

            // example:
            // gunshotSound.clip = "gunshot";
            // gunshotSound.volume = 64;
            // gunshotSound.Play();       // Instant start
            // gunshotSound.Play(2000);   // 2 second fade in
        }

        void ChangeVolume(float vol)
        {
            volume = vol;
            if (m_Channel != -1 && !mute)
            {
                Mix_Volume(m_Channel, static_cast<int>(volume));
            }
        }

        void ToggleMute()
        {
            mute = !mute;
            if (m_Channel != -1)
            {
                int vol = mute ? 0 : static_cast<int>(volume);
                Mix_Volume(m_Channel, vol);
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

        void End()
        {
            if (m_Channel != -1)
            {
                Mix_HaltMusic();
            }
        }

        bool IsPlaying() const
        {
            if (m_Channel == -1)
            {
                return false;
            }
            
            return Mix_Playing(m_Channel) == 1;

            //example:
            // if (gunshotSound.IsPlaying())
            // {
            //     gunshotSound.Pause();
            // or anything like that
            // }
        }

        // Fire-and-forget sound (doesn't affect this instance's current playback)
        /*void PlayOneShot(const std::string& soundName, float volumeScale = 1.0f)
        {
            Mix_Chunk* chunk = SoundManager::GetInstance().GetAudio(soundName);
            if (chunk == nullptr)
            {
                SDL_Log("[AudioPlayer] Failed to play one-shot: sound '%s' not found", soundName.c_str());
                return;
            }
            
            // Play on any free channel (not this instance's m_Channel)
            int tempChannel = Mix_PlayChannel(-1, chunk, 0);
            if (tempChannel != -1)
            {
                // Apply volume with scale
                int vol = static_cast<int>(volume * volumeScale);
                Mix_Volume(tempChannel, vol);
            }

            // example:
            // clickSound.PlayOneShot("ui_click", 0.8f); // Play at 80% of instance volume
        }*/

        // Music loop - loops by specified number of times
        

    private:
        int m_Channel = -1; // SDL_mixer channel this instance is using (-1 = not playing)
    };
}