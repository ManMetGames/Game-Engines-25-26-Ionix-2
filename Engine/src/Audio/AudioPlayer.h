#pragma once
#include "LayerSystem/Layer.h"
#include "UI/UI.h"
#include "../SDL/SDL2_mixer-2.8.0/include/SDL_mixer.h"
#include <iostream>

namespace IonixEngine
{
    class Audio
    {
    public:
        void Audio::PlayAudio(const char* file, int loops)
        {
            Mix_Chunk* chunk = Mix_LoadWAV(file);
            //check if audio format can be played. Add to SDL_Log below if it works.
            //if (chunk == NULL)
            //{
            //    SDL_Log("error incorect audio format, use MP3 or Wav or Flac", Mix_GetError());
            //    return;
            //}

            Mix_PlayChannel(-1, chunk, loops);
        }


        float Audio::volume = 128;
        void Audio::ChangeVolume(float volume)
        {
            Mix_MasterVolume(volume);
        }


        // play/pause function
        void Pause()
        {
            Mix_Pause(-1);//pauses audio
        }

        void Resume()
        {
            Mix_Resume(-1);//resumes audio
        }

        // Music loop - loops by specified number of times
        void Audio::LoopAudioByTimes(Mix_Music* music, int loops)
        {
            Mix_PlayMusic(music, loops); // loops: 0 = once, 1+ = that many times, -1 = infinite
        }
    };
}