#pragma once
#include "LayerSystem/Layer.h"
#include "UI/UI.h"
#include "Architecture/AudioSystem/SoundManager.h"
#include <iostream>

namespace IonixEngine
{
    class Audio
    {
		Mix_Chunk* chunk = nullptr;
    public:
        Audio(const std::string& name);

        void Audio::PlayAudio();


        float volume = 128;

        void Audio::ChangeVolume(float volume);


        // play/pause function
        void Pause();

        void Resume();

        // Music loop
        void Audio::LoopAudio(double music);
    };
}