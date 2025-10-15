#include "Audio.h"
#include <iostream>

namespace IonixEngine
{
    Audio::Audio(const std::string& name, const std::string& filePath)
        : m_Name(name)
    {
        if (!SoundManager::GetInstance().LoadSound(name, filePath))
        {
            std::cerr << "[Audio] Failed to load sound: " << filePath << std::endl;
        }
    }

    void Audio::Play()
    {
        SoundManager::GetInstance().PlaySound(m_Name);
    }

    void Audio::SetVolume(float volume)
    {
        SoundManager::GetInstance().SetVolume(m_Name, volume);
    }

    void Audio::Loop(int loops)
    {
        SoundManager::GetInstance().PlaySound(m_Name, loops);
    }
}