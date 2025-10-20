#pragma once
#include <string>
#include "Architecture/AudioSystem/SoundManager.h"

namespace IonixEngine
{
    class Audio
    {
    public:
        explicit Audio(const std::string& name, const std::string& filePath);
        void Play();
        void SetVolume(float volume); // 0.0f - 1.0f
        void Loop(int loops = -1);
        void PauseAll();
        void ResumeAll();

    private:
        std::string m_Name;
    };
}