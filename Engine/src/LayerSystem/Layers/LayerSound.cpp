#include "LayerSystem/Layers/LayerSound.h"  
#include "EventSystem/Event.h"              
#include <iostream>
#include <SDL.h>

namespace IonixEngine
{
    LayerSound::LayerSound() {}

    void LayerSound::OnAttach()
    {
        // singleton initialize
        auto& soundMgr = SoundManager::GetInstance();
        if (!soundMgr.Init()) {
            std::cerr << "LayerSound: SoundManager init failed!" << std::endl;
            return;
        }
        std::cout << "LayerSound: SoundManager initialized." << std::endl;

        // load the sample
        if (!soundMgr.LoadSound(m_SoundName, m_FilePath)) {
            std::cerr << "LayerSound: Failed to load " << m_FilePath << std::endl;
            return;
        }
        std::cout << "LayerSound: Loaded sound '" << m_SoundName << "'." << std::endl;

        // volume set for now
        soundMgr.SetVolume(m_SoundName, 0.8f);  

        m_LastPlayTime = SDL_GetTicks(); 
    }

    void LayerSound::OnDetach()
    {
        std::cout << "LayerSound: Shutting down SoundManager..." << std::endl;
        SoundManager::GetInstance().Shutdown();
    }

    void LayerSound::OnUpdate()
    {
        // an example for now 
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - m_LastPlayTime >= m_PlayInterval) {
            std::cout << "LayerSound: Playing '" << m_SoundName << "' (timed)..." << std::endl;
            SoundManager::GetInstance().PlaySound(m_SoundName, 0); 
            m_LastPlayTime = currentTime;
        }
    }

    void LayerSound::OnEvent(IonixEvent& e)
    {
       
    }
}
