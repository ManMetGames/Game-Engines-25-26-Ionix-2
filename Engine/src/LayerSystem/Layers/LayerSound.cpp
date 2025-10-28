#include "LayerSystem/Layers/LayerSound.h"  
#include "EventSystem/Event.h"              
#include <iostream>
#include <SDL.h>

namespace IonixEngine
{
    LayerSound::LayerSound() {}

    void LayerSound::OnAttach()
    {
        if (!SoundManager::GetInstance().Init()) {
            std::cerr << "LayerSound: SoundManager init failed!" << std::endl;
        }

        // load the sample
        if (!SoundManager::GetInstance().LoadSound(m_SoundName, m_FilePath)) {
            std::cerr << "LayerSound: Failed to load " << m_FilePath << std::endl;
        }
        std::cout << "LayerSound: Loaded sound '" << m_SoundName << "'." << std::endl;

        // volume set for now
        SoundManager::GetInstance().SetVolume(m_SoundName, 0.8f);
        SoundManager::GetInstance().PlaySound(m_SoundName, -1);
        //SDL_Log("Test audio duration: %f", SoundManager::GetInstance().GetPlayTime("test"));
    }

    void LayerSound::OnDetach()
    {
        std::cout << "LayerSound: Shutting down SoundManager..." << std::endl;
        SoundManager::GetInstance().Shutdown();
    }

    void LayerSound::OnUpdate()
    {
        // an example for now 
        static uint32_t currentTime;
        currentTime++;
        if (currentTime >= 200) {
            //std::cout << "LayerSound: Playing '" << m_SoundName << "' (timed)..." << std::endl;
            currentTime = 0;
            //SoundManager::GetInstance().PlaySound(m_SoundName, 0); 
        }
    }

    void LayerSound::OnEvent(IonixEvent& e)
    {
       
    }
}
