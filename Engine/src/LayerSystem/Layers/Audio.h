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
                if (!chunk) return;

            Mix_PlayChannel(-1, chunk, loops);
        }
    };
}