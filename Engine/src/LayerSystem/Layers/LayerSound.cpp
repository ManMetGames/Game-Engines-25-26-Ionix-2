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
            return;
        }
    }

    void LayerSound::OnDetach()
    {
        std::cout << "LayerSound: Shutting down SoundManager..." << std::endl;
        SoundManager::GetInstance().Shutdown();
    }

    void LayerSound::OnUpdate()
    {
    }

    void LayerSound::OnEvent(IonixEvent& e)
    {
    }
}
