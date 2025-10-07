#pragma once
#include "LayerSystem/Layer.h"
#include "UI/UI.h"
#include "../SDL/SDL2_mixer-2.8.0/include/SDL_mixer.h"


namespace IonixEngine
{
    class Audio
    {
    public :
        void Audio::PlayAudio(const char* file, int loops)
        {
            Mix_Chunk* chunk = Mix_LoadWAV(file);
            //check if audio format is an MP3 or WAV format
                if (!chunk) return;
                //needs testing on pc with audio output /Joshua walker
                //if (chunk == NULL)
                //{
                //    SDL_Log("error incorect audio format, use MP3 or Wav", Mix_GetError());
                //    return;
                //}

            Mix_PlayChannel(-1, chunk, loops);
        }
    };
}