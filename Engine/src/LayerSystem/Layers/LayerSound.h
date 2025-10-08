#pragma once
#include "LayerSystem/Layer.h"
#include "Architecture/AudioSystem/SoundManager.h"  
#include <SDL.h>          

namespace IonixEngine
{
    class LayerSound : public Layer
    {
    public:
        LayerSound();
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(IonixEvent& e) override;

    private:
        std::string m_SoundName = "test";  // THIS IS FOR TESTING!!!
        std::string m_FilePath = "./test_sound.wav";
        Uint32 m_LastPlayTime = 0; 
        const Uint32 m_PlayInterval = 5000;  
    };
}